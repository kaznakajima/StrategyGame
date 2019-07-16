#include "DrawManager.h"
#include <algorithm>

void DrawManager::AddDrawList(shared_ptr<DrawManager>& _drawItem)
{
	drawList.push_back(_drawItem);
	// ���C���[�ԍ����Ƀ\�[�g
	std::sort(drawList.begin(), drawList.end(), [](const shared_ptr<DrawManager> &a, const shared_ptr<DrawManager> &b) {
		return a->layer < b->layer;
	});
}

// �`��p�[�c����菜��
void DrawManager::RemoveDrawList()
{
	// ��菜�����̂�������
	auto lostItem = remove_if(drawList.begin(), drawList.end(), [](const shared_ptr<DrawManager> &a) {
		return a->IsRemove() == true;
	});
	drawList.erase(lostItem);
}

void DrawManager::SetImage(string _fileName, int _layerNum)
{
	if (imgID != 0) return;

	imgID = FileManager::Instance()->GetFileHandle(_fileName);
	layer = _layerNum;
	GetGraphSize(imgID, &width, &height);
}

void DrawManager::SetPosition(int _x, int _y)
{
	x = _x; y = _y;
}

void DrawManager::SetRotate(bool _isRotate, int _rotaX, int _rotaY)
{
	isRotate = _isRotate;
	rotaX = _rotaX;
	rotaY = _rotaY;
}

void DrawManager::Draw()
{
	// ��]�������邩
	if(isRotate) DrawGraph(x, y, imgID, TRUE);
	else DrawRotaGraph(x, y, rotaX, rotaY, imgID, TRUE);
}