#ifndef NEW_PUSH_BUTTON_H
#define NEW_PUSH_BUTTON_H

#include <QDebug>
#include <QIcon>
#include <QPushButton>
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
		if(s < pixmatpList.size()){
			status = s;
			update();
		}
		return 0;
	}

private:

	void setPixmapList(QVector<QString> &list);
	
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
 
#endif // CLOSEBUTTON_H

#endif
