#include "Renderer.h"
#include "Logger.h"

Renderer::Renderer() 
	: _begun(false), _pointLightRenderer(NULL), _directionalLightRenderer(NULL),
	  _spotLightRenderer(NULL), _boDrawTypes(BoundingObjectDrawType::None)
{
	_ambientLight.Color = XMFLOAT3(0.0f, 0.0f, 0.0f);

	for (UINT i = 0; i < 2; i++)
	{
		_ppTextures[i] = NULL;
		_ppShaderResourceViews[i] = NULL;
		_ppRenderTargetViews[i] = NULL;
	}
}

Renderer::~Renderer()
{
}

void Renderer::swapPPBuffers()
{
	ID3D11Texture2D* tmpTex = _ppTextures[0];
	ID3D11ShaderResourceView* tmpSRV = _ppShaderResourceViews[0];
	ID3D11RenderTargetView* tmpRTV = _ppRenderTargetViews[0];

	_ppTextures[0] = _ppTextures[1];
	_ppShaderResourceViews[0] = _ppShaderResourceViews[1];
	_ppRenderTargetViews[0] = _ppRenderTargetViews[1];

	_ppTextures[1] = tmpTex;
	_ppShaderResourceViews[1] = tmpSRV;
	_ppRenderTargetViews[1] = tmpRTV;
}

void Renderer::SetPointLightRenderer(LightRenderer<PointLight>* renderer)
{
	if (!_begun)
	{
		_pointLightRenderer = renderer;
	}
}

void Renderer::SetDirectionalLightRenderer(LightRenderer<DirectionalLight>* renderer)
{
	if (!_begun)
	{
		_directionalLightRenderer = renderer;
	}
}

void Renderer::SetSpotLightRenderer(LightRenderer<SpotLight>* renderer)
{
	if (!_begun)
	{
		_spotLightRenderer = renderer;
	}
}

void Renderer::AddModel(ModelInstance* model)
{
	if (model && _begun)
	{
		_models.push_back(model);
	}
}

void Renderer::AddLight(AmbientLight* light)
{
	_ambientLight.Color.x += light->Color.x;
	_ambientLight.Color.y += light->Color.y;
	_ambientLight.Color.z += light->Color.z;
}

void Renderer::AddLight(DirectionalLight* light, bool shadowed)
{
	if (light && _directionalLightRenderer && _begun)
	{
		_directionalLightRenderer->Add(light, shadowed);
	}
}

void Renderer::AddLight(PointLight* light, bool shadowed)
{
	if (light && _pointLightRenderer && _begun)
	{
		_pointLightRenderer->Add(light, shadowed);
	}
}

void Renderer::AddLight(SpotLight* light, bool shadowed)
{
	if (light && _spotLightRenderer && _begun)
	{
		_spotLightRenderer->Add(light, shadowed);
	}
}

void Renderer::AddPostProcess(PostProcess* postProcess)
{
	if (postProcess && _begun)
	{
		_postProcesses.push_back(postProcess);
	}
}

HRESULT Renderer::Begin()
{
	if (_begun)
	{
		return E_FAIL;
	}
	_begun = true;

	_models.clear();	

	if (_pointLightRenderer)
	{
		_pointLightRenderer->Clear();
	}
	if (_directionalLightRenderer)
	{
		_directionalLightRenderer->Clear();
	}
	if (_spotLightRenderer)
	{
		_spotLightRenderer->Clear();
	}
		
	_postProcesses.clear();
	_postProcesses.push_back(&_combinePP);

	_ambientLight.Color = XMFLOAT3(0.0f, 0.0f, 0.0f);

	_boRenderer.Clear();

	return S_OK;
}

