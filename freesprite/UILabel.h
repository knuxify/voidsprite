#pragma once
#include "drawable.h"
class UILabel :
    public Drawable
{
public:
	std::string text = "";
	SDL_Color color = { 255, 255, 255, 255 };

	bool focusable() override { return false; }
	void render(XY pos) override;
};

