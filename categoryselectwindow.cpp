#include <QCloseEvent>
#include "categoryselectwindow.h"
#include "ui_categoryselectwindow.h"

#include <categoriesrepository.h>

CategorySelectWindow::CategorySelectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategorySelectWindow),
    canBeClosed(false)
{
    ui->setupUi(this);
    CategoriesRepository::Instance()->loadCategoriesFromDir("pics");
}

CategorySelectWindow::~CategorySelectWindow()
{
    delete ui;
}

void CategorySelectWindow::closeEvent(QCloseEvent *e) {
    e->setAccepted(canBeClosed);
}
