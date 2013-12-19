#include <QDebug>
#include <QCloseEvent>
#include "categoryselectwindow.h"
#include "ui_categoryselectwindow.h"

CategorySelectWindow::CategorySelectWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CategorySelectWindow),
    canBeClosed(false)
{
    ui->setupUi(this);
    auto repo = CategoriesRepository::Instance();
    repo->loadCategoriesFromDir("pics");

    viewCategoryDialog = QSharedPointer<ViewCategoryDialog>(new ViewCategoryDialog());
    connect(this, SIGNAL(showCategory(Category::CategoryPtr)), viewCategoryDialog.data(), SLOT(onShowCategory(Category::CategoryPtr)));
}

CategorySelectWindow::~CategorySelectWindow()
{
    delete ui;
}

void CategorySelectWindow::createCategorySelectors()
{
    QSize thumbnailSize(256,256);
    QVector<ClickableLabel*> labels;
    auto categories = CategoriesRepository::Instance()->listCategories();
    foreach (Category::CategoryPtr category, categories) {
        auto art = category->getArt();
        auto artThumb = art->Thumbnail(thumbnailSize);

        auto label = new ClickableLabel();

        label->setPixmap(artThumb);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(false);
        labels.push_back(label);
        connect(label, SIGNAL(clicked(ClickableLabel*)), this, SLOT(onCategoryLabelClicked(ClickableLabel*)));

        _labelToCategory.insert(label, category);
    }

    for(int i = 0; i != labels.size(); ++i) {
        ui->gridLayout->addWidget(labels[i], i / 5, i % 5);
    }
}

void CategorySelectWindow::closeEvent(QCloseEvent *e) {
    e->setAccepted(canBeClosed);
}

void CategorySelectWindow::onCategoryLabelClicked(ClickableLabel *sender)
{
    qDebug() << "clicked label for category " << _labelToCategory[sender]->Name();
    emit showCategory(_labelToCategory[sender]);
}
