#ifndef CALI_H
#define	CALI_H

#include "UiTemplate.h"

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

		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Horizontal");
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
