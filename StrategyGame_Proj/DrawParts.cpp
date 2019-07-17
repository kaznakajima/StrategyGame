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
	// 描画しない
	if (IsVisible() == false) return;

	// コピー用描画
	if (copy) {
		// 自身の攻撃範囲表示
		for (int y = 0; y < StageCreate::Instance()->MAP_SIZEY; y++) {
			for (int x = 0; x < StageCreate::Instance()->MAP_SIZEX; x++) {
				if (StageCreate::Instance()->checkMove[y][x] == true) {
					DrawGraph(x * CHIP_SIZE, y * CHIP_SIZE, imgID, TRUE);
				}
			}
		}
		return;
	}

	// 回転を加えるか
	if (isRotate == false) DrawGraph(x, y, imgID, TRUE);
	else DrawRotaGraph(x, y, rotaX, rotaY, imgID, TRUE);
}