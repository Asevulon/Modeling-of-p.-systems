
// MyDlg.h: файл заголовка
//

#pragma once
#include"Drawer.h"
#include"SetRGDataDialog.h"
// Диалоговое окно MyDlg
class MyDlg : public CDialogEx
{
// Создание
public:
	MyDlg(CWnd* pParent = nullptr);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MODELING_OF_P_SYSTEMS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	Drawer drw;
	std::vector<RG*>models;
	HANDLE thr;
	int Delay = 16;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedModelsSettings();
	afx_msg void OnBnClickedGraphs();
	CEdit DelayEdit;
	afx_msg void OnBnClickedButtonStop();
	bool ContinueProcess = false;
	afx_msg void OnBnClickedButtonPhasePortrait();
	UINT_PTR timerId;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton1();
};
