#include <QThread>
#include <QApplication>
#include <QXmlStreamReader>

#include "mainwindow.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"

enum ColorIndex {
	BLACK		= 0,
	CYAN		= 1,
	MEGENTA		= 2,
	YELLO		= 3,
	LIGHT_CYAN	= 4,
	LIGHT_MEGENTA	= 5
};

#define		RGB_WHITE	QColor(255, 255, 255)
#define		RGB_BLACK	QColor(35, 31, 32)
#define		RGB_CYAN	QColor(0, 159, 221)
#define		RGB_MEGENTA	QColor(194, 0, 144)
#define		RGB_YELLOW	QColor(244, 231, 0)
#define		RGB_LMEGENTA	QColor(231, 143, 194)
#define		RGB_LCYAN	QColor(140, 203, 226)


void InkWidget::paintEvent(QPaintEvent *e)
{
	e = e;
	//QPainter painter(this);
	//painter.drawPixmap(event->rect(), pixmatpList[curIndex]);
	int xCoord[8];
	const int yCoord = 0;
	const int ColorNum = 6;
	//const int lenght = height();
	const int rect_width = width() / ColorNum * 3 / 4;
	const int rect_height = height() - yCoord;

	for(int c = 0; c < ColorNum; c++){
		xCoord[c] = c * width() / ColorNum;
	}

	QPainter painter(this);
	painter.setPen(QPen(QColor(221, 227, 227), 1));

	painter.setBrush(*gradient0);
	painter.drawRect(xCoord[BLACK], yCoord, rect_width, rect_height);

	painter.setBrush(*gradient1);
	painter.drawRect(xCoord[CYAN], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[CYAN]) + "%";
	//label2->setText(persent);

	painter.setBrush(*gradient2);
	painter.drawRect(xCoord[MEGENTA], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[MEGENTA]) + "%";
	//label3->setText(persent);

	painter.setBrush(*gradient3);
	painter.drawRect(xCoord[YELLO], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[YELLO]) + "%";
	//label4->setText(persent);

	painter.setBrush(*gradient4);
	painter.drawRect(xCoord[LIGHT_CYAN], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[LIGHT_CYAN]) + "%";
	//label5->setText(persent);

	painter.setBrush(*gradient5);
	painter.drawRect(xCoord[LIGHT_MEGENTA], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[LIGHT_MEGENTA]) + "%";
	//label6->setText(persent);
}

