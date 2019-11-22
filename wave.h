#ifndef WAVE_H
#define	WAVE_H

#include "UiTemplate.h"

class WaveDialog : public UiTemplate
{
public:
	WaveDialog(QWidget *parent = NULL) : UiTemplate(parent)
	{
		toolLayout->addWidget(Tool->GetMenuButton());

		connect(Tool->GetMenuButton(), SIGNAL(clicked()), this, SLOT(close()));

		widgetlist =  new QTabWidget;

		//AddMachineCheckWidget();
		//AddBidirectonWaveWidget();
		
		AddTempWaveWidget();
		AddVoltageWaveWidget();
		AddPulseWaveWidget();


		Layout(widgetlist);
	}
	/*
	void AddMachineCheckWidget(){
		QPushButton *angleButton = new QPushButton("Angle");
		QPushButton *stepButton = new QPushButton("Temp");
		QPushButton *verticalButton = new QPushButton("Pulse");
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
	*/
	void AddTempWaveWidget(){
	
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Temp");
	}
	void AddVoltageWaveWidget(){

		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Voltage");
	}
	void AddPulseWaveWidget(){
	
		QWidget * widget = new QWidget;
		widgetlist->addTab(widget, "Pulse");
	}
	
private:
	QTabWidget * widgetlist;

};

#endif
