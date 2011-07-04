#include "DeviceStates.h"

HRESULT BlendStates::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;
	D3D11_BLEND_DESC blendDesc;

	blendDesc = getBlendDisabledDesc();
	V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &_blendDisabled));

	blendDesc = getAdditiveBlendDesc();
	V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &_additiveBlend));

	blendDesc = getAlphaBlendDesc();
	V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &_alphaBlend));

	blendDesc = getPreMultipliedAlphaBlendDesc();
	V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &_pmAlphaBlend));

	blendDesc = getColorWriteDisabledDesc();
	V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &_noColor));

	return S_OK;
}
void BlendStates::OnD3D11DestroyDevice()
{
	SAFE_RELEASE(_blendDisabled);
	SAFE_RELEASE(_additiveBlend);
	SAFE_RELEASE(_alphaBlend);
	SAFE_RELEASE(_pmAlphaBlend);
	SAFE_RELEASE(_noColor);
}

HRESULT BlendStates::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	return S_OK;
}

void BlendStates::OnD3D11ReleasingSwapChain()
{

}

D3D11_BLEND_DESC BlendStates::getBlendDisabledDesc()
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < 8; ++i)
	{
		blendDesc.RenderTarget[i].BlendEnable = false;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
	}

	return blendDesc;
}

D3D11_BLEND_DESC BlendStates::getAdditiveBlendDesc()
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < 8; ++i)
	{
		blendDesc.RenderTarget[i].BlendEnable = true;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
	}

	return blendDesc;
}

D3D11_BLEND_DESC BlendStates::getAlphaBlendDesc()
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < 8; ++i)
	{
		blendDesc.RenderTarget[i].BlendEnable = true;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
	}

	return blendDesc;
}

D3D11_BLEND_DESC BlendStates::getPreMultipliedAlphaBlendDesc()
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < 8; ++i)
	{
		blendDesc.RenderTarget[i].BlendEnable = false;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
	}

	return blendDesc;
}

D3D11_BLEND_DESC BlendStates::getColorWriteDisabledDesc()
{
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.IndependentBlendEnable = false;
	for (UINT i = 0; i < 8; ++i)
	{
		blendDesc.RenderTarget[i].BlendEnable = false;
		blendDesc.RenderTarget[i].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[i].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[i].DestBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[i].RenderTargetWriteMask = 0;
		blendDesc.RenderTarget[i].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[i].SrcBlendAlpha = D3D11_BLEND_ONE;
	}

	return blendDesc;
}

HRESULT RasterizerStates::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;
	D3D11_RASTERIZER_DESC rasterDesc;
	
	rasterDesc = getNoCullDesc();
	V_RETURN(pd3dDevice->CreateRasterizerState(&rasterDesc, &_noCull));

	rasterDesc = getFrontFaceCullDesc();
	V_RETURN(pd3dDevice->CreateRasterizerState(&rasterDesc, &_cullFrontFaces));
	
	rasterDesc = getFrontFaceCullScissorDesc();
	V_RETURN(pd3dDevice->CreateRasterizerState(&rasterDesc, &_cullFrontFacesScissor));

	rasterDesc = getBackFaceCullDesc();
	V_RETURN(pd3dDevice->CreateRasterizerState(&rasterDesc, &_cullBackFaces));

	rasterDesc = getBackFaceCullScissorDesc();
	V_RETURN(pd3dDevice->CreateRasterizerState(&rasterDesc, &_cullBackFacesScissor));

	rasterDesc = getNoCullNoMSDesc();
	V_RETURN(pd3dDevice->CreateRasterizerState(&rasterDesc, &_noCullNoMS));

	rasterDesc = getNoCullScissorDesc();
	V_RETURN(pd3dDevice->CreateRasterizerState(&rasterDesc, &_noCullScissor));

	rasterDesc = getWireframeDesc();
	V_RETURN(pd3dDevice->CreateRasterizerState(&rasterDesc, &_wireframe));

	return S_OK;
}
void RasterizerStates::OnD3D11DestroyDevice()
{
	SAFE_RELEASE(_noCull);
	SAFE_RELEASE(_cullFrontFaces);
	SAFE_RELEASE(_cullFrontFacesScissor);
	SAFE_RELEASE(_cullBackFaces);
	SAFE_RELEASE(_cullBackFacesScissor);
	SAFE_RELEASE(_noCullNoMS);
	SAFE_RELEASE(_noCullScissor);
	SAFE_RELEASE(_wireframe);
}

HRESULT RasterizerStates::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	return S_OK;
}

