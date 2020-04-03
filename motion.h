#ifndef MOTION_H
#define	MOTION_H

#include <QLabel>
#include <QMessageBox>
#include <QComboBox>
#include <QGridLayout>
#include <QRadioButton>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include  <QDebug>
#include <QPainter>
#include "UiTemplate.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"
#include "touchkey/lineedit.h"

/*
class MoterGroupBox : public QGroupBox{
public:
	MoterGroupBox(QString name, GPIO& dir, GPIO& pulse, QWidget *parent = NULL) :
		QGroupBox(name, parent),
		Dir(dir),
		Pulse(pulse)
	{
		QGridLayout * Layout = new QGridLayout;
	
		dirLabel = new QLabel("Dir");
		dirWidget = new GpioWidget(dir);

		pulseLabel = new QLabel("Pulse");
		pulseWidget = new GpioWidget(pulse);

		Layout->addWidget(dirLabel,	0, 0);
		Layout->addWidget(dirWidget,	0, 1);
		Layout->addWidget(pulseLabel,	1, 0);
		Layout->addWidget(pulseWidget,	1, 1);

		setLayout(Layout);
	}

private:
	GPIO Dir;
	GPIO Pulse;

	QLabel * dirLabel;
	QLabel * pulseLabel;
	GpioWidget * dirWidget;
	GpioWidget * pulseWidget;
};

*/
class MoveDbgGroupBox : public QGroupBox{
	Q_OBJECT 
public:
	MoveDbgGroupBox(QString name, QWidget *parent = NULL) : 
		QGroupBox(name, parent)
	{

		QGridLayout * dbgLayout = new QGridLayout;
		pulsePushButton = new QPushButton("移动固定(脉冲)");
	        pulseLineEdit = new IntLineEdit;

	        movePushButton = new QPushButton("移动到(mm)");
	        moveLineEdit = new IntLineEdit;

	        posPushButton = new QPushButton("位置");
		posLineEdit = new QLineEdit;
		posLineEdit->setEnabled(false);
		
		gearPushButton = new QPushButton("齿轮比");
		resetPushButton = new QPushButton("reset");
		gearLineEdit = new IntLineEdit;

		dbgLayout->addWidget(pulsePushButton,	1, 0);
		dbgLayout->addWidget(pulseLineEdit,	1, 1, 1, 4);
		dbgLayout->addWidget(movePushButton,	2, 0);
		dbgLayout->addWidget(moveLineEdit,	2, 1, 1, 4);
		dbgLayout->addWidget(posPushButton,	3, 0);
		dbgLayout->addWidget(posLineEdit,	3, 1, 1, 4);
		dbgLayout->addWidget(gearPushButton,	4, 0);
		dbgLayout->addWidget(gearLineEdit,	4, 1);
		dbgLayout->addWidget(resetPushButton,	4, 2);

		setLayout(dbgLayout);
	}
	QPushButton* GetPosPushButton(){
		return posPushButton;
	}
        QLineEdit* GetPosLineEdit(){
		return posLineEdit;
	}
	QPushButton* GetPulsePushButton(){
		return pulsePushButton;
	}
 	QPushButton * GetMovePushButton(){
		return movePushButton;
	}
	IntLineEdit* GetPulseLineEdit(){
		return pulseLineEdit;
	}
	IntLineEdit* GetMoveLineEdit(){
		return moveLineEdit;
	}
/*
        QComboBox * GetSpeedBox(){
		return speedBox;
	}
	QRadioButton * GetRadioButton(){
		return dir_positive;
	}
*/
public:
	QPushButton * posPushButton; 
	QLineEdit * posLineEdit;

	QPushButton * movePushButton; 
	IntLineEdit * moveLineEdit;

	IntLineEdit * pulseLineEdit;
        QPushButton * pulsePushButton;
	
	QPushButton * gearPushButton;
	QPushButton * resetPushButton;
	IntLineEdit * gearLineEdit;
};


