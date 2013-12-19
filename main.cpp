#include <QApplication>
#include "connectdialog.h"
#include "scanworker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectDialog w;
    w.show();

    CreateScannerWorkerThread();

    return a.exec();
}
