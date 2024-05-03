
#include "globals.h"
#include "mathops.h"
#include "FontRenderer.h"
#include "EditorLayerPicker.h"
#include "UILayerButton.h"
#include "maineditor.h"

bool EditorLayerPicker::isMouseIn(XY thisPositionOnScreen, XY mousePos)
{
	return pointInBox(mousePos, SDL_Rect{ thisPositionOnScreen.x, thisPositionOnScreen.y, wxWidth, wxHeight });
}

void EditorLayerPicker::render(XY position)
{

    SDL_Rect r = SDL_Rect{ position.x, position.y, wxWidth, wxHeight };
    SDL_SetRenderDrawColor(g_rd, 0x30, 0x30, 0x30, focused ? 0x80 : 0x30);
    SDL_RenderFillRect(g_rd, &r);

    /*r = SDL_Rect{position.x + wxWidth - 60, position.y + wxHeight - 40, 55, 35};
    SDL_SetRenderDrawColor(g_rd, previewCol.r, previewCol.g, previewCol.b, focused ? 0xff : 0x30);
    SDL_RenderFillRect(g_rd, &r);*/

    g_fnt->RenderString("LAYERS", position.x + 1, position.y + 1);

    layerButtons.renderAll(position);
}

void EditorLayerPicker::handleInput(SDL_Event evt, XY gPosOffset)
{
    if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1 && evt.button.state) {
        layerButtons.tryFocusOnPoint(XY{ evt.button.x, evt.button.y }, position);
    }
    if (!layerButtons.anyFocused()) {

    }
    else {
        layerButtons.passInputToFocused(evt, gPosOffset);
    }
}

void EditorLayerPicker::eventGeneric(int evt_id, int data1, int data2)
{
    if (data1 == 0) {
        caller->selLayer = evt_id;
    }
    else if (data1 == 1) {
        caller->layers[evt_id]->hidden = !caller->layers[evt_id]->hidden;
    }
    layerButtons.forceUnfocus();
    updateLayers();
}

void EditorLayerPicker::updateLayers()
{
    layerButtons.freeAllDrawables();
    int yposition = 40;
    int lid = 0;
    for (Layer*& l : caller->layers) {
        UILayerButton* layerButton = new UILayerButton(l->name);
        layerButton->hideButton->colorBGFocused = layerButton->hideButton->colorBGUnfocused = (l->hidden ? SDL_Color{ 255,255,255,0x80 } : SDL_Color{0,0,0,0x80});
        layerButton->position = { 5, yposition };
        yposition += 30;
        layerButton->setCallbackListener(lid++, this);
        layerButtons.addDrawable(layerButton);
    }
}
