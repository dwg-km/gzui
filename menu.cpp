
#include "menu.h"

#include "exec.h"

void UiSetting::Backup()
{

	//exec_cmd(RESUME_BAK);
}
void UiSetting::Update()
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
	
		//exec_cmd(UPDATE_PKG);
	}
}
