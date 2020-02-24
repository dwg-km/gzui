#include <QThread>
#include <QApplication>
#include <QXmlStreamReader>

#include "mainwindow.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"

enum ColorIndex {
	BLACK		= 0,
	CYAN		= 1,
	MAGENTA		= 2,
	YELLOW		= 3,
	LCYAN		= 4,
	LMAGENTA	= 5
};

#define		RGB_WHITE	QColor(255, 255, 255)
#define		RGB_BLACK	QColor(35, 31, 32)
#define		RGB_CYAN	QColor(0, 159, 221)
#define		RGB_MAGENTA	QColor(194, 0, 144)
#define		RGB_YELLOW	QColor(244, 231, 0)
#define		RGB_LMAGENTA	QColor(231, 143, 194)
#define		RGB_LCYAN	QColor(140, 203, 226)

#define		DRAW_COLOR_BAR(c)	do{\
	QString percent = QString::number(InkPump.InkQuantity[c]) + "%";\
	h =(double)InkPump.InkQuantity[c] / 100 * rect_height;\
	painter.setBrush(QBrush(RGB_##c, Qt::SolidPattern));\
	painter.drawRect(xCoord[c], height() - h, rect_width, h);\
	painter.drawRect(xCoord[c], height() - h, rect_width, h);\
	painter.drawText(xCoord[c], height() - h, percent);\
}while(0)

void InkWidget::paintEvent(QPaintEvent *e)
{
	e = e;
	QPainter painter(this);

	qDebug() << "paint ink event";

	int h = 0;
	//const int yCoord = height() * 0.1;
	const int rect_width = width() / ColorNum * 3 / 4;
	const int rect_height = height() * 0.9;

	int xCoord[8];
	for(int c = 0; c < ColorNum; c++){
		xCoord[c] = c * width() / ColorNum;
	}

	DRAW_COLOR_BAR(BLACK);
	DRAW_COLOR_BAR(CYAN);
	DRAW_COLOR_BAR(MAGENTA);
	DRAW_COLOR_BAR(YELLOW);
	DRAW_COLOR_BAR(LCYAN);
	DRAW_COLOR_BAR(LMAGENTA);
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

	static STATUS  oldstatus = {(unsigned int)~0, {0}};

	STATUS  status;
	GetPrinterStatus(&status);
	if(status.error_code == oldstatus.error_code){
		return;
	}
	/* if the status is same to the last one, return without do anything */
	int s = status.error_code >> 24;
	//int os = oldstatus.error_code >> 24;
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
		Tool->GetNetworkButton()->SetStatus(0);
		
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
		//emit moving();
	}else if(s == PAUSE){
		emit pause();
	}
	//if(s == REMOTE){
	//	Tool->GetNetworkButton()->SetStatus(2);
	//}

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
		std::string str = PrintFiles.toStdString();
		if(PrintImage(str.c_str(), NULL, NULL) != 0){
			qDebug() << "print job error";
		}
	}
private:
	QString PrintFiles;
};

void mainDialog::Flash()
{
	//SendMotionCmd(UI_CMD::CMD_MOTION_FLASH, 0);
	SendHbCmd(UI_CMD::CMD_MOTION_FLASH, 0, 0, 0);
}
void mainDialog::Print()
{
	QFileDialog *fileDialog = new QFileDialog();
	fileDialog->setWindowTitle("选择文件");
	fileDialog->setNameFilter(tr("Images(*.prn *.prt)"));
	fileDialog->setFileMode(QFileDialog::ExistingFiles);

	QDir dir("/media");
	dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	//dir.setSorting(QDir::Size | QDir::Reversed);
	QFileInfoList list = dir.entryInfoList();
	//for (int i = 0; i < list.size(); ++i) {
	//	QFileInfo fileInfo = list.at(i);
	//}
	if(list.size() == 0){
		fileDialog->setDirectory("/home");
	}else if(list.size() == 1){
		QString path = list.at(0).absoluteFilePath();
		qDebug() << "file=" << path;
		fileDialog->setDirectory(path);
	}else{
		fileDialog->setDirectory("/media");
	}

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

void mainDialog::Exit(){
	DestroyPrinter(0);
	//close();
	qApp->exit();
}

void mainDialog::PowerOff()
{
	int r = QMessageBox::warning(NULL, 
		tr("Poweroff"), 
		"Are you sure to turn off your computer?", \
		QMessageBox::Cancel | QMessageBox::Ok);

	if(r == QMessageBox::Ok){
		//disable all the button

		//close printer
		DestroyPrinter(0);

		//disconnect all the remote user

		//umount all the device

		system("poweroff");
	}
}

void mainDialog::deviceAdded(QString uid)
{
	qDebug() << uid;
}

void mainDialog::deviceRemoved(QString uid)
{
	qDebug() << uid;
}

void mainDialog::mediaChanged(const QString& media)
{
	std::string str = media.toStdString();
	SelectMedia(str.c_str());
}

void mainDialog::modelChanged(const QString& model)
{
	std::string str = model.toStdString();
	SelectProductModel(str.c_str());
}

void mainDialog::originChanged()
{
	ORIGIN_SET origin = Origin;
	origin.Coord = originLineEdit->text().toInt();
	origin.GetMode = orgComBox->currentIndex();
	if(origin.GetMode){
		originLineEdit->setEnabled(false);
	}else{
		originLineEdit->setEnabled(true);
	}
	if(memcmp(&Origin, &origin, sizeof(ORIGIN_SET))){
		SetPrinterParam(UI_CMD::CMD_MODE_ORIGIN, &origin);
	}
}