class MoterParamGroupBox : public QGroupBox{
public:
	MoterParamGroupBox(QString name, QWidget *parent = NULL) : 
		QGroupBox(name, parent)
	{
		QLabel * highLabel = new QLabel("高速");
		QLabel * midLabel = new QLabel("中速");
		QLabel * lowLabel = new QLabel("低速");

		QLabel * speedLabel = new QLabel("运动速度(mm/s)");
		QLabel * accLabel = new QLabel("加速距离(mm)");

		highspeedLineEdit = new IntLineEdit;
		highaccLineEdit = new IntLineEdit;
		midspeedLineEdit = new IntLineEdit;
		midaccLineEdit = new IntLineEdit;
		lowspeedLineEdit = new IntLineEdit;
		lowaccLineEdit = new IntLineEdit;

		QGridLayout * Layout = new QGridLayout;

		int y = 0;
		Layout->addWidget(speedLabel, 		y, 1);
		Layout->addWidget(accLabel,	 	y, 2);

		y++;
		Layout->addWidget(highLabel, 		y, 0);
		Layout->addWidget(highspeedLineEdit, 	y, 1);
		Layout->addWidget(highaccLineEdit, 	y, 2);

		y++;
		Layout->addWidget(midLabel, 		y, 0);
		Layout->addWidget(midspeedLineEdit, 	y, 1);
		Layout->addWidget(midaccLineEdit, 	y, 2);

		y++;
		Layout->addWidget(lowLabel, 		y, 0);
		Layout->addWidget(lowspeedLineEdit, 	y, 1);
		Layout->addWidget(lowaccLineEdit, 	y, 2);

		//Layout->addWidget(accLabel, 		3, 0);
		//Layout->addWidget(accLineEdit, 		3, 1);
		//Layout->addWidget(subdivLabel, 		4, 0);
		//Layout->addWidget(subdivLineEdit, 	4, 1);
		setLayout(Layout);
	}

public:
	IntLineEdit * highspeedLineEdit;
	IntLineEdit * midspeedLineEdit;
	IntLineEdit * lowspeedLineEdit;

	IntLineEdit * highaccLineEdit;
	IntLineEdit * midaccLineEdit;
	IntLineEdit * lowaccLineEdit;
};

