#ifndef CALI_H
#define	CALI_H

#include <QComboBox>
#include <QGroupBox>

#include "UiTemplate.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"

class LineEditGroup : public QGroupBox {
public:
	LineEditGroup(QString name, int x, int y, QStringList * color = NULL, QWidget *parent = NULL) 
		: QGroupBox(name),
		colnum(x),
		rownum(y),
		Value(x * y, -1000)
	{
		QGridLayout * horLayout = new QGridLayout;

		printButton = new QPushButton(this);
		printButton->setText(tr("PRINT"));
		//printButton->resize(72, 28);
		//printButton->setStyleSheet("background-color: rgb(9, 148, 220)");
		horLayout->addWidget(printButton, 0, 0, 1, 1);

		for(int j = 0; j < rownum; j++){
			QLabel *label = new QLabel(this);
			label->setText(QString::number(j));
			label->setAlignment(Qt::AlignCenter);
			horLayout->addWidget(label, j + 1, 0);
		}

		for(int i = 0; i < colnum; i++){
			QLabel *label = new QLabel(this);
			//label->setFixedHeight(40);
			if(color){
				label->setText(color->at(i));
			}else{
				label->setText(QString::number(i));
			}

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
	}
	void UpdataContext(int * data){
		//int colnum = property->PrinterColorNum;   //列数
		//int rownum = property->PrinterGroupNum; //行数
		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				int index = j * colnum + i;
				int value = data[index];
				if(Value[index] != value){
					Value[index] = value;
					QString text = QString::number(value);
					matrix[j * colnum + i]->setText(text);
				}
			}
		}
	}
	int CheckDirty(int * data){
		int dirty = 0;
		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				int index = j * colnum + i;
				int value = matrix[index]->text().toInt();
				data[index] = Value[index];
				if(Value[index] != value){
					dirty = 1;
				}
			}
		}
	
		return dirty;
	}
	int Size(){
		return colnum * rownum;
	}
	QPushButton * GetPrintButton(){
		return printButton;
	}
private:
	int colnum;
	int rownum;
	QVector<int> Value;
	QVector<QLineEdit *> matrix;
	QPushButton *printButton;
};

class HorizontalWidget : public QWidget{
	Q_OBJECT
public:
	HorizontalWidget(struct MECHAINE* property, QWidget *parent = NULL) : QWidget(parent){

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
		connect(resComBox, SIGNAL(currentTextChanged(const QString&)),
				this, SLOT(ResChanged(const QString&)));
		
		speedComBox->addItem("低速");
		speedComBox->addItem("中速");
		speedComBox->addItem("高速");
		speed = speedComBox->currentIndex();
		connect(speedComBox, SIGNAL(currentIndexChanged(int)), 
				this, SLOT(SpeedChanged(int)));
		
		QString color = property->PrintColor;
		QStringList colorlist = color.split(";");
		int colnum = property->PrinterColorNum;
		int rownum = property->PrinterGroupNum;
	
		leftGroup = new LineEditGroup("left", colnum, rownum, &colorlist, this);
		rightGroup = new LineEditGroup("right",  colnum, rownum, &colorlist, this);
		connect(leftGroup->GetPrintButton(), SIGNAL(clicked()), this, SLOT(PrintLeftCali()));
		connect(rightGroup->GetPrintButton(), SIGNAL(clicked()), this, SLOT(PrintRightCali()));

		gridLayout->addWidget(resComBox,	0, 0, 1, 1);
		gridLayout->addWidget(speedComBox,	0, 1, 1, 1);
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
public slots:
	void PrintLeftCali(){
		int data[64];
		if(leftGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed,  data, leftGroup->Size());
		}

		PrintCalibration(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed, 0);
	}
	void PrintRightCali(){
		int data[64];
		if(leftGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed,  data, rightGroup->Size());
		}

		PrintCalibration(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed, 0);
	}
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
	int speed;
	int res;

	QComboBox * resComBox;
	QComboBox * speedComBox;
	LineEditGroup *leftGroup;
	LineEditGroup *rightGroup;

	QGridLayout * gridLayout;
};

