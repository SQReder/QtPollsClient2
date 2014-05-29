#ifndef VIEWCATEGORYDIALOG_H
#define VIEWCATEGORYDIALOG_H

#include <QDialog>
#include <Qmap>
#include <category.h>
#include <QSharedPointer>
#include <votedialog.h>

class ClickableLabel;
namespace Ui {
class ViewCategoryDialog;
}

class ViewCategoryDialog : public QWidget
{
    Q_OBJECT

    QVector<QSharedPointer<ClickableLabel>> _labels, _textLables;
    QMap<ClickableLabel*, CategoryImage::CategoryImagePtr> _labelToImage;

    QSharedPointer<VoteDialog> _voteDialog;
public:
    explicit ViewCategoryDialog(QWidget *parent = 0);
    ~ViewCategoryDialog();

private:
    Ui::ViewCategoryDialog *ui;
    bool exitOnClose;

protected:
    void closeEvent(QCloseEvent *);

signals:
    void showVoteDialog(CategoryImage::CategoryImagePtr);
public slots:
    void onShowCategory(Category::CategoryPtr category, bool hideBackButton);
    void OnVoteDialogClosed();

private slots:
    virtual void showEvent(QShowEvent *);
    virtual void resizeEvent(QResizeEvent *);
    void on_pbBack_clicked();
    void onImageLabelClicked(ClickableLabel*sender);
    void onCodeVerified();
};

#endif // VIEWCATEGORYDIALOG_H
