#pragma once
#include "afxdialogex.h"


// Диалоговое окно SetRGDataDialog
#include"RG.h"
#include"SetModelName.h"
class SetRGDataDialog : public CDialogEx
{
	DECLARE_DYNAMIC(SetRGDataDialog)

public:
	SetRGDataDialog(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~SetRGDataDialog();
	std::vector<RG*>* mdata;
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SET_DATA_DIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	CListBox ModelsList;
	afx_msg void OnDblclkModelsList();
	CEdit dtEdit;
	CEdit lEdit;
	CEdit mEdit;
	CEdit kEdit;
	CEdit uEdit;
	CEdit yEdit;
	CEdit wEdit;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedButtonReset();
	CEdit REdit;
	CEdit GEdit;
	CEdit BEdit;
	afx_msg void OnClose();

	void LoadNames();
};
