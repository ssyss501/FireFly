

#include "stdafx.h"
#include "MemoryImage.h"

CMemoryImage* CMemoryImage::m_Instance=NULL;

CMemoryImage::CMemoryImage(void)
{
}

CMemoryImage::~CMemoryImage(void)
{
	// Release在析构中调用，因为GdiplusShutdown的原因己调用的原因会导致内存崩溃
	// Release();
}

CMemoryImage * CMemoryImage::GetInstance()
{
	if(m_Instance==NULL)
	{
		m_Instance=	new CMemoryImage;
	}

	return m_Instance;
}

Image* CMemoryImage::ImageFromResource(UINT nFile)
{
	Image* pImage = NULL;
	if(m_pImage.find(nFile) != m_pImage.end())
	{
		return m_pImage[nFile];
	}

	pImage = FromResource(nFile);
	if(pImage != NULL)
	{
		m_pImage[nFile] = pImage;
	}

	return pImage;
}

void CMemoryImage::Release()
{
	for(map<UINT, Image*>::iterator ciIter = m_pImage.begin();
		ciIter != m_pImage.end(); ciIter++)
	{
		delete ciIter->second;
	}
	m_pImage.clear();
}

Image* CMemoryImage::FromResource( UINT uId )
{
	HMODULE hModule = AfxGetResourceHandle();
	HRSRC hRes = ::FindResource( hModule, MAKEINTRESOURCE(uId), _T("PNG"));
	if( hRes == NULL )
	{
		//assert(FALSE);
		return NULL;
	}

	DWORD dwSize = ::SizeofResource( hModule, hRes); 
	if( dwSize == 0 )
	{
		//assert(FALSE);
		return NULL;
	}

	HGLOBAL hGlobal = ::LoadResource( hModule, hRes); 
	if( hGlobal == NULL )
	{
		//assert(FALSE);
		return NULL;
	}

	LPVOID pBuffer = ::LockResource(hGlobal); 
	if( pBuffer == NULL )
	{
		//assert(FALSE);
		::FreeResource(hGlobal); 
		return NULL;
	}


	HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, dwSize);
	if(hMem == NULL)
	{
		//assert(FALSE);
		::FreeResource(hGlobal); 
		return NULL;
	}

	void* pData = GlobalLock(hMem);
	memcpy(pData, (void *)hGlobal, dwSize);
	GlobalUnlock(hMem);

	Image *pRtImg = NULL;
	IStream* pStream = NULL;
	if(CreateStreamOnHGlobal(hMem, TRUE, &pStream) == S_OK)
	{
		pRtImg = Image::FromStream(pStream, FALSE);
		pStream->Release();
	}

	::FreeResource(hGlobal); 

	return pRtImg;
}

Image* CMemoryImage::GetImage(UINT uID)
{
	return m_pImage[uID];
}

void  CMemoryImage::UnLoad()
{
	if (m_Instance)
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}