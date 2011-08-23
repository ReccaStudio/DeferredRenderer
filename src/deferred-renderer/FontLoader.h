#pragma once

#include "PCH.h"
#include "ContentLoader.h"
#include "ContentType.h"
#include "SpriteFont.h"

struct FontOptions
{
};

class FontLoader : public ContentLoader<FontOptions, SpriteFont>
{
public:
	HRESULT GenerateContentHash(const WCHAR* path, FontOptions* options, ContentHash* hash);
	HRESULT LoadFromContentFile(ID3D11Device* device, ID3DX11ThreadPump* threadPump, const WCHAR* path, 
		FontOptions* options, WCHAR* errorMsg, UINT errorLen, SpriteFont** contentOut);
	HRESULT LoadFromCompiledContentFile(ID3D11Device* device, const WCHAR* path, WCHAR* errorMsg,
		UINT errorLen, SpriteFont** contentOut);
};