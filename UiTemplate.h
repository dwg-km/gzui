#ifndef FINDFILEDIALOG_H
#define FINDFILEDIALOG_H

#include <QLabel>
#include <QDialog>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QFileDialog>

#include <QTableWidget>
#include <QDialog>

#include "toolbar.h"

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTableWidget;

class UiTemplate : public QWidget 
{
	Q_OBJECT

public:
	UiTemplate(QWidget *parent = 0);
	void Layout(QTabWidget * widgets = NULL);
	
protected:
	ToolBar * Tool;
	QLabel *logoLabel;
	QLabel *statusLabel;
	
	QLabel *messageLabel;
	QWidget * mainWidget;
	
	QVBoxLayout *leftLayout;
	QHBoxLayout *logoLayout;
	QGridLayout * mainLayout;
	QVBoxLayout *toolLayout;
	
	QHBoxLayout *mainwindow;
};

#endif
