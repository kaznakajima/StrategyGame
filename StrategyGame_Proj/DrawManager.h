#pragma once
#include "DxLib.h"
#include "Singleton.h"
#include "FileManager.h"
#include <memory>
#include <vector>
using namespace std;

class DrawManager : public Singleton<DrawManager>
{
	bool isRemove;
protected:
	int imgID;
	int x, y;
	int width, height;
	bool isRotate;
	int rotaX, rotaY;
public:
	int layer;

	// •`‰æƒp[ƒc‚ğŠi”[‚µ‚Ä‚¢‚­
	void AddDrawList(shared_ptr<DrawManager>&);
	// •`‰æƒp[ƒc‚ğæ‚èœ‚­
	void RemoveDrawList();

	void SetImage(string, int);
	void SetPosition(int, int);
	void SetRotate(bool, int, int);
	bool IsRemove() { return isRemove; }
	virtual void Draw();

	vector<shared_ptr<DrawManager>> drawList;
};