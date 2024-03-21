#pragma once
#include "BaseBrush.h"

class Brush1x1 : public BaseBrush
{
	std::string getName() override { return "1x1 Pixel"; }
	std::string getIconPath() override { return "assets/brush_px1x1.png"; }
	void clickPress(MainEditor* editor, XY pos) override;
	void clickDrag(MainEditor* editor, XY from, XY to) override;
	void clickRelease(MainEditor* editor, XY pos) override {}
};
