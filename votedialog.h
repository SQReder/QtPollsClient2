#ifndef VOTEDIALOG_H
#define VOTEDIALOG_H

#include <QDialog>
#include <category.h>

namespace Ui {
class VoteDialog;
}

enum VoteStatus { vsAccepted, vsWrong, vsCodeUSed };

class VoteDialog : public QDialog
{
    Q_OBJECT

    bool _waitForCode;
    CategoryImage::CategoryImagePtr _currentImage;

public:
    explicit VoteDialog(QWidget *parent = 0);
    ~VoteDialog();

private:
    Ui::VoteDialog *ui;
    void ShowStatus();
    VoteStatus status;

public slots:
    void onShowVoteDialog(CategoryImage::CategoryImagePtr image);
private slots:
    void on_pbBack_clicked();

    void onCodeScanned(QString code);
    void onCodeVerified();
    void onCodeAlreadyUsed();
    void onWrongCode();

    void onTimerStop();
};

#endif // VOTEDIALOG_H
