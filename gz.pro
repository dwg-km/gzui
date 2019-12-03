
QT       += core gui dbus 
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 

CONFIG		+= debug

INCLUDEPATH	= lib \
		usb \
		touchkey

TEMPLATE      = app
HEADERS       = mainwindow.h \
		menu.h \
		cali.h \
		iconbutton.h\
		toolbar.h \
		UiTemplate.h \
		touchkey/lineedit.h \
		touchkey/touchkey.h \
		lib/ui_interface.h \
		lib/APIDataInterface.hpp \
		lib/Error.h \
		lib/command.h \
		usb/haldevice.h  \
		usb/usbmanager.h

SOURCES       = main.cpp \
		menu.cpp \ 
		iconbutton.cpp \
		mainwindow.cpp \
		toolbar.cpp \ 
		UiTemplate.cpp \
		touchkey/linedit.cpp \
		touchkey/touchkey.cpp \
		usb/haldevice.cpp  \
		usb/usbmanager.cpp

SUBDIRS		= lib \
		touchkey 



unix:!macx: LIBS += ../lib/libpmd.so 
