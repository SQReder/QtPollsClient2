#include <QApplication>
#include "connectdialog.h"
#include "scanworker.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ConnectDialog dialog;
    dialog.show();


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
