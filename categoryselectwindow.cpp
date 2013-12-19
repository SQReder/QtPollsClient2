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
    auto repo = CategoriesRepository::Instance();
    repo->loadCategoriesFromDir("pics");
}

CategorySelectWindow::~CategorySelectWindow()
{
    delete ui;
}

void CategorySelectWindow::createCategorySelectors()
{

}

void CategorySelectWindow::closeEvent(QCloseEvent *e) {
    e->setAccepted(canBeClosed);
}
