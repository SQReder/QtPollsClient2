#ifndef VIEWCATEGORYDIALOG_H
#define VIEWCATEGORYDIALOG_H

#include <QDialog>
#include <Qmap>
#include <category.h>

class ClickableLabel;
namespace Ui {
class ViewCategoryDialog;
}

class ViewCategoryDialog : public QDialog
{
    Q_OBJECT

    QVector<QSharedPointer<ClickableLabel>> _labels;
    QMap<ClickableLabel*, CategoryImage::CategoryImagePtr> _labelToImage;
public:
    explicit ViewCategoryDialog(QWidget *parent = 0);
    ~ViewCategoryDialog();

private:
    Ui::ViewCategoryDialog *ui;

signals:
public slots:
    void onShowCategory(Category::CategoryPtr category);
private slots:
    void on_pbBack_clicked();
    void onImageLabelClicked(ClickableLabel*sender);
};

#endif // VIEWCATEGORYDIALOG_H
