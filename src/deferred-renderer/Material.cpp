#include "PCH.h"
#include "Material.h"
#include "Logger.h"

Material::Material()
	: _ambientColor(0.0f, 0.0f, 0.0f), _diffuseColor(0.0f, 0.0f, 0.0f), _emissiveColor(0.0f, 0.0f, 0.0f),
	  _specularColor(0.0f, 0.0f, 0.0f), _specularPower(0.0f), _alpha(1.0f), _diffuseSRV(NULL), 
	  _normalSRV(NULL), _specularSRV(NULL), _name(NULL)
{
}

Material::~Material()
{
	Destroy();
}

HRESULT loadMaterialTexture(ID3D11Device* device, const WCHAR* modelDir, const CHAR* texturePath,
	ID3D11ShaderResourceView** outSRV, std::map<TexturePathHash, ID3D11ShaderResourceView*>* loadedTextureMap = NULL)
{
	HRESULT hr;

	WCHAR wtexturePath[MAX_PATH];
	if (!AnsiToWString(texturePath, wtexturePath, MAX_PATH))
	{
		return E_FAIL;
	}

	std::wstring fullPath = std::wstring(modelDir);
	fullPath += L"\\";
	fullPath += wtexturePath;
		
	if (loadedTextureMap)
	{
		std::map<TexturePathHash, ID3D11ShaderResourceView*>::iterator it = loadedTextureMap->find(fullPath);
		if (it != loadedTextureMap->end())
		{
			*outSRV = it->second;
			(*outSRV)->AddRef();

			return S_OK;
		}
	}
		
	hr = D3DX11CreateShaderResourceViewFromFile(device, fullPath.c_str(), NULL, NULL, outSRV, NULL);
	if (loadedTextureMap && SUCCEEDED(hr))
	{
		loadedTextureMap->insert(std::pair<TexturePathHash, ID3D11ShaderResourceView*>(fullPath, *outSRV));
	}

	return hr;
}

HRESULT Material::createPropertiesBuffer(ID3D11Device* device)
{
	// Create the buffer
	D3D11_BUFFER_DESC bufferDesc =
	{
		sizeof(CB_MATERIAL_PROPERTIES), //UINT ByteWidth;
		D3D11_USAGE_DYNAMIC, //D3D11_USAGE Usage;
		D3D11_BIND_CONSTANT_BUFFER, //UINT BindFlags;
		D3D11_CPU_ACCESS_WRITE, //UINT CPUAccessFlags;
		0, //UINT MiscFlags;
		0, //UINT StructureByteStride;
	};

	CB_MATERIAL_PROPERTIES bufferData;
	bufferData.AmbientColor = _ambientColor;
	bufferData.DiffuseColor = _diffuseColor;
	bufferData.EmissiveColor = _emissiveColor;
	bufferData.SpecularColor = _specularColor;
	bufferData.SpecularPower = _specularPower;
	bufferData.Alpha = _alpha;

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = &bufferData;
	initData.SysMemPitch = 0;
	initData.SysMemSlicePitch = 0;

	return device->CreateBuffer(&bufferDesc, &initData, &_propertiesBuffer);
}

