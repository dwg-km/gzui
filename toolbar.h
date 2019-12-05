#ifndef TOOL_BAR_H
#define TOOL_BAR_H

#include <QDebug>
#include <QThread>

#include "iconbutton.h"

#include "ui_interface.h"
#include "APIDataInterface.hpp"
/*
	iconButton * ====Button;
		====Button = NULL;
		Add####Button();
	void Add####Button(){
		QVector<QString> icon==== = {
			"resources/====.png"
		};
		====Button = new iconButton(icon====);
	}
	iconButton * Get####Button(){
		return ====Button;
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
	iconButton * waveButton;

	iconButton * mechineButton;
	iconButton * uvButton;
	iconButton * inkButton;
	iconButton * basesettingButton;
	iconButton * warningButton;
	iconButton * advanceButton;
	iconButton * managerButton;
	iconButton * cleanButton;
	iconButton * flashButton;

	iconButton * poweroffButton;
	iconButton * backupButton;
	iconButton * updateButton;
	iconButton * abortButton;
	iconButton * pauseButton;
	iconButton * exitButton;
	iconButton * saveButton;

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
		flashButton = NULL;

		poweroffButton = NULL;
		backupButton = NULL;
		updateButton = NULL;
		abortButton = NULL;
		pauseButton = NULL;
		exitButton = NULL;
		nextButton = NULL;
		previousButton = NULL;
		saveButton = NULL;
		
		printButton = NULL;
		measureButton = NULL;
		nozzleButton = NULL;
		originButton = NULL;

		waveButton = NULL;

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
		AddFlashButton();
		 
		AddCleanButton();
		AddAdvanceButton();
		AddManagerButton();
		AddWarningButton();
		AddBasesettingButton();

		AddPoweroffButton();
		AddUpdateButton();
		AddBackupButton();
		AddSaveButton();
		 
		AddAbortButton();
		AddPauseButton();
		AddExitButton();
		AddNextButton();
		AddPreviousButton();
		
		AddPrintButton();
		AddMeasureButton();
		AddNozzleButton();
		AddOriginButton();


		AddWaveButton();
	}

	void AddLeftButton(){
		QVector<QString> iconleft = {
			"resources/moveleft.png"
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
			"resources/moveright.png"
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
			"resources/moveup.png"
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
			"resources/movedown.png"
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
			"resources/gohome.png"
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
			"resources/menu.png"
		};
		menuButton = new iconButton(iconmenu);
	}
	iconButton * GetMenuButton(){
		return menuButton;
	}
	void AddHomeButton(){
		QVector<QString> iconhome = {
			"resources/home.png"
		};
		homeButton = new iconButton(iconhome);
	}
	iconButton * GetHomeButton(){
		return homeButton;
	}
	void AddCaliButton(){
		QVector<QString> iconcali = {
	    		"resources/cali.png"
	    	};
	    	caliButton = new iconButton(iconcali);
	}
	iconButton * GetCaliButton(){
		return caliButton;
	}
	void AddUvButton(){
	    	QVector<QString> iconmechine = {
	    		"resources/mechine.png"
	    	};
	    	mechineButton = new iconButton(iconmechine);
	}
	iconButton * GetUvButton(){
		return uvButton;
	}
	void AddMechineButton(){
	    	QVector<QString> iconuv = {
	    		"resources/uv.png"
	    	};
	    	uvButton = new iconButton(iconuv);
	}
	iconButton * GetMechineButton(){
		return mechineButton;
	}
	void AddInkButton(){
	    	QVector<QString> iconink = {
	    		"resources/ink.png"
	    	};
	    	inkButton = new iconButton(iconink);
	}
	iconButton * GetInkButton(){
		return inkButton;
	}
	void AddWarningButton(){
	    	QVector<QString> iconwarning = {
	    		"resources/warning.png"
	    	};
	    	warningButton = new iconButton(iconwarning);
	}
	iconButton * GetWarningButton(){
		return warningButton;
	}
	void AddManagerButton(){
	    	QVector<QString> iconmanager = {
	    		"resources/manager.png"
	    	};
	    	managerButton = new iconButton(iconmanager);
	}
	iconButton * GetManagerButton(){
		return managerButton;
	}
	void AddAdvanceButton(){
	    	QVector<QString> iconadvance = {
	    		"resources/advance.png"
	    	};
	    	advanceButton = new iconButton(iconadvance);
	}
	iconButton * GetAdvanceButton(){
		return advanceButton;
	}
	void AddBasesettingButton(){
	    	QVector<QString> iconbasesetting = {
	    		"resources/basesetting.png"
	    	};
	    	basesettingButton = new iconButton(iconbasesetting);
	}
	iconButton * GetBasesettingButton(){
		return basesettingButton;
	}
	void AddCleanButton(){
	    	QVector<QString> iconclean = {
	    		"resources/clean.png"
	    	};
	    	cleanButton = new iconButton(iconclean);
	}
	iconButton * GetCleanButton(){
		return cleanButton;
	}
	void AddPoweroffButton(){
	    	QVector<QString> iconpoweroff = {
	    		"resources/poweroff.png"
	    	};
	    	poweroffButton = new iconButton(iconpoweroff);
	}
	iconButton * GetPoweroffButton(){
		return poweroffButton;
	}
	void AddUpdateButton(){
	    	QVector<QString> iconupdate = {
	    		"resources/update.png"
	    	};
	    	updateButton = new iconButton(iconupdate);
	}
	iconButton * GetUpdateButton(){
		return updateButton;
	}
	void AddBackupButton(){
	    	QVector<QString> iconbackup = {
	    		"resources/backup.png"
	    	};
	    	backupButton = new iconButton(iconbackup);
	}
	iconButton * GetBackupButton(){
		return backupButton;
	}
	void AddPauseButton(){
	    	QVector<QString> iconpause = {
	    		"resources/pause.png"
	    	};
	    	pauseButton = new iconButton(iconpause);
	}
	iconButton * GetPauseButton(){
		return pauseButton;
	}
	void AddAbortButton(){
	    	QVector<QString> iconabort = {
	    		"resources/abort.png"
	    	};
	    	abortButton = new iconButton(iconabort);
	}
	iconButton * GetAbortButton(){
		return abortButton;
	}
	void AddExitButton(){
	    	QVector<QString> iconexit = {
	    		"resources/exit.png"
	    	};
	    	exitButton = new iconButton(iconexit);
	}
	iconButton * GetExitButton(){
		return exitButton;
	}
	void AddPreviousButton(){
	    	QVector<QString> iconprevious = {
	    		"resources/previous.png"
	    	};
	    	previousButton = new iconButton(iconprevious);
	}
	iconButton * GetPreviousButton(){
		return previousButton;
	}
	void AddNextButton(){
	    	QVector<QString> iconnext = {
	    		"resources/next.png"
	    	};
	    	nextButton = new iconButton(iconnext);
	}
	iconButton * GetNextButton(){
		return nextButton;
	}
	void AddPrintButton(){
		QVector<QString> iconprint = {
			"resources/print.png"
		};
		printButton = new iconButton(iconprint);
	}
	iconButton * GetPrintButton(){
		return printButton;
	}
	void AddMeasureButton(){
		QVector<QString> iconmeasure = {
			"resources/measure.png"
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
			"resources/nozzle.png"
		};
		nozzleButton = new iconButton(iconnozzle);
	}
	iconButton * GetNozzleButton(){
		return nozzleButton;
	}
	void AddOriginButton(){
		QVector<QString> iconorigin = {
			"resources/origin.png"
		};
		originButton = new iconButton(iconorigin);
	}
	iconButton * GetOriginButton(){
		return originButton;
	}
	void AddWaveButton(){
		QVector<QString> iconwave = {
			"resources/wave.png"
		};
		waveButton = new iconButton(iconwave);
	}
	iconButton * GetWaveButton(){
		return waveButton;
	}
	void AddSaveButton(){
		QVector<QString> iconsave = {
			"resources/save.png"
		};
		saveButton = new iconButton(iconsave);
	}
	iconButton * GetSaveButton(){
		return saveButton;
	}
	void AddFlashButton(){
		QVector<QString> iconflash = {
			"resources/flash.png"
		};
		flashButton = new iconButton(iconflash);
	}
	iconButton * GetFlashButton(){
		return flashButton;
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
