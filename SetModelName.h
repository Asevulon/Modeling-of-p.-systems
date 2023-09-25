#pragma once
#include "afxdialogex.h"


// Диалоговое окно SetModelName

class SetModelName : public CDialogEx
{
	DECLARE_DYNAMIC(SetModelName)

public:
	SetModelName(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~SetModelName();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CEdit Name;
	CString out;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	bool isOK = false;

};
