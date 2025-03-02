#pragma once
#include "precomp.h"

class overlay
{
public:
	static void menuDrawing();



	static void ApplyTheme();


	static void Render();

	static inline bool Enabled;

	static void drawBox(Vector3 World2Screen);

	static void drawBox(Vector3 World2Screen, float rounding);


	static void drawLine(Vector3 World2Screen);

	static void drawLine(Vector3 World2Screen, Vector3 origin);

	static void drawText(Vector3 World2Screen, float yOffset, const char* text);

	static void drawText(Vector3 World2Screen, float yOffset, ImColor color, const char* text);

	static void drawHealth(Vector3 World2Screen, float health);


};