#include <QtGui>
#include <QLabel>

#include "mainwindow.h"

FindFileDialog::FindFileDialog(QWidget *parent)
    : QDialog(parent)
{
    namedLabel = new QLabel(tr("&Named:"));
    namedLineEdit = new QLineEdit;
    namedLabel->setBuddy(namedLineEdit);

    lookInLabel = new QLabel(tr("&Look in:"));
    lookInLineEdit = new QLineEdit;
    lookInLabel->setBuddy(lookInLineEdit);

    subfoldersCheckBox = new QCheckBox(tr("Include subfolders"));

    QStringList labels;
    labels << tr("Name") << tr("In Folder") << tr("Size")
           << tr("Modified");

    tableWidget = new QTableWidget;
    tableWidget->setColumnCount(4);
    tableWidget->setHorizontalHeaderLabels(labels);

    messageLabel = new QLabel(tr("0 files found"));
    messageLabel->setFrameShape(QLabel::Panel);
    messageLabel->setFrameShadow(QLabel::Sunken);

    QVector<QString> iconleft = { "../resource/moveleft.png",  "../resource/2.png",  "../resource/th.png"};
    QVector<QString> iconright = { "../resource/moveright.png",  "../resource/2.png",  "../resource/th.png"};
    QVector<QString> iconup = { "../resource/moveup.png",  "../resource/2.png",  "../resource/th.png"};
    QVector<QString> icondown = { "../resource/movedown.png",  "../resource/2.png",  "../resource/th.png"};
    QVector<QString> iconhome = { "../resource/gohome.png",  "../resource/2.png",  "../resource/th.png"};

    leftButton = new iconButton(iconleft);
    rightButton = new iconButton(iconright);
    upButton = new iconButton(iconup);
    downButton = new iconButton(icondown);
    homeButton = new iconButton(iconhome);

    findButton = new QPushButton(tr("&Find"));
    stopButton = new QPushButton(tr("Stop"));
    closeButton = new QPushButton(tr("Close"));
    helpButton = new QPushButton(tr("Help"));

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->addWidget(namedLabel, 0, 0);
    mainLayout->addWidget(namedLineEdit, 0, 1);
    mainLayout->addWidget(lookInLabel, 1, 0);
    mainLayout->addWidget(lookInLineEdit, 1, 1);
    mainLayout->addWidget(subfoldersCheckBox, 2, 0, 1, 2);
    mainLayout->addWidget(tableWidget, 3, 0, 1, 2);

    QVBoxLayout *toolbar = new QVBoxLayout;

    toolbar->addWidget(leftButton);
    toolbar->addWidget(rightButton);
    toolbar->addWidget(upButton);
    toolbar->addWidget(downButton);
    toolbar->addWidget(homeButton);


    //toolbar->addStretch();
    //toolbar->addWidget(helpButton);
    //toolbar->addWidget(findButton);
    //toolbar->addWidget(stopButton);
    toolbar->addWidget(closeButton);
    
    QPixmap *pixmap = new QPixmap("../resource/logo.png");
    logoLabel = new QLabel();
    logoLabel->setText("GZLogo");
    logoLabel->setPixmap(*pixmap);
    logoLabel->setStyleSheet("background-color: rgb(9, 148, 220);");

    QVBoxLayout *leftLayout = new QVBoxLayout;
    leftLayout->addWidget(logoLabel);
    leftLayout->addLayout(mainLayout);

    leftLayout->addStretch();
    leftLayout->addWidget(messageLabel);
    
    QHBoxLayout *mainwindow = new QHBoxLayout;
    mainwindow->addLayout(leftLayout);
    mainwindow->addLayout(toolbar);
    setLayout(mainwindow);

    setFixedSize(1024, 768);
    setWindowFlags(Qt::FramelessWindowHint);

    setWindowTitle(tr("Find Files or Folders"));
}
