#ifndef CATEGORYSELECTWINDOW_H
#define CATEGORYSELECTWINDOW_H

#include <QDialog>
#include <QMap>
#include <clicablelabel.h>
#include <categoriesrepository.h>
#include <viewcategorydialog.h>

namespace Ui {
class CategorySelectWindow;
}

class CategorySelectWindow : public QDialog
{
    Q_OBJECT
    Ui::CategorySelectWindow *ui;
    bool canBeClosed;

    QMap<ClickableLabel*, Category::CategoryPtr> _labelToCategory;
    QSharedPointer<ViewCategoryDialog> viewCategoryDialog;
public:
    explicit CategorySelectWindow(QWidget *parent = 0);
    ~CategorySelectWindow();

    void createCategorySelectors();

protected:

private slots:
    void onCategoryLabelClicked(ClickableLabel *sender);
signals:
    void showCategory(Category::CategoryPtr);
private:
};

#endif // CATEGORYSELECTWINDOW_H
