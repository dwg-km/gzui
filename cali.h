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
		printButton->resize(72, 28);
		printButton->setStyleSheet("background-color: rgb(9, 148, 220)");
		horLayout->addWidget(printButton, 0, 0, 1, 1);

		for(int j = 0; j < rownum; j++){
			QLabel *label = new QLabel();
			label->setText(QString::number(j));
			label->setAlignment(Qt::AlignCenter);
			horLayout->addWidget(label, j + 1, 0);
		}

		for(int i = 0; i < colnum; i++){
			QLabel *label = new QLabel();
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
class StepcalWidget : public QWidget{
	Q_OBJECT
public:
	StepcalWidget (struct MECHAINE* property, QWidget *parent = NULL) : QWidget(parent){

		QGridLayout * gridLayout = new QGridLayout;

		printButton = new QPushButton;
		printButton->setText(tr("PRINT"));
		printButton->resize(72, 28);
		printButton->setStyleSheet("background-color: rgb(9, 148, 220)");
		gridLayout->addWidget(printButton, 2, 2);		

		repairButton = new QPushButton;
		repairButton->setText(tr("基准步进"));
		gridLayout->addWidget(repairButton, 2, 0);		

		modelComBox = new QComboBox;
		passNumComBox = new QComboBox;
		gridLayout->addWidget(printButton, 1, 2, 1, 1);
	
		for(int j = 1; j < 17; j++){
			passNumComBox->addItem(QString::number(j));
		
		}
		modelComBox->addItem("model0");
		modelComBox->addItem("model1");
		modelComBox->addItem("model2");

		gridLayout->addWidget(modelComBox, 0, 1);
		gridLayout->addWidget(passNumComBox, 0, 3);

		QLabel *label1 = new QLabel(tr("打印模式"));
		label1->setAlignment(Qt::AlignCenter);
		gridLayout->addWidget(label1, 0, 0);

		QLabel *label2 = new QLabel(tr("pass数"));
		label2->setAlignment(Qt::AlignCenter);
		gridLayout->addWidget(label2, 0, 2);

		QLabel *label3 = new QLabel(tr("基准步进"));
		label3->setAlignment(Qt::AlignCenter);
		gridLayout->addWidget(label3, 1, 0);

		QLabel *label4 = new QLabel(tr("repairNum"));
		label4->setAlignment(Qt::AlignCenter);
		gridLayout->addWidget(label4, 1, 2);
		
		stdStepNum =  new QLineEdit;
		gridLayout->addWidget(stdStepNum, 1, 1);
		repairStepNum = new QLineEdit;
		gridLayout->addWidget(repairStepNum, 2, 1);
		stepNum = new QLineEdit;
		gridLayout->addWidget(stepNum, 1, 3);

		setLayout(gridLayout);
	}
public slots:
	void repairStep()
	{

	}
	
	void StepCaliPrint()
	{
		//SaveCalibrationParam(UI_CMD::CMD_CALI_STEP,
		//		0,
		//		modelComBox->currentText().toStdString().c_str(),0,0);
		
		//PrintCalibration(UI_CMD::CMD_CALI_STEP,0,0,0);
	} 
private:
	QComboBox * modelComBox;
	QPushButton * printButton;
	QPushButton * repairButton;
	QLineEdit * stdStepNum;
	QLineEdit * stepNum;
	QLineEdit * repairStepNum;
	//QComboBox * modelComBox;
	QComboBox * passNumComBox;
};

class VerticalWidget :public QWidget{
	Q_OBJECT
public:
	VerticalWidget (struct MECHAINE* property, QWidget *parent = NULL) : QWidget(parent){
	QGridLayout *layout = new QGridLayout;	

   	QString color = property->PrintColor;
    	QStringList colorList;
	colorList = color.split(";");

	int colnum = property->PrinterColorNum;
	int rownum =property->PrinterGroupNum;
	
	int res ,speed = 0;

	lineGroup = new LineEditGroup("Vertical", colnum, 1, &colorList, this);
	connect(lineGroup->GetPrintButton(), SIGNAL(clicked()), this, SLOT(PrintVerticalCali()));

	overlapGroup = new LineEditGroup("Overlap", colnum, rownum-1, &colorList, this);
	connect(overlapGroup->GetPrintButton(), SIGNAL(clicked()), this, SLOT(PrintOverlapCali()));
	UpdataData();	
	//	for(int i = 0; i < colNum; i++)
    		//{
         	//	QLabel *label = new QLabel(this);
       		//	label->setText(colorList.at(i));
       		//	label->setAlignment(Qt::AlignCenter);
		//	layout->addWidget(label,0,i,1,1);
          	//	QLineEdit *edit = new QLineEdit(this);
           	//	lineEdit.push_back(edit);
		//	layout->addWidget(edit,1,i,1,1);
   		 //}
		layout->addWidget(lineGroup,0,1,1,3);
		layout->addWidget(overlapGroup,1,1,1,3);
		setLayout(layout);
	}
	void UpdataData(){
		int data[64];
 		int res , speed =0;
		if(LoadCalibrationParam(UI_CMD::CMD_CALI_VERTICAL, 
			res, speed, data) <= 0){
			memset(data, 0, sizeof(data));
		}
		lineGroup->UpdataContext(data);
		res = speed=0;
		if(LoadCalibrationParam(UI_CMD::CMD_CALI_OVERLAP, 
			res, speed, data) <= 0){
			memset(data, 0, sizeof(data));
		}
		overlapGroup->UpdataContext(data);

	};
	virtual void showEvent(QShowEvent * event){
		UpdataData();
	}
 public slots:

	void PrintVerticalCali(){
		int data[64];
		int res ,speed =0;
		if(lineGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD::CMD_CALI_VERTICAL,
				res,speed,data,0);
		  
		}

		PrintCalibration(UI_CMD::CMD_CALI_VERTICAL,
				res, speed, 0);
	}
	void PrintOverlapCali(){
		int data[64];
		int res ,speed =0;
		if(lineGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD::CMD_CALI_OVERLAP,
				res,speed,data,0);
		  
		}

		PrintCalibration(UI_CMD::CMD_CALI_VERTICAL,res,speed,0);
	}
