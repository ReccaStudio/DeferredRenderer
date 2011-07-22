#pragma once

#include "Defines.h"
#include "Lights.h"
#include "Camera.h"
#include "PostProcess.h"
#include "CombinePostProcess.h"
#include "ModelInstance.h"
#include "GBuffer.h"
#include "LightBuffer.h"
#include "IHasContent.h"
#include "ModelRenderer.h"
#include "PointLightRenderer.h"
#include "DirectionalLightRenderer.h"
#include "SpotLightRenderer.h"
#include "BoundingObjectRenderer.h"
#include <vector>

class Renderer : public IHasContent
{
private:
	bool _begun;
	GBuffer _gBuffer;
	LightBuffer _lightBuffer;
	std::vector<ModelInstance*> _models;
	std::vector<PostProcess*> _postProcesses;
	ModelRenderer _modelRenderer;
	CombinePostProcess _combinePP;

	bool _drawBoundingObjects;
	BoundingObjectRenderer _boRenderer;
	
	ID3D11Texture2D* _ppTextures[2];
	ID3D11ShaderResourceView* _ppShaderResourceViews[2];
	ID3D11RenderTargetView* _ppRenderTargetViews[2];

	AmbientLight _ambientLight;
	PointLightRenderer _pointLightRenderer;
	DirectionalLightRenderer _directionalLightRenderer;
	SpotLightRenderer _spotLightRenderer;

public:
	Renderer();
	~Renderer();

	void AddModel(ModelInstance* model);
	void AddLight(AmbientLight* light);
	void AddLight(DirectionalLight* light, bool shadowed);
	void AddLight(PointLight* light, bool shadowed);
	void AddLight(SpotLight* light, bool shadowed);
	void AddPostProcess(PostProcess* postProcess);

	bool GetDrawBoundingObjects() const { return _drawBoundingObjects; }
	void SetDrawBoundingObjects(bool draw) { _drawBoundingObjects = draw; }

	HRESULT Begin();
	HRESULT End(ID3D11DeviceContext* pd3dImmediateContext, Camera* camera);

	HRESULT OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);	
	void OnD3D11DestroyDevice();

	HRESULT OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                            const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc);
	void OnD3D11ReleasingSwapChain();
};