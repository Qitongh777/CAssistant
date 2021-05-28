#include <QtWidgets/QApplication>
#include <QPushButton>
#include "LineChart.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPushButton pb("CAssistant");
    pb.show();

    LineChart line("gogogo");
    line.show();
    return a.exec();
}

