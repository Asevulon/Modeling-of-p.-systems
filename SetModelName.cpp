// SetModelName.cpp: файл реализации
//

#include "pch.h"
#include "Modeling of p. systems.h"
#include "afxdialogex.h"
#include "SetModelName.h"


// Диалоговое окно SetModelName

IMPLEMENT_DYNAMIC(SetModelName, CDialogEx)

SetModelName::SetModelName(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

SetModelName::~SetModelName()
{
}

void SetModelName::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, Name);
}


BEGIN_MESSAGE_MAP(SetModelName, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetModelName::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SetModelName::OnBnClickedCancel)
END_MESSAGE_MAP()


// Обработчики сообщений SetModelName


void SetModelName::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	isOK = true;
	Name.GetWindowTextW(out);
	CDialogEx::OnOK();
}


void SetModelName::OnBnClickedCancel()
{
	// TODO: добавьте свой код обработчика уведомлений
	isOK = false;
	CDialogEx::OnCancel();
}
