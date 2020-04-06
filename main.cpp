#include <unistd.h>
#include <stdlib.h>

#include <QProcess>
#include <QApplication>

#include "mainwindow.h"

#include "ui_interface.h"


int main(int argc, char *argv[])
{

        int ch;
        while ((ch = getopt(argc, argv, "v:d")) != -1) {
                switch (ch) {
                        case 'v':
			{
				qDebug() << UI_VERSION;
			       	//qDebug() << GetPmVersion();
				return 0;
			}
			case 'd':
			{
#ifdef DEBUG
				printf("debug\n");
				return 1;
#else
				printf("release\n");
				return 0;
#endif
			}
			default:
			break;
		}
	}

	if(CreatePrinter(0)){
		return -1;	
	}

	QApplication app(argc, argv);

	mainDialog dialog;
	dialog.show();

	int ret = app.exec();

	qDebug() << "exit";
	//if(ret == 773){
	//	QProcess::startDetached(qApp->applicationFilePath(), QStringList());
	//	return 0;
	//}

	return ret;
}
