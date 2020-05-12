#include <QProcess>
#include <QApplication>
#include <QMessageBox>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <iostream>
#include <fstream>
using namespace std;

#include "menu.h"
#include "sha256.h"

struct UpadtaPackage{
    unsigned int Header;
    unsigned int Length;
    unsigned int Target;
    unsigned int Offset;
    unsigned int Ency;
    unsigned char Hash[32];
};


void UiSetting::Backup()
{
    system("./backup.sh");
}

//删除文目录
bool removeDir(QString path)
{
    if (path.isEmpty()){
        return false;
    }
    QDir dir(path);
    if(!dir.exists()){
        return true;
    }
    dir.setFilter(QDir::AllEntries | QDir::NoDotAndDotDot); //设置过滤
    QFileInfoList fileList = dir.entryInfoList(); // 获取所有的文件信息
    foreach (QFileInfo file, fileList)
    {
        //遍历文件信息
        if (file.isFile())
        {
            // 是文件，删除
            file.dir().remove(file.fileName());
        }
        else
        {
            // 递归删除
            removeDir(file.absoluteFilePath());
        }
    }
    return dir.rmpath(dir.absolutePath()); // 删除文件夹
}

//判断文件目录是否存在
bool isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists())
    {
      return true;
    }
    return false;
}

//目录复制(复制前需要删除目录下目标文件)
bool FileCopy(QString OriginPath,QString TargetPath)
{
    QDir OriginDir(OriginPath);
    QDir TargetDir(TargetPath);
    
    if(!TargetDir.exists())
    {
        if(!TargetDir.mkdir(TargetDir.absolutePath()))
        {
            printf("The TargetDir Create Fail!\n");
            return false;
        }
    }
    
    if(isDirExist(OriginPath))
    {
        QStringList OriginFile = OriginDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
        if(!OriginFile.isEmpty())
        {
            for(int i=0; i<OriginFile.count();i++)
            {
                QStringList FileList = OriginFile.at(i).split("/");
                QString TarCopyFile = TargetPath + "/" + FileList.last();
                QString OrigDateFile = OriginPath + "/" + OriginFile.at(i);

                OrigDateFile.replace("\\","/");
                TarCopyFile.replace("\\","/");
                QFileInfo tarfo(TarCopyFile);
                if(tarfo.isFile())
                {
                    QFile::remove(TarCopyFile);
                }
                bool flag = QFile::copy(OrigDateFile,TarCopyFile);
                printf("%s\n",flag?"success!":"failed!");
                if(!flag)
                {
                    perror("copy");
                    qDebug() << OrigDateFile;
                    qDebug() << TarCopyFile;
                }

            }
        }
        QStringList OriginFileDir = OriginDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
        if(!OriginFileDir.isEmpty())
        {
            for(int i=0; i<OriginFileDir.count(); i++)
            {
                if(!FileCopy(OriginPath + "/" + OriginFileDir.at(i),TargetPath + "/" + OriginFileDir.at(i)))
                {
                    perror("copy");
                    qDebug() << OriginPath + "/" + OriginFileDir.at(i);
                    qDebug() << TargetPath + "/" + OriginFileDir.at(i);
                    return false;
                }

            }
        }
    }
    else
    {
        return false;
    }
    return true;

}

