#include "PalettizedEditorLayerPicker.h"
#include "MainEditorPalettized.h"

PalettizedEditorLayerPicker::PalettizedEditorLayerPicker(MainEditorPalettized* editor)
{
    caller = editor;
    upcastCaller = editor;

    UIButton* addBtn = new UIButton();
    addBtn->position = { 5, 30 };
    //addBtn->text = "+";
    addBtn->wxWidth = 30;
    addBtn->setCallbackListener(-1, this);
    addBtn->icon = g_iconLayerAdd;
    layerControlButtons.addDrawable(addBtn);

    UIButton* removeBtn = new UIButton();
    removeBtn->position = { addBtn->wxWidth + 5 + 5, 30 };
    //removeBtn->text = "-";
    removeBtn->wxWidth = 30;
    removeBtn->icon = g_iconLayerDelete;
    removeBtn->setCallbackListener(-2, this);
    layerControlButtons.addDrawable(removeBtn);

    UIButton* upBtn = new UIButton();
    upBtn->position = { addBtn->wxWidth + removeBtn->wxWidth + 5 + 5 + 5, 30 };
    //upBtn->text = "Up";
    upBtn->wxWidth = 30;
    upBtn->icon = g_iconLayerUp;
    upBtn->setCallbackListener(-3, this);
    layerControlButtons.addDrawable(upBtn);

    UIButton* downBtn = new UIButton();
    downBtn->position = { addBtn->wxWidth + removeBtn->wxWidth + upBtn->wxWidth + 5 + 5 + 5 + 5, 30 };
    //downBtn->text = "Dn.";
    downBtn->wxWidth = 30;
    downBtn->icon = g_iconLayerDown;
    downBtn->setCallbackListener(-4, this);
    layerControlButtons.addDrawable(downBtn);

    UIButton* mergeDownBtn = new UIButton();
    mergeDownBtn->position = { addBtn->wxWidth + removeBtn->wxWidth + upBtn->wxWidth + downBtn->wxWidth + 5 + 5 + 5 + 5 + 5, 30 };
    //mergeDownBtn->text = "Mrg";
    mergeDownBtn->wxWidth = 30;
    mergeDownBtn->icon = g_iconLayerDownMerge;
    mergeDownBtn->setCallbackListener(-5, this);
    layerControlButtons.addDrawable(mergeDownBtn);

    UIButton* duplicateBtn = new UIButton();
    duplicateBtn->position = { addBtn->wxWidth + removeBtn->wxWidth + upBtn->wxWidth + downBtn->wxWidth + mergeDownBtn->wxWidth + 5 + 5 + 5 + 5 + 5 + 5, 30 };
    duplicateBtn->wxWidth = 30;
    duplicateBtn->icon = g_iconLayerDuplicate;
    duplicateBtn->setCallbackListener(-6, this);
    layerControlButtons.addDrawable(duplicateBtn);
}
