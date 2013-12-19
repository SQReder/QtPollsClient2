#include "viewcategorydialog.h"
#include "ui_viewcategorydialog.h"
#include "clicablelabel.h"
#include <QDebug>

ViewCategoryDialog::ViewCategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewCategoryDialog)
{
    ui->setupUi(this);
}

ViewCategoryDialog::~ViewCategoryDialog()
{
    delete ui;
}

void ViewCategoryDialog::onShowCategory(Category::CategoryPtr category)
{
    QSize thumbSize(256,256);

    _labels.clear();

    auto images = category->listImages();
    for (auto item = images.begin(); item != images.end(); ++item) {
        auto image = *item;
        auto thumb = image->Thumbnail(thumbSize);

        auto label = QSharedPointer<ClickableLabel>(new ClickableLabel());
        label->setPixmap(thumb);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(false);

        connect(label.data(), SIGNAL(clicked(ClickableLabel*)), this, SLOT(onImageLabelClicked(ClickableLabel*)));

        _labels.push_back(label);
        _labelToImage.insert(label.data(), image);
    }


    for(int i = 0; i != _labels.size(); ++i) {
        ui->scrollAreaGrid->addWidget(_labels[i].data(), i / 5, i % 5);
    }

    this->showFullScreen();
    this->raise();
}

void ViewCategoryDialog::on_pbBack_clicked()
{
    this->hide();
}

void ViewCategoryDialog::onImageLabelClicked(ClickableLabel *sender)
{
    auto image = _labelToImage[sender];
    qDebug() << image->name();
}