void RasterizerStates::OnD3D11ReleasingSwapChain()
{

}

D3D11_RASTERIZER_DESC RasterizerStates::getNoCullDesc()
{
	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = true;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0;

	return rastDesc;
}

D3D11_RASTERIZER_DESC RasterizerStates::getFrontFaceCullDesc()
{
	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = true;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0;

	return rastDesc;
}

D3D11_RASTERIZER_DESC RasterizerStates::getFrontFaceCullScissorDesc()
{
	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.CullMode = D3D11_CULL_FRONT;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = TRUE;
	rastDesc.MultisampleEnable = TRUE;
	rastDesc.ScissorEnable = TRUE;
	rastDesc.SlopeScaledDepthBias = 0;

	return rastDesc;
}

D3D11_RASTERIZER_DESC RasterizerStates::getBackFaceCullDesc()
{
	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = true;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0;

	return rastDesc;
}

D3D11_RASTERIZER_DESC RasterizerStates::getBackFaceCullScissorDesc()
{
	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.CullMode = D3D11_CULL_BACK;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = FALSE;
	rastDesc.MultisampleEnable = TRUE;
	rastDesc.ScissorEnable = TRUE;
	rastDesc.SlopeScaledDepthBias = 0;

	return rastDesc;
}

D3D11_RASTERIZER_DESC RasterizerStates::getNoCullNoMSDesc()
{
	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = false;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0;

	return rastDesc;
}

D3D11_RASTERIZER_DESC RasterizerStates::getNoCullScissorDesc()
{
	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_SOLID;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = true;
	rastDesc.ScissorEnable = true;
	rastDesc.SlopeScaledDepthBias = 0;

	return rastDesc;
}

D3D11_RASTERIZER_DESC RasterizerStates::getWireframeDesc()
{
	D3D11_RASTERIZER_DESC rastDesc;

	rastDesc.AntialiasedLineEnable = FALSE;
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.DepthBias = 0;
	rastDesc.DepthBiasClamp = 0.0f;
	rastDesc.DepthClipEnable = TRUE;
	rastDesc.FillMode = D3D11_FILL_WIREFRAME;
	rastDesc.FrontCounterClockwise = false;
	rastDesc.MultisampleEnable = true;
	rastDesc.ScissorEnable = false;
	rastDesc.SlopeScaledDepthBias = 0;

	return rastDesc;
}

HRESULT DepthStencilStates::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;
	D3D11_DEPTH_STENCIL_DESC desc;

	desc = getStencilReplaceDesc();
	V_RETURN(pd3dDevice->CreateDepthStencilState(&desc, &_stencilReplace));

	desc = getStencilEqualDesc();
	V_RETURN(pd3dDevice->CreateDepthStencilState(&desc, &_stencilEqual));

	desc = getStencilNotEqualDesc();
	V_RETURN(pd3dDevice->CreateDepthStencilState(&desc, &_stencilNotEqual));

	desc = getDepthDisabledDesc();
	V_RETURN(pd3dDevice->CreateDepthStencilState(&desc, &_depthDisabled));

	desc = getDepthEnabledDesc();
	V_RETURN(pd3dDevice->CreateDepthStencilState(&desc, &_depthEnabled));

	desc = getReverseDepthEnabledDesc();
	V_RETURN(pd3dDevice->CreateDepthStencilState(&desc, &_revDepthEnabled));

	desc = getDepthWriteEnabledDesc();
	V_RETURN(pd3dDevice->CreateDepthStencilState(&desc, &_depthWriteEnabled));

	desc = getReverseDepthWriteEnabledDesc();
	V_RETURN(pd3dDevice->CreateDepthStencilState(&desc, &_revDepthWriteEnabled));

	return S_OK;
}
void DepthStencilStates::OnD3D11DestroyDevice()
{
	SAFE_RELEASE(_stencilReplace);
	SAFE_RELEASE(_stencilEqual);
	SAFE_RELEASE(_stencilNotEqual);
	SAFE_RELEASE(_depthDisabled);
	SAFE_RELEASE(_depthEnabled);
	SAFE_RELEASE(_revDepthEnabled);
	SAFE_RELEASE(_depthWriteEnabled);
	SAFE_RELEASE(_revDepthWriteEnabled);
}

HRESULT DepthStencilStates::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	return S_OK;
}

void DepthStencilStates::OnD3D11ReleasingSwapChain()
{
}

