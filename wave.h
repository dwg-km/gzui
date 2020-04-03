#ifndef WAVE_H
#define	WAVE_H

#include <QMessageBox>
#include <QGroupBox>
#include <QComboBox>
#include <QPainter>

#include "command.h"
#include "ui_interface.h"
#include "APIDataInterface.hpp"
#include "touchkey/lineedit.h"

#include "UiTemplate.h"

class RateTimeGroup : public QGroupBox {
public:
	RateTimeGroup(QString name, int x, int y, int z, char * c, QWidget *parent = NULL) 
		: QGroupBox(name),
		colnum(x),
		rownum(y),
		block(z)
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
					IntLineEdit *lineEdit = new IntLineEdit(this);
					horLayout->addWidget(lineEdit, (z + 1) * j + b + 1, i);
					matrix.push_back(lineEdit);
				}
			}
		}
		
		setLayout(horLayout);
	}
	void UpdataContext(float * data){
		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				for(int b = 0; b < block; b++){
					int index = j * colnum * block + i * block + b;
					QString text = QString::number(data[index]);
					matrix[index]->setText(text);
				}
			}
		}
	}
 
	int CheckDirty(float * data){
		for(int j = 0; j < rownum; j++){
			for(int i = 0; i < colnum; i++){
				for(int b = 0; b < block; b++){
					int index = j * colnum * block + i * block + b;
      					float value = matrix[index]->text().toFloat();
					data[index] = value;
				}
			}
		}
	
		return 1;
	}
	int Size(){
		return rownum * colnum * block;
	}
	int SetEnabled(){
		//
		return 0;
	}
public:
	int colnum;
	int rownum;
	int block;
	QVector<QLineEdit *> matrix;
};

class TempWidget : public QWidget {
	Q_OBJECT
public:
	TempWidget(struct MECHAINE &p, QWidget * parent = NULL) : 
		QWidget(parent),
		property(p)
	{			
		int colnum = property.PrinterColorNum;
		int rownum = property.PrinterGroupNum;

		TargetTempGroup = new RateTimeGroup("设置温度", colnum, rownum, 1, property.PrintColor);

		RealTempGroup = new RateTimeGroup("实时温度", colnum, rownum, 1, property.PrintColor);
		RealTempGroup->setEnabled(false);

		QVBoxLayout *layout = new QVBoxLayout;
		layout->addWidget(TargetTempGroup);
		layout->addWidget(RealTempGroup);

		setLayout(layout);

		//GetRealTemp();
		//GetTargetTemp();
	}
	virtual void showEvent(QShowEvent * event){
		event = event;
		GetRealTemp();
		GetTargetTemp();
	}
/*
	virtual void hideEvent(QHideEvent * event){
		event = event;
		float temp[64];	
		if(TargetTempGroup->CheckDirty(temp)){
			QMessageBox msgBox;
			msgBox.setText("The  temp document has been modified.");
			msgBox.setInformativeText("Do you want to save your changes?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();
			if(ret == QMessageBox::Save){
				SendHbCmd(CMD_HB_TEMP_TARGET, 1, temp, TargetTempGroup->Size());
			}
		}
	}
*/
private:
	void GetTargetTemp(){
		float temp[64];	
		int len = property.PrinterGroupNum * property.PrinterColorNum;
		SendHbCmd(CMD_HB_TEMP_TARGET, READ, temp, len);
		TargetTempGroup->UpdataContext(temp);
	}
public slots:
	void GetRealTemp(){
		float temp[64];	
		int len = property.PrinterGroupNum * property.PrinterColorNum;
		SendHbCmd(CMD_HB_TEMP_REAL, READ, temp, len);
		RealTempGroup->UpdataContext(temp);
	}
public slots:
	void SaveParam(){
		float temp[64];	
		if(TargetTempGroup->CheckDirty(temp)){
			qDebug() << " is dirty";
			SendHbCmd(CMD_HB_TEMP_TARGET, WRITE, temp, TargetTempGroup->Size());
		}	
	}
private:
	struct MECHAINE property;
	RateTimeGroup * RealTempGroup;
	RateTimeGroup * TargetTempGroup;  
};

