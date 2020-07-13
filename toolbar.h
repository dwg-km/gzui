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
	iconButton * networkButton;


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
		networkButton = NULL;
		
		printButton = NULL;
		measureButton = NULL;
		nozzleButton = NULL;
		originButton = NULL;

		waveButton = NULL;

		/*
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

		AddNetworkButton();

		AddWaveButton();
		*/
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
		if(leftButton == NULL)
		{
			AddLeftButton();
		}
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
		if(rightButton == NULL)
		{
			AddRightButton();
		}
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
		if(upButton == NULL)
		{
			AddUpButton();
		}
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
		if(downButton == NULL)
		{
			AddDownButton();
		}
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
		if(gohomeButton == NULL)
		{
			AddGoHomeButton();
		}
		return gohomeButton;
	}
	void AddMenuButton(){
		QVector<QString> iconmenu = {
			"resources/menu.png"
		};
		menuButton = new iconButton(iconmenu);
	}
	iconButton * GetMenuButton(){
		if(menuButton == NULL)
		{
			AddMenuButton();
		}
		return menuButton;
	}
	void AddHomeButton(){
		QVector<QString> iconhome = {
			"resources/home.png"
		};
		homeButton = new iconButton(iconhome);
	}
	iconButton * GetHomeButton(){
		if(homeButton == NULL)
		{
			AddHomeButton();
		}
		return homeButton;
	}
	void AddCaliButton(){
		QVector<QString> iconcali = {
	    		"resources/cali.png"
	    	};
	    	caliButton = new iconButton(iconcali);
	}
	iconButton * GetCaliButton(){
		if(caliButton == NULL)
		{
			AddCaliButton();
		}
		return caliButton;
	}
	void AddUvButton(){
	    	QVector<QString> iconmechine = {
	    		"resources/mechine.png"
	    	};
	    	mechineButton = new iconButton(iconmechine);
	}
	iconButton * GetUvButton(){
		if(uvButton == NULL)
		{
			AddUvButton();
		}
		return uvButton;
	}
	void AddMechineButton(){
	    	QVector<QString> iconuv = {
	    		"resources/uv.png"
	    	};
	    	uvButton = new iconButton(iconuv);
	}
	iconButton * GetMechineButton(){
		if(mechineButton == NULL)
		{
			AddMechineButton();
		}
		return mechineButton;
	}
	void AddInkButton(){
	    	QVector<QString> iconink = {
	    		"resources/ink.png"
	    	};
	    	inkButton = new iconButton(iconink);
	}
	iconButton * GetInkButton(){
		if(inkButton == NULL)
		{
			AddInkButton();
		}
		return inkButton;
	}
	void AddWarningButton(){
	    	QVector<QString> iconwarning = {
	    		"resources/warning.png"
	    	};
	    	warningButton = new iconButton(iconwarning);
	}
	iconButton * GetWarningButton(){
		if(warningButton == NULL)
		{
			AddWarningButton();
		}
		return warningButton;
	}
	void AddManagerButton(){
	    	QVector<QString> iconmanager = {
	    		"resources/manager.png"
	    	};
	    	managerButton = new iconButton(iconmanager);
	}
	iconButton * GetManagerButton(){
		if(managerButton == NULL)
		{
			AddManagerButton();
		}
		return managerButton;
	}
	void AddAdvanceButton(){
	    	QVector<QString> iconadvance = {
	    		"resources/advance.png"
	    	};
	    	advanceButton = new iconButton(iconadvance);
	}
	iconButton * GetAdvanceButton(){
		if(advanceButton == NULL)
		{
			AddAdvanceButton();
		}
		return advanceButton;
	}
	void AddBasesettingButton(){
	    	QVector<QString> iconbasesetting = {
	    		"resources/basesetting.png"
	    	};
	    	basesettingButton = new iconButton(iconbasesetting);
	}
	iconButton * GetBasesettingButton(){
		if(basesettingButton == NULL)
		{
			AddBasesettingButton();
		}
		return basesettingButton;
	}
	void AddCleanButton(){
	    	QVector<QString> iconclean = {
	    		"resources/clean.png"
	    	};
	    	cleanButton = new iconButton(iconclean);
	}
	iconButton * GetCleanButton(){
		if(cleanButton == NULL)
		{
			AddCleanButton();
		}
		return cleanButton;
	}
	void AddPoweroffButton(){
	    	QVector<QString> iconpoweroff = {
	    		"resources/poweroff.png"
	    	};
	    	poweroffButton = new iconButton(iconpoweroff);
	}
	iconButton * GetPoweroffButton(){
		if(poweroffButton == NULL)
		{
			AddPoweroffButton();
		}
		return poweroffButton;
	}
	void AddUpdateButton(){
	    	QVector<QString> iconupdate = {
	    		"resources/update.png"
	    	};
	    	updateButton = new iconButton(iconupdate);
	}
	iconButton * GetUpdateButton(){
		if(updateButton == NULL)
		{
			AddUpdateButton();
		}
		return updateButton;
	}
	void AddBackupButton(){
	    	QVector<QString> iconbackup = {
	    		"resources/backup.png"
	    	};
	    	backupButton = new iconButton(iconbackup);
	}
	iconButton * GetBackupButton(){
		if(backupButton == NULL)
		{
			AddBackupButton();
		}
		return backupButton;
	}
	void AddPauseButton(){
	    	QVector<QString> iconpause = {
	    		"resources/pause.png",
			"resources/resume.png"
	    	};
		if(pauseButton == NULL){
	    		pauseButton = new iconButton(iconpause);
			connect(pauseButton, SIGNAL(clicked()), this, SLOT(Pause()));
		}
	}
	iconButton * GetPauseButton(){
		if(pauseButton == NULL)
		{
			AddPauseButton();
		}
		return pauseButton;
	}
	void AddAbortButton(){
	    	QVector<QString> iconabort = {
	    		"resources/abort.png"
	    	};
		if(abortButton == NULL){
	    		abortButton = new iconButton(iconabort);
			connect(abortButton, SIGNAL(clicked()), this, SLOT(Abort()));
		}
	}
	iconButton * GetAbortButton(){
		if(abortButton == NULL)
		{
			AddAbortButton();
		}
		return abortButton;
	}
	void AddExitButton(){
	    	QVector<QString> iconexit = {
	    		"resources/exit.png"
	    	};
	    	exitButton = new iconButton(iconexit);
	}
	iconButton * GetExitButton(){
		if(exitButton == NULL)
		{
			AddExitButton();
		}
		return exitButton;
	}
	void AddPreviousButton(){
	    	QVector<QString> iconprevious = {
	    		"resources/previous.png"
	    	};
	    	previousButton = new iconButton(iconprevious);
	}
	iconButton * GetPreviousButton(){
		if(previousButton == NULL)
		{
			AddPreviousButton();
		}
		return previousButton;
	}
	void AddNextButton(){
	    	QVector<QString> iconnext = {
	    		"resources/next.png"
	    	};
	    	nextButton = new iconButton(iconnext);
	}
	iconButton * GetNextButton(){
		if(nextButton == NULL)
		{
			AddNextButton();
		}
		return nextButton;
	}
	void AddPrintButton(){
		QVector<QString> iconprint = {
			"resources/print.png"
		};
		printButton = new iconButton(iconprint);
	}
	iconButton * GetPrintButton(){
		if(printButton == NULL)
		{
			AddPrintButton();
		}
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
		if(measureButton == NULL)
		{
			AddMeasureButton();
		}
		return measureButton;
	}
	void AddNozzleButton(){
		QVector<QString> iconnozzle = {
			"resources/nozzle.png"
		};
		nozzleButton = new iconButton(iconnozzle);
	}
	iconButton * GetNozzleButton(){
		if(nozzleButton == NULL)
		{
			AddNozzleButton();
		}
		return nozzleButton;
	}
	void AddOriginButton(){
		QVector<QString> iconorigin = {
			"resources/origin.png"
		};
		if(originButton == NULL){
			originButton = new iconButton(iconorigin);
			connect(originButton, SIGNAL(clicked()), this, SLOT(SetAsOrigin()));
		}
	}
	iconButton * GetOriginButton(){
		if(originButton == NULL){
			AddOriginButton();
		}
		return originButton;
	}
	void AddWaveButton(){
		QVector<QString> iconwave = {
			"resources/wave.png"
		};
		waveButton = new iconButton(iconwave);
	}
	iconButton * GetWaveButton(){
		if(waveButton == NULL)
		{
			AddWaveButton();
		}
		return waveButton;
	}
	void AddSaveButton(){
		QVector<QString> iconsave = {
			"resources/save.png"
		};
		saveButton = new iconButton(iconsave);
	}
	iconButton * GetSaveButton(){
		if(saveButton == NULL)
		{
			AddSaveButton();
		}
		return saveButton;
	}
	void AddFlashButton(){
		QVector<QString> iconflash = {
			"resources/flash.png"
		};
		flashButton = new iconButton(iconflash);
	}
	iconButton * GetFlashButton(){
		if(flashButton == NULL)
		{
			AddFlashButton();
		}
		return flashButton;
	}
	void AddNetworkButton(){
		QVector<QString> iconnetwork = {
			"resources/internet.png",
			"resources/offline.png",
			"resources/user.png"
		};
		networkButton = new iconButton(iconnetwork);
	}
	iconButton * GetNetworkButton(){
		if(networkButton == NULL)
		{
			AddNetworkButton();
		}
		return networkButton;
	}

