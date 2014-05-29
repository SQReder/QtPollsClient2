#include <QApplication>
#include "connectdialog.h"
#include "scanworker.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectDialog w;
    w.show();


    CreateScannerWorkerThread();
    QObject::connect(&a, SIGNAL(aboutToQuit()), ScanWorker::Instance(), SLOT(EndWork()));

    try {
        return a.exec();
    } catch (std::exception &e) {
        QMessageBox msgBox;
        msgBox.setText(QString("Fatal error: ") + e.what());
        msgBox.exec();
        ScanWorker::Instance()->StopCam();
    }

    return 0;
}
