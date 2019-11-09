#ifndef TOOL_BAR_H
#define TOOL_BAR_H

#include <QDebug>
#include <QThread>

#include "iconbutton.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"
/*
	iconButton * ****Button;
		****Button = NULL;
		Add####Button();
	void Add####Button(){
		QVector<QString> icon**** = {
			"../resource/----.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		****Button = new iconButton(icon****);
	}
	iconButton * Get####Button(){
		return ****Button;
	}
*/

class MotionThread : public QThread{
public:
	MotionThread(UI_CMD cmd, MOVE * move){
		Cmd = cmd;
		if(move){
			move = new MOVE;
			memcpy(Move, move, sizeof(MOVE));	
		}else{
			Move = NULL;
		}	
	}
	virtual void run(){
		int ret = SendMotionCmd(Cmd, Move);
		if(ret == 0){
			switch(Cmd){
			case CMD_MOTION_MEASURE_MEDIA:
				qDebug() << "measure okay";
				break;
			default:
				qDebug() << "unknow move cmd";
				break;
			}	
		}
	}
	virtual ~MotionThread(){
		if(Move){
			delete Move;
		}
	}
private:
	UI_CMD Cmd;
	MOVE * Move;
};

class ToolBar : public QWidget {
	Q_OBJECT
private:
	iconButton * leftButton;
	iconButton * rightButton;
	iconButton * upButton;
	iconButton * downButton;
	iconButton * gohomeButton;

	iconButton * homeButton;
	iconButton * menuButton;

	iconButton * caliButton;
	iconButton * mechineButton;
	iconButton * uvButton;
	iconButton * inkButton;
	iconButton * basesettingButton;
	iconButton * warningButton;
	iconButton * advanceButton;
	iconButton * managerButton;
	iconButton * cleanButton;

	iconButton * poweroffButton;
	iconButton * backupButton;
	iconButton * updateButton;
	iconButton * abortButton;
	iconButton * pauseButton;
	iconButton * exitButton;

	iconButton * nextButton;
	iconButton * previousButton;

	iconButton * printButton;
	iconButton * measureButton;
	iconButton * nozzleButton;
	iconButton * originButton;

public:
	void setMoveEnabled(bool enable);
public:
	ToolBar(){
		leftButton = NULL;
		rightButton = NULL;
		upButton = NULL;
		downButton = NULL;
		gohomeButton = NULL;

		homeButton = NULL;
		menuButton = NULL;

		caliButton = NULL;
		mechineButton = NULL;
		uvButton = NULL;
		inkButton = NULL;
		basesettingButton = NULL;
		warningButton = NULL;
		advanceButton = NULL;
		cleanButton = NULL;

		poweroffButton = NULL;
		backupButton = NULL;
		updateButton = NULL;
		abortButton = NULL;
		pauseButton = NULL;
		exitButton = NULL;
		nextButton = NULL;
		previousButton = NULL;
		
		printButton = NULL;
		measureButton = NULL;
		nozzleButton = NULL;
		originButton = NULL;

		AddLeftButton();
		AddRightButton();
		AddUpButton();
		AddDownButton();
		AddGoHomeButton();
		
		AddHomeButton();
		AddMenuButton();
		
		AddCaliButton();
		AddMechineButton();
		AddUvButton();
		AddInkButton();
		 
		AddCleanButton();
		AddAdvanceButton();
		AddManagerButton();
		AddWarningButton();
		AddBasesettingButton();

		AddPoweroffButton();
		AddUpdateButton();
		AddBackupButton();
		 
		AddAbortButton();
		AddPauseButton();
		AddExitButton();
		AddNextButton();
		AddPreviousButton();
		
		AddPrintButton();
		AddMeasureButton();
		AddNozzleButton();
		AddOriginButton();
	}

