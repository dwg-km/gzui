#ifndef WAVE_H
#define	WAVE_H

#include "UiTemplate.h"

class RateTimeGroup : public QGroupBox {
public:
	RateTimeGroup(QString name, int x, int y, int z, char * c, QWidget *parent = NULL) 
		: QGroupBox(name),
		colnum(x),
		rownum(y),
		Value(x * y, -1000)
	{
		QGridLayout * horLayout = new QGridLayout;

		for(int j = 0; j < rownum; j++){
			QLabel *label = new QLabel(this);
			label->setText(QString::number(j));
			label->setAlignment(Qt::AlignCenter);
			horLayout->addWidget(label, j + 1, 0);
		}

		QString color = c;
		QStringList colorlist = color.split(";");
		for(int i = 0; i < colnum; i++){
			QLabel *label = new QLabel(this);
			//label->setFixedHeight(40);
			label->setText(colorlist.at(i));
			label->setAlignment(Qt::AlignCenter);
			horLayout->addWidget(label, 0, i + 2, 1, 1);
		}

		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				QLineEdit *lineEdit0 = new QLineEdit(this);
				//lineEdit1->setEnabled(false);
				//lineEdit->resize(40, 28);
				horLayout->addWidget(lineEdit0, j + 1, i + 2);
				//matrix.push_back(lineEdit);
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
	int SetEnabled(){
	
	}
private:
	int colnum;
	int rownum;
	QVector<int> Value;
	QVector<QLineEdit *> matrix;
};

class WaveDialog : public UiTemplate
{
public:
	WaveDialog(QWidget *parent = NULL) : UiTemplate(parent)
	{
		toolLayout->addWidget(Tool->GetMenuButton());
		toolLayout->addWidget(Tool->GetSaveButton());
		toolLayout->addWidget(Tool->GetUpdateButton());
		connect(Tool->GetMenuButton(), SIGNAL(clicked()), this, SLOT(close()));

		GetPrinterProperty(&property);

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
		int colnum = property.ColumnPerHead;
		int rownum = property.PrinterGroupNum;

		QWidget * widget = new QWidget;
		RateTimeGroup * temp = new RateTimeGroup("Temp", colnum, rownum, 2, property.PrintColor);
		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(temp);
		widget->setLayout(layout);

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
	struct MECHAINE property;
};

#endif
