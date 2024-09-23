
#include "globals.h"
#include "mathops.h"
#include "FontRenderer.h"
#include "EditorLayerPicker.h"
#include "UILayerButton.h"
#include "maineditor.h"
#include "Panel.h"
#include "UISlider.h"

EditorLayerPicker::EditorLayerPicker(MainEditor* editor) {
    caller = editor;

    wxWidth = 250;
    wxHeight = 400;

    UIButton* addBtn = new UIButton();
    addBtn->position = { 5, 30 };
    //addBtn->text = "+";
    addBtn->wxWidth = 30;
    addBtn->setCallbackListener(-1, this);
    addBtn->icon = g_iconLayerAdd;
    addBtn->tooltip = "New layer";
    subWidgets.addDrawable(addBtn);

    UIButton* removeBtn = new UIButton();
    removeBtn->position = { addBtn->wxWidth + 5 + 5, 30 };
    //removeBtn->text = "-";
    removeBtn->wxWidth = 30;
    removeBtn->icon = g_iconLayerDelete;
    removeBtn->tooltip = "Delete layer";
    removeBtn->setCallbackListener(-2, this);
    subWidgets.addDrawable(removeBtn);

    UIButton* upBtn = new UIButton();
    upBtn->position = { addBtn->wxWidth + removeBtn->wxWidth + 5 + 5 + 5, 30 };
    upBtn->wxWidth = 30;
    upBtn->icon = g_iconLayerUp;
    upBtn->tooltip = "Move layer up";
    upBtn->setCallbackListener(-3, this);
    subWidgets.addDrawable(upBtn);

    UIButton* downBtn = new UIButton();
    downBtn->position = { addBtn->wxWidth + removeBtn->wxWidth + upBtn->wxWidth + 5 + 5 + 5 + 5, 30 };
    downBtn->wxWidth = 30;
    downBtn->icon = g_iconLayerDown;
    downBtn->tooltip = "Move layer down";
    downBtn->setCallbackListener(-4, this);
    subWidgets.addDrawable(downBtn);

    UIButton* mergeDownBtn = new UIButton();
    mergeDownBtn->position = { addBtn->wxWidth + removeBtn->wxWidth + upBtn->wxWidth + downBtn->wxWidth + 5 + 5 + 5 + 5 + 5, 30 };
    //mergeDownBtn->text = "Mrg";
    mergeDownBtn->wxWidth = 30;
    mergeDownBtn->icon = g_iconLayerDownMerge;
    mergeDownBtn->tooltip = "Merge down";
    mergeDownBtn->setCallbackListener(-5, this);
    subWidgets.addDrawable(mergeDownBtn);

    UIButton* duplicateBtn = new UIButton();
    duplicateBtn->position = { addBtn->wxWidth + removeBtn->wxWidth + upBtn->wxWidth + downBtn->wxWidth + mergeDownBtn->wxWidth + 5 + 5 + 5 + 5 + 5 + 5, 30 };
    duplicateBtn->wxWidth = 30;
    duplicateBtn->icon = g_iconLayerDuplicate;
    duplicateBtn->tooltip = "Duplicate layer";
    duplicateBtn->setCallbackListener(-6, this);
    subWidgets.addDrawable(duplicateBtn);

    UILabel* opacityLabel = new UILabel();
    opacityLabel->position = { 5, 65 };
    opacityLabel->text = "Opacity";
    subWidgets.addDrawable(opacityLabel);

    opacitySlider = new UISlider();
    opacitySlider->position = { 80, 70 };
    opacitySlider->wxWidth = 165;
    opacitySlider->wxHeight = 20;
    opacitySlider->setCallbackListener(EVENT_LAYERPICKER_OPACITYSLIDER, this);
    subWidgets.addDrawable(opacitySlider);

    layerListPanel = new Panel();
    layerListPanel->position = { 5, 100 };
    subWidgets.addDrawable(layerListPanel);
}

