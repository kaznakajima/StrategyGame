#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include "FileManager.h"
#include "Renderer.h"
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
	void AddDrawList(shared_ptr<Renderer>&);
	// �`��p�[�c����菜��
	void RemoveDrawList();

	void SetImage(string, int);
	void SetPosition(int, int);
	void SetRotate(int, int);
	bool IsRemove() { return isRemove; }
	void SetRemove();
	bool IsVisible() { return isVisible; }
	virtual void Draw();
	void Draw(string, int, int);

	// ���ۂɕ\�����郊�X�g
	vector<shared_ptr<DrawManager>> drawList;
	vector<shared_ptr<Renderer>> rendererList;
	map<string, shared_ptr<DrawManager>> drawData;
	// �ړ���A�U����G���A�\���p
	vector<shared_ptr<Renderer>> characterParts;

	const shared_ptr<DrawManager>& GetDrawParts(string);
	const shared_ptr<Renderer>& GetDrawParts(int, int, int);
};