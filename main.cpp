#include <QtWidgets/QApplication>
#include <QPushButton>
#include "LineChart.h"
int main(int argc, char *argv[])
{
    QString str = "123123123";
    QApplication a(argc, argv);
    //QPushButton pb("CAssistant");
    //pb.show();

    LineChart line("gogogo");
    line.show();
    return QApplication::exec();
}