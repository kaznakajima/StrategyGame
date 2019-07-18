#include "DrawParts.h"
#include "StageCreate.h"

DrawParts::DrawParts(string _fileName, int _layerNum)
{
	SetImage(_fileName, _layerNum);
}

DrawParts::DrawParts(string _fileName, bool _isCopy, int _layerNum)
{
	copy = _isCopy;

	SetImage(_fileName, _layerNum);
}

DrawParts::DrawParts(string _fileName, int _layerNum, int _rotaX, int _rotaY)
{
	isRotate = TRUE;
	SetRotate(_rotaX, _rotaY);
	SetImage(_fileName, _layerNum);
}

void DrawParts::Draw()
{
	// �`�悵�Ȃ�
	if (IsVisible() == false) return;

	// ��]�������邩
	if (isRotate == false) DrawGraph(x, y, imgID, TRUE);
	else DrawRotaGraph(x, y, rotaX, rotaY, imgID, TRUE);
}