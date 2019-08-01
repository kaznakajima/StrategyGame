#pragma once

// レンダラークラス
class Renderer
{
public:
	// 描画の種類
	enum DrawType
	{
		NORMAL = 0,
		ROTATE,
	};

	// コンストラクタ
	Renderer(int, int, int);

	int imgID;
	int x, y;
	int width, height;
	bool isRotate;
	float rotaX, rotaY;
	int layerNum;

	bool isVisible;
	bool isRemove;

	void SetImageID(int);
	void SetPosition(int, int);
	void SetRotate(float, float);
	virtual void Draw();

	// プロパティ
	void SetVisible(bool);
	bool GetVisible() { return isVisible; }
	void SetRemove(bool);
    bool GetRemove() { return isRemove; }
private:
	DrawType myType;
	void SetDrawType(DrawType);
};