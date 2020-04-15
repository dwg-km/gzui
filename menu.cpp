#include <QProcess>
#include <QApplication>
#include <QMessageBox>

#include "menu.h"




void UiSetting::Backup()
{
	system("./backup.sh");
}
void UiSetting::Update()
{
	QFileDialog *fileDialog = new QFileDialog();
	fileDialog->setWindowTitle("选择文件");
	fileDialog->setDirectory(".");
	fileDialog->setFileMode(QFileDialog::ExistingFiles);
	
	QDir dir("/media");
	dir.setFilter(QDir::Dirs | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	QFileInfoList list = dir.entryInfoList();
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
	
		QString cmd = "./update.sh ";
		cmd += filename;
		qDebug() << cmd;
		std::string str = cmd.toStdString();
		system(str.c_str());

		int r = QMessageBox::warning(NULL, 
			tr("Update"), "Update finished. click yes to restart", QMessageBox::Cancel | QMessageBox::Yes);
		if (r == QMessageBox::Yes){
			DestroyPrinter(0);
			qApp->exit(773);
		}
	}
}