/*
class VoltageWidget : public QWidget {
	Q_OBJECT
public :
	VoltageWidget(struct MECHAINE &p, QWidget * parent = NULL) :
		QWidget(parent),
		property(p)
	{
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

		setLayout(layout);
	}
	virtual void showEvent(QShowEvent * event){
		event = event;
		GetBaseVoltage();
		GetAdjustVoltage();
	}
	virtual void hideEvent(QHideEvent * event){
		float temp[128];	
		if(baseVoltageGroup->CheckDirty(temp) || adjustVoltageGroup->CheckDirty(temp)){
			QMessageBox msgBox;
			msgBox.setText("The volage  document has been modified.");
			msgBox.setInformativeText("Do you want to save your changes?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			int ret = msgBox.exec();
			if(ret == QMessageBox::Save){
				SaveParam();
			}
		}
	}
private:
	void GetBaseVoltage(){
		float temp[64];	
		int len = property.PrinterGroupNum * property.PrinterColorNum;
		SendHbCmd(CMD_HB_VOL_BASE, 0, temp, len);
		baseVoltageGroup->UpdataContext(temp);
	}
	void GetAdjustVoltage(){
		float temp[64];	
		int len = property.PrinterGroupNum * property.PrinterColorNum;
		SendHbCmd(CMD_HB_VOL_ADJUST, 0, temp, len);
		adjustVoltageGroup->UpdataContext(temp);
	}
	void SetBaseVoltage(){
		float temp[64];	
		if(baseVoltageGroup->CheckDirty(temp)){
			SendHbCmd(CMD_HB_VOL_BASE, 1, temp, baseVoltageGroup->Size());
		}	
	}
	void SetAdjustVoltage(){
		float temp[64];	
		if(adjustVoltageGroup->CheckDirty(temp)){
			SendHbCmd(CMD_HB_VOL_ADJUST, 1, temp, adjustVoltageGroup->Size());
		}	
	}
public slots:
	void SaveParam(){
		SetBaseVoltage();
		SetAdjustVoltage();
	}
private:
	struct MECHAINE property;
	RateTimeGroup *	baseVoltageGroup;
	RateTimeGroup *	adjustVoltageGroup;
};
*/
class pulseWidget : public QWidget {
public:
	pulseWidget(QWidget *parent = NULL) : QWidget(parent)
	{
		setFixedSize(600, 128);
	}

	virtual void paintEvent(QPaintEvent *e)
	{
		e = e; //no warning

		QPainter painter(this);
	
		QVector<QPoint> points;
		points.push_back(QPoint(0,				0));
		points.push_back(QPoint(10,				0));
		points.push_back(QPoint(10,				v0));
		points.push_back(QPoint(10+p0,				v0));
		points.push_back(QPoint(10+p0,				0));
		points.push_back(QPoint(10+p0+d0,			0));
		points.push_back(QPoint(10+p0+d0,			v1));
		points.push_back(QPoint(10+p0+d0+p1,			v1));
		points.push_back(QPoint(10+p0+d0+p1,			0));
		points.push_back(QPoint(10+p0+d0+p1+d1,			0));
		points.push_back(QPoint(10+p0+d0+p1+d1,			v2));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2,		v2));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2,		0));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2+d2,		0));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2+d2,		v3));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2+d2+p3,	v3));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2+d2+p3,	0));
		points.push_back(QPoint(10+p0+d0+p1+d1+p2+d2+p3+d3,	0));

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
		/*
		painter.drawText(points[1].rx()+p0/2, points[1].ry()-5, "p0");
		painter.drawText(points[2], "v0");
		painter.drawText(points[4], "d0");

		painter.drawText(points[5], "p1");
		painter.drawText(points[6], "v1");
		painter.drawText(points[8], "d1");

		painter.drawText(points[9], "p2");
		painter.drawText(points[10], "v2");
		painter.drawText(points[12], "d2");
		*/
	}
	void Updata(float * data){
		p0 = (int)data[0] * 10;
		p1 = (int)data[1] * 10;
		p2 = (int)data[2] * 10;
		p3 = (int)data[3] * 10;

		v0 = (int)data[4];
		v1 = (int)data[5];
		v2 = (int)data[6];
		v3 = (int)data[7];

		d0 = (int)data[8] * 10;
		d1 = (int)data[9] * 10;
		d2 = (int)data[10] * 10;
		d3 = (int)data[11] * 10;

		update();
	}
private:
	int p0;
	int p1;
	int p2;
	int p3;

	int d0;
	int d1;
	int d2;
	int d3;

	int v0;
	int v1;
	int v2;
	int v3;
};

