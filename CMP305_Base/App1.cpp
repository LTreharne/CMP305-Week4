#include "App1.h"

App1::App1()
{
	m_Terrain = nullptr;
	shader = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input *in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);

	// Load textures
	textureMgr->loadTexture(L"grass", L"res/grass.png");
	textureMgr->loadTexture(L"water", L"res/water2.png");
	textureMgr->loadTexture(L"Rock", L"res/rocks.png");
	textureMgr->loadTexture(L"Snow", L"res/snow.png");
	textureMgr->loadTexture(L"Sand", L"res/sand.png");
	textureMgr->loadTexture(L"white", L"res/DefaultDiffuse.png");

	// Create Mesh object and shader object
	m_Terrain = new TerrainMesh(renderer->getDevice(), renderer->getDeviceContext());
	waterTerrain = new WaterPlane(renderer->getDevice(), renderer->getDeviceContext());

	shader = new LightShader(renderer->getDevice(), hwnd);
	waterShader = new WaterShader(renderer->getDevice(), hwnd);
	waterTimer = 0;

	// Initialise light
	light = new Light();
	light->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light->setDirection(0.5f, -0.6f, 0.5f);
	srand(time(nullptr));

}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (m_Terrain)
	{
		delete m_Terrain;
		m_Terrain = 0;
	}

	if (waterTerrain)
	{
		delete waterTerrain;
		waterTerrain = 0;
	}

	if (shader)
	{
		delete shader;
		shader = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	//WATER MOVEMENT
	waterTimer += timer->getTime();
	if (waterTimer>10) {
		//waterTimer -= 10;
	}

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;

	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);

	// Generate the view matrix based on the camera's position.
	camera->update();
	//waterTerrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(), false, waterTimer/100.f);


	// Get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	worldMatrix = renderer->getWorldMatrix();
	viewMatrix = camera->getViewMatrix();
	projectionMatrix = renderer->getProjectionMatrix();

	// Send geometry data, set shader parameters, render object with shader
	m_Terrain->sendData(renderer->getDeviceContext());
	shader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"grass"),textureMgr->getTexture(L"Rock"), textureMgr->getTexture(L"Snow"),textureMgr->getTexture(L"Sand"), light);
	shader->render(renderer->getDeviceContext(), m_Terrain->getIndexCount());


	// Send geometry data, set shader parameters, render object with shader
	waterTerrain->sendData(renderer->getDeviceContext());
	XMFLOAT3 waterValues = XMFLOAT3(waterTerrain->size() ,  terrainResolution, waterTimer);

	waterShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"water"), light, waterValues );
	waterShader->render(renderer->getDeviceContext(), waterTerrain->getIndexCount());



	// Render GUI
	gui();

	// Swap the buffers
	renderer->endScene();

	return true;
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	bool changed=false;
	changed |=ImGui::SliderInt("Terrain Resolution", &terrainResolution, 2, 1024);
	changed|=ImGui::SliderFloat("Y offset", &yoff, -33, -27);

	
		if (changed) {
			m_Terrain->Resize(terrainResolution,yoff);
			m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(),false);
		}
	

	if (ImGui::Button("Smooth Terrain")) {
		m_Terrain->smooth();
		m_Terrain->Regenerate(renderer->getDevice(), renderer->getDeviceContext(),true);
	}

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

