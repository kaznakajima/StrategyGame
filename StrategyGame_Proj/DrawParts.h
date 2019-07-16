#pragma once
#include "DrawManager.h"

class DrawParts : public DrawManager
{
public:
	DrawParts(string, int);

	void Draw();
};