HRESULT Material::CreateFromSDKMeshMaterial(ID3D11Device* device, const WCHAR* modelDir, 
	SDKMesh* model, UINT materialIdx, std::map<TexturePathHash, ID3D11ShaderResourceView*>* loadedTextureMap)
{
	HRESULT hr;

	SDKMESH_MATERIAL* sdkmat = model->GetMaterial(materialIdx);

	// Copy the name
	UINT nameLen = strlen(sdkmat->Name);
	if (nameLen > 0)
	{
		_name = new WCHAR[nameLen + 1];
		AnsiToWString(sdkmat->Name, _name, nameLen + 1);
	}
	else
	{
		_name = new WCHAR[MAX_PATH];
		swprintf_s(_name, MAX_PATH, L"Material %u", materialIdx);
	}	

	_ambientColor = XMFLOAT3(sdkmat->Ambient.x, sdkmat->Ambient.y, sdkmat->Ambient.z);
	_diffuseColor = XMFLOAT3(sdkmat->Diffuse.x, sdkmat->Diffuse.y, sdkmat->Diffuse.z);
	_specularColor = XMFLOAT3(sdkmat->Specular.x, sdkmat->Specular.y, sdkmat->Specular.z);
	_emissiveColor = XMFLOAT3(sdkmat->Emissive.x, sdkmat->Emissive.y, sdkmat->Emissive.z);
    _alpha = sdkmat->Diffuse.w;
    _specularPower = sdkmat->Power;
	
	if (strlen(sdkmat->DiffuseTexture) > 0 && 
		FAILED(loadMaterialTexture(device, modelDir, sdkmat->DiffuseTexture, &_diffuseSRV, loadedTextureMap)))
	{
		_diffuseSRV = NULL;		
	}

	if (strlen(sdkmat->NormalTexture) > 0 && 
		FAILED(loadMaterialTexture(device, modelDir, sdkmat->NormalTexture, &_normalSRV, loadedTextureMap)))
	{
		_normalSRV = NULL;		
	}

	if (strlen(sdkmat->SpecularTexture) > 0 && 
		FAILED(loadMaterialTexture(device, modelDir, sdkmat->SpecularTexture, &_specularSRV, loadedTextureMap)))
	{
		_specularSRV = NULL;		
	}

	V_RETURN(createPropertiesBuffer(device));

	return S_OK;
}

HRESULT Material::CreateFromASSIMPMaterial(ID3D11Device* device, const WCHAR* modelDir, 
	const aiScene* scene, UINT materialIdx,std::map<TexturePathHash, ID3D11ShaderResourceView*>* loadedTextureMap)
{
	HRESULT hr;

	aiMaterial* material = scene->mMaterials[materialIdx];
	
	// Get the name
	aiString name;
	if (material->Get(AI_MATKEY_NAME, name) == aiReturn_SUCCESS && name.length > 0)
	{
		_name = new WCHAR[name.length + 1];
		AnsiToWString(name.data, _name, name.length + 1);
	}
	else
	{
		_name = new WCHAR[MAX_PATH];
		swprintf_s(_name, MAX_PATH, L"Material %u", materialIdx);
	}

	// Gather material colors
	aiColor3D col;

	if (material->Get(AI_MATKEY_COLOR_AMBIENT, col) == aiReturn_SUCCESS)
	{
		_ambientColor = XMFLOAT3(col.r, col.g, col.b);
	}
	else
	{
		_ambientColor = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, col) == aiReturn_SUCCESS)
	{	
		_diffuseColor = XMFLOAT3(col.r, col.g, col.b);
	}
	else
	{
		_diffuseColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	}

	if (material->Get(AI_MATKEY_SHININESS, col) == aiReturn_SUCCESS)
	{	
		_specularColor = XMFLOAT3(col.r, col.g, col.b);
	}
	else
	{
		_specularColor = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	if (material->Get(AI_MATKEY_SHININESS_STRENGTH, _specularPower) != aiReturn_SUCCESS)
	{
		_specularPower = 0.0f;
	}

	if (material->Get(AI_MATKEY_COLOR_EMISSIVE, col) == aiReturn_SUCCESS)
	{	
		_emissiveColor = XMFLOAT3(col.r, col.g, col.b);
	}
	else
	{
		_emissiveColor = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}
	
	// Load textures
	aiString path;
	
	_diffuseSRV = NULL;
	if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{		
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);

		loadMaterialTexture(device, modelDir, path.data, &_diffuseSRV, loadedTextureMap);
	}

	_normalSRV = NULL;
	if (material->GetTextureCount(aiTextureType_NORMALS) > 0)
	{		
		material->GetTexture(aiTextureType_NORMALS, 0, &path);

		loadMaterialTexture(device, modelDir, path.data, &_normalSRV, loadedTextureMap);
	}

	_specularSRV = NULL;
	if (material->GetTextureCount(aiTextureType_SPECULAR) > 0)
	{		
		material->GetTexture(aiTextureType_SPECULAR, 0, &path);

		loadMaterialTexture(device, modelDir, path.data, &_specularSRV, loadedTextureMap);
	}

	V_RETURN(createPropertiesBuffer(device));

	return S_OK;
}

void Material::Destroy()
{
	SAFE_DELETE(_name);
	SAFE_RELEASE(_diffuseSRV);
	SAFE_RELEASE(_normalSRV);
	SAFE_RELEASE(_specularSRV);
	SAFE_RELEASE(_propertiesBuffer);
}