	void AddLeftButton(){
		QVector<QString> iconleft = {
			"../resource/moveleft.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		if(leftButton == NULL){
			leftButton = new iconButton(iconleft);
			connect(leftButton, SIGNAL(keyPressed()), this, SLOT(MoveLeft()));
			connect(leftButton, SIGNAL(keyReleased()), this, SLOT(MoveXStop()));
		}
	}
	iconButton * GetLeftButton(){
		return leftButton;
	}
	void AddRightButton(){
		QVector<QString> iconright = {
			"../resource/moveright.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		if(rightButton == NULL){
			rightButton = new iconButton(iconright);
			connect(rightButton, SIGNAL(keyPressed()), this, SLOT(MoveRight()));
			connect(rightButton, SIGNAL(keyReleased()), this, SLOT(MoveXStop()));
		}
	}
	iconButton * GetRightButton(){
		return rightButton;
	}
	void AddUpButton(){
		QVector<QString> iconup = {
			"../resource/moveup.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		if(upButton == NULL){
			upButton = new iconButton(iconup);
			connect(upButton, SIGNAL(keyPressed()), this, SLOT(MoveForward()));
			connect(upButton, SIGNAL(keyReleased()), this, SLOT(MoveYStop()));
		}
	}
	iconButton * GetUpButton(){
		return upButton;
	}
	void AddDownButton(){
		QVector<QString> icondown = {
			"../resource/movedown.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		if(downButton == NULL){
			downButton = new iconButton(icondown);
			connect(downButton, SIGNAL(keyPressed()), this, SLOT(MoveBackward()));
			connect(downButton, SIGNAL(keyReleased()), this, SLOT(MoveYStop()));
		}
	}
	iconButton * GetDownButton(){
		return downButton;
	}
	void AddGoHomeButton(){
		QVector<QString> icongohome = {
			"../resource/gohome.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		if(gohomeButton == NULL){
			gohomeButton = new iconButton(icongohome);
			connect(gohomeButton, SIGNAL(clicked()), this, SLOT(GoHome()));
		}
	}
	iconButton * GetGoHomeButton(){
		return gohomeButton;
	}
	void AddMenuButton(){
		QVector<QString> iconmenu = {
			"../resource/menu.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		menuButton = new iconButton(iconmenu);
	}
	iconButton * GetMenuButton(){
		return menuButton;
	}
	void AddHomeButton(){
		QVector<QString> iconhome = {
			"../resource/home.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		homeButton = new iconButton(iconhome);
	}
	iconButton * GetHomeButton(){
		return homeButton;
	}
	void AddCaliButton(){
		QVector<QString> iconcali = {
	    		"../resource/cali.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	caliButton = new iconButton(iconcali);
	}
	iconButton * GetCaliButton(){
		return caliButton;
	}
	void AddUvButton(){
	    	QVector<QString> iconmechine = {
	    		"../resource/mechine.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	mechineButton = new iconButton(iconmechine);
	}
	iconButton * GetUvButton(){
		return uvButton;
	}
	void AddMechineButton(){
	    	QVector<QString> iconuv = {
	    		"../resource/uv.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	uvButton = new iconButton(iconuv);
	}
	iconButton * GetMechineButton(){
		return mechineButton;
	}
	void AddInkButton(){
	    	QVector<QString> iconink = {
	    		"../resource/ink.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	inkButton = new iconButton(iconink);
	}
	iconButton * GetInkButton(){
		return inkButton;
	}
	void AddWarningButton(){
	    	QVector<QString> iconwarning = {
	    		"../resource/warning.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	warningButton = new iconButton(iconwarning);
	}
	iconButton * GetWarningButton(){
		return warningButton;
	}
	void AddManagerButton(){
	    	QVector<QString> iconmanager = {
	    		"../resource/manager.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	managerButton = new iconButton(iconmanager);
	}
	iconButton * GetManagerButton(){
		return managerButton;
	}
	void AddAdvanceButton(){
	    	QVector<QString> iconadvance = {
	    		"../resource/advance.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	advanceButton = new iconButton(iconadvance);
	}
	iconButton * GetAdvanceButton(){
		return advanceButton;
	}
	void AddBasesettingButton(){
	    	QVector<QString> iconbasesetting = {
	    		"../resource/basesetting.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	basesettingButton = new iconButton(iconbasesetting);
	}
	iconButton * GetBasesettingButton(){
		return basesettingButton;
	}
	void AddCleanButton(){
	    	QVector<QString> iconclean = {
	    		"../resource/clean.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	cleanButton = new iconButton(iconclean);
	}
	iconButton * GetCleanButton(){
		return cleanButton;
	}
	void AddPoweroffButton(){
	    	QVector<QString> iconpoweroff = {
	    		"../resource/poweroff.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	poweroffButton = new iconButton(iconpoweroff);
	}
	iconButton * GetPoweroffButton(){
		return poweroffButton;
	}
	void AddUpdateButton(){
	    	QVector<QString> iconupdate = {
	    		"../resource/update.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	updateButton = new iconButton(iconupdate);
	}
	iconButton * GetUpdateButton(){
		return updateButton;
	}
	void AddBackupButton(){
	    	QVector<QString> iconbackup = {
	    		"../resource/backup.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	backupButton = new iconButton(iconbackup);
	}
	iconButton * GetBackupButton(){
		return backupButton;
	}
	void AddPauseButton(){
	    	QVector<QString> iconpause = {
	    		"../resource/pause.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	pauseButton = new iconButton(iconpause);
	}
	iconButton * GetPauseButton(){
		return pauseButton;
	}
	void AddAbortButton(){
	    	QVector<QString> iconabort = {
	    		"../resource/abort.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	abortButton = new iconButton(iconabort);
	}
	iconButton * GetAbortButton(){
		return abortButton;
	}
	void AddExitButton(){
	    	QVector<QString> iconexit = {
	    		"../resource/exit.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	exitButton = new iconButton(iconexit);
	}
	iconButton * GetExitButton(){
		return exitButton;
	}
	void AddPreviousButton(){
	    	QVector<QString> iconprevious = {
	    		"../resource/previous.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	previousButton = new iconButton(iconprevious);
	}
	iconButton * GetPreviousButton(){
		return previousButton;
	}
	void AddNextButton(){
	    	QVector<QString> iconnext = {
	    		"../resource/next.png",
	    		"../resource/2.png",
	    		"../resource/th.png"
	    	};
	    	nextButton = new iconButton(iconnext);
	}
	iconButton * GetNextButton(){
		return nextButton;
	}
	void AddPrintButton(){
		QVector<QString> iconprint = {
			"../resource/print.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		printButton = new iconButton(iconprint);
	}
	iconButton * GetPrintButton(){
		return printButton;
	}
	void AddMeasureButton(){
		QVector<QString> iconmeasure = {
			"../resource/measure.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		if(measureButton == NULL){
			measureButton = new iconButton(iconmeasure);
			connect(measureButton, SIGNAL(clicked()), this, SLOT(Measure()));
		}
	}
	iconButton * GetMeasureButton(){
		return measureButton;
	}
	void AddNozzleButton(){
		QVector<QString> iconnozzle = {
			"../resource/nozzle.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		nozzleButton = new iconButton(iconnozzle);
	}
	iconButton * GetNozzleButton(){
		return nozzleButton;
	}
	void AddOriginButton(){
		QVector<QString> iconorigin = {
			"../resource/origin.png",
			"../resource/2.png",
			"../resource/th.png"
		};
		originButton = new iconButton(iconorigin);
	}
	iconButton * GetOriginButton(){
		return originButton;
	}

private slots:
	void MoveLeft(){
		MOVE move;
		move.Axis = AXIS_X;
		move.Distance = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE, &move);
	}
	void MoveRight(){
		MOVE move;
		move.Axis = AXIS_X;
		move.Distance = 0;
		move.Dir = 1;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE, &move);
	}
	void MoveXStop(){
		MOVE move;
		move.Axis = AXIS_X;
		move.Distance = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE_STOP, &move);
	}
	void MoveForward(){
		MOVE move;
		move.Axis = AXIS_Y;
		move.Distance = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE, &move);
	}
	void MoveBackward(){
		MOVE move;
		move.Axis = AXIS_Y;
		move.Distance = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE, &move);
	}
	void MoveYStop(){
		MOVE move;
		move.Axis = AXIS_Y;
		move.Distance = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE_STOP, &move);
	}
	void GoHome(){
		MOVE move;
		move.Axis = AXIS_X;
		move.Distance = 10;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVETO, &move);
	}
	void Measure(){
		MotionThread * measure = new MotionThread(UI_CMD::CMD_MOTION_MEASURE_MEDIA, 0);
		measure->start();

		qDebug() << "measure";
	}
};

#endif
