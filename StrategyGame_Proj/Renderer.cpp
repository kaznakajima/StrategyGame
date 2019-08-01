#include "Renderer.h"
#include "DxLib.h"

Renderer::Renderer(int _id, int _layer, int _type)
{
	SetImageID(_id);
	layerNum = _layer;
	SetDrawType((DrawType)_type);
}

void Renderer::SetImageID(int _id)
{
	imgID = _id;
}

void Renderer::SetPosition(int _x, int _y)
{
	x = _x; y = _y;
}

void Renderer::SetRotate(float _rotaX, float _rotaY)
{
	rotaX = _rotaX; rotaY = _rotaY;
}

void Renderer::SetDrawType(DrawType _type)
{
	myType = _type;
}

void Renderer::Draw()
{
	// 可視状態でないならリターン
	if (isVisible == false) return;

	// Drawタイプに応じて描画
	switch (myType)
	{
	case NORMAL:
		DrawGraph(x, y, imgID, TRUE);
		break;
	case ROTATE:
		DrawRotaGraph(x, y, rotaX, rotaY, imgID, TRUE);
		break;
	default:
		break;
	}
}

// プロパティ
void Renderer::SetVisible(bool _value) { isVisible = _value; }
void Renderer::SetRemove(bool _value) { isRemove = _value; }