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
	        pulseLineEdit = new QLineEdit;

	        movePushButton = new QPushButton("移动到(mm)");
	        moveLineEdit = new QLineEdit;

	        posPushButton = new QPushButton("位置");
		posLineEdit = new QLineEdit;
		
		QPushButton * gearPushButton = new QPushButton("齿轮比");
		QLineEdit * gearLineEdit = new QLineEdit;

		dbgLayout->addWidget(pulsePushButton,	1, 0);
		dbgLayout->addWidget(pulseLineEdit,	1, 1, 1, 4);
		dbgLayout->addWidget(movePushButton,	2, 0);
		dbgLayout->addWidget(moveLineEdit,	2, 1, 1, 4);
		dbgLayout->addWidget(posPushButton,	3, 0);
		dbgLayout->addWidget(posLineEdit,	3, 1, 1, 4);
		dbgLayout->addWidget(gearPushButton,	4, 0);
		dbgLayout->addWidget(gearLineEdit,	4, 1, 1, 4);

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
	QLineEdit* GetPulseLineEdit(){
		return pulseLineEdit;
	}
	QLineEdit* GetMoveLineEdit(){
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
private:
	QPushButton * posPushButton; 
	QLineEdit * posLineEdit;

	QPushButton * movePushButton; 
	QLineEdit * moveLineEdit;

	QLineEdit * pulseLineEdit;
        QPushButton * pulsePushButton;
	
};


class MoterParamGroupBox : public QGroupBox{
public:
	MoterParamGroupBox(QString name, QWidget *parent = NULL) : 
		QGroupBox(name, parent)
	{
		QLabel * maxfreqLabel = new QLabel("最高频率");
		QLabel * minfreqLabel = new QLabel("启动频率");
		QLabel * torqueLabel = new QLabel("力矩");
		QLabel * accLabel = new QLabel("加速度");
		QLabel * subdivLabel = new QLabel("细分数");

		QLineEdit * maxfreqLineEdit = new QLineEdit;
		QLineEdit * minfreqLineEdit = new QLineEdit;
		QLineEdit * torqueLineEdit = new QLineEdit;
		QLineEdit * accLineEdit = new QLineEdit;
		QLineEdit * subdivLineEdit = new QLineEdit;


		QGridLayout * Layout = new QGridLayout;
		Layout->addWidget(maxfreqLabel, 	0, 0);
		Layout->addWidget(maxfreqLineEdit, 	0, 1);
		Layout->addWidget(minfreqLabel, 	1, 0);
		Layout->addWidget(minfreqLineEdit, 	1, 1);
		Layout->addWidget(torqueLabel, 		2, 0);
		Layout->addWidget(torqueLineEdit, 	2, 1);
		Layout->addWidget(accLabel, 		3, 0);
		Layout->addWidget(accLineEdit, 		3, 1);
		Layout->addWidget(subdivLabel, 		4, 0);
		Layout->addWidget(subdivLineEdit, 	4, 1);

		setLayout(Layout);
	}
private:

};

class MotionDialog : public UiTemplate
{
public:
	MotionDialog(QWidget *parent = NULL) : UiTemplate(parent)
	{
		toolLayout->addWidget(Tool->GetMenuButton());
		toolLayout->addWidget(Tool->GetSaveButton());
		toolLayout->addWidget(Tool->GetUpdateButton());
		connect(Tool->GetMenuButton(), SIGNAL(clicked()), this, SLOT(close()));

		GetPrinterProperty(&property);

		QLabel *axisLabel = new QLabel(tr("移动轴"));
		axisLabel->setFixedWidth(72);
		//axisLabel->setFixedWidth(axisLabel->width());
		QComboBox * axisBox = new QComboBox();
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
		QGroupBox * paramGroupBox = new QGroupBox("");
		QGridLayout *paramLayout = new QGridLayout();
		paramLayout->addWidget(axisLabel,	0, 0);
		paramLayout->addWidget(axisBox,		0, 1);
		paramLayout->addWidget(speedLabel, 	1, 0);
		paramLayout->addWidget(speedBox, 	1, 1);
		paramLayout->addWidget(dir_positive, 	2, 0);
		paramLayout->addWidget(dir_reverse, 	2, 1);
		paramGroupBox->setLayout(paramLayout);	

		QCheckBox * orgCheck = new QCheckBox(tr("光栅"));
		QPushButton * rasterPushButton = new QPushButton("分辨率");
		QLineEdit * rasterLineEdit = new QLineEdit;
		QGroupBox * rasterGroup = new QGroupBox("光栅/编码器");
		QGridLayout *rasterLayout = new QGridLayout();
		rasterLayout->addWidget(orgCheck, 0, 0);
		rasterLayout->addWidget(rasterPushButton, 1, 0);
		rasterLayout->addWidget(rasterLineEdit, 1, 1);
		rasterGroup->setLayout(rasterLayout);
		
		MoveDbgGroupBox * moter = new MoveDbgGroupBox("运动调试");
		MoterParamGroupBox * param = new MoterParamGroupBox("电机参数");
		//QWidget * widget = new QWidget;
		QGridLayout *layout = new QGridLayout;

		//layout->addWidget(axisLabel,		0, 0);
		//layout->addWidget(axisBox,		0, 1);
		//layout->addWidget(speedLabel, 		0, 2);
		//layout->addWidget(speedBox, 		0, 3);
		//layout->addWidget(dir_positive, 	0, 4);
		//layout->addWidget(dir_reverse, 		0, 5);
		layout->addWidget(paramGroupBox, 	0, 0, 1, 1);
		
		//layout->addWidget(orgCheck,		1, 0, 1, 2);
		//layout->addWidget(rasterLabel,		1, 2, 1, 1);
		//layout->addWidget(rasterLineEdit,	1, 3, 1, 2);
		layout->addWidget(rasterGroup,		1, 0, 1, 1);

		layout->addWidget(moter,		0, 1, 2, 1);
		layout->addWidget(param,		0, 2, 2, 1);

		mainWidget->setLayout(layout);

		Layout();
	}
private:
	QRadioButton* dir_positive;
	QRadioButton* dir_reverse;

	QComboBox *speedBox;
	//QTabWidget * widgetlist;
	struct MECHAINE property;
};

#endif
