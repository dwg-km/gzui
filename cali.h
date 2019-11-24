#ifndef CALI_H
#define	CALI_H

#include "UiTemplate.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"

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
		horWidget = new QWidget;
		LayoutHoriCali();
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
	void LayoutHoriCali()
	{
		QGridLayout * leftLayout = new QGridLayout;

		QComboBox * rateComBox = new QComboBox;
		QComboBox * speedComBox = new QComboBox;
		struct MECHAINE* property = new MECHAINE;
		GetPrinterProperty(property);

		int resolution[8] = {0};

		int num = LoadPrintResList(resolution);
		if(0 != num){
			for(int i = 0; i < num; i++){
				rateComBox->addItem(QString::number(resolution[i]));
			}
		}
		
		speedComBox->addItem("低");
		speedComBox->addItem("中");
		speedComBox->addItem("高");

		int y=0;
		leftLayout->addWidget(rateComBox, y, 0);
		leftLayout->addWidget(speedComBox, y++, 1);


		int rownum = property->PrinterColorNum;   //列数
		int colNum = property->PrinterGroupNum; //行数
		
		QString color = property->PrintColor;
		QPushButton *left = new QPushButton(this);
		left->setText("左校准");
		//left->resize(72, 28);
		//left->setStyleSheet("background-color: rgb(9, 148, 220)");
		//connect(left, SIGNAL(clicked()), this, SLOT(on_left_clicked()));
		//
		QPushButton *right = new QPushButton(this);
		right->setText("右校准");
		//right->resize(72, 28);
		//right->setStyleSheet("background-color: rgb(9, 148, 220)");
		//connect(right, SIGNAL(clicked()), this, SLOT(on_right_clicked()));

		QStringList colorList;
		colorList = color.split(";");

		//QRegExp reg("[a-zA-Z0-9]+$");
		//QValidator *validator = new QRegExpValidator(reg)
/*
		for(int i = 0; i < rownum; i++){
			QLabel *label = new QLabel();
			label->setText(colorList.at(i));
			label->setAlignment(Qt::AlignCenter);
			leftLayout->addWidget(label, y, i + 1);
		}
		y++;
*/		
		QLabel *label = new QLabel("aaa"); 
		qDebug()<<"label height"<<label->height();
		label->setFixedHeight(40);
		qDebug()<<"label height"<<label->height();
		leftLayout->addWidget(label, y++, 2, 1, 1);

		leftLayout->addWidget(left, y, 0);
		leftLayout->addWidget(right, colNum + y, 0);
		for(int i = 0; i < colNum * 2; i++){
			for(int j = 0; j < rownum; j++){
				QLineEdit *lineEdit = new QLineEdit(this);
				//lineEdit->resize(40, 28);
				//lineEdit->setValidator (validator);
				leftLayout->addWidget(lineEdit, i + y, j + 1);
				//m_lineEdit.push_back(lineEdit);
			}
		}

		horWidget->setLayout(leftLayout);
	}
	
private:
	QTabWidget * widgetlist;
	QWidget * horWidget;

};

#endif
