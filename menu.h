#ifndef 	_SETTING_H
#define		_SETTING_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <QLabel>
#include <QLineEdit>
#include <QButtonGroup>

#include <QDebug>
#include "qdebug.h"
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>
#include <QFileDialog>

#include "wave.h"
#include "cali.h"
#include "toolbar.h"
#include "motion.h"
#include "UiTemplate.h"
#include "iconbutton.h"
#include "ip_cfg.h"

#include "lineedit.h"

#include "command.h"
#include "ui_interface.h"
#include "APIDataInterface.hpp"

using namespace std;


class GroupBox : public QGroupBox{
	Q_OBJECT
public:
	GroupBox(QString name, QWidget *parent = NULL) : 
		QGroupBox(name), 
		Name(name),
		Dirty(0)
	{	
	}
	/*
	virtual void changeEvent(QEvent * ev){
		Dirty = 1;
	}
	*/
	int IsDirty(){
		return Dirty;
	}
public slots:
	void setDirty(const QString& value){
		Dirty = 1;
		qDebug() << Name << "is dirty" << value;
	}
private:
	QString Name;
	int Dirty;
};

class BaseWidget : public QWidget
{
	Q_OBJECT
public:
	void LayoutPrintDir(){
        dirBox = new QGroupBox(tr("Bidirection"));
        radio_unidir = new QRadioButton(tr("Unidirection"));
        radio_bidir = new QRadioButton(tr("Bidirection"));

		QHBoxLayout * hbox = new QHBoxLayout;
		hbox->addWidget(radio_unidir);
		hbox->addWidget(radio_bidir);
		
		dirBox->setLayout(hbox);
	}
	void LoadPrintDir(){
		GetPrinterParam(UI_CMD::CMD_MODE_BASE, &BaseSetting);
	
		if(BaseSetting.Bidirection){
			radio_bidir->setChecked(true);
		}else{
			radio_unidir->setChecked(true);
		}
		if(BaseSetting.xReverse){
			xCheckBox->setCheckState(Qt::Checked);
		}
		if(BaseSetting.yReverse){
			yCheckBox->setCheckState(Qt::Checked);
		}
	}
	void LayoutPrintReverse(){
        revBox = new QGroupBox(tr("Reverse Print"));
		
            xCheckBox = new QCheckBox(tr("X Dir"));
        yCheckBox = new QCheckBox(tr("Y Dir"));

		QHBoxLayout * hbox = new QHBoxLayout;
		hbox->addWidget(xCheckBox);
		hbox->addWidget(yCheckBox);
		
		revBox->setLayout(hbox);
	}
	void LayoutPrintSpeed(){
        speedBox = new QGroupBox(tr("Speed"));
        speedLabel = new QLabel(tr("Scanning Speed"));
		speedComBox = new QComboBox();

		QGridLayout * layout = new QGridLayout;

        speedComBox->addItem(tr("High Speed"));
        speedComBox->addItem(tr("Medium Speed"));
        speedComBox->addItem(tr("Low Speed"));

		layout->addWidget(speedLabel, 0, 0);
		layout->addWidget(speedComBox, 0, 1);

		speedBox->setLayout(layout);
	}
	void LoadPrintSpeed(){
		GetPrinterParam(UI_CMD::CMD_MODE_SPEED, &PrintSpeed);
		speedComBox->setCurrentIndex(PrintSpeed.CurSpeed);
	
	}

