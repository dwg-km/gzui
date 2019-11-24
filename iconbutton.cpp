
#include <QPainter>
#include <QPaintEvent>
#include <QPixmap>
#include <QLabel>
#include <QDebug>


#include "iconbutton.h"

iconButton::iconButton( QString FileName, int xnum , QWidget *parent, 
		int ynum, QString bkGrnd)
#if WITH_QLABEL
	: QLabel(parent),
#else
	: QPushButton(parent), curIndex(0)
#endif
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
}
 
iconButton::iconButton(QVector<QString> &list, QWidget *parent, QString bkGrnd)
#if WITH_QLABEL
	: QLabel(parent), 
#else
	: QPushButton(parent), curIndex(0)
#endif
{
	setPixmapList(list);
	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
	if (bkGrnd != NULL)
		background  = new QPixmap(bkGrnd);
	else
		background = NULL;

	const QImage image(list[0]);
	icon = QPixmap::fromImage(image);
	pixmatpList.push_back( 
		icon.pixmap(QSize(60, 60), QIcon::Selected, QIcon::Off));
	pixmatpList.push_back( 
		icon.pixmap(QSize(60, 60), QIcon::Disabled, QIcon::Off));
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
 
#ifndef	WITH_QLABEL
void iconButton::paintEvent ( QPaintEvent * event)
{
	QPainter painter(this);
	painter.drawPixmap(event->rect(), pixmatpList[curIndex]);
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
	setPixmap(pixmatpList[2]);
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
	setPixmap(pixmatpList[0]);
	update();
#else	
	curIndex = 1;
	QPushButton::mouseReleaseEvent(event);
#endif

	emit keyReleased();
}
 
QSize iconButton::sizeHint() const
{
	return background != NULL? QSize(background->width(), background->height()):
		QSize(pixmatpList[0].width(), pixmatpList[0].height());
}

