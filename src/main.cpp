#include "MainWindow.h"

#include <QApplication>

int32_t main(int32_t argc, char* argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Zip View");
    window.show();
    return app.exec();
}