private:
	QPushButton * print;
	QVector<QLineEdit *>lineEdit;


	LineEditGroup *overlapGroup;
	LineEditGroup *lineGroup;
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
				res, speed,  data, 0);
		}

		PrintCalibration(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed, 0);
	}
	void PrintRightCali(){
		int data[64];
		if(leftGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD::CMD_CALI_HORIZON_RIGHT,
				res, speed,  data, 0);
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
			SaveCalibrationParam(UI_CMD(cmd), res, speed,  data, 0);
		}

		PrintCalibration(UI_CMD(cmd), res, speed, 0);
	}
	void PrintRightCali(){
		int data[64];
		int cmd = UI_CMD::CMD_CALI_HORIZON_RIGHT_SUB | (colorIndex & 0x0F);
		if(leftGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD(cmd), res, speed,  data, 0);
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
class BiDirectionWidget : public QWidget{
	Q_OBJECT
public:
	BiDirectionWidget(struct MECHAINE* property, QWidget *parent = NULL) : QWidget(parent){

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
		
		int colnum = 1;
		int rownum = 1;
		QList<QString>::iterator iter;
	
		bidirectionGroup = new LineEditGroup("bidirection", colnum, rownum, NULL, this);
		connect(bidirectionGroup->GetPrintButton(), SIGNAL(clicked()), this, SLOT(PrintBiDirection()));

		gridLayout->addWidget(resComBox,	0, 0, 1, 1);
		gridLayout->addWidget(speedComBox,	0, 1, 1, 1);
		gridLayout->addWidget(bidirectionGroup,	1, 0, 1, 3);

		setLayout(gridLayout);
	}
	void UpdataData(){
		int data[64];
		int cmd = UI_CMD::CMD_CALI_HORIZON_BIDRECTION;
		if(LoadCalibrationParam(UI_CMD(cmd), res, speed, data) <= 0){
			memset(data, 0, sizeof(data));
		}
		bidirectionGroup->UpdataContext(data);
	}
	void SaveData(){
	
	}
	virtual void showEvent(QShowEvent * event){
		UpdataData();
	}
	virtual void hideEvent(QHideEvent * event){

	}
public slots:
	void PrinDirectionCali(){
		int data[64];
		int cmd = UI_CMD::CMD_CALI_HORIZON_BIDRECTION;
		if(bidirectionGroup->CheckDirty(data)){
			SaveCalibrationParam(UI_CMD(cmd), res, speed,  data, 0);
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
private:
	int speed;
	int res;

	QComboBox * resComBox;
	QComboBox * speedComBox;
	LineEditGroup *bidirectionGroup;
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
	
		QWidget * widget = new StepcalWidget(&property);
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
	
		QWidget * widget = new BiDirectionWidget(&property);
		widgetlist->addTab(widget, "Bidrection");
	}
	void AddVerticalCaliWidget(){
	
		QWidget * widget = new VerticalWidget(&property);
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
