
// Modeling of p. systems.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CModelingofpsystemsApp:
// Сведения о реализации этого класса: Modeling of p. systems.cpp
//

class CModelingofpsystemsApp : public CWinApp
{
public:
	CModelingofpsystemsApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CModelingofpsystemsApp theApp;
