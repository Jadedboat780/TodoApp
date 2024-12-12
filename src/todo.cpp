#include "todo.h"
#include <fstream>
#include <QMessageBox>
#include <QTcpSocket>
#include <QTcpServer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    // Инициализация виджетов
    taskList = new QListWidget(this);
    taskInput = new QLineEdit(this);
    addButton = new QPushButton("Add", this);
    removeButton = new QPushButton("Remove", this);
    doneButton = new QPushButton("Done", this);

    QPushButton *saveButton = new QPushButton("Save", this);
    QPushButton *loadButton = new QPushButton("Load", this);
    QPushButton *connectButton = new QPushButton("Connect", this);

    // Применение стилей
    QString styleSheet = R"(
        QMainWindow {
            background-color: #f5f5f5;
        }
        QListWidget {
            border: 1px solid #ccc;
            border-radius: 5px;
            padding: 5px;
            font-size: 14px;
            background-color: #ffffff;
        }
        QLineEdit {
            border: 1px solid #ccc;
            border-radius: 5px;
            padding: 5px;
            font-size: 14px;
            background-color: #ffffff;
        }
        QPushButton {
            border: 1px solid #2d89ef;
            border-radius: 5px;
            padding: 5px 10px;
            background-color: #2d89ef;
            color: white;
            font-size: 14px;
        }
        QPushButton:disabled {
            background-color: #aaa;
            color: #eee;
        }
        QPushButton:hover {
            background-color: #1e5cb3;
        }
        QPushButton:pressed {
            background-color: #16418b;
        }
        QStatusBar {
            background-color: #2d89ef;
            color: white;
        }
    )";
    setStyleSheet(styleSheet);

    // Структура интерфейса
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);
    buttonLayout->addWidget(doneButton);
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(connectButton);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(taskList);
    mainLayout->addWidget(taskInput);
    mainLayout->addLayout(buttonLayout);

    auto *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // Связь сигналов и слотов
    connect(addButton, &QPushButton::clicked, this, &MainWindow::addTask);
    connect(removeButton, &QPushButton::clicked, this, &MainWindow::removeTask);
    connect(doneButton, &QPushButton::clicked, this, &MainWindow::markTaskDone);
    connect(saveButton, &QPushButton::clicked, this, &MainWindow::saveTasks);
    connect(loadButton, &QPushButton::clicked, this, &MainWindow::loadTasks);
    connect(connectButton, &QPushButton::clicked, this, &MainWindow::connectToServer);
    connect(taskList, &QListWidget::itemSelectionChanged, this, &MainWindow::updateButtonStates);

    // Инициализация состояния кнопок
    updateButtonStates();

    // Настройки окна
    statusBar()->showMessage("Welcome to Collaborative To-Do List!");
    setWindowTitle("To-Do List - Collaborative Mode");
    resize(500, 400);
}

MainWindow::~MainWindow() = default;

void MainWindow::addTask() {
    QString task = taskInput->text().trimmed();
    if (task.isEmpty()) {
        statusBar()->showMessage("Task cannot be empty!", 2000);
        return;
    }
    taskList->addItem(task);
    taskInput->clear();
    statusBar()->showMessage("Task added!", 2000);
}

void MainWindow::removeTask() {
    auto selectedItems = taskList->selectedItems();
    for (auto *item : selectedItems) {
        delete taskList->takeItem(taskList->row(item));
    }
    statusBar()->showMessage("Task removed!", 2000);
    updateButtonStates();
}

void MainWindow::markTaskDone() {
    auto selectedItems = taskList->selectedItems();
    for (auto *item : selectedItems) {
        item->setForeground(QBrush(Qt::gray));
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    }
    statusBar()->showMessage("Task marked as done!", 2000);
}

void MainWindow::updateButtonStates() {
    bool hasSelection = !taskList->selectedItems().isEmpty();
    removeButton->setEnabled(hasSelection);
    doneButton->setEnabled(hasSelection);
}

void MainWindow::saveTasks() {
    std::ofstream outFile("tasks.txt");
    if (!outFile.is_open()) {
        QMessageBox::critical(this, "Error", "Unable to save tasks to file.");
        return;
    }

    for (int i = 0; i < taskList->count(); ++i) {
        outFile << taskList->item(i)->text().toStdString() << "\n";
    }

    outFile.close();
    statusBar()->showMessage("Tasks saved successfully!", 2000);
}

void MainWindow::loadTasks() {
    std::ifstream inFile("tasks.txt");
    if (!inFile.is_open()) {
        QMessageBox::critical(this, "Error", "Unable to load tasks from file.");
        return;
    }

    taskList->clear();
    std::string line;
    while (std::getline(inFile, line)) {
        taskList->addItem(QString::fromStdString(line));
    }

    inFile.close();
    statusBar()->showMessage("Tasks loaded successfully!", 2000);
}

void MainWindow::connectToServer() {
    QTcpSocket *socket = new QTcpSocket(this);
    socket->connectToHost("127.0.0.1", 12345);

    if (!socket->waitForConnected(3000)) {
        QMessageBox::critical(this, "Error", "Unable to connect to server.");
        return;
    }

    statusBar()->showMessage("Connected to server!", 2000);
}
