#include "PalettizedEditorColorPicker.h"
#include "FontRenderer.h"
#include "UIDropdown.h"
#include "Notification.h"

PalettizedEditorColorPicker::PalettizedEditorColorPicker(MainEditorPalettized* c)
{
    caller = c;
    upcastCaller = c;

    colorPaletteTabs = new TabbedView({ {"Palette"}, {"Options"} }, 75);
    colorPaletteTabs->position = { 20,30 };
    subWidgets.addDrawable(colorPaletteTabs);

    eraserButton = new UIButton();
    eraserButton->position = { 20, 350 };
    //eraserButton->text = "E";
    eraserButton->icon = g_iconEraser;
    eraserButton->wxWidth = 30;
    eraserButton->setCallbackListener(EVENT_COLORPICKER_TOGGLEERASER, this);
    subWidgets.addDrawable(eraserButton);

    std::vector<std::string> palettes;
    for (auto& pal : g_palettes) {
		palettes.push_back(pal.first);
	}

    UIButton* buttonSavePalette = new UIButton();
    buttonSavePalette->position = { 20, 60 };
    buttonSavePalette->text = "Save palette";
    buttonSavePalette->wxWidth = 120;
    buttonSavePalette->wxHeight = 30;
    buttonSavePalette->setCallbackListener(EVENT_PALETTECOLORPICKER_SAVEPALETTE, this);
    colorPaletteTabs->tabs[1].wxs.addDrawable(buttonSavePalette);

    UIButton* buttonLoadPalette = new UIButton();
    buttonLoadPalette->position = { 150, 60 };
    buttonLoadPalette->text = "Load palette";
    buttonLoadPalette->wxWidth = 120;
    buttonLoadPalette->wxHeight = 30;
    buttonLoadPalette->setCallbackListener(EVENT_PALETTECOLORPICKER_LOADPALETTE, this);
    colorPaletteTabs->tabs[1].wxs.addDrawable(buttonLoadPalette);

    UIDropdown* defaultpalettePicker = new UIDropdown(palettes);
    defaultpalettePicker->position = XY{ 20, 20 };
    defaultpalettePicker->wxWidth = 300;
    defaultpalettePicker->wxHeight = 30;
    defaultpalettePicker->text = "Default palettes";
    defaultpalettePicker->setCallbackListener(EVENT_PALETTECOLORPICKER_PALETTELIST, this);
    colorPaletteTabs->tabs[1].wxs.addDrawable(defaultpalettePicker);

    updateForcedColorPaletteButtons();
}

void PalettizedEditorColorPicker::render(XY position)
{
    uint32_t colorNow = (upcastCaller->pickedPaletteIndex == -1 || upcastCaller->pickedPaletteIndex >= upcastCaller->palette.size()) ? 0x00000000 : upcastCaller->palette[upcastCaller->pickedPaletteIndex];
    SDL_Color colorNowB = { (colorNow >> 16) & 0xff, (colorNow >> 8) & 0xff, colorNow & 0xff, (colorNow >> 24) & 0xff };
    hsv colorNowHSV = rgb2hsv({colorNowB.r / 255.0, colorNowB.g / 255.0, colorNowB.b / 255.0});

    SDL_Color previewCol = colorNowB;

    SDL_Rect r = SDL_Rect{ position.x, position.y, wxWidth, wxHeight };
    SDL_Color devalColor = rgb2sdlcolor(hsv2rgb(hsv{ colorNowHSV.h, colorNowHSV.s, dxmax(colorNowHSV.v / 6, 0.1) }));
    SDL_Color devalColor2 = rgb2sdlcolor(hsv2rgb(hsv{ colorNowHSV.h, colorNowHSV.s, dxmax(colorNowHSV.v / 18, 0.05) }));
    devalColor.a = devalColor2.a = focused ? 0xaf : 0x30;
    renderGradient(r, sdlcolorToUint32(devalColor2), sdlcolorToUint32(devalColor), sdlcolorToUint32(devalColor), sdlcolorToUint32(devalColor));
    //SDL_SetRenderDrawColor(g_rd, previewCol.r/6, previewCol.g / 6, previewCol.b / 6, focused ? 0xaf : 0x30);
    //SDL_RenderFillRect(g_rd, &r);

    SDL_Color valCol = rgb2sdlcolor(hsv2rgb(hsv{ colorNowHSV.h, colorNowHSV.s, dxmin(colorNowHSV.v + 0.4, 1.0) }));
    if (focused) {
        SDL_SetRenderDrawColor(g_rd, valCol.r, valCol.g, valCol.b, 255);
        drawLine({ position.x, position.y }, { position.x, position.y + wxHeight }, XM1PW3P1(focusTimer.percentElapsedTime(300)));
        drawLine({ position.x, position.y }, { position.x + wxWidth, position.y }, XM1PW3P1(focusTimer.percentElapsedTime(300)));
    }

    XY tabOrigin = xyAdd(position, colorPaletteTabs->position);
    tabOrigin.y += colorPaletteTabs->buttonsHeight;

    r = SDL_Rect{ position.x + wxWidth - 60, position.y + wxHeight - 40, 55, 35 };
    SDL_SetRenderDrawColor(g_rd, previewCol.r, previewCol.g, previewCol.b, focused ? 0xff : 0x30);
    SDL_RenderFillRect(g_rd, &r);
    SDL_SetRenderDrawColor(g_rd, valCol.r, valCol.g, valCol.b, focused ? 0xff : 0x30);
    SDL_RenderDrawRect(g_rd, &r);

    g_fnt->RenderString("COLOR PICKER", position.x + 5, position.y + 1);

    subWidgets.renderAll(position);
}

