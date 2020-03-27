
QT       += core gui  
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 

CONFIG		+= debug

INCLUDEPATH	= lib \
		touchkey

TEMPLATE      = app
HEADERS       = mainwindow.h \
		menu.h \
		cali.h \
		motion.h \
		wave.h \
		iconbutton.h\
		toolbar.h \
		UiTemplate.h \
		touchkey/lineedit.h \
		touchkey/touchkey.h \
		lib/ui_interface.h \
		lib/APIDataInterface.hpp \
		lib/Error.h \
		lib/command.h 

SOURCES       = main.cpp \
		menu.cpp \ 
		iconbutton.cpp \
		mainwindow.cpp \
		toolbar.cpp \ 
		UiTemplate.cpp \
		touchkey/linedit.cpp \
		touchkey/touchkey.cpp \
		udisk_detect.cpp

SUBDIRS		= lib \
		touchkey 

my_cmd_line = cp ../gz_run.sh ./
QMAKE_POST_LINK += $$quote($$my_cmd_line)

unix:!macx: LIBS += ../lib/libpmd.so 