HRESULT Renderer::End(ID3D11DeviceContext* pd3dImmediateContext, Camera* camera)
{
	HRESULT hr;

	if (!_begun)
	{
		return E_FAIL;
	}
	_begun = false;
	
	BEGIN_EVENT(L"Render");

	ID3D11RenderTargetView* pOrigRTV = NULL;
    ID3D11DepthStencilView* pOrigDSV = NULL;
    pd3dImmediateContext->OMGetRenderTargets( 1, &pOrigRTV, &pOrigDSV );

	// Calculate the scene bounds
	AxisAlignedBox sceneBounds;
	if (_models.size() > 0)
	{
		sceneBounds = _models[0]->GetAxisAlignedBox();
		for (UINT i = 1; i < _models.size(); i++)
		{
			AxisAlignedBox modelAABB = _models[i]->GetAxisAlignedBox();

			Collision::MergeAxisAlignedBoxes(&sceneBounds, &sceneBounds, &modelAABB);
		}
	}

	// render the shadow maps
	BEGIN_EVENT(L"Shadow Maps");
	if (_directionalLightRenderer)
	{
		V_RETURN(_directionalLightRenderer->RenderShadowMaps(pd3dImmediateContext, &_models, camera, &sceneBounds));
	}
	if (_pointLightRenderer)
	{
		V_RETURN(_pointLightRenderer->RenderShadowMaps(pd3dImmediateContext, &_models, camera, &sceneBounds));
	}
	if (_spotLightRenderer)
	{
		V_RETURN(_spotLightRenderer->RenderShadowMaps(pd3dImmediateContext, &_models, camera, &sceneBounds));
	}
	END_EVENT();

	// Render the scene to the gbuffer
	BEGIN_EVENT(L"G-Buffer");
	V_RETURN(_gBuffer.SetRenderTargetsAndDepthStencil(pd3dImmediateContext));
	V_RETURN(_gBuffer.Clear(pd3dImmediateContext));

	V_RETURN(_modelRenderer.RenderModels(pd3dImmediateContext, &_models, camera));

	V_RETURN(_gBuffer.UnsetRenderTargetsAndDepthStencil(pd3dImmediateContext));
	END_EVENT();

	// render the lights
	BEGIN_EVENT(L"Lights");
	V_RETURN(_lightBuffer.SetRenderTargets(pd3dImmediateContext, _gBuffer.GetReadOnlyDepthStencilView()));

	_lightBuffer.SetAmbientColor(_ambientLight.Color);
	V_RETURN(_lightBuffer.Clear(pd3dImmediateContext));

	if (_directionalLightRenderer)
	{
		V_RETURN(_directionalLightRenderer->RenderLights(pd3dImmediateContext, camera, &_gBuffer));
	}
	if (_pointLightRenderer)
	{
		V_RETURN(_pointLightRenderer->RenderLights(pd3dImmediateContext, camera, &_gBuffer));
	}
	if (_spotLightRenderer)
	{
		V_RETURN(_spotLightRenderer->RenderLights(pd3dImmediateContext, camera, &_gBuffer));
	}
	END_EVENT();

	V_RETURN(_lightBuffer.UnsetRenderTargetsAndDepthStencil(pd3dImmediateContext));

	BEGIN_EVENT(L"Post-Processes");
	
	// Find the final non-additive pp
	UINT lastNonAdditive = 0;
	for (UINT i = _postProcesses.size() - 1; i > 0; i--)
	{
		if (!_postProcesses[i]->GetIsAdditive())
		{
			lastNonAdditive = i;
			break;
		}
	}	
	
	// render the post processes	
	for (UINT i = 0; i < _postProcesses.size(); i++)
	{
		PostProcess* pp = _postProcesses[i];
		bool isAdditive = pp->GetIsAdditive();
		
		// calculate source resource view
		ID3D11ShaderResourceView* srcSRV = NULL;
		if (i > 0 && i <= lastNonAdditive)
		{
			srcSRV = _ppShaderResourceViews[0];
		}
		
		// Calculate destination render target
		ID3D11RenderTargetView* dstRTV = NULL;
		if (i >= lastNonAdditive)
		{
			dstRTV = pOrigRTV;
		}
		else if (isAdditive)
		{
			dstRTV = _ppRenderTargetViews[0];
		}
		else
		{
			dstRTV = _ppRenderTargetViews[1];
		}

		// Render the post process
		V_RETURN(pp->Render(pd3dImmediateContext, srcSRV, dstRTV, camera, &_gBuffer, &_lightBuffer));

		if (!isAdditive)
		{
			swapPPBuffers();
		}
	}
	END_EVENT();

	// Render the bounding shapes
	if (_boDrawTypes != BoundingObjectDrawType::None)
	{
		BEGIN_EVENT(L"Bounding Objects");
		
		if (_boDrawTypes & BoundingObjectDrawType::Models ||
			_boDrawTypes & BoundingObjectDrawType::ModelMeshes)
		{
			for (UINT i = 0; i < _models.size(); i++)
			{
				if (_boDrawTypes & BoundingObjectDrawType::Models)
				{
					OrientedBox obb = _models[i]->GetOrientedBox();
				}
				if (_boDrawTypes & BoundingObjectDrawType::ModelMeshes)
				{
					UINT meshCount = _models[i]->GetModelMeshCount();
					for (UINT j = 0; j < meshCount; j++)
					{
						OrientedBox obb = _models[i]->GetMeshOrientedBox(j);
						_boRenderer.Add(obb);
					}
				}				
			}
		}

		if (_boDrawTypes & BoundingObjectDrawType::Lights)
		{
			if (_pointLightRenderer)
			{
				UINT lightCount = _pointLightRenderer->GetCount();
				for (UINT i = 0; i < lightCount; i++)
				{
					PointLight* light = _pointLightRenderer->GetLight(i);

					Sphere lightSphere;
					lightSphere.Center = light->Position;
					lightSphere.Radius = light->Radius;
		
					_boRenderer.Add(lightSphere);
				}
			}
		}

		if (_boDrawTypes & BoundingObjectDrawType::CameraFrustums)
		{
			XMFLOAT4X4 fCameraProj = camera->GetProjection();
			XMMATRIX cameraProj = XMLoadFloat4x4(&fCameraProj);
		
			Frustum cameraFrust;
			Collision::ComputeFrustumFromProjection(&cameraFrust, &cameraProj);
			cameraFrust.Origin = camera->GetPosition();
			cameraFrust.Orientation = camera->GetOrientation();

			_boRenderer.Add(cameraFrust);
		}

		V_RETURN(_boRenderer.Render(pd3dImmediateContext, camera));
		
		END_EVENT();
	}
	
	SAFE_RELEASE(pOrigRTV);
    SAFE_RELEASE(pOrigDSV);
		
	END_EVENT();

	return S_OK;
}