class HeadHorizonWidget : public QWidget{
	Q_OBJECT
public:
	HeadHorizonWidget(struct MECHAINE* property, QWidget *parent = NULL) : QWidget(parent){

		colorComBox = new QComboBox;
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
		connect(resComBox, SIGNAL(currentTextChanged(const QString&)),
				this, SLOT(ResChanged(const QString&)));
		
		speedComBox->addItem("低速");
		speedComBox->addItem("中速");
		speedComBox->addItem("高速");
		speed = speedComBox->currentIndex();
		connect(speedComBox, SIGNAL(currentIndexChanged(int)), 
				this, SLOT(SpeedChanged(int)));
		
		colorIndex = 0;
		QString color = property->PrintColor;
		QStringList colorlist = color.split(";");
		int colnum = property->ColumnPerHead;
		int rownum = property->PrinterGroupNum;
		QList<QString>::iterator iter;
		for(iter = colorlist.begin(); iter != colorlist.end(); iter++){
			colorComBox->addItem(*iter);
		}
		connect(colorComBox, SIGNAL(currentIndexChanged(int)),
				this, SLOT(ColorChanged(int)));
	
		leftGroup = new LineEditGroup("left", colnum, rownum, NULL, this);
		rightGroup = new LineEditGroup("right",  colnum, rownum, NULL, this);
		connect(leftGroup->GetPrintButton(), SIGNAL(clicked()), this, SLOT(PrintLeftCali()));
		connect(rightGroup->GetPrintButton(), SIGNAL(clicked()), this, SLOT(PrintRightCali()));

		gridLayout->addWidget(resComBox,	0, 0, 1, 1);
		gridLayout->addWidget(speedComBox,	0, 1, 1, 1);
		gridLayout->addWidget(colorComBox,	0, 2, 1, 1);
		gridLayout->addWidget(leftGroup,	1, 0, 1, 3);
		gridLayout->addWidget(rightGroup,	2, 0, 1, 3);

		setLayout(gridLayout);
	}
	void UpdataData(){
		int data[64];
		int cmd = UI_CMD::CMD_CALI_HORIZON_LEFT_SUB | (colorIndex & 0x0F);
		if(LoadCalibrationParam(UI_CMD(cmd), res, speed, data) <= 0){
			memset(data, 0, sizeof(data));
		}
		leftGroup->UpdataContext(data);

		cmd = UI_CMD::CMD_CALI_HORIZON_RIGHT_SUB | (colorIndex & 0x0F);
		if(LoadCalibrationParam(UI_CMD(cmd), res, speed, data) <= 0){
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
public slots:
	void PrintLeftCali(){
		int data[64];
		int cmd = UI_CMD::CMD_CALI_HORIZON_LEFT_SUB | (colorIndex & 0x0F);
		if(leftGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD(cmd), res, speed,  data, leftGroup->Size());
		}

		PrintCalibration(UI_CMD(cmd), res, speed, 0);
	}
	void PrintRightCali(){
		int data[64];
		int cmd = UI_CMD::CMD_CALI_HORIZON_RIGHT_SUB | (colorIndex & 0x0F);
		if(leftGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD(cmd), res, speed,  data, rightGroup->Size());
		}

		PrintCalibration(UI_CMD(cmd), res, speed, 0);
	}
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
	void ColorChanged(int index){
		colorIndex = index;
		UpdataData();
	}
private:
	int speed;
	int res;
	int colorIndex;

	QComboBox * colorComBox;
	QComboBox * resComBox;
	QComboBox * speedComBox;
	LineEditGroup *leftGroup;
	LineEditGroup *rightGroup;

	QGridLayout * gridLayout;
};
class CaliDialog : public UiTemplate
{
	Q_OBJECT
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
		
		//property = new MECHAINE;
		GetPrinterProperty(&property);

		widgetlist =  new QTabWidget;

		AddMachineCheckWidget();
		
		AddStepCaliWidget();
		AddHorizontalCaliWidget();
		AddHeadHorizonCaliWidget();
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
		QWidget * horWidget = new HorizontalWidget(&property);
		widgetlist->addTab(horWidget, "Horizontal");
	}
	void AddHeadHorizonCaliWidget(){
		QWidget * headWidget = new HeadHorizonWidget(&property);
		widgetlist->addTab(headWidget, "HeadHorizontal");
	}
	void AddBidirectonCaliWidget(){
	
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Bidrection");
	}
	void AddVerticalCaliWidget(){
	
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Vertical");
	}
	
public slots:
	void SetReady(){
		Tool->setMoveEnabled(true);
		Tool->GetAbortButton()->setDisabled();
		Tool->GetPauseButton()->setDisabled();
		messageLabel->setText(tr("ready"));
	}
	void SetMoving(){
		Tool->setMoveEnabled(false);
		messageLabel->setText(tr("moving"));
	}
	void SetPause(){
	}
	void SetError(QString msg){
		Tool->setMoveEnabled(false);
		Tool->GetAbortButton()->setDisabled();
		Tool->GetPauseButton()->setDisabled();
		messageLabel->setText(msg);
	}
	void SetPrinting(QString msg){
		Tool->GetAbortButton()->setEnabled();
		Tool->GetPauseButton()->setEnabled();
		Tool->setMoveEnabled(false);
		messageLabel->setText(msg);
	}
private:
	QTabWidget * widgetlist;
	struct MECHAINE property;
};

#endif
