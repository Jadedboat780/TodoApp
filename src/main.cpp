#include <QApplication>
#include <QStatusBar>
#include "todo.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}