HRESULT Renderer::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;
		
	// Call the sub content holders
	V_RETURN(_gBuffer.OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc));
	V_RETURN(_lightBuffer.OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc));
	V_RETURN(_modelRenderer.OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc));
	V_RETURN(_combinePP.OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc));
	V_RETURN(_boRenderer.OnD3D11CreateDevice(pd3dDevice, pBackBufferSurfaceDesc));

	return S_OK;
}

void Renderer::OnD3D11DestroyDevice()
{
	_gBuffer.OnD3D11DestroyDevice();
	_lightBuffer.OnD3D11DestroyDevice();
	_modelRenderer.OnD3D11DestroyDevice();
	_combinePP.OnD3D11DestroyDevice();
	_boRenderer.OnD3D11DestroyDevice();
}

HRESULT Renderer::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
                            const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;

	// Create the pp textures
	D3D11_TEXTURE2D_DESC ppTextureDesc = 
    {
        pBackBufferSurfaceDesc->Width,//UINT Width;
        pBackBufferSurfaceDesc->Height,//UINT Height;
        1,//UINT MipLevels;
        1,//UINT ArraySize;
        DXGI_FORMAT_R32G32B32A32_FLOAT,//DXGI_FORMAT Format;
        1,//DXGI_SAMPLE_DESC SampleDesc;
        0,
        D3D11_USAGE_DEFAULT,//D3D11_USAGE Usage;
        D3D11_BIND_RENDER_TARGET|D3D11_BIND_SHADER_RESOURCE,//UINT BindFlags;
        0,//UINT CPUAccessFlags;
        0//UINT MiscFlags;    
    };

	V_RETURN(pd3dDevice->CreateTexture2D(&ppTextureDesc, NULL, &_ppTextures[0]));
	SET_DEBUG_NAME(_ppTextures[0], "Renderer post process texture 0");

	V_RETURN(pd3dDevice->CreateTexture2D(&ppTextureDesc, NULL, &_ppTextures[1]));
	SET_DEBUG_NAME(_ppTextures[1], "Renderer post process texture 1");

	// Create the shader resource views
	D3D11_SHADER_RESOURCE_VIEW_DESC ppSRVDesc = 
    {
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        D3D11_SRV_DIMENSION_TEXTURE2D,
        0,
        0
    };
	ppSRVDesc.Texture2D.MipLevels = 1;

	V_RETURN(pd3dDevice->CreateShaderResourceView(_ppTextures[0], &ppSRVDesc, &_ppShaderResourceViews[0]));
	SET_DEBUG_NAME(_ppShaderResourceViews[0], "Renderer post process SRV 0");

	V_RETURN(pd3dDevice->CreateShaderResourceView(_ppTextures[1], &ppSRVDesc, &_ppShaderResourceViews[1]));
	SET_DEBUG_NAME(_ppShaderResourceViews[1], "Renderer post process SRV 1");

	// create the render target views
	D3D11_RENDER_TARGET_VIEW_DESC ppRTVDesc = 
	{
        DXGI_FORMAT_R32G32B32A32_FLOAT,
        D3D11_RTV_DIMENSION_TEXTURE2D,
        0,
        0
    };

	V_RETURN(pd3dDevice->CreateRenderTargetView(_ppTextures[0], &ppRTVDesc, &_ppRenderTargetViews[0]));
	SET_DEBUG_NAME(_ppRenderTargetViews[0], "Renderer post process RTV 0");

	V_RETURN(pd3dDevice->CreateRenderTargetView(_ppTextures[1], &ppRTVDesc, &_ppRenderTargetViews[1]));
	SET_DEBUG_NAME(_ppRenderTargetViews[1], "Renderer post process RTV 1");

	V_RETURN(_gBuffer.OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc));
	V_RETURN(_lightBuffer.OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc));
	V_RETURN(_modelRenderer.OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc));
	V_RETURN(_combinePP.OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc));
	V_RETURN(_boRenderer.OnD3D11ResizedSwapChain(pd3dDevice, pSwapChain, pBackBufferSurfaceDesc));

	return S_OK;
}

void Renderer::OnD3D11ReleasingSwapChain()
{	
	SAFE_RELEASE(_ppTextures[0]);
	SAFE_RELEASE(_ppTextures[1]);
	SAFE_RELEASE(_ppShaderResourceViews[0]);
	SAFE_RELEASE(_ppShaderResourceViews[1]);
	SAFE_RELEASE(_ppRenderTargetViews[0]);
	SAFE_RELEASE(_ppRenderTargetViews[1]);

	_gBuffer.OnD3D11ReleasingSwapChain();
	_lightBuffer.OnD3D11ReleasingSwapChain();
	_modelRenderer.OnD3D11ReleasingSwapChain();
	_combinePP.OnD3D11ReleasingSwapChain();
	_boRenderer.OnD3D11ReleasingSwapChain();
}