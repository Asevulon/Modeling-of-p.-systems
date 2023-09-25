#pragma once
#include "afxdialogex.h"


// Диалоговое окно GraphDialog
#include"Drawer.h"
#define TREDRAW 16
class GraphDialog : public CDialogEx
{
	DECLARE_DYNAMIC(GraphDialog)

public:
	GraphDialog(DrawMode drwMode, CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~GraphDialog();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	Drawer drw;
	DrawMode drwM;
	std::vector<RG*>*mdata;
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
