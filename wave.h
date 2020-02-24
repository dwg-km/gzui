#ifndef WAVE_H
#define	WAVE_H

#include <QGroupBox>
#include <QComboBox>
#include <QPainter>

#include "command.h"
#include "ui_interface.h"
#include "APIDataInterface.hpp"

#include "UiTemplate.h"

class RateTimeGroup : public QGroupBox {
public:
	RateTimeGroup(QString name, int x, int y, int z, char * c, QWidget *parent = NULL) 
		: QGroupBox(name),
		colnum(x),
		rownum(y),
		block(z),
		Value(x * y * z, -1000)
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

				for(int b = 0; b < block; b++){
					QLineEdit *lineEdit = new QLineEdit(this);
					horLayout->addWidget(lineEdit, (z + 1) * j + b + 1, i);
					matrix.push_back(lineEdit);
				}
			}
		}
		
		setLayout(horLayout);
	}
	void UpdataContext(float * data){
		//int colnum = property->PrinterColorNum;   //列数
		//int rownum = property->PrinterGroupNum; //行数
		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				for(int b = 0; b < block; b++){
					int index = j * colnum * block + i * block + b;
					if(Value[index] != data[index]){
						Value[index] = data[index];
						QString text = QString::number(data[index]);
						matrix[index]->setText(text);
					}
				}
			}
		}
	}
	int CheckDirty(float * data){
		int dirty = 0;
		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				for(int b = 0; b < block; b++){
					int index = j * colnum * block + i * block + b;
					int value = matrix[index]->text().toInt();
					data[index] = Value[index];
					if(Value[index] != value){
						dirty = 1;
					}
				}
			}
		}
	
		return dirty;
	}
	int Size(){
		return Value.size();
	}
	int SetEnabled(){
	
	}
private:
	int colnum;
	int rownum;
	int block;
	QVector<float> Value;
	QVector<QLineEdit *> matrix;
};

class pulseWidget : public QWidget {
public:
	pulseWidget(QWidget *parent = NULL) : QWidget(parent)
	{
		setFixedSize(600, 120);
	}

