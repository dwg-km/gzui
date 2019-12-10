#ifndef 	_SETTING_H
#define		_SETTING_H

#include <string.h>
#include <QLabel>
#include <QLineEdit>

#include <QDebug>
#include <QGroupBox>
#include <QComboBox>
#include <QRadioButton>

#include "wave.h"
#include "cali.h"
#include "toolbar.h"
#include "UiTemplate.h"

#include "lineedit.h"

#include "command.h"
#include "ui_interface.h"
#include "APIDataInterface.hpp"

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
		dirBox = new QGroupBox(tr("双向"));
		radio_unidir = new QRadioButton(tr("单向"));
		radio_bidir = new QRadioButton(tr("双向"));

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
	}
	void LayoutPrintReverse(){
		revBox = new QGroupBox(tr("反向打印"));
		
	       	x_rev = new QRadioButton(tr("扫描"));
		y_rev = new QRadioButton(tr("步进"));

		QHBoxLayout * hbox = new QHBoxLayout;
		hbox->addWidget(x_rev);
		hbox->addWidget(y_rev);
		
		revBox->setLayout(hbox);
	}
	void LayoutPrintSpeed(){
		speedBox = new QGroupBox("Speed");
		speedLabel = new QLabel(tr("扫描速度"));
		speedComBox = new QComboBox();

		QHBoxLayout * hbox = new QHBoxLayout;

		speedComBox->addItem(tr("高速"));
		speedComBox->addItem(tr("中速"));
		speedComBox->addItem(tr("低速"));

		hbox->addWidget(speedLabel);
		hbox->addWidget(speedComBox);

		speedBox->setLayout(hbox);
	}
	void LoadPrintSpeed(){
	
	}

	void LayoutStrip(){
		stripBox = new QGroupBox(tr("彩条"));

		QGridLayout * stripLayout = new QGridLayout;

		typeLabel = new QLabel(tr("彩条位置"));
		typeBox = new QComboBox();
		widthLabel = new QLabel(tr("彩条宽度"));
		offsetLabel = new QLabel(tr("彩条间距"));
		widthLineEdit = new DoubleLineEdit;
		offsetLineEdit = new DoubleLineEdit;


		typeBox->addItem("无彩条");
		typeBox->addItem("左侧彩条");
		typeBox->addItem("右侧彩条");
		typeBox->addItem("双边彩条");

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
		featherBox = new QGroupBox(tr("羽化"));

		QGridLayout * featherLayout = new QGridLayout;

		hugeLabel = new QLabel(tr("强"));
		middleLabel = new QLabel(tr("中"));
		smallLabel = new QLabel(tr("弱"));
		fastLabel = new QLabel(tr("快速"));

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
		flashBox = new QGroupBox(tr("闪喷"));
		flashRadio = new QRadioButton(tr("空闲闪喷"));
		flashLabel = new QLabel(tr("闪喷周期"));
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
		if(radio_bidir->isChecked()){
			basesetting.Bidirection = 1;
		}
		//if(radio_x->isChecked()){
		//	basesetting.xReverse = 1;
		//}
		if(memcmp(&BaseSetting, &basesetting, sizeof(BASE_SETTING))){
			SetPrinterParam(UI_CMD::CMD_MODE_BASE, &basesetting);
		}
	}
private:

	FLASH Flash;
	STRIP Strip;
	FEATHER_CFG FeatherCfg;
	BASE_SETTING BaseSetting;

	QGroupBox * dirBox;
	QGroupBox * revBox;
	QRadioButton * radio_unidir;
	QRadioButton * radio_bidir;
	QRadioButton * x_rev;
	QRadioButton * y_rev;

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

		statusLabel->setText("Menu");

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

		connect(Tool->GetBackupButton(),SIGNAL(clicked()), this, SLOT(Backup()));
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
		AddInkWidget();
		AddMechineWidget();
		AddCleanWidget();
		AddUVWidget();
		AddAdvanceWidget();
		AddWarningWidget();

		Layout(widgetlist);
	}
	void AddBaseSettingWidget(){
		QWidget * widget = new BaseWidget;
		//connect(this, SIGNAL(clicked()), this, SLOT(close()));

		widgetlist->addTab(widget, "Base");
	}
	void AddMechineWidget(){
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Mechine");
	}
	void AddCleanWidget(){
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Clean");
	}
	void AddUVWidget(){
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "UV");
	}
	void AddAdvanceWidget(){
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Advance");
	}
	void AddWarningWidget(){
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Warning");
	}
	void AddInkWidget(){
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Ink");
	}

public slots:
	void Backup();
	void Update();
private:
	QTabWidget * widgetlist;

	CaliDialog * caliDialog;
	WaveDialog * waveDialog;
};

#endif
