#pragma once

#include "PCH.h"
#include "ContentLoader.h"
#include "ContentType.h"

struct ComponentContent : public ContentType
{
	std::map<std::string, std::string> Properties;

	ComponentContent() { }
	~ComponentContent() { }
};

struct EntityContent : public ContentType
{
	std::map<std::string, ComponentContent*> Components;

	EntityContent() { }	
	~EntityContent()
	{
		std::map<std::string, ComponentContent*>::iterator it;
		for (it = Components.begin(); it != Components.end(); it++)
		{
			it->second->Release();
		}
	}
};

struct EntityOptions
{
};

class EntityLoader : public ContentLoader<EntityOptions, EntityContent>
{
public:
	HRESULT GenerateContentHash(const WCHAR* path, EntityOptions* options, ContentHash* hash);
	HRESULT LoadFromContentFile(ID3D11Device* device, ID3DX11ThreadPump* threadPump, const WCHAR* path, 
		EntityOptions* options, WCHAR* errorMsg, UINT errorLen, EntityContent** contentOut);
	HRESULT LoadFromCompiledContentFile(ID3D11Device* device, const WCHAR* path, WCHAR* errorMsg,
		UINT errorLen, EntityContent** contentOut);
};