InkWidget::InkWidget(QWidget *parent) : QWidget(parent)
{
	INK_PUMP * pump = new INK_PUMP;
	GetPrinterParam(UI_CMD::CMD_MECHINE_INK_PUMP, pump);

	int xCoord[8];
	const int yCoord = 0;
	const int ColorNum = 6;
	//const int lenght = height();
	const int rect_width = width() / ColorNum * 3 / 4;
	const int rect_height = height() - yCoord;

	for(int c = 0; c < ColorNum; c++){
		xCoord[c] = c * width() / ColorNum;
	}

	float colData = 1.0 - (float)pump->InkQuantity[BLACK] / 100;
	gradient0 = new QLinearGradient(xCoord[BLACK], yCoord, xCoord[BLACK], yCoord + rect_height);
	
	gradient0->setSpread(QGradient::RepeatSpread);
	gradient0->setColorAt(0, RGB_WHITE);
	gradient0->setColorAt(colData-0.01, RGB_WHITE);
	gradient0->setColorAt(colData, RGB_BLACK);
	gradient0->setColorAt(1, RGB_BLACK);

	colData = 1.0 - (float)pump->InkQuantity[CYAN] / 100;
	gradient1 = new QLinearGradient(xCoord[CYAN], yCoord, xCoord[CYAN], yCoord + rect_height);
	
	gradient1->setSpread(QGradient::RepeatSpread);
	gradient1->setColorAt(0, RGB_WHITE);
	gradient1->setColorAt(1, RGB_CYAN);
	gradient1->setColorAt(colData-0.01, RGB_WHITE);
	gradient1->setColorAt(colData, RGB_CYAN);
	
       	colData = 1.0 - (float)pump->InkQuantity[2] / 100;
	gradient2 = new QLinearGradient(370, yCoord, 370, 440);
	gradient2->setSpread(QGradient::RepeatSpread);
	gradient2->setColorAt(0, RGB_WHITE);
	gradient2->setColorAt(colData - 0.01, RGB_WHITE);
	gradient2->setColorAt(colData, RGB_MEGENTA);
	gradient2->setColorAt(1, RGB_MEGENTA);

	colData = 1.0 - (float)pump->InkQuantity[3] / 100;
	gradient3 = new QLinearGradient(370, yCoord, 370, 440);
	gradient3->setSpread(QGradient::RepeatSpread);
	gradient3->setColorAt(0, RGB_WHITE);
	gradient3->setColorAt(colData - 0.01, RGB_WHITE);
	gradient3->setColorAt(colData, RGB_YELLOW);
	gradient3->setColorAt(1, RGB_YELLOW);

	colData = 1.0 - (float)pump->InkQuantity[4] / 100;
	gradient4 = new QLinearGradient(370, yCoord, 370, 440);
	gradient4->setSpread(QGradient::RepeatSpread);
	gradient4->setColorAt(0, RGB_WHITE);
	gradient4->setColorAt(colData - 0.01, RGB_WHITE);
	gradient4->setColorAt(colData, RGB_LCYAN);
	gradient4->setColorAt(1, RGB_LCYAN);

	colData = 1.0 - (float)pump->InkQuantity[5] / 100;
	gradient5 = new QLinearGradient(370, yCoord, 370, 440);
	gradient5->setSpread(QGradient::RepeatSpread);
	gradient5->setColorAt(0, RGB_WHITE);
	gradient5->setColorAt(colData - 0.01, RGB_WHITE);
	gradient5->setColorAt(colData, RGB_LMEGENTA);
	gradient5->setColorAt(1, RGB_LMEGENTA);
}

enum Status{
	READY		= 0,
	UPDATE		= 1,
	ERROR		= 3,
	OFFLINE		= 5,
	INITIALIZE	= 7,
	ABORT		= 9,
	PAUSE		= 11,
	PRINTING	= 13,
	MOVING		= 15,
	WARNING		= 21,
	REMOTE		= 23
};
static QString StatusLabel[] = {
	"Ready",
	"UPDATING",	//1
	"",
	"ERROR",	//3
	"",
	"OFFLINE",	//5
	"",
	"INITIALIZE",	//7
	"",
	"ABORT",	//9
	"",
	"PAUSE",	//11
	"",
	"PRINTING",	//13
	"",
	"MOVING",	//15
	"",
	"",
	"",
	"",
	"",
	"WARNING",	//21
	"",
	"REMOTE",	//23
};

void FindErrorExplain(unsigned int err, QString& explain)
{
	QString hexstr = "hex" +  QString::number(err, 16);
	QString inputFilePath("./error_code.xml");

	if (!QFile::exists(inputFilePath)){
		qDebug() << "invalied path";
		explain = "unknow error: " + hexstr;
		return;
	}
	QFile inputFile(inputFilePath);
	if (!inputFile.open(QIODevice::ReadOnly)){
		qDebug() << "open failed";
		explain = "unknow error: " + hexstr;
		return;
	}
	QXmlStreamReader reader(&inputFile);
	while (!reader.atEnd())
	{
		if (reader.error()){
			break;
		}else{
			if(reader.isStartElement()){
				if(reader.name() == hexstr){
					explain = reader.readElementText();
					break;
				}
			}
			reader.readNext();
		}
	}

	if(explain == ""){
		explain = "unknow error: " + hexstr;
	}
}

