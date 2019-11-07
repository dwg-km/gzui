
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
	INK_PUMP * pump = new INK_PUMP;
	GetPrinterParam(UI_CMD::CMD_MECHINE_INK_PUMP, pump);

	int xCoord[8];
	const int yCoord = 0;
	const int ColorNum = 6;
	const int lenght = height();
	const int rect_width = width() / ColorNum * 3 / 4;
	const int rect_height = height() - yCoord;

	for(int c = 0; c < ColorNum; c++){
		xCoord[c] = c * width() / ColorNum;
	}

	QPainter painter(this);
	painter.setPen(QPen(QColor(221, 227, 227), 1));

	float colData = 1.0 - (float)pump->InkQuantity[BLACK] / 100;
	QLinearGradient gradient0(xCoord[BLACK], yCoord, xCoord[BLACK], yCoord + rect_height);
	
	//gradient0.setSpread(QGradient::RepeatSpread);
	gradient0.setColorAt(0, RGB_WHITE);
	gradient0.setColorAt(colData-0.01, RGB_WHITE);
	gradient0.setColorAt(colData, RGB_BLACK);
	gradient0.setColorAt(1, RGB_BLACK);

	//QRect(xCoord[BLACK], yCoord, xCoord[BLACK], yCoord + rect_height);
	//QRect(xCoord[BLACK], yCoord, xCoord[BLACK], yCoord + rect_height * colData);
	//gradient0.setColorAt(0, RGB_BLACK);
	//gradient0.setColorAt(1, RGB_BLACK);
	
	painter.setBrush(gradient0);
	painter.drawRect(xCoord[BLACK], yCoord, rect_width, rect_height);
	//QString persent = QString::number(pump->InkQuantity[BLACK]) + "%";
	//label1->setText(persent);

	colData = 1.0 - (float)pump->InkQuantity[CYAN] / 100;
	QLinearGradient gradient1(xCoord[CYAN], yCoord, xCoord[CYAN], yCoord + rect_height);
	
	gradient1.setSpread(QGradient::RepeatSpread);
	gradient1.setColorAt(0, RGB_WHITE);
	gradient1.setColorAt(1, RGB_CYAN);
	gradient1.setColorAt(colData-0.01, RGB_WHITE);
	gradient1.setColorAt(colData, RGB_CYAN);
	
	painter.setBrush(gradient1);
	painter.drawRect(xCoord[CYAN], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[CYAN]) + "%";
	//label2->setText(persent);

       	colData = 1.0 - (float)pump->InkQuantity[2] / 100;
	QLinearGradient gradient2(370, yCoord, 370, 440);
	gradient2.setSpread(QGradient::RepeatSpread);
	gradient2.setColorAt(0, RGB_WHITE);
	gradient2.setColorAt(colData - 0.01, RGB_WHITE);
	gradient2.setColorAt(colData, RGB_MEGENTA);
	gradient2.setColorAt(1, RGB_MEGENTA);
	painter.setBrush(gradient2);
	painter.drawRect(xCoord[MEGENTA], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[MEGENTA]) + "%";
	//label3->setText(persent);

	colData = 1.0 - (float)pump->InkQuantity[3] / 100;
	QLinearGradient gradient3(370, yCoord, 370, 440);
	gradient3.setSpread(QGradient::RepeatSpread);
	gradient3.setColorAt(0, RGB_WHITE);
	gradient3.setColorAt(colData - 0.01, RGB_WHITE);
	gradient3.setColorAt(colData, RGB_YELLOW);
	gradient3.setColorAt(1, RGB_YELLOW);
	painter.setBrush(gradient3);
	painter.drawRect(xCoord[YELLO], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[YELLO]) + "%";
	//label4->setText(persent);

	colData = 1.0 - (float)pump->InkQuantity[4] / 100;
	QLinearGradient gradient4(370, yCoord, 370, 440);
	gradient4.setSpread(QGradient::RepeatSpread);
	gradient4.setColorAt(0, RGB_WHITE);
	gradient4.setColorAt(colData - 0.01, RGB_WHITE);
	gradient4.setColorAt(colData, RGB_LCYAN);
	gradient4.setColorAt(1, RGB_LCYAN);
	painter.setBrush(gradient4);
	painter.drawRect(xCoord[LIGHT_CYAN], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[LIGHT_CYAN]) + "%";
	//label5->setText(persent);


	colData = 1.0 - (float)pump->InkQuantity[5] / 100;
	QLinearGradient gradient5(370, yCoord, 370, 440);
	gradient5.setSpread(QGradient::RepeatSpread);
	gradient5.setColorAt(0, RGB_WHITE);
	gradient5.setColorAt(colData - 0.01, RGB_WHITE);
	gradient5.setColorAt(colData, RGB_LMEGENTA);
	gradient5.setColorAt(1, RGB_LMEGENTA);
	painter.setBrush(gradient5);
	painter.drawRect(xCoord[LIGHT_MEGENTA], yCoord, rect_width, rect_height);
	//persent = QString::number(pump->InkQuantity[LIGHT_MEGENTA]) + "%";
	//label6->setText(persent);
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

void mainDialog::GetPrintStatus()
{
	STATUS  status;
	GetPrinterStatus(&status);
	int s = status.error_code >> 24 & 0xFF;
	if(s == READY){
		statusLabel->setText(StatusLabel[s]);

		INK_PUMP * pump = (INK_PUMP*)&status;

		if(memcpy(&InkPump, pump, sizeof(INK_PUMP))){
			memcpy(&InkPump, pump, sizeof(INK_PUMP));
			cycleLineEdit->setText(QString::number(pump->InkCyclePressure));
			pumpLineEdit->setText(QString::number(pump->InkSupplyPressure));
		}
	}else if(s == ERROR){
		statusLabel->setText(StatusLabel[s]);
		if(status.error_code & SOFTWARE){
			int r = QMessageBox::warning(this, tr("ERROR"), tr("click yes to clean the error\n"),
				QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
			if (r == QMessageBox::Yes) {
				ClearPrinterStatus(status.error_code);
			}
		}
	}else if(s < 24){
		statusLabel->setText(StatusLabel[s]);
	}else {
		statusLabel->setText("Unkown Error");
	}
/*
    QString errorParam;
    if(0 != currentStatus)
    {
        QString str = QString(status.context);
        if(0 == str.size())
        {
            errorParam = QString("unknown error:") + QString::number(status.error_code, 16);
        }
        else
        {
            errorParam = QString(status.context) + QString(":") + QString::number(status.error_code, 16);
        }
        ui->labelCurStatus->setText(errorParam);
    }
*/
}

void mainDialog::OpenFile()
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
		
		//PRINT_MODER mode;
		//JOB_INFO  job;
		if(PrintImage(filename.toStdString().c_str(), NULL, NULL) != 0){
			qDebug() << "print job error";
		}
	}
}


