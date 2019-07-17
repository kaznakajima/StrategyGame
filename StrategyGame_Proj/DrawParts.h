#pragma once
#include "DrawManager.h"

class DrawParts : public DrawManager
{
public:
	DrawParts(string, int);
	DrawParts(string, bool, int);
	DrawParts(string, int, int, int);

	void Draw();
};