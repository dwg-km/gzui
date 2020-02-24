#ifndef WAVE_Hi
#define	WAVE_H

#include "command.h"
#include "ui_interface.h"
#include "APIDataInterface.hpp"

#include <QPainter>
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

		QString color = c;
		QStringList colorlist = color.split(";");
		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				QLabel *label = new QLabel(this);
				QString text = colorlist.at(i) + QString::number(j);

				label->setText(text);
				//lineEdit->resize(40, 28);
				//lineEdit1->setEnabled(false);
				label->setAlignment(Qt::AlignCenter);
				horLayout->addWidget(label, (z + 1) * j, i);

				for(int b = 0; b < z; b++){
					QLineEdit *lineEdit = new QLineEdit(this);
					horLayout->addWidget(lineEdit, (z + 1) * j + b + 1, i);
					matrix.push_back(lineEdit);
				}
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
	void save(){
	    	for(int i = 0; i < rownum; i++)
			for( int j = 0; j < colnum; j++){
				int index = i * rownum + j;
				Value.push_back(matrix[index]->text().toFloat());
			}
	}
public:
	int colnum;
	int rownum;
	QVector<float> Value;
	QVector<QLineEdit *> matrix;
};

class TempWaveWidget : public QWidget{
	public:
		TempWaveWidget(struct MECHAINE & property){
				
			int colnum = property.PrinterColorNum;   //列数
			int rownum = property.PrinterGroupNum; //行数
	        	temp = new RateTimeGroup("设置温度", colnum, rownum, 1, property.PrintColor);
		        realtemp = new RateTimeGroup("实时温度", colnum, rownum, 1, property.PrintColor);
			realtemp->setEnabled(false);
			layout = new QVBoxLayout;
			layout->addWidget(temp);
  			layout->addWidget(realtemp);
			setLayout(layout);
		 	
		}	
//		void save(){
  //    			temp.save();
//			SendHbCmd(CMD_HB_TEMP_HEATD, 0 ,temp.Value);		
		
public:
	RateTimeGroup * temp;
	RateTimeGroup * realtemp;
	QVBoxLayout *layout;
};
class BaseVoltageWaveWidget : public QWidget {
public :
	BaseVoltageWaveWidget(struct MECHAINE &property){
		int colnum = property.ColumnPerHead;
		int rownum = property.PrinterGroupNum;

		//RateTimeGroup * basevol = new RateTimeGroup("基准电压", colnum, rownum, 2, property.PrintColor);
		//RateTimeGroup * adjustvol = new RateTimeGroup("矫正电压", colnum, rownum, 2, property.PrintColor);
		//RateTimeGroup * realvol = new RateTimeGroup("实时电压", colnum, rownum, 2, property.PrintColor);

		char * color = "K;C;M;Y;Lm;Lc;Lk;Ly";
		colnum = 8;
	        basevol = new RateTimeGroup("基准电压", colnum, rownum, 3, color);
		//RateTimeGroup * adjustvol = new RateTimeGroup("矫正电压", colnum, rownum, 2, color);
		//RateTimeGroup * realvol = new RateTimeGroup("实时电压", colnum, rownum, 2, color);
		//realvol->setEnabled(false);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(basevol);
		//layout->addWidget(adjustvol);
		//layout->addWidget(realvol);

		QWidget * widget = new QWidget;
		setLayout(layout);
	}
public :
	RateTimeGroup * basevol;
};
class pulseWidget : public QWidget {
public:
	pulseWidget(QWidget *parent = NULL) : QWidget(parent)
	{
	}

	virtual void paintEvent(QPaintEvent *e)
	{
		QPainter painter(this);

		int p0 = 90;
		int p1 = 70;
		int p2 = 50; 

		int d0 = 60;
		int d1 = 50;
		int d2 = 40;

		int v0 = 60;
		int v1 = 50;
		int v2 = 40;
	
		QVector<QPoint> points;
		points.push_back(QPoint(0,			0));
		points.push_back(QPoint(10,			0));
		points.push_back(QPoint(10,			v0));
		points.push_back(QPoint(10+p0,			v0));
		points.push_back(QPoint(10+p0,			0));
		points.push_back(QPoint(10+p0+d0,		0));
		points.push_back(QPoint(10+p0+d0,		v1));
		points.push_back(QPoint(10+p0+d0+p1,		v1));
		points.push_back(QPoint(10+p0+d0+p1,		0));
		points.push_back(QPoint(10+p0+d0+p1+d1,		0));
		points.push_back(QPoint(10+p0+d0+p1+d1,		v2));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2,	v2));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2,	0));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2+d2,	0));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2+d2+10,0));

		int x = 10;
		int y = 20;
		int ymax = 0;
		int xmax = 0;
		for(int i = 0; i < points.size(); i++){
			if(ymax < points[i].ry()){
				ymax = points[i].ry();
			}
			if(xmax < points[i].rx()){
				xmax = points[i].rx();
			}
		}
		for(int i = 0; i < points.size(); i++){
			points[i].ry() = ymax - points[i].ry() + y;
			//points[i].rx() = xmax - points[i].rx() + x;
		}

		//painter.setBrush(QBrush(Qt::black, Qt::SolidPattern));
		for(int i = 0; i < points.size() - 1; i++){
			painter.drawLine(points[i], points[i+1]);
		}
		painter.drawText(points[1].rx()+p0/2, points[1].ry()-5, "p0");
		painter.drawText(points[2], "v0");
		painter.drawText(points[4], "d0");

		painter.drawText(points[5], "p1");
		painter.drawText(points[6], "v1");
		painter.drawText(points[8], "d1");

		painter.drawText(points[9], "p2");
		painter.drawText(points[10], "v2");
		painter.drawText(points[12], "d2");
	}

