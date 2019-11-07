
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QLabel>
#include <QDebug>
 
#include "iconbutton.h"

iconButton::iconButton( QString FileName, int xnum , QWidget *parent, 
		int ynum, QString bkGrnd)
	: QPushButton(parent)
{
	QPixmap pixmap(FileName);
	
	for (int cnt = 0; cnt < xnum; ++cnt)
	{
		for (int y = 0; y < ynum; ++ y)
		{
			pixmatpList.push_back( pixmap.copy(cnt * (pixmap.width() / xnum),
						y * (pixmap.height() / ynum),
						pixmap.width() / xnum ,
						pixmap.height() / ynum));
		}
	}
	
	if (bkGrnd != NULL)
		background  = new QPixmap(bkGrnd);
	else
		background = NULL;
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed	);
	curIndex = 0;
}
 
iconButton::iconButton(QVector<QString> &list, QWidget *parent, QString bkGrnd)
	: QPushButton(parent)
{
	setPixmapList(list);
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	if (bkGrnd != NULL)
		background  = new QPixmap(bkGrnd);
	else
		background = NULL;
	curIndex = 0;
}
 
void iconButton::setPixmapList(QVector<QString> &list)
{
	if (list.size() <= 0)
		return;
	pixmatpList.clear();
	
	for (QVector<QString>::iterator pos = list.begin();
			pos != list.end(); ++pos)
	{
		pixmatpList.push_back(QPixmap(*pos));
	}
}
 
void iconButton::paintEvent ( QPaintEvent * event)
{
	QPainter painter(this);
	painter.drawPixmap(event->rect(), pixmatpList[curIndex]);
}
/* 
void iconButton::enterEvent(QEvent *event)
{
	if (pixmatpList.size() > 1)
		curIndex = 1;
	else
		curIndex = 0;
	update();
	QPushButton::enterEvent(event);
}
*/
void iconButton::leaveEvent(QEvent *event)
{
	curIndex = 0;
	update();
	QPushButton::leaveEvent(event);
}
 
void iconButton::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton){
		if (pixmatpList.size() > 2)
		{
			curIndex = 2;
			update();
		}
	}
	
	QPushButton::mousePressEvent(event);
}
 
void iconButton::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		if (pixmatpList.size() > 1)
		{
			curIndex = 1;
			update();
		}
	}
	QPushButton::mouseReleaseEvent(event);
}
 
QSize iconButton::sizeHint() const
{
	return background != NULL? QSize(background->width(), background->height()):
		QSize(pixmatpList[0].width(), pixmatpList[0].height());
}