class MotionDialog : public UiTemplate
{
	Q_OBJECT 
public:
	MotionDialog(QWidget *parent = NULL) : UiTemplate(parent)
	{
		toolLayout->addWidget(Tool->GetExitButton());
		toolLayout->addWidget(Tool->GetSaveButton());
		toolLayout->addWidget(Tool->GetUpdateButton());
		connect(Tool->GetExitButton(), SIGNAL(clicked()), this, SLOT(close()));

		statusLabel->setText("机械设置");

		GetPrinterProperty(&property);

		QLabel *axisLabel = new QLabel(tr("移动轴"));
		axisLabel->setFixedWidth(72);
		//axisLabel->setFixedWidth(axisLabel->width());
		axisBox = new QComboBox();
		axisBox->addItem("x轴");
		axisBox->addItem("y轴");
		axisBox->addItem("z轴");
		axisBox->addItem("4轴");

		QLabel *speedLabel = new QLabel(tr("移动速度"));
		speedBox = new QComboBox();
		for(int i = 0; i < 4; i++){
			QString s = "Speed";
			speedBox->addItem(s);
		}
		dir_positive = new QRadioButton(tr("正向"));
		dir_reverse  = new QRadioButton(tr("反向"));
		dir_positive->setChecked(true);

		QGroupBox * paramGroupBox = new QGroupBox("");
		QGridLayout *paramLayout = new QGridLayout();
		paramLayout->addWidget(axisLabel,	0, 0);
		paramLayout->addWidget(axisBox,		0, 1);
		paramLayout->addWidget(speedLabel, 	1, 0);
		paramLayout->addWidget(speedBox, 	1, 1);
		paramLayout->addWidget(dir_positive, 	2, 0);
		paramLayout->addWidget(dir_reverse, 	2, 1);
		paramGroupBox->setLayout(paramLayout);	

		rasterPushButton = new QPushButton(tr("分辨率"));

		resetPushButton = new QPushButton("reset");
		rasterLineEdit = new IntLineEdit;
		rasterGroup = new QGroupBox("光栅/编码器");

		QGridLayout *rasterLayout = new QGridLayout();
		rasterLayout->addWidget(rasterPushButton, 0, 0);
		rasterLayout->addWidget(rasterLineEdit, 0, 1);
		rasterLayout->addWidget(resetPushButton, 0, 2);
		rasterGroup->setLayout(rasterLayout);
		
		moter = new MoveDbgGroupBox("运动调试");
		param = new MoterParamGroupBox("运动参数");

		connect(moter->movePushButton, SIGNAL(clicked()), this, SLOT(MoveTo()));
		connect(moter->pulsePushButton, SIGNAL(clicked()), this, SLOT(MovePulse()));
		connect(moter->posPushButton, SIGNAL(clicked()), this, SLOT(ReadPos()));
		connect(moter->gearPushButton, SIGNAL(clicked()), this, SLOT(GetGearRatio()));
		connect(moter->resetPushButton, SIGNAL(clicked()), this, SLOT(SetGearRatio()));

		connect(axisBox, SIGNAL(currentIndexChanged(int)), this, SLOT(AxisChanged(int)));
		connect(rasterPushButton, SIGNAL(clicked()), this, SLOT(ReadRaster()));
		connect(resetPushButton, SIGNAL(clicked()), this, SLOT(SetRaster()));

		moter->pulsePushButton->setVisible(false);
		moter->pulseLineEdit->setVisible(false);
		dir_positive->setVisible(false);
		dir_reverse->setVisible(false);
		
		//ReadRaster();		//光栅读取
		//GetGearRatio();	//齿轮比读取
		//GetMoveParam();	//运动参数读取
		
		connect(Tool->GetUpdateButton(), SIGNAL(clicked()), this, SLOT(ReadRaster()));
		connect(Tool->GetUpdateButton(), SIGNAL(clicked()), this, SLOT(GetGearRatio()));
		connect(Tool->GetUpdateButton(), SIGNAL(clicked()), this, SLOT(GetMoveParam()));

		//connect(Tool->GetSaveButton(), SIGNAL(clicked()), this, SLOT(SeadRaster()));
		//connect(Tool->GetSaveButton(), SIGNAL(clicked()), this, SLOT(SetGearRatio()));
		connect(Tool->GetSaveButton(), SIGNAL(clicked()), this, SLOT(SetMoveParam()));
		
		//QWidget * widget = new QWidget;
		QGridLayout *layout = new QGridLayout;

		layout->addWidget(paramGroupBox, 	0, 0, 1, 1);
		layout->addWidget(rasterGroup,		1, 0, 1, 1);
		layout->addWidget(moter,		0, 1, 2, 1);

		layout->addWidget(param,		2, 0, 1, 1);

		mainWidget->setLayout(layout);

		Layout();
	}
public slots:
	void AxisChanged(int axis){
		if(axis == 0){
			moter->moveLineEdit->setVisible(true);
			moter->movePushButton->setVisible(true);
			//moter->gearLineEdit->setVisible(true);
			//moter->gearPushButton->setVisible(true);
			moter->pulsePushButton->setVisible(false);
			moter->pulseLineEdit->setVisible(false);
			dir_positive->setVisible(false);
			dir_reverse->setVisible(false);
		}else if(axis == 1){
			moter->moveLineEdit->setVisible(false);
			moter->movePushButton->setVisible(false);
			//moter->gearLineEdit->setVisible(false);
			//moter->gearPushButton->setVisible(false);
			moter->pulsePushButton->setVisible(true);
			moter->pulseLineEdit->setVisible(true);
			dir_positive->setVisible(true);
			dir_reverse->setVisible(true);
		}
		moter->posLineEdit->setText("");
		moter->gearLineEdit->setText("");
	}
	void MovePulse(){
		MOVE move;
		move.Axis = 1 << axisBox->currentIndex(); 
		move.Distance = moter->pulseLineEdit->text().toInt();
		move.Speed = speedBox->currentIndex();
		
		if(move.Axis == AXIS_X){
			move.Dir = 1;
		}
		else if(move.Axis == AXIS_Y){
			move.Dir = dir_positive->isChecked();
		}

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVETO, &move);
	}
	void MoveTo(){
		MOVE move;
		move.Axis = 1 << axisBox->currentIndex(); 
		move.Distance = moter->moveLineEdit->text().toInt();
		move.Speed = speedBox->currentIndex();

		if(move.Axis == AXIS_X){
			move.Dir = 0;
		}

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVETO, &move);
	}
	int ReadPos(){
		uint64_t pos[4] = {0, 0, 0, 0};
		SendMotionCmd(UI_CMD::CMD_MOTION_GET_COORD_PULSE, pos);
		
		int axis = axisBox->currentIndex(); 
		moter->posLineEdit->setText(QString::number(pos[axis]));
		return 0;
	}
	int ReadRaster(){
		qDebug() << "read raster";
		//int raster[4] = {0, 0, 0, 0};
		//SendMotionCmd(UI_CMD::CMD_MOTION_GET_RASTER, raster);
		//rasterLineEdit->setText(QString::number(raster[0]));	
		unsigned int buf[4] = {0x0070, 0, 0, 0};
		SendMotionCmd(UI_CMD::CMD_MOTION_GET_MODE, buf);
		rasterLineEdit->setText(QString::number(buf[0]));	
		return 0;
	}
	int SetRaster(){
		unsigned int buf[4] = {0x0070, 0, 0, 0};
		buf[1] = rasterLineEdit->text().toInt();	

		SendMotionCmd(UI_CMD::CMD_MOTION_SET_MODE, buf);

		return 0;
	}
	int GetGearRatio(){
		qDebug() << "get gear ratio";
		int axis = axisBox->currentIndex(); 
		if(axis == 0){
			unsigned int buf[4] = {0x0071, 0, 0, 0};
			SendMotionCmd(UI_CMD::CMD_MOTION_GET_MODE, buf);
			moter->gearLineEdit->setText(QString::number(buf[0]));	
		}
		return 0;
	}
	int SetGearRatio(){
		int axis = axisBox->currentIndex(); 
		if(axis == 0){
			unsigned int buf[4] = {0x0071, 0, 0, 0};
			buf[1] = moter->gearLineEdit->text().toInt();
			SendMotionCmd(UI_CMD::CMD_MOTION_SET_MODE, buf);
		}	
		return 0;
	}
	void GetMoveParam(){
		qDebug() << "get move param";
		unsigned int buf[4] = {0x0023, 0, 0, 0};
		buf[1] = moter->gearLineEdit->text().toInt();
		SendMotionCmd(UI_CMD::CMD_MOTION_GET_MODE, buf);
		
		unsigned short * data = (unsigned short*)buf;

		param->highspeedLineEdit->setText(QString::number(data[5]));
		param->midspeedLineEdit->setText(QString::number(data[4]));
		param->lowspeedLineEdit->setText(QString::number(data[3]));

		param->highaccLineEdit->setText(QString::number(data[2]));
		param->midaccLineEdit->setText(QString::number(data[1]));
		param->lowaccLineEdit->setText(QString::number(data[0]));
	}
	void SetMoveParam(){
		unsigned int buf[4] = {0x0023, 0, 0, 0};
		buf[1] = moter->gearLineEdit->text().toInt();
		SendMotionCmd(UI_CMD::CMD_MOTION_SET_MODE, buf);
	}

private:
	MoveDbgGroupBox * moter;
	MoterParamGroupBox * param;
	QGroupBox * rasterGroup;

	QPushButton * rasterPushButton;
	QPushButton * resetPushButton;
	IntLineEdit * rasterLineEdit;

	QRadioButton* dir_positive;
	QRadioButton* dir_reverse;

	QComboBox *axisBox;
	QComboBox *speedBox;
	//QTabWidget * widgetlist;
	struct MECHAINE property;
};

#endif