private slots:
	void MoveLeft(){
		MOVE move;
		move.Axis = AXIS_X;
		move.Distance = 0;
		move.Speed = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE, &move);
	}
	void MoveRight(){
		MOVE move;
		move.Axis = AXIS_X;
		move.Distance = 0;
		move.Speed = 0;
		move.Dir = 1;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE, &move);
	}
	void MoveXStop(){
		MOVE move;
		move.Axis = AXIS_X;
		move.Distance = 0;
		move.Speed = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE_STOP, &move);
	}
	void MoveForward(){
		MOVE move;
		move.Axis = AXIS_Y;
		move.Distance = 0;
		move.Speed = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE, &move);
	}
	void MoveBackward(){
		MOVE move;
		move.Axis = AXIS_Y;
		move.Distance = 0;
		move.Speed = 0;
		move.Dir = 1;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE, &move);
	}
	void MoveYStop(){
		MOVE move;
		move.Axis = AXIS_Y;
		move.Distance = 0;
		move.Speed = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVE_STOP, &move);
	}
	void GoHome(){
		MOVE move;
		move.Axis = AXIS_X;
		move.Distance = 10;
		move.Speed = 0;
		move.Dir = 0;

		SendMotionCmd(UI_CMD::CMD_MOTION_MOVETO, &move);
	}
	void Measure(){
		MotionThread * measure = new MotionThread(UI_CMD::CMD_MOTION_MEASURE_MEDIA, 0);
		measure->start();
	}
	void Pause(){
		PrintPause();
	}
	
	void Abort(){
		PrintAbort();
	}

	void SetAsOrigin()
	{
		qDebug() << "set as origiin";
		int ret = SendMotionCmd(UI_CMD::CMD_MOTION_ORIGIN, NULL);
		if(ret == 0){
			//LoadOrigin();	
			emit updateOrigin();
		}
	}
signals:
	void updateOrigin();
};

#endif
