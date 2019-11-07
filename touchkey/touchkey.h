#ifndef TOUCHKEY_H
#define TOUCHKEY_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>

//#include "lineedit.h"

//class LineEdit;

class TouchKey:public QDialog
{

	Q_OBJECT

public:
	static TouchKey* touchkey;

	static TouchKey* newInstance();

	~TouchKey();
	
	void show();
	void setRet(QLineEdit * line);

signals:
	void ok();

public slots:
	void onCalculate();
	void add();
	void setPointEnable(bool enable){
		m_buttons[10]->setEnabled(enable);
	}
	
private:
	TouchKey();
	bool Construct();
	
private:
	QPushButton * m_buttons[15];
	QString str;
	QLineEdit * label;
	QLineEdit * m_edit;
};

#endif // TOUCHKEY_H
