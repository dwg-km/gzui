#ifndef TOUCHKEY_H
#define TOUCHKEY_H


#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>

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

	void setPointEnable(bool enable){
		m_buttons[10]->setEnabled(enable);
                typefloat = enable;
	}

    void setIsNotString(bool state)
    {
        IsNotString = state;
    }

signals:
	void ok();

public slots:
	void onCalculate();
	void add();
	void KillTouchkey(){
		if(++timer > 3){
			close();
			killTimer->stop();
		}
	}
	
private:
	TouchKey();
	bool Construct();

private:

	QTimer * killTimer;
	unsigned int timer = 0;
/*
	virtual void focusOutEvent(QFocusEvent *event){
		//qDebug() << "focus out event";
		//touchkey->setWindowFlags(Qt::WindowStaysOnTopHint);
	}
	virtual void hideEvent(QHideEvent * event){
		qDebug() << "hide event";
		//close();
	}
*/
private:
	QPushButton * m_buttons[15];
	QString str;
        bool typefloat;
        bool IsNotString = true;
	QLineEdit * label;
	QLineEdit * m_edit;
};

#endif // TOUCHKEY_H