private:
};

class WaveWidget : public QWidget {
public:
	WaveWidget(struct MECHAINE * property )
	{

		QGridLayout * layout = new QGridLayout;

		pulseWidget * pulse = new pulseWidget;

		QString color = property->PrintColor;
		QStringList colorlist = color.split(";");
		char * name = "Pause;Voltage;Delay";
		QComboBox * indexComBox = new QComboBox;
		for(int g = 0; g < property->PrinterGroupNum; g++){
			for(int c = 0; c < property->PrinterColorNum; c++){
				QString text = colorlist.at(c) + QString::number(g);
				indexComBox->addItem(text);
			}
		}
		QPushButton * format = new QPushButton("copy to all head");
		 wave = new RateTimeGroup("脉宽", 3, 1, 3, name);

		layout->addWidget(pulse, 0, 0);
		layout->addWidget(indexComBox, 1, 0, 1, 1);
		layout->addWidget(format, 1, 1, 1, 1);
		layout->addWidget(wave, 2, 0, 1, 3);

		setLayout(layout);
	}



public:
	
        RateTimeGroup * wave ;
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
		connect(Tool->GetSaveButton(),SIGNAL(clicked()),this, SLOT(save()));
		GetPrinterProperty(&property);

		waveWidget = new WaveWidget(&property);
	        tempWidget = new TempWaveWidget(property);
 		baseVoltageWidget = new BaseVoltageWaveWidget(property);

		widgetlist =  new QTabWidget;

		widgetlist->addTab(tempWidget, "Temp");
		widgetlist->addTab(baseVoltageWidget, "BaseVoltage");
		widgetlist->addTab(waveWidget, "Wave");
		//AddMachineCheckWidget();
		//AddBidirectonWaveWidget();
		
		//AddTempWaveWidget();
		//AddBaseVoltageWaveWidget();
		//AddVoltageWaveWidget();
		//AddPulseWaveWidget();

		Layout(widgetlist);
	}
	virtual void showEvent(QShowEvent * event){
		float data[256];
		SendHbCmd(CMD_HB_TEMP_HEAT, 0, data, 32);
		
		int colnum = property.PrinterColorNum;
		int rownum = property.PrinterGroupNum;
		int index =0;
		QString str;
   		for(int i = 0; i < rownum; i++)
			for(int j = 0; j < colnum; j++)
			{
			        index =  i * colnum + j ;
				str =QString::number( data[i]);
 				tempWidget->temp->matrix[index]->setText(str);
			} 
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
		widget->setLnn/save
nnnnayout(mainLayout);

		widgetlist->addTab(widget, "Mechanical");
	}
	*/
	void AddVoltageWaveWidget(){
		int colnum = property.ColumnPerHead;
		int rownum = property.PrinterGroupNum;

		//RateTimeGroup * basevol = new RateTimeGroup("基准电压", colnum, rownum, 2, property.PrintColor);
		//RateTimeGroup * adjustvol = new RateTimeGroup("矫正电压", colnum, rownum, 2, property.PrintColor);
		//RateTimeGroup * realvol = new RateTimeGroup("实时电压", colnum, rownum, 2, property.PrintColor);

		char * color = "K;C;M;Y;Lm;Lc;Lk;Ly";
		colnum = 8;
		//RateTimeGroup * basevol = new RateTimeGroup("基准电压", colnum, rownum, 2, color);
		RateTimeGroup * adjustvol = new RateTimeGroup("矫正电压", colnum, rownum, 3, color);
		RateTimeGroup * realvol = new RateTimeGroup("实时电压", colnum, rownum, 3, color);
		realvol->setEnabled(false);

		QVBoxLayout *layout = new QVBoxLayout;
		//layout->addWidget(basevol);
		layout->addWidget(adjustvol);
		layout->addWidget(realvol);

		QWidget * widget = new QWidget;
		widget->setLayout(layout);
		widgetlist->addTab(widget, "Voltage");
	}

public slots:

	void save(){
		int index = widgetlist->currentIndex();
 		switch(index){
			case 0:
			{
				tempWidget->temp->save();
				int const size = tempWidget->temp->Value.size();
				float data[size];
				for(int i = 0; i < size; i++){
					data[i] = tempWidget->temp->Value[i];
				}
				SendHbCmd(CMD_HB_TEMP_HEAT, 1, data, size);
				break;
			}
			case 1:
				baseVoltageWidget->basevol->save();
			//	SendHbCmd(CMD_HB_VOL_BASE, 0 ,&baseVoltageWidget->basevol>Value[0]);
				//SendHbCmd(CMD_HB_VOL_BASE, 0 ,&baseVoltageWidget->basevol>Value[0]);
				break;
			case 2:
				//waveWidget->
//				SendHbCmd(CMD_HB_XXXX, 0 ,tempWidget->temp.Value);
				break;
			default:
				break;
		}
						
	}
private:
	TempWaveWidget *tempWidget;
	WaveWidget * waveWidget;
  	BaseVoltageWaveWidget * baseVoltageWidget;
	QTabWidget * widgetlist;
	struct MECHAINE property;
};
#endif
