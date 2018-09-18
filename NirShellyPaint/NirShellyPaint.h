
// NirShellyPaint.h : main header file for the PROJECT_NAME application
//

#if !defined(AFX_PaintBG_H__A7561E47_8B22_413E_8611_6D285DD57136__INCLUDED_)
#define AFX_PaintBG_H__A7561E47_8B22_413E_8611_6D285DD57136__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CNirShellyPaintApp:
// See NirShellyPaint.cpp for the implementation of this class
//

class CNirShellyPaintApp : public CWinApp
{
public:
	CNirShellyPaintApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CNirShellyPaintApp theApp;

#endif