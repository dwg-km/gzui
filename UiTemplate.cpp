#include <QtGui>
#include <QLabel>

#include "UiTemplate.h"

UiTemplate::UiTemplate(QWidget *parent) : 
	QWidget(parent)
{
	Tool = new ToolBar();

	QPixmap *pixmap = new QPixmap("../resources/logo.png");

	logoLabel = new QLabel();
	logoLabel->setPixmap(*pixmap);
	logoLabel->setFixedWidth(110);
	logoLabel->setStyleSheet("QLabel {background-color: rgb(9, 148, 220); }");

	QFont font;
	font.setPointSize(21);
	statusLabel = new QLabel();
	statusLabel->setFont(font);
	statusLabel->setStyleSheet("QLabel {background-color: rgb(9, 148, 220); }");
	statusLabel->setAlignment(Qt::AlignVCenter | Qt::AlignCenter);

	messageLabel = new QLabel(tr("0 files found"));
	messageLabel->setFrameShape(QLabel::Panel);
	messageLabel->setFrameShadow(QLabel::Sunken);

	mainWidget = new QWidget;

	logoLayout = new QHBoxLayout;
	toolLayout = new QVBoxLayout;
	leftLayout = new QVBoxLayout;
	mainLayout = new QGridLayout;
	mainwindow = new QHBoxLayout;

	setFixedSize(1024, 768);
	setWindowFlags(Qt::FramelessWindowHint);
}

void UiTemplate::Layout(QTabWidget * widgets)
{
	logoLayout->setSpacing(0);
	logoLayout->addWidget(logoLabel);
	logoLayout->addWidget(statusLabel);

	leftLayout->addLayout(logoLayout);

	if(widgets == NULL){
		leftLayout->addWidget(mainWidget);
	}else{
		leftLayout->addWidget(widgets);
	}
	
	leftLayout->addStretch();
	leftLayout->addWidget(messageLabel);
	
	mainwindow->addLayout(leftLayout);
	mainwindow->addLayout(toolLayout);
	setLayout(mainwindow);
	
	setWindowTitle(tr("Find Files or Folders"));
}