	virtual void paintEvent(QPaintEvent *e)
	{
		e = e; //no warning

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

class waveWidget : public QWidget {
	Q_OBJECT
public:
	waveWidget(struct MECHAINE * p, QWidget *parent = NULL) : QWidget(parent), property(p)
	{
		QGridLayout * layout = new QGridLayout;

		pulseWidget * pulse = new pulseWidget;

		QString color = property->PrintColor;
		QStringList colorlist = color.split(";");

		char * name = "Pause;Voltage;Delay";
		indexComBox = new QComboBox;
		for(int g = 0; g < property->PrinterGroupNum; g++){
			for(int c = 0; c < property->PrinterColorNum; c++){
				QString text = colorlist.at(c) + QString::number(g);
				indexComBox->addItem(text);
			}
		}

		QPushButton * format = new QPushButton("copy to all head");
		waveGroup = new RateTimeGroup("脉宽", 3, 1, 3, name);

		layout->addWidget(pulse,		0, 0, 1, 2);
		layout->addWidget(indexComBox,		2, 0, 1, 1);
		layout->addWidget(format,		2, 1, 1, 1);
		layout->addWidget(waveGroup,		3, 0, 1, 3);

		setLayout(layout);
	}

	virtual void showEvent(QShowEvent * event){
		float temp[128];	
		int len = property->PrinterGroupNum * property->PrinterColorNum * 9;
		SendHbCmd(CMD_HB_WAVE, 0, temp, len);
		waveGroup->UpdataContext(&temp[indexComBox->currentIndex() * 9]);
	}
public slots:

private:
	struct MECHAINE * property;
	RateTimeGroup * waveGroup;
	QComboBox * indexComBox;
};

class WaveDialog : public UiTemplate
{
	Q_OBJECT
public:
	WaveDialog(QWidget *parent = NULL) : UiTemplate(parent)
	{
		toolLayout->addWidget(Tool->GetMenuButton());
		toolLayout->addWidget(Tool->GetSaveButton());
		toolLayout->addWidget(Tool->GetUpdateButton());
		connect(Tool->GetMenuButton(), SIGNAL(clicked()), this, SLOT(close()));
		connect(Tool->GetSaveButton(), SIGNAL(clicked()), this, SLOT(SaveParam()));

		GetPrinterProperty(&property);

		widgetlist =  new QTabWidget;

		AddTempWaveWidget();
		AddVoltageWidget();
		AddPulseWaveWidget();

		Layout(widgetlist);
	}
	void AddTempWaveWidget(){
		int colnum = property.PrinterColorNum;
		int rownum = property.PrinterGroupNum;

		TargetTempGroup = new RateTimeGroup("设置温度", colnum, rownum, 1, property.PrintColor);

		RealTempGroup = new RateTimeGroup("实时温度", colnum, rownum, 1, property.PrintColor);
		RealTempGroup->setEnabled(false);

		TempWidget = new QWidget;
		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(TargetTempGroup);
		layout->addWidget(RealTempGroup);

		TempWidget->setLayout(layout);

		widgetlist->addTab(TempWidget, "Temp");
	}
	void AddVoltageWidget(){
		int colnum = property.PrinterColorNum;
		int rownum = property.PrinterGroupNum;

		baseVoltageGroup = new RateTimeGroup("基准电压", colnum, rownum, 1, property.PrintColor);
		adjustVoltageGroup = new RateTimeGroup("矫正电压", colnum, rownum, 1, property.PrintColor);
		//RateTimeGroup * realvol = new RateTimeGroup("实时电压", colnum, rownum, 2, color);
		//realvol->setEnabled(false);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(baseVoltageGroup);
		layout->addWidget(adjustVoltageGroup);
		//layout->addWidget(realvol);

		voltageWidget = new QWidget;
		voltageWidget->setLayout(layout);
		widgetlist->addTab(voltageWidget, "Voltage");
	}
	void AddPulseWaveWidget(){
		QWidget * widget = new waveWidget(&property);

		widgetlist->addTab(widget, "Wave");
	}

	virtual void showEvent(QShowEvent * event){
		event = event;
		GetRealTemp();
		GetTargetTemp();
		GetBaseVoltage();
		GetAdjustVoltage();
	}

private:
	void GetTargetTemp(){
		float temp[64];	
		int len = property.PrinterGroupNum * property.PrinterColorNum;
		SendHbCmd(CMD_HB_TEMP_HEAT, 0, temp, len);
		TargetTempGroup->UpdataContext(temp);
	}
	void GetRealTemp(){
		float temp[64];	
		int len = property.PrinterGroupNum * property.PrinterColorNum;
		SendHbCmd(CMD_HB_TEMP_HEAT, 0, temp, len);
		RealTempGroup->UpdataContext(temp);
	}
	void SetTargetTemp(){
		float temp[64];	
		if(TargetTempGroup->CheckDirty(temp)){
			SendHbCmd(CMD_HB_TEMP_HEAT, 1, temp, TargetTempGroup->Size());
		}	
	}
	void GetBaseVoltage(){
		float temp[64];	
		int len = property.PrinterGroupNum * property.PrinterColorNum;
		SendHbCmd(CMD_HB_TEMP_HEAT, 0, temp, len);
		baseVoltageGroup->UpdataContext(temp);
	}
	void SetBaseVoltage(){
		float temp[64];	
		if(baseVoltageGroup->CheckDirty(temp)){
			SendHbCmd(CMD_HB_TEMP_HEAT, 1, temp, baseVoltageGroup->Size());
		}	
	}
	void GetAdjustVoltage(){
		float temp[64];	
		int len = property.PrinterGroupNum * property.PrinterColorNum;
		SendHbCmd(CMD_HB_TEMP_HEAT, 0, temp, len);
		adjustVoltageGroup->UpdataContext(temp);
	}
	void SetAdjustVoltage(){
		float temp[64];	
		if(adjustVoltageGroup->CheckDirty(temp)){
			SendHbCmd(CMD_HB_TEMP_HEAT, 1, temp, adjustVoltageGroup->Size());
		}	
	}
public slots:
	void SaveParam(){
		SetTargetTemp();
		SetBaseVoltage();
		SetAdjustVoltage();
	}
private:
	QWidget * TempWidget;
	RateTimeGroup * RealTempGroup;
	RateTimeGroup * TargetTempGroup;  

	QWidget * voltageWidget; 
	RateTimeGroup *	baseVoltageGroup;
	RateTimeGroup *	adjustVoltageGroup;

	QTabWidget * widgetlist;
	struct MECHAINE property;
};

#endif
