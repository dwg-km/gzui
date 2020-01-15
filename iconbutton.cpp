
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QLabel>
#include <QDebug>


#include "iconbutton.h"
 
iconButton::iconButton(QVector<QString> &list, QWidget *parent)
#if WITH_QLABEL
	: QLabel(parent), 
#else
	: QPushButton(parent), curIndex(0), status(0)
#endif
{
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);

	QVector<QString>::iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++){
		QVector<QPixmap> pixel;
		QIcon icon = QPixmap::fromImage(QImage(*iter));
		pixel.push_back( 
			icon.pixmap(QSize(60, 60), QIcon::Active,  QIcon::Off));
		pixel.push_back( 
			icon.pixmap(QSize(60, 60), QIcon::Selected, QIcon::Off));
		pixel.push_back( 
			icon.pixmap(QSize(60, 60), QIcon::Disabled, QIcon::Off));

		pixmatpList.push_back(pixel);
	}
}
 
void iconButton::setPixmapList(QVector<QString> &list)
{
	/*
	if (list.size() <= 0)
		return;
	pixmatpList.clear();
	
	for (QVector<QString>::iterator pos = list.begin();
			pos != list.end(); ++pos)
	{
		pixmatpList.push_back(QPixmap(*pos));
	}
	*/
}
 
#ifndef	WITH_QLABEL
void iconButton::paintEvent ( QPaintEvent * event)
{
	QPainter painter(this);
	painter.drawPixmap(event->rect(), pixmatpList[status][curIndex]);
}
#endif

void iconButton::enterEvent(QEvent *event)
{
	curIndex = 1;
	//QPushButton::enterEvent(event);
}

void iconButton::leaveEvent(QEvent *event)
{
	curIndex = 0;
	//QPushButton::leaveEvent(event);
}

void iconButton::mousePressEvent(QMouseEvent *event)
{
#if WITH_QLABEL
	setPixmap(pixmatpList[status][2]);
	update();
#else	
	curIndex = 2;
	QPushButton::mousePressEvent(event);
#endif	

	emit keyPressed();
}
 
void iconButton::mouseReleaseEvent(QMouseEvent *event)
{
#if WITH_QLABEL
	setPixmap(pixmatpList[status][0]);
	update();
#else	
	curIndex = 1;
	QPushButton::mouseReleaseEvent(event);
#endif

	emit keyReleased();
}
 
QSize iconButton::sizeHint() const
{
	return QSize(pixmatpList[status][0].width(), pixmatpList[status][0].height());
}

