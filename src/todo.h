#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QStatusBar>
#include <QHBoxLayout>
#include <QTcpSocket>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addTask();
    void removeTask();
    void markTaskDone();
    void saveTasks();
    void loadTasks();
    void connectToServer();

private:
    QListWidget *taskList;
    QLineEdit *taskInput;
    QPushButton *addButton;
    QPushButton *removeButton;
    QPushButton *doneButton;

    void updateButtonStates();
};

#endif // MAINWINDOW_H
