#include <QApplication>

#include "mainwindow.h"

#include "ui_interface.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);

	if(CreatePrinter(0)){
		return -1;
	}


	mainDialog dialog;
	dialog.show();
	
	return app.exec();
}
