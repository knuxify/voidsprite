#include "Brush1x1ArcX.h"
#include "maineditor.h"

void Brush1x1ArcX::clickPress(MainEditor* editor, XY pos) {
	editor->SetPixel(pos, 0xFF000000 | editor->pickedColor);
}

void Brush1x1ArcX::clickDrag(MainEditor* editor, XY from, XY to) {
	//editor->DrawLine(from, to, 0xFF000000 | editor->pickedColor);
	rasterizeLine(from, to, [&](XY a)->void {
		editor->SetPixel(a, editor->pickedColor);
	}, 1);
}
