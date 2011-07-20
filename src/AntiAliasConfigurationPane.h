#pragma once

#include "Defines.h"
#include "ConfigurationPane.h"
#include "AntiAliasPostProcess.h"
#include "Gwen/Controls/HorizontalSlider.h"
#include "Gwen/Controls/CheckBox.h"

class AntiAliasConfigurationPane : public ConfigurationPane<AntiAliasPostProcess>
{
private:
	Gwen::Controls::CheckBoxWithLabel* _depthDetectionCheckBox;
	Gwen::Controls::HorizontalSlider* _depthThresholdSlider;
	Gwen::Controls::Label* _depthThresholdLabel;

	Gwen::Controls::CheckBoxWithLabel* _normalDetectionCheckBox;
	Gwen::Controls::HorizontalSlider* _normalThresholdSlider;
	Gwen::Controls::Label* _normalThresholdLabel;

	Gwen::Controls::HorizontalSlider* _lumThresholdSlider;
	Gwen::Controls::Label* _lumThresholdLabel;
	Gwen::Controls::CheckBoxWithLabel* _lumDetectionCheckBox;

	Gwen::Controls::HorizontalSlider* _maxSearchStepsSlider;
	Gwen::Controls::Label* _maxSearchStepsLabel;

public:
	AntiAliasConfigurationPane(Gwen::Controls::Base* parent, AntiAliasPostProcess* pp);
	~AntiAliasConfigurationPane();

	void OnFrameMove(double totalTime, float dt);
};