D3D11_DEPTH_STENCIL_DESC DepthStencilStates::getStencilReplaceDesc()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_ZERO;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_ZERO;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace = dsDesc.FrontFace;

	return dsDesc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilStates::getStencilEqualDesc()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dsDesc.BackFace = dsDesc.FrontFace;

	return dsDesc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilStates::getStencilNotEqualDesc()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = true;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_NOT_EQUAL;
	dsDesc.BackFace = dsDesc.FrontFace;

	return dsDesc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilStates::getDepthDisabledDesc()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace = dsDesc.FrontFace;

	return dsDesc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilStates::getDepthEnabledDesc()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace = dsDesc.FrontFace;

	return dsDesc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilStates::getReverseDepthEnabledDesc()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace = dsDesc.FrontFace;

	return dsDesc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilStates::getDepthWriteEnabledDesc()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace = dsDesc.FrontFace;

	return dsDesc;
}

D3D11_DEPTH_STENCIL_DESC DepthStencilStates::getReverseDepthWriteEnabledDesc()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	dsDesc.StencilEnable = false;
	dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace = dsDesc.FrontFace;

	return dsDesc;
}

HRESULT SamplerStates::OnD3D11CreateDevice(ID3D11Device* pd3dDevice, const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	HRESULT hr;
	D3D11_SAMPLER_DESC desc;

	desc = getLinearDesc();
	V_RETURN(pd3dDevice->CreateSamplerState(&desc, &_linear));

	desc = getPointDesc();
	V_RETURN(pd3dDevice->CreateSamplerState(&desc, &_point));

	desc = getAnisotropic2Desc();
	V_RETURN(pd3dDevice->CreateSamplerState(&desc, &_anisotropic2));

	desc = getAnisotropic4Desc();
	V_RETURN(pd3dDevice->CreateSamplerState(&desc, &_anisotropic4));

	desc = getAnisotropic8Desc();
	V_RETURN(pd3dDevice->CreateSamplerState(&desc, &_anisotropic8));

	desc = getAnisotropic16Desc();
	V_RETURN(pd3dDevice->CreateSamplerState(&desc, &_anisotropic16));
	
	desc = getShadowMapDesc();
	V_RETURN(pd3dDevice->CreateSamplerState(&desc, &_shadowMap));
	
	return S_OK;
}
void SamplerStates::OnD3D11DestroyDevice()
{
	SAFE_RELEASE(_linear);
	SAFE_RELEASE(_point);
	SAFE_RELEASE(_anisotropic2);
	SAFE_RELEASE(_anisotropic4);
	SAFE_RELEASE(_anisotropic8);
	SAFE_RELEASE(_anisotropic16);
	SAFE_RELEASE(_shadowMap);
}

HRESULT SamplerStates::OnD3D11ResizedSwapChain( ID3D11Device* pd3dDevice, IDXGISwapChain* pSwapChain,
	const DXGI_SURFACE_DESC* pBackBufferSurfaceDesc)
{
	return S_OK;
}

void SamplerStates::OnD3D11ReleasingSwapChain()
{

}

D3D11_SAMPLER_DESC SamplerStates::getLinearDesc()
{
	D3D11_SAMPLER_DESC sampDesc;

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return sampDesc;
}

D3D11_SAMPLER_DESC SamplerStates::getPointDesc()
{
	D3D11_SAMPLER_DESC sampDesc;

	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return sampDesc;
}

D3D11_SAMPLER_DESC SamplerStates::getAnisotropic2Desc()
{
	D3D11_SAMPLER_DESC sampDesc;

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 2;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return sampDesc;
}

D3D11_SAMPLER_DESC SamplerStates::getAnisotropic4Desc()
{
	D3D11_SAMPLER_DESC sampDesc;

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 4;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return sampDesc;
}

D3D11_SAMPLER_DESC SamplerStates::getAnisotropic8Desc()
{
	D3D11_SAMPLER_DESC sampDesc;

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 8;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return sampDesc;
}

D3D11_SAMPLER_DESC SamplerStates::getAnisotropic16Desc()
{
	D3D11_SAMPLER_DESC sampDesc;

	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return sampDesc;
}

D3D11_SAMPLER_DESC SamplerStates::getShadowMapDesc()
{
	D3D11_SAMPLER_DESC sampDesc;

	sampDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.MipLODBias = 0.0f;
	sampDesc.MaxAnisotropy = 1;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	sampDesc.BorderColor[0] = sampDesc.BorderColor[1] = sampDesc.BorderColor[2] = sampDesc.BorderColor[3] = 0;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	return sampDesc;
}