#include <QApplication>
#include "calculatorwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QApplication::setApplicationName(QStringLiteral("吴小东的计算器"));
    CalculatorWindow window;
    window.show();
    return app.exec();
}
