#pragma once

#include "Defines.h"
#include "IUpdateable.h"
#include "Gwen/Controls/ScrollControl.h"

class ConfigurationPaneBase : public IUpdateable, public Gwen::Controls::ScrollControl
{
public:
	ConfigurationPaneBase(Gwen::Controls::Base* parent) 
		: Gwen::Controls::ScrollControl(parent) 
	{ 
		SetScroll(false, true);
		SetAutoHideBars(true);
	}
	
	virtual void OnFrameMove(double totalTime, float dt) = 0;
};