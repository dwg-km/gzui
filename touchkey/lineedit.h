#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QDebug>
#include <QLineEdit>
#include <QValidator>

#include "touchkey.h"


#define	TOUCH_KEYBOARD

#ifdef	TOUCH_KEYBOARD
/*
class TouchKey;
class LineEdit : public QLineEdit
{
public:
	LineEdit(QWidget *parent = NULL) :
		QLineEdit(parent)
	{
	
	}
	virtual void setText(const QString& str) = 0;

protected:
	int Dirty;
};
*/
class IntLineEdit : public QLineEdit
{
public:
	IntLineEdit(QWidget *parent = NULL) :
		QLineEdit(parent)
	{
		touchkey = TouchKey::newInstance();
		setFocusPolicy(Qt::ClickFocus);
	}
	virtual void focusInEvent(QFocusEvent *event){
		touchkey->setPointEnable(false);
		touchkey->setRet(this);
		touchkey->exec();
	}
/*
	virtual void setText(const QString& str){
		if(str != QLineEdit::text()){
			Dirty = 1;
			QLineEdit::setText(str);
			qDebug() << "line reset to " << str;
		}
	}

	int IsDirty(){
		return Dirty;	
	}
private:
	int Dirty;
*/
private :
	TouchKey *touchkey;
};

class DoubleLineEdit : public QLineEdit
{
public:
	DoubleLineEdit(QWidget *parent = NULL) :
		QLineEdit(parent)
	{
		touchkey = TouchKey::newInstance();
		setFocusPolicy(Qt::ClickFocus);
	}
	virtual void focusInEvent(QFocusEvent *event){
		touchkey->setPointEnable(true);
		touchkey->setRet(this);
		touchkey->exec();
	}
private :
	TouchKey *touchkey;
};
#else

#endif


#endif // LINEEDIT_H
