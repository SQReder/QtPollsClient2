#ifndef CATEGORYSELECTWINDOW_H
#define CATEGORYSELECTWINDOW_H

#include <QDialog>

namespace Ui {
class CategorySelectWindow;
}

class CategorySelectWindow : public QDialog
{
    Q_OBJECT
    Ui::CategorySelectWindow *ui;
    bool canBeClosed;

public:
    explicit CategorySelectWindow(QWidget *parent = 0);
    ~CategorySelectWindow();

    void createCategorySelectors();

protected:
    void closeEvent(QCloseEvent *);

private slots:

private:
};

#endif // CATEGORYSELECTWINDOW_H
