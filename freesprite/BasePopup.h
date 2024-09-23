#pragma once
#include "globals.h"
#include "BaseScreen.h"
#include "DrawableManager.h"

class BasePopup :
    public BaseScreen
{
public:
    Timer64 startTimer;

    virtual bool takesInput() { return true; }

    void takeInput(SDL_Event evt) override {
        DrawableManager::processHoverEventInMultiple({ wxsManager }, evt, getPopupOrigin());
        if (!DrawableManager::processInputEventInMultiple({ wxsManager }, evt, getPopupOrigin())) {
			defaultInputAction(evt);
		}
    }

protected:
    DrawableManager wxsManager;
    int wxWidth = 600;
    int wxHeight = 400;

    void renderDrawables() {
        wxsManager.renderAll(getPopupOrigin());
    }

    void renderDefaultBackground(SDL_Color bgColor = SDL_Color{0,0,0,0xD0}) {
        SDL_SetRenderDrawColor(g_rd, 0xff, 0xff, 0xff, (uint8_t)(0x30 * startTimer.percentElapsedTime(300)));
        SDL_RenderFillRect(g_rd, NULL);
        XY origin = getPopupOrigin();
        SDL_Rect bgRect = SDL_Rect{origin.x, origin.y, wxWidth, (int)(wxHeight * XM1PW3P1(startTimer.percentElapsedTime(300)))};
        SDL_SetRenderDrawColor(g_rd, bgColor.r, bgColor.g, bgColor.b, bgColor.a);
        SDL_RenderFillRect(g_rd, &bgRect);
    }

    XY getPopupOrigin() {
        return XY{ g_windowW / 2 - wxWidth / 2, g_windowH / 2 - wxHeight / 2 };
    }

    XY getDefaultTitlePosition() {
        return xyAdd(getPopupOrigin(), XY{5,5});
    }
    XY getDefaultContentPosition() {
        return xyAdd(getPopupOrigin(), XY{ 5,50 });
    }

    void closePopup();

    virtual void defaultInputAction(SDL_Event evt) {}
};

