#pragma once
#include "PlaneMesh.h"
#include <vector>

using std::vector;
struct Vector3f
{
	float x;
	float y;
	float z;
};

struct Line
{
	Vector3f start;
	Vector3f end;
};
class TerrainMesh :
	public PlaneMesh {
public:
	TerrainMesh( ID3D11Device* device, ID3D11DeviceContext* deviceContext, int resolution = 128, float yoff=-29 );
	~TerrainMesh();


	void Resize( int newResolution,float );
	void Regenerate( ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool smooth );
	void smooth();
	void fault();
	
	float length(Line);
	const inline int GetResolution(){ return resolution; }
private:
	void CreateBuffers( ID3D11Device* device, VertexType* vertices, unsigned long* indices );
	void BuildHeightMap();

	float yoffset;
	const float m_UVscale = 10.0f;			//Tile the UV map 10 times across the plane
	const float terrainSize = 100.0f;		//What is the width and height of our terrain
	vector<vector<float>> heightMap;
};