void mainDialog::ProcessPrintStatus()
{	QWidget * window = QApplication::activeWindow();
	if(window != this){

	}
	static STATUS  oldstatus = {(unsigned int)~0};

	STATUS  status;
	GetPrinterStatus(&status);
	if(status.error_code == oldstatus.error_code){
		return;
	}
	/* if the status is same to the last one, return without do anything */
	int s = status.error_code >> 24;
	int os = oldstatus.error_code >> 24;
	if(s < 24){
		statusLabel->setText(StatusLabel[s]);
	}else{
		statusLabel->setText("Unkown Error");
	}

	QString info;
	FindErrorExplain(status.error_code, info);
	if(s > 0 && s != ERROR){
		messageLabel->setText(info);
	}
	if((s != PRINTING) && (s != PAUSE) /* || (s == ABORT) */){
		Tool->GetAbortButton()->setDisabled();
		Tool->GetPauseButton()->setDisabled();
	}

	if(s == READY){
		Tool->setMoveEnabled(true);
		
		INK_PUMP * pump = (INK_PUMP*)&status;
		if(memcpy(&InkPump, pump, sizeof(INK_PUMP))){
			memcpy(&InkPump, pump, sizeof(INK_PUMP));
			cycleLineEdit->setText(QString::number(pump->InkCyclePressure));
			pumpLineEdit->setText(QString::number(pump->InkSupplyPressure));
		}
		emit ready();
	}else if(s == ERROR){
		if(status.error_code & SOFTWARE){
			int r = QMessageBox::warning(NULL, 
				tr("ERROR"), info, QMessageBox::Cancel);
			if (r == QMessageBox::Cancel) {
				ClearPrinterStatus(status.error_code);
			}
		}
		emit error(info);
	}else if(s == OFFLINE){
		if(status.error_code & MOTION_BOARD){
			Tool->setMoveEnabled(false);
		}else if (status.error_code & HEAD_BOARD){
			int r = QMessageBox::critical(NULL, 
				tr("ERROR"), info, QMessageBox::Cancel | QMessageBox::Yes);
			if (r == QMessageBox::Cancel) {
				ClearPrinterStatus(status.error_code);
			}else{
				Tool->setMoveEnabled(false);
			}
		}
		emit error(info);
	}else if(s == PRINTING){
		Tool->GetAbortButton()->setEnabled();
		Tool->GetPauseButton()->setEnabled();
		Tool->setMoveEnabled(false);
		emit printing(info);
	}else if(s == MOVING){
		qDebug() << "moving...";
		emit moving();
	}else if(s == PAUSE){
		emit pause();
	}

	oldstatus.error_code = status.error_code;
}

class PrintThread : public QThread {
public:
	PrintThread(QString &files){
		PrintFiles = files;
	}
protected:
	virtual void run(){
		//PRINT_MODER mode;
		//JOB_INFO  job;
		if(PrintImage(PrintFiles.toStdString().c_str(), NULL, NULL) != 0){
			qDebug() << "print job error";
		}
	}
private:
	QString PrintFiles;
};

void mainDialog::Print()
{
	QFileDialog *fileDialog = new QFileDialog();
	fileDialog->setWindowTitle("选择文件");
	fileDialog->setDirectory(".");
	fileDialog->setNameFilter(tr("Images(*.prn *.prt)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	
	QString filename;
	if(fileDialog->exec()){
		filename = fileDialog->selectedFiles()[0];
		qDebug() << "selected file is" << filename;
	
		PrintThread * thread = new PrintThread(filename);	
		thread->start();
	}
}

void mainDialog::PrintNozzleCheck()
{
	PrintCalibration(CMD_MECHINE_CHECK_NOZZLE, 0, 0, 0);
}

void mainDialog::Pause()
{
	PrintPause();
}

void mainDialog::Abort()
{
	PrintAbort();
}

void mainDialog::PowerOff()
{
	int r = QMessageBox::warning(NULL, 
		tr("Poweroff"), 
		"Are you sure to turn off your computer?", \
		QMessageBox::Cancel | QMessageBox::Ok);

	if(r == QMessageBox::Ok){
		system("poweroff");
	}
}
