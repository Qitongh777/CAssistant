#include <QCoreApplication>
#include <QtWidgets/QApplication>
#include <QPushButton>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPushButton pb("CAssistant");
    pb.show();
    return a.exec();
}

