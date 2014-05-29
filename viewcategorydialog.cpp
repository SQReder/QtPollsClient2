#include "viewcategorydialog.h"
#include "ui_viewcategorydialog.h"
#include "clicablelabel.h"
#include <QDebug>
#include <QScroller>
#include <QCloseEvent>
#include <clientconnection.h>

ViewCategoryDialog::ViewCategoryDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ViewCategoryDialog),
    exitOnClose(false)
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
    QObject::connect(_voteDialog.data(), SIGNAL(ThisHides()), this, SLOT(OnVoteDialogClosed()));

    auto client = ClientConnection::Instance();
    ClientConnection *cl = client.data();
    QObject::connect(cl, SIGNAL(CodeVerified()), this, SLOT(onCodeVerified()));
}

ViewCategoryDialog::~ViewCategoryDialog()
{
    delete ui;
}

void ViewCategoryDialog::closeEvent(QCloseEvent *event)
{
    if (exitOnClose) {
//        event->accept();
        QApplication::quit();
    } else {
        event->ignore();
    }
}

void ViewCategoryDialog::onShowCategory(Category::CategoryPtr category, bool hideBackButton)
{
    QSize thumbSize(500,400);
    const QFont font("MS Shell Dlg", 24);

    if (hideBackButton)
        ui->pbBack->hide();
    else
        ui->pbBack->show();
    exitOnClose = hideBackButton;

    _labels.clear();
    _textLables.clear();

    auto images = category->listImages();
    for (auto item = images.begin(); item != images.end(); ++item) {
        auto image = *item;
        auto thumb = image->Thumbnail(thumbSize);

        auto label = QSharedPointer<ClickableLabel>(new ClickableLabel());
        label->setPixmap(thumb);
        label->setAlignment(Qt::AlignCenter);
        label->setScaledContents(false);

        auto textLabel = QSharedPointer<ClickableLabel>(new ClickableLabel());
        textLabel->setText(image->name());
        textLabel->setMaximumHeight(25);
        textLabel->setAlignment(Qt::AlignCenter);
        textLabel->setFont(font);


        connect(label.data(), SIGNAL(clicked(ClickableLabel*)), this, SLOT(onImageLabelClicked(ClickableLabel*)));

        _labels.push_back(label);
        _textLables.push_back(textLabel);
        _labelToImage.insert(label.data(), image);
    }


    const int itemsInRow = 2;
    for(int i = 0; i != _labels.size(); ++i) {
        ui->scrollAreaGrid->addWidget(_labels[i].data(), i / itemsInRow * 2, i % itemsInRow);
        ui->scrollAreaGrid->addWidget(_textLables[i].data(), i / itemsInRow * 2 + 1, i % itemsInRow);
    }
    ui->scrollArea->verticalScrollBar()->setValue(0);

    this->show();
    this->setWindowState(Qt::WindowFullScreen);
    this->raise();
}

void ViewCategoryDialog::OnVoteDialogClosed()
{
    this->showFullScreen();
}

void ViewCategoryDialog::showEvent(QShowEvent *)
{
    ui->verticalScrollBar->setMaximum(ui->scrollArea->verticalScrollBar()->maximum());
    ui->verticalScrollBar->setMinimum(ui->scrollArea->verticalScrollBar()->minimum());
}

void ViewCategoryDialog::resizeEvent(QResizeEvent *)
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
