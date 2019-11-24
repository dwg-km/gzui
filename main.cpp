#include <unistd.h>
#include <stdlib.h>

#include <QApplication>

#include "mainwindow.h"

#include "ui_interface.h"

#define	UI_VERSION	"ui 1.0.0"

int main(int argc, char *argv[])
{

        int ch;
        while ((ch = getopt(argc, argv, "v")) != -1) {
                switch (ch) {
                        case 'v':
			{
				qDebug() << UI_VERSION;
			       	qDebug() << GetPmVersion();
				return 0;
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
	
	return app.exec();
}
