#include "DrawParts.h"

DrawParts::DrawParts(string _fileName, int _layerNum)
{
	SetImage(_fileName, _layerNum);
}

void DrawParts::Draw()
{
	// ‰ñ“]‚ð‰Á‚¦‚é‚©
	if (isRotate) DrawGraph(x, y, imgID, TRUE);
	else DrawRotaGraph(x, y, rotaX, rotaY, imgID, TRUE);
}