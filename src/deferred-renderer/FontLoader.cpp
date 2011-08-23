#include "PCH.h"
#include "FontLoader.h"

HRESULT FontLoader::GenerateContentHash(const WCHAR* path, FontOptions* options, ContentHash* hash)
{
	if (!hash)
	{
		return FWP_E_NULL_POINTER;
	}

	locale loc;
	const collate<WCHAR>& wcoll = use_facet<collate<WCHAR>>(loc);

	*hash = wcoll.hash(path, path + wcslen(path));
	return S_OK;
}

HRESULT FontLoader::LoadFromContentFile(ID3D11Device* device, ID3DX11ThreadPump* threadPump, const WCHAR* path, 
		FontOptions* options, WCHAR* errorMsg, UINT errorLen, SpriteFont** contentOut)
{
	WCHAR logMsg[MAX_LOG_LENGTH];
	swprintf_s(logMsg, L"Loading - %s", path);
	LOG_INFO(L"Font Loader", logMsg);

	SpriteFont* font = new SpriteFont();

	HRESULT hr = font->CreateFromFile(device, path);
	if (FAILED(hr))
	{
		delete font;
		return hr;
	}

	*contentOut = font;
	return S_OK;
}

HRESULT FontLoader::LoadFromCompiledContentFile(ID3D11Device* device, const WCHAR* path, WCHAR* errorMsg,
		UINT errorLen, SpriteFont** contentOut)
{
	return E_NOTIMPL;
}