void PalettizedEditorColorPicker::eventButtonPressed(int evt_id)
{
    if (evt_id == EVENT_COLORPICKER_TOGGLEERASER) {
        toggleEraser();
    }
    else if (evt_id == EVENT_COLORPICKER_TOGGLEBLENDMODE) {
        toggleAlphaBlendMode();
    }
    else if (evt_id == EVENT_PALETTECOLORPICKER_SAVEPALETTE) {
        platformTrySaveOtherFile(this, { {".voidplt", "voidsprite palette"} }, "save palette", EVENT_PALETTECOLORPICKER_SAVEPALETTE);
    }
    else if (evt_id == EVENT_PALETTECOLORPICKER_LOADPALETTE) {
        platformTryLoadOtherFile(this, { {".voidplt", "voidsprite palette"} }, "load palette", EVENT_PALETTECOLORPICKER_LOADPALETTE);
    }
    else if (evt_id >= 200) {
        //uint32_t col = upcastCaller->palette[evt_id - 200];
        setPickedPaletteIndex(evt_id - 200);
    }
}

void PalettizedEditorColorPicker::eventButtonRightClicked(int evt_id)
{
    if (evt_id >= 200) {
        //todo: open popup to edit the color
    }
}

void PalettizedEditorColorPicker::eventDropdownItemSelected(int evt_id, int index, std::string name)
{
    if (evt_id == EVENT_PALETTECOLORPICKER_PALETTELIST) {
		upcastCaller->setPalette(g_palettes[name]);
	}
}

void PalettizedEditorColorPicker::eventFileSaved(int evt_id, PlatformNativePathString name, int exporterIndex)
{
    if (evt_id == EVENT_PALETTECOLORPICKER_SAVEPALETTE) {
        FILE* f = platformOpenFile(name, PlatformFileModeWB);
        if (f != NULL) {
            fwrite("VOIDPLT", 7, 1, f);
            uint8_t fileversion = 1;
            fwrite(&fileversion, 1, 1, f);
            uint32_t count = upcastCaller->palette.size();
            fwrite(&count, 1, 4, f);
			for (uint32_t col : upcastCaller->palette) {
				fwrite(&col, 1, 4, f);
			}
			fclose(f);
            g_addNotification(Notification("Success", "Palette file saved", 4000, NULL, COLOR_INFO));
        }
        else {
            g_addNotification(ErrorNotification("Error", "Could not save palette file"));
        }
	}
}

void PalettizedEditorColorPicker::eventFileOpen(int evt_id, PlatformNativePathString name, int exporterIndex)
{
    if (evt_id == EVENT_PALETTECOLORPICKER_LOADPALETTE) {
        FILE* f = platformOpenFile(name, PlatformFileModeRB);
        if (f != NULL) {
            char header[7];
            fread(header, 7, 1, f);
            if (memcmp(header, "VOIDPLT", 7) == 0) {
                uint8_t fileversion;
                fread(&fileversion, 1, 1, f);
                if (fileversion == 1) {
                    std::vector<uint32_t> newPalette;
                    uint32_t count;
                    fread(&count, 1, 4, f);
                    for (int x = 0; x < count; x++) {
                        uint32_t col;
                        fread(&col, 1, 4, f);
                        newPalette.push_back(col);
                    }
                    upcastCaller->setPalette(newPalette);
                    updateForcedColorPaletteButtons();
                    g_addNotification(Notification("Success", "Loaded palette file", 4000, NULL, COLOR_INFO));
                }
                else {
                    g_addNotification(ErrorNotification("Error", "Unsupported palette file version"));
                }
            }
            else {
                g_addNotification(ErrorNotification("Error", "Invalid palette file"));
            }
            fclose(f);
        }
        else {
            g_addNotification(ErrorNotification("Error", "Could not open palette file"));
        }
    }
	
}

void PalettizedEditorColorPicker::updateForcedColorPaletteButtons()
{
    colorPaletteTabs->tabs[0].wxs.freeAllDrawables();

    int paletteindex = 0;
    for (int y = 0; y < 16 && paletteindex < upcastCaller->palette.size(); y++) {
        for (int x = 0; x < 16 && paletteindex < upcastCaller->palette.size(); x++) {
            uint32_t col = upcastCaller->palette[paletteindex++];
            UIButton* colBtn = new UIButton();
            colBtn->colorBGFocused = colBtn->colorBGUnfocused = SDL_Color{ (uint8_t)((col >> 16) & 0xff), (uint8_t)((col >> 8) & 0xff), (uint8_t)(col & 0xff), (uint8_t)((col >> 24) & 0xff) };
            colBtn->wxHeight = 16;
            colBtn->wxWidth = 22;
            colBtn->position = { x * colBtn->wxWidth, 10 + y * colBtn->wxHeight };
            colBtn->setCallbackListener(200 + (y * 16 + x), this);
            colorPaletteTabs->tabs[0].wxs.addDrawable(colBtn);
        }
    }
}

void PalettizedEditorColorPicker::setPickedPaletteIndex(int32_t index)
{
    upcastCaller->pickedPaletteIndex = index;
}