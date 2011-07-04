#pragma once

#include "Defines.h"
#include "PostProcess.h"
#include "FullscreenQuad.h"

struct CB_SKY_PROPERTIES
{
	UINT SunEnabled;
	float SunWidth;
	_DECLSPEC_ALIGN_16_ XMFLOAT3 SkyColor;
	_DECLSPEC_ALIGN_16_ XMFLOAT3 SunColor;	
	_DECLSPEC_ALIGN_16_ XMFLOAT3 SunDirection;
	_DECLSPEC_ALIGN_16_ XMFLOAT3 CameraPosition;	
	_DECLSPEC_ALIGN_16_ XMFLOAT4X4 InverseViewProjection;
};

class SkyPostProcess : public PostProcess
{
private:
	XMFLOAT3 _skyColor;
	XMFLOAT3 _sunColor;
	XMFLOAT3 _sunDirection;
	float _sunWidth;
	bool _enableSun;

	ID3D11PixelShader* _skyPS;

	ID3D11Buffer* _skyProperties;

	FullscreenQuad _fsQuad;
public:
	SkyPostProcess();
	~SkyPostProcess();

	const XMFLOAT3& GetSkyColor() const { return _skyColor; }
	const XMFLOAT3& GetSunColor() const { return _sunColor; }
	const XMFLOAT3& GetSunDirection() const { return _sunDirection; }
	float GetSunWidth() const { return _sunWidth; }
	bool GetSunEnabled() const { return _enableSun; }

	void SetSkyColor(const XMFLOAT3& skyCol) { _skyColor = skyCol; }
	void SetSunColor(const XMFLOAT3& sunCol) { _sunColor = sunCol; }
	void SetSunDirection(const XMFLOAT3& sunDir);
	void SetSunWidth(float width) { _sunWidth = max(width, 0.0f); }
	void SetSunEnabled(bool enable) { _enableSun = enable; }

	HRESULT Render(ID3D11DeviceContext* pd3dImmediateContext, ID3D11ShaderResourceView* src,
		ID3D11RenderTargetView* dstRTV, Camera* camera, GBuffer* gBuffer, LightBuffer* lightBuffer);

	HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
	void OnD3D11DestroyDevice();

	HRESULT OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
		const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
	void OnD3D11ReleasingSwapChain();
};