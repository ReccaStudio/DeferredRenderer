#pragma once

const float Pi = 3.14159f;
const float PiOver2 = Pi / 2.0f;
const float PiOver4 = Pi / 4.0f;
const float PiOver8 = Pi / 8.0f;

#ifndef clamp
#define clamp(val, min, max) (((val) < (min)) ? (min) : (((val) > (max)) ? (max) : (val)))
#endif

#ifndef saturate
#define saturate(val) clamp((val), 0, 1)
#endif

inline int AnsiToWString(const char* ansiString, WCHAR* output, UINT outputLength)
{
    return MultiByteToWideChar(CP_ACP, 0, ansiString, -1, output, outputLength);
}

inline int GetDirectoryFromFileName(const WCHAR* fileName, WCHAR* output, UINT outputLength)
{
    _ASSERT(fileName);

    std::wstring filePath(fileName);
    size_t idx = filePath.rfind(L'\\');
    if (idx != std::wstring::npos)
	{
		wcsncpy_s(output, outputLength, filePath.substr(0, idx + 1).c_str(), outputLength);
	}
    else
	{
        wcsncpy_s(output, outputLength, L"", outputLength);
	}

	return 1;
}

#define SET_DEBUG_NAME(obj, name) ((obj)->SetPrivateData(WKPDID_D3DDebugObjectName, lstrlenA(name), (name)))

#if defined(DEBUG) || defined(_DEBUG)
#ifndef V
#define V(x)           { hr = (x); if( FAILED(hr) ) { DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return DXUTTrace( __FILE__, (DWORD)__LINE__, hr, L#x, true ); } }
#endif
#else
#ifndef V
#define V(x)           { hr = (x); }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if (p) { delete (p);     (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p);   (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p)=NULL; } }
#endif