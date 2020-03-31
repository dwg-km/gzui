
#include "toolbar.h"


void ToolBar::setMoveEnabled(bool enable)
{
	if(enable){
		if(leftButton)
			leftButton->setEnabled();
		if(rightButton)
			rightButton->setEnabled();
		if(upButton)
			upButton->setEnabled();
		if(downButton)
			downButton->setEnabled();
		if(gohomeButton)
			gohomeButton->setEnabled();
		if(measureButton)
			measureButton->setEnabled();
		if(cleanButton)
			cleanButton->setEnabled();
		if(printButton)
			printButton->setEnabled();
		if(nozzleButton)
			nozzleButton->setEnabled();
	}else{
		if(leftButton)
			leftButton->setDisabled();
		if(rightButton)
			rightButton->setDisabled();
		if(upButton)
			upButton->setDisabled();
		if(downButton)
			downButton->setDisabled();
		if(gohomeButton)
			gohomeButton->setDisabled();
		if(measureButton)
			measureButton->setDisabled();
		if(cleanButton)
			cleanButton->setDisabled();
		if(printButton)
			printButton->setDisabled();
		if(nozzleButton)
			nozzleButton->setDisabled();
	}
}
