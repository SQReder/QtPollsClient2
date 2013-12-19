#include <QApplication>
#include "connectdialog.h"
#include "scanworker.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectDialog w;
    w.show();

    a.connect(&w, SIGNAL(finished(int)), SLOT(quit()));

    CreateScannerWorkerThread();

    return a.exec();
}