void EditorLayerPicker::render(XY position)
{
    SDL_Rect r = SDL_Rect{ position.x, position.y, wxWidth, wxHeight };
    //SDL_SetRenderDrawColor(g_rd, 0x30, 0x30, 0x30, focused ? 0x80 : 0x30);
    //SDL_RenderFillRect(g_rd, &r);

    SDL_Color colorBG1 = { 0x30, 0x30, 0x30, focused ? 0xa0 : 0x90};
    SDL_Color colorBG2 = { 0x10, 0x10, 0x10, focused ? 0xa0 : 0x90};
    renderGradient(r, sdlcolorToUint32(colorBG2), sdlcolorToUint32(colorBG1), sdlcolorToUint32(colorBG1), sdlcolorToUint32(colorBG1));
    if (focused) {
        SDL_SetRenderDrawColor(g_rd, 0xff, 0xff, 0xff, 255);
        drawLine({ position.x, position.y }, { position.x, position.y + wxHeight }, XM1PW3P1(focusTimer.percentElapsedTime(300)));
        drawLine({ position.x, position.y }, { position.x + wxWidth, position.y }, XM1PW3P1(focusTimer.percentElapsedTime(300)));
    }

    /*r = SDL_Rect{position.x + wxWidth - 60, position.y + wxHeight - 40, 55, 35};
    SDL_SetRenderDrawColor(g_rd, previewCol.r, previewCol.g, previewCol.b, focused ? 0xff : 0x30);
    SDL_RenderFillRect(g_rd, &r);*/

    g_fnt->RenderString("LAYERS", position.x + 4, position.y + 1);

    DraggablePanel::render(position);
}

void EditorLayerPicker::handleInput(SDL_Event evt, XY gPosOffset)
{
    DraggablePanel::handleInput(evt, gPosOffset);

    /*if (evt.type == SDL_MOUSEBUTTONDOWN && evt.button.button == 1 && evt.button.state) {
        if (!layerButtons.tryFocusOnPoint(XY{ evt.button.x, evt.button.y }, position)) {
            subWidgets.tryFocusOnPoint(XY{ evt.button.x, evt.button.y }, position);
        }
    }
    if (layerButtons.anyFocused()) {
        layerButtons.passInputToFocused(evt, gPosOffset);
    }
    else if (subWidgets.anyFocused()) {
        subWidgets.passInputToFocused(evt, gPosOffset);
    }
    else {
        processDrag(evt);
    }*/
}

void EditorLayerPicker::eventGeneric(int evt_id, int data1, int data2)
{
    if (data1 == 0) {
        caller->switchActiveLayer(evt_id);
    }
    else if (data1 == 1) {
        caller->layers[evt_id]->hidden = !caller->layers[evt_id]->hidden;
    }
    updateLayers();
}

void EditorLayerPicker::eventButtonPressed(int evt_id)
{
    if (evt_id == -1) {
        caller->newLayer();
    }
    else if (evt_id == -2) {
        caller->deleteLayer(caller->selLayer);
    }
    else if (evt_id == -3) {
        caller->moveLayerUp(caller->selLayer);
    }
    else if (evt_id == -4) {
        caller->moveLayerDown(caller->selLayer);
    }
    else if (evt_id == -5) {
        caller->mergeLayerDown(caller->selLayer);
    }
    else if (evt_id == -6) {
        caller->duplicateLayer(caller->selLayer);
    }
    updateLayers();
}

void EditorLayerPicker::eventSliderPosChanged(int evt_id, float value)
{
    if (evt_id == EVENT_LAYERPICKER_OPACITYSLIDER) {
        caller->getCurrentLayer()->layerAlpha = (uint8_t)(value * 255);
    }
}

void EditorLayerPicker::eventSliderPosFinishedChanging(int evt_id, float value)
{
    if (evt_id == EVENT_LAYERPICKER_OPACITYSLIDER) {
        caller->layer_setOpacity((uint8_t)(value * 255));
        //printf("eventSliderPosFinishedChanging, %x\n", (uint8_t)(value * 255));
    }
}

void EditorLayerPicker::updateLayers()
{
    layerListPanel->subWidgets.freeAllDrawables();

    int yposition = 0;
    for (int lid = caller->layers.size(); lid --> 0;) {
        Layer* l = caller->layers[lid];
        UILayerButton* layerButton = new UILayerButton(l->name);
        layerButton->hideButton->colorBGFocused = layerButton->hideButton->colorBGUnfocused = (l->hidden ? SDL_Color{ 255,255,255,0x80 } : SDL_Color{0,0,0,0x80});
        layerButton->position = { 0, yposition };
        layerButton->mainButton->colorBGFocused = layerButton->mainButton->colorBGUnfocused = (caller->selLayer == lid ? SDL_Color{ 255,255,255,0x60 } : SDL_Color{ 0,0,0,0x80 });
        yposition += 30;
        layerButton->setCallbackListener(lid, this);
        layerListPanel->subWidgets.addDrawable(layerButton);
    }

    if (opacitySlider != NULL) {
        opacitySlider->sliderPos = caller->getCurrentLayer()->layerAlpha / 255.0f;
    }
}
