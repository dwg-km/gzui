
#include "toolbar.h"


void ToolBar::setMoveEnabled(bool enable)
{
	if(enable){
		leftButton->setEnabled();
		rightButton->setEnabled();
		upButton->setEnabled();
		downButton->setEnabled();
		gohomeButton->setEnabled();
		measureButton->setEnabled();
		cleanButton->setEnabled();
		printButton->setEnabled();
		nozzleButton->setEnabled();
	}else{
		leftButton->setDisabled();
		rightButton->setDisabled();
		upButton->setDisabled();
		downButton->setDisabled();
		gohomeButton->setDisabled();
		measureButton->setDisabled();
		cleanButton->setDisabled();
		printButton->setDisabled();
		nozzleButton->setDisabled();
	}
}
