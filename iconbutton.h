#ifndef NEW_PUSH_BUTTON_H
#define NEW_PUSH_BUTTON_H

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
 
#include <QPushButton>
#include <QString>
#include <QWidget>
 
class iconButton : public QPushButton
{
	Q_OBJECT

	Q_PROPERTY(int curIndex READ getcurIndex WRITE setcurIndex)
			 
public:
	
	iconButton( QString FileName, int xnum  = 1,  QWidget *parent = 0,
					                int ynum = 1, QString bkGrnd = NULL);
	iconButton(QVector<QString> &list, QWidget *parent = 0, QString bkGrnd = NULL);
				 
	QList<QPixmap> *getPixmapList(void){return &pixmatpList;}
	void setcurIndex(int index){curIndex = index; update();}
	int getcurIndex(void){return curIndex;}

private:

	void setPixmapList(QVector<QString> &list);
	
	QPixmap *background;
						     
protected:
	
	QList<QPixmap> pixmatpList;
	
	int curIndex;
							    
protected:
	virtual void paintEvent ( QPaintEvent * event);
	//virtual void enterEvent(QEvent *event);
	virtual void leaveEvent ( QEvent * event );
	virtual void mousePressEvent ( QMouseEvent * event ) ;
	virtual void mouseReleaseEvent ( QMouseEvent * event );
	virtual  QSize sizeHint () const;									     
};
 
#endif // CLOSEBUTTON_H

#endif
