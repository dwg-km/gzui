#ifndef CALI_H
#define	CALI_H

#include <QComboBox>
#include <QGroupBox>

#include "UiTemplate.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"

class LineEditGroup : public QGroupBox {
public:
	LineEditGroup(QString name, MECHAINE * p, QWidget *parent = NULL) 
		: QGroupBox(name),
		property(p)
	{
		QGridLayout * horLayout = new QGridLayout;

		QPushButton *left = new QPushButton(this);
		left->setText(tr("PRINT"));
		left->resize(72, 28);
		left->setStyleSheet("background-color: rgb(9, 148, 220)");
		horLayout->addWidget(left, 0, 0, 1, 1);

		QString color = property->PrintColor;
		QStringList colorList;
		colorList = color.split(";");
	
		int colnum = property->PrinterColorNum;   //列数
		int rownum = property->PrinterGroupNum; //行数
		for(int j = 0; j < rownum; j++){
			QLabel *label = new QLabel();
			label->setText(QString::number(j));
			label->setAlignment(Qt::AlignCenter);
			horLayout->addWidget(label, j + 1, 0);
		}
		for(int i = 0; i < colnum; i++){
			QLabel *label = new QLabel();
			//label->setFixedHeight(40);
			label->setText(colorList.at(i));
			label->setAlignment(Qt::AlignCenter);
			horLayout->addWidget(label, 0, i + 2, 1, 1);
		}

		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				QLineEdit *lineEdit = new QLineEdit(this);
				//lineEdit->resize(40, 28);
				horLayout->addWidget(lineEdit, j + 1, i + 2);
				matrix.push_back(lineEdit);
			}
		}
		
		setLayout(horLayout);

		//connect(left, SIGNAL(clicked()), this, SLOT(on_left_clicked()));
	}
	void UpdataContext(int * data){
		int colnum = property->PrinterColorNum;   //列数
		int rownum = property->PrinterGroupNum; //行数
		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				QString text = QString::number(data[j * colnum + i]);
				matrix[j * colnum + i]->setText(text);
			}
		}	
	}
private:
	struct MECHAINE* property;
	QVector<QLineEdit *> matrix;
};

class HorizontalWidget : public QWidget{
	Q_OBJECT
public:
	HorizontalWidget(QWidget *parent = NULL) : QWidget(parent){
		property = new MECHAINE;
		GetPrinterProperty(property);

		resComBox = new QComboBox;
		speedComBox = new QComboBox;
		gridLayout = new QGridLayout;

		int resolution[8] = {0};
		int num = LoadPrintResList(resolution);
		if(0 != num){
			for(int i = 0; i < num; i++){
				resComBox->addItem(QString::number(resolution[i]));
			}
		}
		res = resComBox->currentText().toInt();
		connect(resComBox, SIGNAL(currentTextChanged(const QString&)), this, SLOT(ResChanged(const QString&)));
		
		speedComBox->addItem("低速");
		speedComBox->addItem("中速");
		speedComBox->addItem("高速");
		speed = speedComBox->currentIndex();
		connect(speedComBox, SIGNAL(currentIndexChanged(int)), this, SLOT(SpeedChanged(int)));
		
		leftGroup = new LineEditGroup("left", property, this);
		rightGroup = new LineEditGroup("right", property, this);

		gridLayout->addWidget(resComBox,	0, 1, 1, 1);
		gridLayout->addWidget(speedComBox,	0, 2, 1, 1);
		gridLayout->addWidget(leftGroup,	1, 0, 1, 3);
		gridLayout->addWidget(rightGroup,	2, 0, 1, 3);

		setLayout(gridLayout);
	}
	void UpdataData(){
		int data[64];
		if(LoadCalibrationParam(UI_CMD::CMD_CALI_HORIZON_LEFT, 
			res, speed, data) <= 0){
			memset(data, 0, sizeof(data));
		}
		leftGroup->UpdataContext(data);

		if(LoadCalibrationParam(UI_CMD::CMD_CALI_HORIZON_RIGHT, 
			res, speed, data) <= 0){
			memset(data, 0, sizeof(data));
		}
		rightGroup->UpdataContext(data);
	}
	void SaveData(){
	
	}
	virtual void showEvent(QShowEvent * event){
		UpdataData();
	}
	virtual void hideEvent(QHideEvent * event){

	}
	void PrintLeftCali(){
		int data[64];
		SaveCalibrationParam(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed,  data, 0);

		PrintCalibration(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed, 0);
	}
	void PrintRightCali(){
		int data[64];
		SaveCalibrationParam(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed,  data, 0);

		PrintCalibration(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed, 0);
	}
public slots:
	void ResChanged(const QString & text){
		res = text.toInt();
		qDebug() << "res changed " << res;
		UpdataData();
	}
	void SpeedChanged(int index){
		speed = index;
		//speed = speedComBox->currentIndex();
		qDebug() << "speed changed " << speed;
		UpdataData();
	}
private:
	struct MECHAINE* property;
	int speed;
	int res;

