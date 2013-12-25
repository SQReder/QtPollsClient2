#include "viewcategorydialog.h"
#include "ui_viewcategorydialog.h"
#include "clicablelabel.h"
#include <QDebug>
#include <QScroller>
#include <clientconnection.h>

ViewCategoryDialog::ViewCategoryDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ViewCategoryDialog)
{
    ui->setupUi(this);
    QScroller::grabGesture(ui->scrollArea, QScroller::LeftMouseButtonGesture);

    QObject::connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)),
                     ui->scrollArea->verticalScrollBar(), SLOT(setValue(int)));

    QObject::connect(ui->scrollArea->verticalScrollBar(), SIGNAL(valueChanged(int)),
                     ui->verticalScrollBar, SLOT(setValue(int)));

    QObject::connect(ui->verticalScrollBar, SIGNAL(rangeChanged(int,int)),
                     ui->scrollArea->verticalScrollBar(), SLOT(setRange(int, int)));

    _voteDialog = QSharedPointer<VoteDialog>(new VoteDialog());
    QObject::connect(this, SIGNAL(showVoteDialog(CategoryImage::CategoryImagePtr)), _voteDialog.data(), SLOT(onShowVoteDialog(CategoryImage::CategoryImagePtr)));

    auto client = ClientConnection::Instance();
    ClientConnection *cl = client.data();
    QObject::connect(cl, SIGNAL(CodeVerified()), this, SLOT(onCodeVerified()));

}

ViewCategoryDialog::~ViewCategoryDialog()
{
    delete ui;
}

void ViewCategoryDialog::onShowCategory(Category::CategoryPtr category)
{
    QSize thumbSize(500,400);

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


    const int itemsInRow = 2;
    for(int i = 0; i != _labels.size(); ++i) {
        ui->scrollAreaGrid->addWidget(_labels[i].data(), i / itemsInRow, i % itemsInRow);
    }
    ui->scrollArea->verticalScrollBar()->setValue(0);

    this->showFullScreen();
    this->raise();
}

void ViewCategoryDialog::showEvent(QShowEvent *)
{
    ui->verticalScrollBar->setMaximum(ui->scrollArea->verticalScrollBar()->maximum());
    ui->verticalScrollBar->setMinimum(ui->scrollArea->verticalScrollBar()->minimum());
}

void ViewCategoryDialog::resizeEvent(QResizeEvent *e)
{
    ui->verticalScrollBar->setMaximum(ui->scrollArea->verticalScrollBar()->maximum());
    ui->verticalScrollBar->setMinimum(ui->scrollArea->verticalScrollBar()->minimum());
}

void ViewCategoryDialog::on_pbBack_clicked()
{
    this->hide();
}

void ViewCategoryDialog::onImageLabelClicked(ClickableLabel *sender)
{
    auto image = _labelToImage[sender];
    emit showVoteDialog(image);
    qDebug() << image->name();
}

void ViewCategoryDialog::onCodeVerified()
{
    this->hide();
}
