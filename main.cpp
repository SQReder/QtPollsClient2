#include <QApplication>
#include "connectdialog.h"
#include "scanworker.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{

    QStringList paths = QCoreApplication::libraryPaths();
    paths.append(".");
    paths.append("imageformats");
    paths.append("platforms");
    QApplication::setLibraryPaths(paths);

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
