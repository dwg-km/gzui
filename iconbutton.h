#ifndef NEW_PUSH_BUTTON_H
#define NEW_PUSH_BUTTON_H

#include <QHBoxLayout>
#include <QLineEdit>

#include <QDebug>
#include <QIcon>
#include <QPushButton>

#include "touchkey/touchkey.h"
#include "touchkey/lineedit.h"
/*
class NewPushButton : public QPushButton {
public:
	NewPushButton()

private:
};
*/

#ifndef CLOSEBUTTON_H
#define CLOSEBUTTON_H

#include <QLabel>
#include <QString>
#include <QPushButton>
#include <QString>
#include <QWidget>
 
//#define	WITH_QLABEL

class iconButton :
#ifdef	WITH_QLABEL
	public QLabel
#else
	public QPushButton
#endif
{
	Q_OBJECT

	Q_PROPERTY(int curIndex READ getcurIndex WRITE setcurIndex)
			 
public:
	
	iconButton(QVector<QString> &list, QWidget *parent = NULL);
				 
	//QList<QPixmap> *getPixmapList(void){return &pixmatpList;}
	void setcurIndex(int index){curIndex = index; update();}
	int getcurIndex(void){return curIndex;}
	
	void setDisabled(){
#ifdef	WITH_QLABEL
		setPixmap(pixmatpList[1]);
		update();
#else
		curIndex = 2;
		QPushButton::setEnabled(false);
#endif
	}
	void setEnabled(){
#ifdef	WITH_QLABEL
		setPixmap(pixmatpList[1]);
		update();
#else
		curIndex = 0;
		QPushButton::setEnabled(true);
#endif
	}
	int SetStatus(int s){
		if((s < pixmatpList.size()) && (status != s)){
			status = s;
			update();
		}
		return 0;
	}

private:

	//void setPixmapList(QVector<QString> &list);
	
	QPixmap *background;
						     
protected:
	
	//QIcon icon;
	QVector<QVector<QPixmap> > pixmatpList;

	int status;	
	int curIndex;
signals:
	void keyPressed(void);
	void keyReleased(void);	
protected:
#ifndef	WITH_QLABEL
	virtual void paintEvent ( QPaintEvent * event);
#endif
	virtual void enterEvent(QEvent *event);
	virtual void leaveEvent ( QEvent * event );
	virtual void mousePressEvent ( QMouseEvent * event ) ;
	virtual void mouseReleaseEvent ( QMouseEvent * event );
	virtual  QSize sizeHint () const;									     
};
//将标签和输入框组成一个窗口类
class TipLineEdit: public QWidget
{
    Q_OBJECT
public:
    TipLineEdit(QWidget *parent = NULL): QWidget(parent)
    {
        TipEdit = new QLabel;
        LineEdit = new DoubleLineEdit;
        TipLineLayout = new QGridLayout();
        m_TipLineEdit = new QWidget;

        LineEdit->getTouch()->setIsNotString(false);
        TipLineLayout->addWidget(TipEdit,0,0,1,1);
        TipLineLayout->addWidget(LineEdit,0,1,1,2);

        //m_TipLineEdit->setLayout(TipLineLayout);
        setLayout(TipLineLayout);

    }

public:

    void setLab(QString list)
    {
        TipEdit->setText(list);
    }

    void setLine(QString list)
    {
        LineEdit->setText(list);
    }

    QString getLine()
    {
        return LineEdit->text();
    }

    void setLineDisabel(bool state)
    {
        LineEdit->setDisabled(state);
    }

    QLineEdit * GetLineEditReturn()
    {
        return LineEdit;
    }


private:
    QLabel * TipEdit;
    DoubleLineEdit * LineEdit;
    QGridLayout * TipLineLayout;
    QWidget * m_TipLineEdit;

    TouchKey *touchkey;


};

 
#endif // CLOSEBUTTON_H

#endif

