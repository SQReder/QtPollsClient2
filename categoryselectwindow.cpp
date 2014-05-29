#include <QDebug>
#include <QCloseEvent>
#include "categoryselectwindow.h"
#include "ui_categoryselectwindow.h"

CategorySelectWindow::CategorySelectWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CategorySelectWindow),
    canBeClosed(false)
{
    ui->setupUi(this);
    auto repo = CategoriesRepository::Instance();
    repo->loadCategoriesFromDir("pics");

    viewCategoryDialog = QSharedPointer<ViewCategoryDialog>(new ViewCategoryDialog());
    connect(this, SIGNAL(showCategory(Category::CategoryPtr, bool)), viewCategoryDialog.data(), SLOT(onShowCategory(Category::CategoryPtr, bool)));
}

CategorySelectWindow::~CategorySelectWindow()
{
    delete ui;
}

void CategorySelectWindow::createCategorySelectors()
{
    QSize thumbnailSize(800,600);
    QVector<ClickableLabel*> labels;
    const QFont font("MS Shell Dlg", 32);
    auto categories = CategoriesRepository::Instance()->listCategories();
    if (categories.count() != 1) {
        foreach (Category::CategoryPtr category, categories) {
            auto art = category->getArt();
            auto artThumb = art->getImage(thumbnailSize);

            auto label = new ClickableLabel();

            label->setPixmap(artThumb);
            label->setAlignment(Qt::AlignCenter);
            label->setScaledContents(false);
            labels.push_back(label);
            connect(label, SIGNAL(clicked(ClickableLabel*)), this, SLOT(onCategoryLabelClicked(ClickableLabel*)));

            auto textLabel = new ClickableLabel();
            textLabel->setAlignment(Qt::AlignCenter);
            textLabel->setText(category->Name());
            textLabel->setMaximumHeight(30);
            textLabel->setFont(font);
            labels.push_back(textLabel);

            _labelToCategory.insert(label, category);
            _labelToCategory.insert(textLabel, category);
        }

        for(int i = 0; i != labels.size(); ++i) {
            ui->gridLayout->addWidget(labels[i], i, 0);
        }
    } else if (categories.count() == 1) {
        emit showCategory(categories.first(), true);
        this->hide();
    }
}

void CategorySelectWindow::onCategoryLabelClicked(ClickableLabel *sender)
{
    qDebug() << "clicked label for category " << _labelToCategory[sender]->Name();
    emit showCategory(_labelToCategory[sender], false);
}
