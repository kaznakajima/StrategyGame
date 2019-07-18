#include "DrawManager.h"

void DrawManager::AddDrawList(shared_ptr<DrawManager>& _drawItem)
{
	// 中身がない場合、既に登録されている場合は除く
	if (_drawItem->fileName == "") return;

	drawList.push_back(_drawItem);
	// レイヤー番号順にソート
	std::sort(drawList.begin(), drawList.end(), [](const shared_ptr<DrawManager> &a, const shared_ptr<DrawManager> &b) {
		return a->layer < b->layer;
	});
	drawData.emplace(_drawItem->fileName, _drawItem);
}

// 描画パーツを取り除く
void DrawManager::RemoveDrawList()
{
	// 取り除くものを見つける
	auto lostItem = remove_if(drawList.begin(), drawList.end(), [](const shared_ptr<DrawManager>& a) {
		return a->isRemove == true;
	});
	if (lostItem != drawList.end()) {
		drawList.erase(lostItem);
		RemoveDrawList();
	}
}

void DrawManager::SetRemove()
{
	for_each(drawList.begin(), drawList.end(), [](const shared_ptr<DrawManager>& draw)
	{
		if (draw->copy) draw->isRemove = true;
	});
	RemoveDrawList();
}

// 描画する素材を返す
const shared_ptr<DrawManager>& DrawManager::GetDrawParts(string _fileName)
{
	auto parts = drawData.find(_fileName);
	if (parts != drawData.end()) {
		return parts->second;
	}
}

void DrawManager::SetImgID(int _ID)
{
	imgID = _ID;
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
	// 回転を加えるか
	if(isRotate == false) DrawGraph(x, y, imgID, TRUE);
	else DrawRotaGraph(x, y, rotaX, rotaY, imgID, TRUE);
}

void DrawManager::Draw(string _fileName, int _x, int _y)
{
	DrawGraph(x, y, imgID, TRUE);
}