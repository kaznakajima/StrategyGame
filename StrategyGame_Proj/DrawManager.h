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

	// 描画パーツを格納していく
	void AddDrawList(shared_ptr<DrawManager>&);
	void AddDrawList(shared_ptr<Renderer>&);
	// 描画パーツを取り除く
	void RemoveDrawList();

	void SetImage(string, int);
	void SetPosition(int, int);
	void SetRotate(int, int);
	bool IsRemove() { return isRemove; }
	void SetRemove();
	bool IsVisible() { return isVisible; }
	virtual void Draw();
	void Draw(string, int, int);

	// 実際に表示するリスト
	vector<shared_ptr<DrawManager>> drawList;
	vector<shared_ptr<Renderer>> rendererList;
	map<string, shared_ptr<DrawManager>> drawData;
	// 移動先、攻撃先エリア表示用
	vector<shared_ptr<Renderer>> characterParts;

	const shared_ptr<DrawManager>& GetDrawParts(string);
	const shared_ptr<Renderer>& GetDrawParts(int, int, int);
};