	void LayoutStrip(){
        stripBox = new QGroupBox(tr("Color Bar"));

		QGridLayout * stripLayout = new QGridLayout;

        typeLabel = new QLabel(tr("Position"));
		typeBox = new QComboBox();
        widthLabel = new QLabel(tr("Width"));
        offsetLabel = new QLabel(tr("Interval"));
		widthLineEdit = new DoubleLineEdit;
		offsetLineEdit = new DoubleLineEdit;


        typeBox->addItem(tr("None"));
        typeBox->addItem(tr("Left Side"));
        typeBox->addItem(tr("Right Side"));
        typeBox->addItem(tr("Both Side"));

		stripLayout->addWidget(typeLabel, 0, 0);
		stripLayout->addWidget(typeBox, 0, 1);
		stripLayout->addWidget(widthLabel, 1, 0);
		stripLayout->addWidget(widthLineEdit, 1, 1);
		stripLayout->addWidget(offsetLabel, 2, 0);
		stripLayout->addWidget(offsetLineEdit, 2, 1);

		stripBox->setLayout(stripLayout);
	}
	void LoadStrip(){
		GetPrinterParam(UI_CMD::CMD_MAINTAIN_STRIP, &Strip);

		typeBox->setCurrentIndex(Strip.Position);

		widthLineEdit->setText(QString::number(Strip.Width));
		offsetLineEdit->setText(QString::number(Strip.Offset));
		}
	void LayoutFeatherCfg(){
        featherBox = new QGroupBox(tr("Feather"));

		QGridLayout * featherLayout = new QGridLayout;

        hugeLabel = new QLabel(tr("Powerful"));
        middleLabel = new QLabel(tr("Medium"));
        smallLabel = new QLabel(tr("Low"));
        fastLabel = new QLabel(tr("Fast"));

		hugeLabelEdit = new IntLineEdit;
		middleLabelEdit = new IntLineEdit;
		smallLabelEdit = new IntLineEdit;
		fastLabelEdit = new IntLineEdit;

		featherLayout->addWidget(hugeLabel, 0, 0);
		featherLayout->addWidget(hugeLabelEdit, 0, 1);

		featherLayout->addWidget(middleLabel, 1, 0);
		featherLayout->addWidget(middleLabelEdit, 1, 1);

		featherLayout->addWidget(smallLabel, 2, 0);
		featherLayout->addWidget(smallLabelEdit, 2, 1);

		featherLayout->addWidget(fastLabel, 3, 0);
		featherLayout->addWidget(fastLabelEdit, 3, 1);

		featherBox->setLayout(featherLayout);
	}
	void LoadFeather(){
		GetPrinterParam(UI_CMD::CMD_MODE_FEATHER, &FeatherCfg);
	
		hugeLabelEdit->setText(QString::number(FeatherCfg.Percent[All]));
		middleLabelEdit->setText(QString::number(FeatherCfg.Percent[Middle]));
		smallLabelEdit->setText(QString::number(FeatherCfg.Percent[Low]));
		fastLabelEdit->setText(QString::number(FeatherCfg.Percent[Quick]));
	}
	void LayoutFlash(){
        flashBox = new QGroupBox(tr("Flash"));
        flashRadio = new QRadioButton(tr("Idle Flash"));
        flashLabel = new QLabel(tr("Flash Period"));
		flashLineEdit = new IntLineEdit;
	
		QGridLayout * flashLayout = new QGridLayout;
		flashLayout->addWidget(flashRadio, 0, 0);
		flashLayout->addWidget(flashLabel, 1, 0);
		flashLayout->addWidget(flashLineEdit, 1, 1);

		flashBox->setLayout(flashLayout);
	}
	void LoadFlash(){
		GetPrinterParam(UI_CMD::CMD_MAINTAIN_FLASH, &Flash);
	
		if(Flash.Open){
			flashRadio->setChecked(true);
		}
		flashLineEdit->setText(QString::number(Flash.Period));
	}

	BaseWidget(QWidget *parent = NULL) : QWidget(parent)
	{
		Dirty = 0;

		LayoutPrintDir();
		LayoutPrintReverse();
		LayoutStrip();
		LayoutPrintSpeed();
		LayoutFeatherCfg();
		LayoutFlash();

		grid = new QGridLayout;

		grid->addWidget(dirBox,		0, 0, 1, 1);
		grid->addWidget(revBox,		1, 0, 1, 1);

		grid->addWidget(speedBox,	0, 1, 1, 1);
		grid->addWidget(flashBox,	1, 1, 1, 1);

		grid->addWidget(stripBox,	0, 2, 2, 1);

		//grid->addWidget(featherBox,	0, 2, 2, 1);


		setLayout(grid);
	}

