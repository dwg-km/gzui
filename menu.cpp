
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
	
	QString filename;
	if(fileDialog->exec()){
		filename = fileDialog->selectedFiles()[0];
		qDebug() << "selected file is" << filename;
	
		QString cmd = "./update.sh ";
		cmd += filename;
		qDebug() << cmd;
		system(cmd.toStdString().c_str());
	}
}
