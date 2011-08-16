#pragma once

#include "PCH.h"
#include "ContentLoader.h"
#include "ContentType.h"

struct PixelShaderContent : public ContentType
{
	ID3D11PixelShader* PixelShader;

	PixelShaderContent() : PixelShader(NULL) { }
	~PixelShaderContent() { SAFE_RELEASE(PixelShader); }
};

struct PixelShaderOptions
{
	const char* EntryPoint;
	D3D_SHADER_MACRO* Defines;
	const char* DebugName;
};

template <>
HRESULT GenerateContentHash<PixelShaderOptions>(const WCHAR* path, PixelShaderOptions* options, long* hash);

class PixelShaderLoader : public ContentLoader<PixelShaderOptions, PixelShaderContent>
{
public:
	HRESULT Load(ID3D11Device* device, ID3DX11ThreadPump* threadPump, const WCHAR* path, 
		PixelShaderOptions* options, WCHAR* errorMsg, UINT errorLen, PixelShaderContent** contentOut);
};