	int IsDirty(){
		return Dirty;
	}
	virtual void showEvent(QShowEvent * event){
		event = event;

		LoadPrintDir();
		LoadPrintSpeed();
		LoadFlash();
		LoadStrip();
		LoadFeather();
	}
	virtual void hideEvent(QHideEvent * event){
		event = event;
		STRIP strip = Strip;
		//memset(&strip, 0, sizeof(STRIP));
		strip.Position = typeBox->currentIndex();
		strip.Width    = widthLineEdit->text().toFloat();
		strip.Offset   = offsetLineEdit->text().toFloat();
		if(memcmp(&Strip, &strip, sizeof(STRIP))){
			SetPrinterParam(UI_CMD::CMD_MAINTAIN_STRIP, &strip);
		}


		//羽化设置
		FEATHER_CFG cfg = FeatherCfg;
		//memset(&cfg, 0, sizeof(FEATHER_CFG));
		cfg.Percent[Quick] = fastLabelEdit->text().toInt();
		cfg.Percent[Low] = smallLabelEdit->text().toInt();
		cfg.Percent[Middle] = middleLabelEdit->text().toInt();
		cfg.Percent[All] = hugeLabelEdit->text().toInt();
		if(memcmp(&FeatherCfg, &cfg, sizeof(FEATHER_CFG))){
			SetPrinterParam(UI_CMD::CMD_MODE_FEATHER, &cfg);
		}

		//待机闪喷
		FLASH flash = Flash;
		//memset(&flash, 0, sizeof(FLASH));
		flash.Open = flashRadio->isChecked();
		flash.Period = flashLineEdit->text().toInt();
		if(memcmp(&flash, &Flash, sizeof(FLASH))){
			SetPrinterParam(UI_CMD::CMD_MAINTAIN_FLASH, &flash);
		}

		BASE_SETTING basesetting = BaseSetting;
		//memset(&basesetting, 0, sizeof(BASE_SETTING));
		basesetting.Bidirection = radio_bidir->isChecked() ? 1 : 0;
		basesetting.xReverse = xCheckBox->isChecked() ? 1 : 0;
		basesetting.yReverse = yCheckBox->isChecked() ? 1 : 0;
		if(memcmp(&BaseSetting, &basesetting, sizeof(BASE_SETTING))){
			SetPrinterParam(UI_CMD::CMD_MODE_BASE, &basesetting);
		}

		//save print speed
		PRINT_MOVE speed = PrintSpeed;
		speed.CurSpeed = speedComBox->currentIndex();
		if(memcmp(&speed, &PrintSpeed, sizeof(PRINT_MOVE))){
			SetPrinterParam(UI_CMD::CMD_MODE_SPEED, &speed);
		}
	}
private:

	PRINT_MOVE PrintSpeed;
	FLASH Flash;
	STRIP Strip;
	FEATHER_CFG FeatherCfg;
	BASE_SETTING BaseSetting;

	QGroupBox * dirBox;
	QGroupBox * revBox;
	QRadioButton * radio_unidir;
	QRadioButton * radio_bidir;
	QCheckBox * xCheckBox;
	QCheckBox * yCheckBox;

	QGroupBox * speedBox;
	QComboBox * speedComBox;
	QLabel *speedLabel;

	QGroupBox * stripBox;
	QLabel *typeLabel;
	QLabel *widthLabel;
	QLabel *offsetLabel;
	QComboBox * typeBox;
	DoubleLineEdit *widthLineEdit;
	DoubleLineEdit *offsetLineEdit;

	QGroupBox * featherBox;
	QLabel *hugeLabel;
	QLabel *middleLabel;
	QLabel *smallLabel;
	QLabel *fastLabel;
	IntLineEdit *hugeLabelEdit;
	IntLineEdit *middleLabelEdit;
	IntLineEdit *smallLabelEdit;
	IntLineEdit *fastLabelEdit;

	QGroupBox * flashBox;
	QRadioButton * flashRadio;
	QLabel *flashLabel;
	IntLineEdit *flashLineEdit;

	QGridLayout * grid;

	int Dirty;
};



