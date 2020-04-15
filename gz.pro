QT	 += network
QT       += core gui  
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets 

#QMAKE_CXXFLAGS += -Wno-unused_result

CONFIG+=debug_and_release
CONFIG(debug, debug|release): {
	DEFINES += DEBUG
}

#CONFIG		+= debug

INCLUDEPATH	= lib \
		touchkey

TEMPLATE      = app
HEADERS       = mainwindow.h \
		menu.h \
		cali.h \
		motion.h \
		wave.h \
		ip_cfg.hpp \
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
		ip_cfg.cpp \
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

DEFINES += GIT_VERSION=\\\"'$(shell git log -1 --format=%H)'\\\"

DEFINES += UI_VERSION=\\\"v1.0.0\\\"

#unix:!macx: LIBS += ../lib/libpmd.so

unix:!macx: LIBS += -L$$PWD/lib/ -lpmd

INCLUDEPATH += $$PWD/lib
DEPENDPATH += $$PWD/lib