	QComboBox * resComBox;
	QComboBox * speedComBox;
	LineEditGroup *leftGroup;
	LineEditGroup *rightGroup;

	QGridLayout * gridLayout;
};

class CaliDialog : public UiTemplate
{
public:
	CaliDialog(QWidget *parent = NULL) : UiTemplate(parent)
	{
		toolLayout->addWidget(Tool->GetMenuButton());

		toolLayout->addWidget(Tool->GetLeftButton());
		toolLayout->addWidget(Tool->GetRightButton());
		//toolLayout->addWidget(Tool->GetHomeButton());
		//toolLayout->addWidget(Tool->GetUpButton());
		//toolLayout->addWidget(Tool->GetDownButton());
		toolLayout->addWidget(Tool->GetPauseButton());
		toolLayout->addWidget(Tool->GetAbortButton());

		//toolLayout->addWidget(Tool->GetNextButton());
		//toolLayout->addWidget(Tool->GetPreviousButton());
		
		connect(Tool->GetMenuButton(), SIGNAL(clicked()), this, SLOT(close()));

		statusLabel->setText("Calibration");

		/*
		QLabel *  = new QLabel;
		QLineEdit * = new QLineEditi;
		namedLabel->setBuddy(namedLineEdit);
		
		QWidget * widget = new QWidget(this);
		
		widget->
		*/
		
		widgetlist =  new QTabWidget;

		AddMachineCheckWidget();
		
		AddStepCaliWidget();
		AddHorizontalCaliWidget();
		AddBidirectonCaliWidget();
		AddVerticalCaliWidget();


		Layout(widgetlist);
	}
	void AddMachineCheckWidget(){
		QPushButton *angleButton = new QPushButton("Angle");
		QPushButton *stepButton = new QPushButton("Step");
		QPushButton *verticalButton = new QPushButton("Vertical");
		QPushButton *overlapButton = new QPushButton("Overlap");
		
		int x = 0;
		int y = 0;
		mainLayout->addWidget(angleButton, y, x++);
		mainLayout->addWidget(stepButton, y, x++);
		mainLayout->addWidget(verticalButton, y, x++);
		mainLayout->addWidget(overlapButton, y, x++);

		QWidget * widget = new QWidget;
		widget->setLayout(mainLayout);

		widgetlist->addTab(widget, "Mechanical");
	}
	void AddStepCaliWidget(){
	
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Step");
	}
	void AddHorizontalCaliWidget(){

		//QWidget * widget = new QWidget;
		//widgetlist->addTab(widget, "Horizontal");
		QWidget * horWidget = new HorizontalWidget;
		widgetlist->addTab(horWidget, "Horizontal");
	}
	void AddBidirectonCaliWidget(){
	
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Bidrection");
	}
	void AddVerticalCaliWidget(){
	
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Vertical");
	}
	
private:
	QTabWidget * widgetlist;

};

#endif
