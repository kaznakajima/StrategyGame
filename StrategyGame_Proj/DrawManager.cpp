#include "DrawManager.h"

void DrawManager::AddDrawList(shared_ptr<DrawManager>& _drawItem)
{
	drawList.push_back(_drawItem);
	// ���C���[�ԍ����Ƀ\�[�g
	std::sort(drawList.begin(), drawList.end(), [](const shared_ptr<DrawManager> &a, const shared_ptr<DrawManager> &b) {
		return a->layer < b->layer;
	});
	drawData.emplace(_drawItem->fileName, _drawItem);
}

// �`��p�[�c�̊i�[
void DrawManager::AddDrawList(shared_ptr<Renderer>& _drawItem)
{
	rendererList.push_back(_drawItem);
	// ���C���[�ԍ����Ƀ\�[�g
	std::sort(rendererList.begin(), rendererList.end(), [](const shared_ptr<Renderer> &a, const shared_ptr<Renderer> &b) {
		return a->layerNum < b->layerNum;
	});
}

// �`��p�[�c����菜��
void DrawManager::RemoveDrawList()
{
	// ��菜�����̂�������
	auto lostItem = remove_if(rendererList.begin(), rendererList.end(), [](const shared_ptr<Renderer>& a) {
		return a->GetRemove() == true;
	});
	if(lostItem != rendererList.end()) rendererList.erase(lostItem);
}

void DrawManager::SetRemove()
{
	for_each(drawList.begin(), drawList.end(), [](const shared_ptr<DrawManager>& draw)
	{
		if (draw->copy) draw->isRemove = true;
	});
}

// �`�悷��f�ނ�Ԃ�
const shared_ptr<DrawManager>& DrawManager::GetDrawParts(string _fileName)
{
	auto parts = drawData.find(_fileName);
	if (parts != drawData.end()) {
		return parts->second;
	}
}

// �w����W�ɑ��݂���`��p�[�c��Ԃ�
const shared_ptr<Renderer>& DrawManager::GetDrawParts(int _id, int _x, int _y)
{
	// �z��̒��g�𒲂ׂ�
	for (int num = 0; num < rendererList.size(); num++) {
		if (rendererList[num]->imgID == _id && rendererList[num]->x == _x && rendererList[num]->y == _y) return rendererList[num];
	}

	return nullptr;
}

void DrawManager::SetImage(string _fileName, int _layerNum)
{
	if (imgID != 0) return;

	fileName = _fileName;
	imgID = FileManager::Instance()->GetFileHandle(_fileName);
	layer = _layerNum;
	GetGraphSize(imgID, &width, &height);
}

void DrawManager::SetPosition(int _x, int _y)
{
	x = _x; y = _y;
}

void DrawManager::SetRotate(int _rotaX, int _rotaY)
{
	rotaX = _rotaX, rotaY = _rotaY;
}

void DrawManager::Draw()
{
	for_each(rendererList.begin(), rendererList.end(), [](const shared_ptr<Renderer>& _ren)
	{
		_ren->Draw();
	});
}

void DrawManager::Draw(string _fileName, int _x, int _y)
{
	DrawGraph(x, y, imgID, TRUE);
}