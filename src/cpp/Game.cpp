#include "Game.h"

Game::Game()
	: _renderer(), _camera(0.1f, 50.0f, 1.0f, 1.0f), _powerPlant(L"\\models\\powerplant\\powerplant.sdkmesh"),
	 _tankScene(L"\\models\\tankscene\\tankscene.sdkmesh")
{
	_camera.SetPosition(XMVectorSet(-5.0f, 5.0f, -5.0f, 1.0f));
	_camera.SetXRotation(PiOver4);
	_camera.SetYRotation(PiOver8);

	//_directionalLights.push_back(
	//	DirectionalLight(XMVectorSet(1.0f, 0.4f, 0.2f, 1.0f), 0.4f, XMVectorSet(-0.2f, 0.5f, -0.2f, 1.0f)));
	//_directionalLights.push_back(
	//	DirectionalLight(XMVectorSet(0.6f, 1.0f, 0.3f, 1.0f), 0.3f, XMVectorSet(0.5f, 0.5f, -0.5f, 1.0f)));
	_directionalLights.push_back(
		DirectionalLight(XMVectorSet(1.0f, 1.0f, 0.3f, 1.0f), 1.0f, XMVectorSet(0.8f, 0.9, 0.8f, 1.0f)));

	_pointLights.push_back(
		PointLight(XMVectorSet(0.0f, 1.0f, 0.2f, 1.0f), 2.0f, XMVectorSet(0.0f, 4.0f, 0.0f, 1.0f), 15.0f));
}

Game::~Game()
{
}

void Game::OnKeyboard(UINT nChar, bool bKeyDown, bool bAltDown)
{
}

void Game::OnFrameMove(double totalTime, float dt)
{
	KeyboardState kb = KeyboardState::GetState();
	MouseState mouse = MouseState::GetState();
	
	if (mouse.IsButtonDown(LeftButton))
	{
		const float mouseRotateSpeed = 0.002f;
		_camera.SetXRotation(_camera.GetXRotation() + (mouse.GetDX() * mouseRotateSpeed));
		_camera.SetYRotation(_camera.GetYRotation() + (mouse.GetDY() * mouseRotateSpeed));

		// Set the mouse back one frame 
		MouseState::SetCursorPosition(mouse.GetX() - mouse.GetDX(), mouse.GetY() - mouse.GetDY());

		XMFLOAT2 moveDir = XMFLOAT2(0.0f, 0.0f);
		if (kb.IsKeyDown(W) || kb.IsKeyDown(Up))
		{
			moveDir.y++;
		}
		if (kb.IsKeyDown(S) || kb.IsKeyDown(Down))
		{
			moveDir.y--;
		}
		if (kb.IsKeyDown(A) || kb.IsKeyDown(Left))
		{
			moveDir.x--;
		}
		if (kb.IsKeyDown(D) || kb.IsKeyDown(Right))
		{
			moveDir.x++;
		}
				
		const float cameraMoveSpeed = 5.0f;
		XMVECTOR cameraMove = ((moveDir.y * _camera.GetForward()) + (moveDir.x * _camera.GetRight())) * 
			(cameraMoveSpeed * dt);

		_camera.SetPosition(XMVectorAdd(_camera.GetPosition(), cameraMove));
	}
}

void Game::OnD3D11FrameRender(ID3D11Device* pd3dDevice, ID3D11DeviceContext* pd3dImmediateContext)
{
	HRESULT hr;

	V(_renderer.Begin());

	//_renderer.AddModel(&_powerPlant);
	_renderer.AddModel(&_tankScene);

	for (vector<DirectionalLight>::iterator i = _directionalLights.begin(); i != _directionalLights.end(); i++)
	{
		DirectionalLight* light = &(*i);
		_renderer.AddLight(light, true);
	}

	for (vector<PointLight>::iterator i = _pointLights.begin(); i != _pointLights.end(); i++)
	{
		PointLight* light = &(*i);
		_renderer.AddLight(light, false);
	}

	V(_renderer.End(pd3dDevice, pd3dImmediateContext, &_camera));
}

HRESULT Game::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;

	V_RETURN(_renderer.OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc));
	//V_RETURN(_powerPlant.OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc));
	V_RETURN(_tankScene.OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc));

	return S_OK;
}

void Game::OnD3D11DestroyDevice()
{
	_renderer.OnD3D11DestroyDevice();
	//_powerPlant.OnD3D11DestroyDevice();
	_tankScene.OnD3D11DestroyDevice();
}

HRESULT Game::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                        const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;

	float fAspectRatio = pBackBufferSurfaceDesc->Width / (float)pBackBufferSurfaceDesc->Height;
	_camera.SetAspectRatio(fAspectRatio);
	
	V_RETURN(_renderer.OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc));
	//V_RETURN(_powerPlant.OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc));
	V_RETURN(_tankScene.OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc));

	return S_OK;
}
void Game::OnD3D11ReleasingSwapChain()
{
	_renderer.OnD3D11ReleasingSwapChain();
	//_powerPlant.OnD3D11ReleasingSwapChain();
	_tankScene.OnD3D11ReleasingSwapChain();
}