class UiSetting : public UiTemplate
{
	Q_OBJECT
public:
	UiSetting(QWidget *parent = NULL)
	    : UiTemplate()
	{
		caliDialog = new CaliDialog();
		waveDialog = new WaveDialog();
		motionDialog = new MotionDialog();

        statusLabel->setText(tr("Menu"));

		//layout tool bar
		toolLayout->addWidget(Tool->GetHomeButton());
		toolLayout->addWidget(Tool->GetCaliButton());
		toolLayout->addWidget(Tool->GetWaveButton());
		toolLayout->addWidget(Tool->GetBackupButton());
		toolLayout->addWidget(Tool->GetUpdateButton());

		//toolLayout->addWidget(Tool->GetLeftButton());
		//toolLayout->addWidget(Tool->GetRightButton());
		//toolLayout->addWidget(Tool->GetHomeButton());
		//toolLayout->addWidget(Tool->GetUpButton());
		//toolLayout->addWidget(Tool->GetDownButton());
		
		connect(Tool->GetHomeButton(), SIGNAL(clicked()), this, SLOT(close()));

		connect(Tool->GetCaliButton(), SIGNAL(clicked()), caliDialog, SLOT(show()));
		connect(Tool->GetWaveButton(), SIGNAL(clicked()), waveDialog, SLOT(show()));
		connect(Tool->GetBackupButton(),SIGNAL(clicked()), motionDialog, SLOT(show()));

		//connect(Tool->GetBackupButton(),SIGNAL(clicked()), this, SLOT(Backup()));
		connect(Tool->GetUpdateButton(),SIGNAL(clicked()), this, SLOT(Update()));

		connect(parent, SIGNAL(ready()), caliDialog, SLOT(SetReady()));
		connect(parent, SIGNAL(pause()), caliDialog, SLOT(SetPause()));
		connect(parent, SIGNAL(moving()), caliDialog, SLOT(SetMoving()));
		connect(parent, SIGNAL(error(QString)), caliDialog, SLOT(SetError(QString)));
		connect(parent, SIGNAL(printing(QString)), caliDialog, SLOT(SetPrinting(QString)));

		/*
		int x = 0;
		int y = 2;
		mainLayout->addWidget(Tool->GetBasesettingButton(), y, x++);
		mainLayout->addWidget(Tool->GetMechineButton(), y, x++);
		mainLayout->addWidget(Tool->GetManagerButton(), y, x++);

		x = 0;
		y = 3;
		mainLayout->addWidget(Tool->GetInkButton(), y, x++);
		mainLayout->addWidget(Tool->GetUvButton(), y, x++);
		mainLayout->addWidget(Tool->GetAdvanceButton(), y, x++);
		mainLayout->addWidget(Tool->GetCleanButton(), y, x++);

		mainWidget->setLayout(mainLayout);
		*/

		widgetlist =  new QTabWidget;
		
        AddBaseSettingWidget();
		//AddInkWidget();
        //AddMechineWidget();
		//AddCleanWidget();
		//AddUVWidget();
		//AddAdvanceWidget();
		//AddWarningWidget();		
        AddSetNetworkWidget();
        AddVersionWidget();

		Layout(widgetlist);
	}
	void AddBaseSettingWidget(){
		QWidget * widget = new BaseWidget;
		//connect(this, SIGNAL(clicked()), this, SLOT(close()));

        widgetlist->addTab(widget, tr("Base"));
	}
	void AddMechineWidget(){
		QWidget * widget = new QWidget;
        widgetlist->addTab(widget, tr("Mechine"));
	}
	void AddCleanWidget(){
		QWidget * widget = new QWidget;
        widgetlist->addTab(widget, tr("Clean"));
	}
	void AddUVWidget(){
		QWidget * widget = new QWidget;
        widgetlist->addTab(widget, tr("UV"));
	}
	void AddAdvanceWidget(){
		QWidget * widget = new QWidget;
        widgetlist->addTab(widget, tr("Advance"));
	}
	void AddWarningWidget(){
		QWidget * widget = new QWidget;
        widgetlist->addTab(widget, tr("Warning"));
	}
	void AddInkWidget(){
		QWidget * widget = new QWidget;
        widgetlist->addTab(widget, tr("Ink"));
	}
	void AddVersionWidget(){
		QWidget * widget = new QWidget;
        widgetlist->addTab(widget, tr("version"));

		QLabel * version = new QLabel(widget);
		version->setWordWrap(true);

		char pm_info[128];
		GetPmVersion(pm_info);

		QString info;
		info.sprintf("UI INFO\nversion: %s\nbuild date: %s\nbuild time: %s\nsignature: %s\n\n%s",
			UI_VERSION, __DATE__, __TIME__, GIT_VERSION, pm_info);
		
		version->setText(info);
	}

    void AddSetNetworkWidget()
    {
        NetWidget = new NetworkWidget;
        widgetlist->addTab(NetWidget, tr("network"));

        connect(widgetlist, SIGNAL(currentChanged(int)), this, SLOT(TabGetOpen(int)));
    }

    bool CopyTheFile(QString FilePath, QString BuildPath);

public slots:
	void Backup();
	void Update();

    void TabGetOpen(int TabIndex)
    {
        QString title = widgetlist->tabText(TabIndex);
         if(title == tr("network"))
        {
            NetWidget->TabOpen();
        }
    }

private:
	QTabWidget * widgetlist;

    NetworkWidget * NetWidget;
    int m_NetWidgetIndex = -1;

	CaliDialog * caliDialog;
	WaveDialog * waveDialog;
	MotionDialog * motionDialog;
};


#endif
