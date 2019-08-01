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
	// ����ԂłȂ��Ȃ烊�^�[��
	if (isVisible == false) return;

	// Draw�^�C�v�ɉ����ĕ`��
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

// �v���p�e�B
void Renderer::SetVisible(bool _value) { isVisible = _value; }
void Renderer::SetRemove(bool _value) { isRemove = _value; }