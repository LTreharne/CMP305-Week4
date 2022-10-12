// Application.h
#ifndef _APP1_H
#define _APP1_H

// Includes
#include "DXF.h"	// include dxframework
#include "LightShader.h"
#include "TerrainMesh.h"
#include "WaterPlane.h"
#include "WaterShader.h"

class App1 : public BaseApplication
{
public:

	App1();
	~App1();
	void init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN);

	bool frame();

protected:
	bool render();
	void gui();

private:
	LightShader* shader;
	WaterShader* waterShader;
	TerrainMesh* m_Terrain;
	WaterPlane* waterTerrain;

	Light* light;

	float yoff = -29, waterTimer;
	int terrainResolution = 128;
};

#endif