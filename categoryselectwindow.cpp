#include <QCloseEvent>
#include "categoryselectwindow.h"
#include "ui_categoryselectwindow.h"

CategorySelectWindow::CategorySelectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategorySelectWindow),
    canBeClosed(false)
{
    ui->setupUi(this);
}

CategorySelectWindow::~CategorySelectWindow()
{
    delete ui;
}

void CategorySelectWindow::closeEvent(QCloseEvent *e) {
    e->setAccepted(canBeClosed);
}