class WaveWidget : public QWidget {
	Q_OBJECT
public:
	WaveWidget(struct MECHAINE& p, QWidget *parent = NULL) :
		QWidget(parent),
		Index(0),
		Row(4),
		Col(3),
		property(p),
		LineEditPerHead(12)
	{

		QGridLayout * layout = new QGridLayout;

		pulse = new pulseWidget;

		QString color = property.PrintColor;
		QStringList colorlist = color.split(";");

		Size = property.PrinterGroupNum * property.PrinterColorNum * LineEditPerHead;

		char * name = (char*)"Pause;Voltage;Delay";
		indexComBox = new QComboBox;
		for(int g = 0; g < property.PrinterGroupNum; g++){
			for(int c = 0; c < property.PrinterColorNum; c++){
				QString text = colorlist.at(c) + QString::number(g);
				indexComBox->addItem(text);
			}
		}
		connect(indexComBox, SIGNAL(currentIndexChanged(int)), 
				this, SLOT(IndexChanged(int)));

		grayComBox = new QComboBox;
		for(int g = 1; g < 4; g++){
			QString text = QString::number(g) + "级灰度";
			grayComBox->addItem(text);
		}

		QPushButton * format = new QPushButton("copy to all head");
		connect(format, SIGNAL(clicked()), 
				this, SLOT(CpyToAllHead()));

		waveGroup = new RateTimeGroup("脉宽", Col, 1, Row, name);

		layout->addWidget(pulse,		0, 0, 1, 2);
		layout->addWidget(indexComBox,		2, 0, 1, 1);
		layout->addWidget(grayComBox,		2, 1, 1, 1);
		layout->addWidget(format,		2, 2, 1, 1);
		layout->addWidget(waveGroup,		3, 0, 1, 3);

		setLayout(layout);

		//SendHbCmd(CMD_HB_WAVE, READ, (float*)WaveCurve, Size);
		//pulse->Updata(WaveCurve[Index]);
		//waveGroup->UpdataContext(WaveCurve[Index]);
	}
	virtual void showEvent(QShowEvent * event){
		event = event;
		int index = indexComBox->currentIndex();
		SendHbCmd(CMD_HB_WAVE, READ, (float*)WaveCurve, Size);
		waveGroup->UpdataContext(WaveCurve[index]);
		pulse->Updata(WaveCurve[Index]);
	}
/*
	virtual void hideEvent(QHideEvent * event){
		event = event;
		if(waveGroup->CheckDirty(WaveCurve[Index])){
			Dirty = 1;
		}
		if(Dirty){
			QMessageBox msgBox;
			msgBox.setText("The wave has been modified.");
			msgBox.setInformativeText("Do you want to save your changes?");
			msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
			msgBox.setDefaultButton(QMessageBox::Save);
			if(msgBox.exec() == QMessageBox::Save){
				//SendHbCmd(CMD_HB_WAVE, WRITE, (float*)WaveCurve, Size);
				SaveData();
			}
		}
	}
*/
	void SaveData(){
		SendHbCmd(CMD_HB_WAVE, WRITE, (float*)WaveCurve, Size);
	}
public slots:
	void IndexChanged(int index){
		waveGroup->CheckDirty(WaveCurve[Index]);
		Index = index;
		pulse->Updata(WaveCurve[Index]);
		waveGroup->UpdataContext(WaveCurve[index]);
	}
	void SaveParam(){
		waveGroup->CheckDirty(WaveCurve[Index]);
		SaveData();
	}
	void CpyToAllHead(){
		waveGroup->CheckDirty(WaveCurve[Index]);
		for(int i = 0; i < 16; i++){
			for(int j = 0; j < LineEditPerHead; j++){
				WaveCurve[i][j] =  WaveCurve[Index][j];
			}
		}
	}
private:
	int Index;
	int Size;
	int Row;
	int Col;
	float WaveCurve[16][12];
	struct MECHAINE property;
	int LineEditPerHead;
	
	pulseWidget * pulse;
	RateTimeGroup * waveGroup;
	QComboBox * indexComBox;
	QComboBox * grayComBox;
};

class WaveDialog : public UiTemplate
{
	Q_OBJECT
public:
	WaveDialog(QWidget *parent = NULL) : UiTemplate(parent)
	{
		toolLayout->addWidget(Tool->GetExitButton());
		toolLayout->addWidget(Tool->GetSaveButton());
		toolLayout->addWidget(Tool->GetUpdateButton());
		connect(Tool->GetExitButton(), SIGNAL(clicked()), this, SLOT(close()));

		statusLabel->setText("波形设置");


		GetPrinterProperty(&property);

		widgetlist =  new QTabWidget;

		AddTempWaveWidget();
		//AddVoltageWidget();
		AddPulseWaveWidget();

		Layout(widgetlist);
	}
	void AddTempWaveWidget(){
	        tempWidget = new TempWidget(property);
		connect(Tool->GetSaveButton(), SIGNAL(clicked()), tempWidget, SLOT(SaveParam()));
		connect(Tool->GetUpdateButton(), SIGNAL(clicked()), tempWidget, SLOT(GetRealTemp()));

		widgetlist->addTab(tempWidget, "Temp");
	}
	//void AddVoltageWidget(){
	//	voltageWidget = new VoltageWidget(property);
	//	connect(Tool->GetSaveButton(), SIGNAL(clicked()), voltageWidget, SLOT(SaveParam()));
	//
	//	widgetlist->addTab(voltageWidget, "Voltage");
	//}
	void AddPulseWaveWidget(){
		waveWidget = new WaveWidget(property);
		connect(Tool->GetSaveButton(), SIGNAL(clicked()), waveWidget, SLOT(SaveParam()));


		widgetlist->addTab(waveWidget, "Wave");
	}
private:
	TempWidget * tempWidget;

	//VoltageWidget * voltageWidget; 
	
	WaveWidget * waveWidget;
	
	QTabWidget * widgetlist;
	struct MECHAINE property;
};

#endif
