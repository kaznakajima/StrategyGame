#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include "FileManager.h"
#include <memory>
#include <vector>
#include <algorithm>
using namespace std;

class DrawManager : public Singleton<DrawManager>
{
protected:
	string fileName;
	int imgID;
	int x, y;
	int width, height;
	bool isRotate;
	int rotaX, rotaY;
	bool copy = false;
public:
	int layer;
	bool isRemove = false;
	bool isVisible = true;

	// �`��p�[�c���i�[���Ă���
	void AddDrawList(shared_ptr<DrawManager>&);
	// �`��p�[�c����菜��
	void RemoveDrawList();

	void SetImage(string, int);
	void SetImgID(int);
	void SetPosition(int, int);
	void SetRotate(int, int);
	bool IsRemove() { return isRemove; }
	void SetRemove();
	bool IsVisible() { return isVisible; }
	virtual void Draw();
	void Draw(string, int, int);

	// ���ۂɕ\�����郊�X�g
	vector<shared_ptr<DrawManager>> drawList;
	map<string, shared_ptr<DrawManager>> drawData;

	const shared_ptr<DrawManager>& GetDrawParts(string);
};