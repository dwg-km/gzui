#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QDebug>
#include <QLineEdit>
#include <QValidator>
#include <QFocusEvent>

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
		QLineEdit::focusInEvent(event);
		//if(event->lostFocus()){
		//	return;
		//}
		
		touchkey->setPointEnable(false);
		touchkey->setRet(this);
		touchkey->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		//touchkey->setWindowModality(Qt::WindowModal);
		touchkey->setWindowModality(Qt::ApplicationModal);
		touchkey->exec();
		clearFocus();
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

    TouchKey * getTouch()
    {
        return touchkey;
    }

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
		QLineEdit::focusInEvent(event);
		//if(event->lostFocus()){
		//	return;
		//}
		touchkey->setPointEnable(true);
		touchkey->setRet(this);
		touchkey->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
		touchkey->setWindowModality(Qt::ApplicationModal);

		touchkey->exec();
		clearFocus();
	}

    TouchKey * getTouch()
    {
        return touchkey;
    }
private :
	TouchKey *touchkey;
};
#else

#endif


#endif // LINEEDIT_H
