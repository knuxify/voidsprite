#include "EditorSpritesheetPreview.h"
#include "FontRenderer.h"
#include "maineditor.h"

void EditorSpritesheetPreview::render(XY at)
{
	XY tileSize = caller->caller->tileDimensions;
	int wxWidth = ixmax(80, tileSize.x * caller->canvasZoom) + 8;
	int wxHeight = ixmax(30, tileSize.y * caller->canvasZoom) + 20 + 8;
	XY origin = { g_windowW - wxWidth - 4, g_windowH - wxHeight - 4 - 40 };
	SDL_Rect drawRect = {
		origin.x,
		origin.y,
		wxWidth,
		wxHeight
	};
	SDL_SetRenderDrawColor(g_rd, 0xd, 0xd, 0xd, 0xd0);
	renderGradient(drawRect, 0x200d0d0d, 0x800d0d0d, 0x800d0d0d, 0xe0000000);
	//SDL_RenderFillRect(g_rd, &drawRect);
	g_fnt->RenderString("PREVIEW", origin.x+5, origin.y+1);
	caller->drawPreview(xyAdd(origin, XY{ 4, 24 }));
}
