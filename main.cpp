#include "blockingclient.h"
#include "httpwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // Blockingclient w;
    // w.show();

    HttpWindow httpw;
    httpw.show();

    return a.exec();
}