//升级文件文件拷贝分函数
bool UiSetting::CopyTheFile(QString FilePath, QString BuildPath)
{
    QString SettingPath = FilePath + "/setting";
    if(isDirExist(SettingPath))
    {
        int r = QMessageBox::warning(nullptr,
            tr("Warning"), "Are you sure to replace the file named 'setting'? ",QMessageBox::No|QMessageBox::Yes);
        if (r == QMessageBox::Yes){
            qDebug()<<"current currentPath: "<<QDir::currentPath();
            FileCopy(SettingPath,BuildPath + "/setting");
        }

        QDir FileDir(FilePath);
        QStringList FileDataList = FileDir.entryList(QDir::NoDotAndDotDot | QDir::Files);
        if(!FileDataList.isEmpty())
        {
            for(int i=0; i<FileDataList.count(); i++)
            {
                QString origpath = FilePath + "/" + FileDataList.at(i);
                origpath.replace("\\","/");
                if(FileDataList.at(i) == QString::fromLocal8Bit("libpmd.so"))
                {
                    QFileInfo tarinfo("/usr/lib/libpmd.so");
                    if(tarinfo.isFile())
                    {
                        QFile::remove("/usr/lib/libpmd.so");
                    }
                    if(!QFile::copy(origpath,"/usr/lib/libpmd.so"))
                    {
                        perror("Out QFile::copy");
                        qDebug() << origpath;
                        qDebug() << "/usr/lib/libpmd.so";
                    }
                }
                else
                {
                    QString tarpath = BuildPath + "/" + FileDataList.at(i);
                    tarpath.replace("\\","/");
                    QFileInfo tarfo(tarpath);
                    if(tarfo.isDir())
                    {
                        FileCopy(origpath,tarpath);
                    }
                    else
                    {
                        QFileInfo tarinfo(tarpath);
                        if(tarinfo.isFile())
                        {
                            QFile::remove(tarpath);
                        }
                        if(!QFile::copy(origpath,tarpath))
                        {
                            perror("Out QFile::copy");
                            qDebug() << origpath;
                            qDebug() << tarpath;
                        }
                    }

                }
            }
        }

        QStringList FileDirList = FileDir.entryList(QDir::NoDotAndDotDot | QDir::Dirs);
        for(int i=0; i<FileDirList.count(); i++)
        {
            if(FileDirList.at(i) == QString::fromLocal8Bit("setting"))
            {
                continue;
            }
            else
            {
                QString origpath = FilePath + "/" + FileDirList.at(i);
                origpath.replace("\\","/");
                QString tarpath = BuildPath + "/" + FileDirList.at(i);
                tarpath.replace("\\","/");
                if(!FileCopy(origpath,tarpath))
                {
                    perror("Out QFile::copy");
                    qDebug() << origpath;
                    qDebug() << tarpath;
                }
            }
        }
        
    }
    return true;
}


void UiSetting::Update()
{
	QFileDialog *fileDialog = new QFileDialog();
    fileDialog->setWindowTitle(tr("Selection File"));
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
	
        /*
		QString cmd = "./update.sh ";
		cmd += filename;
		qDebug() << cmd;
		std::string str = cmd.toStdString();
		system(str.c_str());
        */
        UpadtaPackage date;
        //FILE *fp = fopen("update.gz","rb");
        FILE *fp = fopen(filename.toStdString().c_str(),"rb");
        fseek(fp,0,SEEK_SET);
        fread(&date,sizeof(date)+1,1,fp);

        /*
        cout << date.Header << endl
             << date.Length << endl
             << date.Target << endl
             << date.Offset << endl
             << date.Ency << endl
             << date.Hash << endl;
        */
        /*
        for(int i=0;i<strlen((char*)date.Hash);i++)
        {
            printf("%2x ",date.Hash[i]);
        }
        printf("\n");
        */

        fseek(fp,date.Offset,SEEK_SET);
        unsigned char* Filebuf = new unsigned char [date.Length];
        size_t BufSize = fread(Filebuf, 1, date.Length,fp);
        if(BufSize == NULL)
        {
            perror("fread");
        }
        fclose(fp);


        //shar256
        unsigned char shaOut[32];
        sha256_run(Filebuf,BufSize,shaOut);
        int compare_i = 0;
        for(; compare_i<32; compare_i++)
        {
            if(shaOut[compare_i] != date.Hash[compare_i] || BufSize != date.Length)
            {
                int r = QMessageBox::critical(nullptr,
                    tr("Error"), "The file is not the same! Update failed!",QMessageBox::Yes);
                if (r == QMessageBox::Yes){
                    delete[] Filebuf;
                    qApp->exit(773);
                }
            }
            //printf("%2x ",shaOut[compare_i]);
        }
        //printf("\n");

        if(compare_i >= 32)
        {
            FILE *data_tar = fopen("dataUp.tar","wb");
            fwrite(Filebuf,date.Length,1,data_tar);
            fclose(data_tar);
            delete[] Filebuf;

            system("tar -xvf dataUp.tar");
            QString gzuiPath =  QDir::currentPath() + "/gzui";
            CopyTheFile(gzuiPath,QDir::currentPath());
        }

        remove("dataUp.tar");
        removeDir(QDir::currentPath() + "/gzui");



        int r = QMessageBox::warning(nullptr,
			tr("Update"), "Update finished. click yes to restart", QMessageBox::Cancel | QMessageBox::Yes);
		if (r == QMessageBox::Yes){
            DestroyPrinter(nullptr);
			qApp->exit(773);
		}
        else if(r == QMessageBox::Cancel){
            return;
        }
	}
}

