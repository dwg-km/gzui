#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QTimer>
#include <QLabel>
#include <QDialog>
#include <QPainter>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTableWidget>
#include <QDialog>

#include "UiTemplate.h"
#include "menu.h"

//#include <QtDBus>
//#include "usbmanager.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

extern int udisk_thread();
class uDiskThread : public QThread {
public:
	uDiskThread(){
	}
protected:
	virtual void run(){
		udisk_thread();
	}
private:
};

class InkWidget : public QWidget {
public:
	InkWidget(QWidget *parent = NULL) : QWidget(parent), ColorNum(6)
	{
		GetPrinterParam(UI_CMD::CMD_MECHINE_INK_PUMP, &InkPump);
	}

	virtual void paintEvent(QPaintEvent *e);

private:
	INK_PUMP InkPump;
	const int ColorNum;
};
/*
void SplitSemicolon(char * s, QStringList & str)
{
	while(*s){
		char * p = s;
		while(*s && (*s != ';'))
			s++;
		*s++ = '\0';
		if(s != p){
			str.push_back(QString(p));
		}
		//while(*s && (*s == ';'))
		//	s++;
	}
}
*/
class mainDialog : public UiTemplate{
	Q_OBJECT
public:
	mainDialog(QWidget *parent = NULL)
	    : UiTemplate(parent)
	{
		LayoutWidget();

		Layout();
	}
	void LayoutToolBar(){
		toolLayout->addWidget(Tool->GetMenuButton());
		toolLayout->addWidget(Tool->GetPauseButton());
		toolLayout->addWidget(Tool->GetAbortButton());
		toolLayout->addWidget(Tool->GetPoweroffButton());
		toolLayout->addWidget(Tool->GetExitButton());

		Tool->GetAbortButton()->setDisabled();
		Tool->GetPauseButton()->setDisabled();

		connect(Tool->GetExitButton(), SIGNAL(clicked()), this, SLOT(close()));
		connect(Tool->GetPauseButton(), SIGNAL(clicked()), this, SLOT(Pause()));
		connect(Tool->GetAbortButton(), SIGNAL(clicked()), this, SLOT(Abort()));
		connect(Tool->GetPoweroffButton(), SIGNAL(clicked()), this, SLOT(PowerOff()));

		menuDialog = new UiSetting(this);
		connect(Tool->GetMenuButton(),SIGNAL(clicked()),menuDialog,SLOT(show()));
	}
	void LayoutWidget(){
		GetPrinterProperty(&Property);

		LayoutToolBar();
		LayoutMenuButtons();
		LayoutMoveButtons();
		LayoutSetting();

		inkWidget = new InkWidget;
		inkWidget->setFixedSize(450, 270);

		mainLayout->addWidget(toolBox, 0, 0, 1, 2);
		mainLayout->addWidget(moveBox, 0, 2, 1, 1);
		mainLayout->addWidget(setBox,  1, 0, 2, 1);
		mainLayout->addWidget(inkWidget,  1, 1, 2, 2);

		mainWidget->setLayout(mainLayout);
	}
	void LayoutMenuButtons(){
		toolBox = new QGroupBox;
		QGridLayout *menuLayout = new QGridLayout;
		
		QLabel * homeLabel = new QLabel(tr("打印作业"));
		QLabel * managerLabel = new QLabel(tr("测量纸宽"));
		QLabel * originLabel = new QLabel(tr("设为原点"));

		QLabel * nozzleLabel = new QLabel(tr("喷嘴检查"));
		QLabel * cleanLabel = new QLabel(tr("喷头维护"));
		QLabel * flashLabel = new QLabel(tr("喷嘴闪喷"));

		menuLayout->addWidget(Tool->GetPrintButton(),		0, 0, 2, 1);
		menuLayout->addWidget(Tool->GetMeasureButton(),		0, 1, 2, 1);
		menuLayout->addWidget(Tool->GetOriginButton(),		0, 2, 2, 1);

		menuLayout->addWidget(homeLabel,			2, 0, 1, 1);
		menuLayout->addWidget(managerLabel,			2, 1, 1, 1);
		menuLayout->addWidget(originLabel,			2, 2, 1, 1);

		menuLayout->addWidget(Tool->GetNozzleButton(),		3, 0, 2, 1);
		menuLayout->addWidget(Tool->GetCleanButton(),		3, 1, 2, 1);
		menuLayout->addWidget(Tool->GetFlashButton(),		3, 2, 2, 1);

		menuLayout->addWidget(nozzleLabel,			5, 0, 1, 1);
		menuLayout->addWidget(cleanLabel,			5, 1, 1, 1);
		menuLayout->addWidget(flashLabel,			5, 2, 1, 1);

		toolBox->setLayout(menuLayout);
		connect(Tool->GetPrintButton(),SIGNAL(clicked()), this, SLOT(Print()));
		connect(Tool->GetNozzleButton(),SIGNAL(clicked()), this, SLOT(PrintNozzleCheck()));
	}
	void LayoutMoveButtons(){
		moveBox = new QGroupBox;
		QGridLayout *moveLayout = new QGridLayout;

		moveBox->setFixedSize(300, 300);
		moveLayout->addWidget(Tool->GetLeftButton(),   1, 0);
		moveLayout->addWidget(Tool->GetGoHomeButton(), 1, 1);
		moveLayout->addWidget(Tool->GetRightButton(),  1, 2);

		moveLayout->addWidget(Tool->GetUpButton(),     0, 1);
		moveLayout->addWidget(Tool->GetDownButton(),   2, 1);

		moveBox->setLayout(moveLayout);
	}
	void LoadOrigin(){
		GetPrinterParam(UI_CMD::CMD_MODE_ORIGIN, &Origin);

		if(Origin.GetMode < 2){
			orgComBox->setCurrentIndex(Origin.GetMode);
		}
		typeLabelEdit->setText(QString::number(Origin.Coord));
	}
	void SaveOrigin(){
		//X像原点设置
		ORIGIN_SET origin = Origin;
		//memset(&origin, 0, sizeof(ORIGIN_SET));
		origin.Coord = typeLabelEdit->text().toInt();
		origin.GetMode = orgComBox->currentIndex();
		if(memcmp(&Origin, &origin, sizeof(ORIGIN_SET))){
			SetPrinterParam(UI_CMD::CMD_MODE_ORIGIN, &origin);
		}
	}
	void LayoutSetting(){
		setBox = new QGroupBox;
		QGridLayout *setLayout = new QGridLayout;

		QLabel *mediaLabel = new QLabel(tr("打印材料"));
		QLabel *modelLabel = new QLabel(tr("生产模式"));
		QLabel *stepLabel = new QLabel(tr("进步量"));
		QLabel *bidLabel = new QLabel(tr("双向值"));
		QLabel *cycleLabel = new QLabel(tr("循环压力"));
		QLabel *pumpLabel = new QLabel(tr("供墨压力"));

		IntLineEdit * stepLineEdit = new IntLineEdit;
		IntLineEdit * bidLineEdit = new IntLineEdit;
		cycleLineEdit = new QLineEdit;
		pumpLineEdit = new QLineEdit;

		cycleLineEdit->setEnabled(false);
		pumpLineEdit->setEnabled(false);

		char buf[256];
		QComboBox * mediaBox = new QComboBox();
		if(LoadMediaList(buf) > 0){
			QString str = buf;
        		QStringList mediaList = str.split(';');
			mediaBox->addItems(mediaList);
		}
		connect(mediaBox, SIGNAL(currentTextChanged(QString&)), 
				this, SLOT(mediaChanged(QString&)));

		QComboBox * modelBox = new QComboBox();
		if(LoadProductModels(buf) > 0){
			QString str = buf;
        		QStringList modelList = str.split(';');
			modelBox->addItems(modelList);
		}
		connect(modelBox, SIGNAL(currentTextChanged(QString&)), 
				this, SLOT(modelChanged(QString&)));
		/*
		*/
		orgBox = new QGroupBox(tr("打印原点"));
		orgLabel = new QLabel(tr("获取原点"));
		orgComBox = new QComboBox();
		getorgLabel = new QLabel(tr("打印原点"));
		typeLabelEdit = new IntLineEdit;
	
		orgComBox->addItem(tr("手动"));
		orgComBox->addItem(tr("自动"));
		connect(orgBox, SIGNAL(currentIndexChanged(int)), 
				this, SLOT(orginChanged(int)));
		
		LoadOrigin();

		setLayout->addWidget(mediaLabel,	0, 0);
		setLayout->addWidget(mediaBox,		0, 1);
		setLayout->addWidget(modelLabel,	0, 2);
		setLayout->addWidget(modelBox,		0, 3);

		setLayout->addWidget(orgLabel,		1, 0);
		setLayout->addWidget(orgComBox,		1, 1);
		setLayout->addWidget(getorgLabel,	1, 2);
		setLayout->addWidget(typeLabelEdit,	1, 3);

		setLayout->addWidget(stepLabel,		2, 0);
		setLayout->addWidget(stepLineEdit,	2, 1);
		setLayout->addWidget(bidLabel,		2, 2);
		setLayout->addWidget(bidLineEdit,	2, 3);

		setLayout->addWidget(cycleLabel,	3, 0);
		setLayout->addWidget(cycleLineEdit,	3, 1);
		setLayout->addWidget(pumpLabel,		3, 2);
		setLayout->addWidget(pumpLineEdit,	3, 3);
		
		//QGridLayout * orgLayout = new QGridLayout;
		//orgBox->setLayout(orgLayout);
		//grid->addWidget(orgBox,		2, 1, 1, 1);
/*
		QDBusConnection::systemBus().connect(
				"org.freedesktop.Hal",
				"/org/freedesktop/Hal/Manager",
				"org.freedesktop.Hal.Manager",
				"DeviceAdded",
				this,
				SLOT(deviceAdded(QString )));

		QDBusConnection::systemBus().connect(
				"org.freedesktop.Hal",
				"/org/freedesktop/Hal/Manager",
				"org.freedesktop.Hal.Manager",
				"DeviceRemoved",
				this,
				SLOT(deviceRemoved(QString )));
		usbmanager * usb = new usbmanager(this);
*/
		uDiskThread * usb = new uDiskThread();
		usb->start();

		QTimer * StatusTimer = new QTimer(this);
		connect(StatusTimer, SIGNAL(timeout()), this,  SLOT(ProcessPrintStatus()));
		StatusTimer->start(1000);

		ProcessPrintStatus();

		setBox->setLayout(setLayout);
	}
public slots:
	void mediaChanged(QString&);
	void modelChanged(QString&);
	void originChanged(int);

	void Print();
	void Pause();
	void Abort();
	void ProcessPrintStatus();
	void PrintNozzleCheck();

	void PowerOff();
	void deviceAdded(QString);
	void deviceRemoved(QString);
signals:
	void ready();
	void pause();
	void moving();
	void error(QString msg);
	void printing(QString msg);
private:
	InkWidget * inkWidget;
	UiSetting *menuDialog;

	QGroupBox * toolBox;
	QGroupBox * moveBox;
	QGroupBox * setBox;
	QGroupBox * orgBox;

	QLabel *orgLabel;
	QLabel *getorgLabel;
	IntLineEdit *typeLabelEdit;
	QComboBox * orgComBox;

	INK_PUMP InkPump;
	QLineEdit * cycleLineEdit;
	QLineEdit * pumpLineEdit;

	ORIGIN_SET Origin;
	struct MECHAINE Property;
};

#endif
