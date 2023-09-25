// SetRGDataDialog.cpp: файл реализации
//

#include "pch.h"
#include "Modeling of p. systems.h"
#include "afxdialogex.h"
#include "SetRGDataDialog.h"
using namespace std;

// Диалоговое окно SetRGDataDialog

IMPLEMENT_DYNAMIC(SetRGDataDialog, CDialogEx)

SetRGDataDialog::SetRGDataDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SET_DATA_DIALOG, pParent)
{

}

SetRGDataDialog::~SetRGDataDialog()
{
	
}

void SetRGDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODELS_LIST, ModelsList);
	DDX_Control(pDX, IDC_EDIT1, dtEdit);
	DDX_Control(pDX, IDC_EDIT2, lEdit);
	DDX_Control(pDX, IDC_EDIT3, mEdit);
	DDX_Control(pDX, IDC_EDIT4, kEdit);
	DDX_Control(pDX, IDC_EDIT5, uEdit);
	DDX_Control(pDX, IDC_EDIT6, yEdit);
	DDX_Control(pDX, IDC_EDIT7, wEdit);
	DDX_Control(pDX, IDC_EDIT8, REdit);
	DDX_Control(pDX, IDC_EDIT9, GEdit);
	DDX_Control(pDX, IDC_EDIT10, BEdit);
}


BEGIN_MESSAGE_MAP(SetRGDataDialog, CDialogEx)
	ON_BN_CLICKED(IDOK, &SetRGDataDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &SetRGDataDialog::OnBnClickedButton1)
	ON_LBN_DBLCLK(IDC_MODELS_LIST, &SetRGDataDialog::OnDblclkModelsList)
	ON_BN_CLICKED(IDC_DELETE_BUTTON, &SetRGDataDialog::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON_RESET, &SetRGDataDialog::OnBnClickedButtonReset)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// Обработчики сообщений SetRGDataDialog


void SetRGDataDialog::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	int id = ModelsList.GetCurSel();
	if (id < 0)return;
	RG* rg = (RG*)ModelsList.GetItemDataPtr(id);

	CString str;

	lEdit.GetWindowTextW(str);
	double l = _wtof(str);

	mEdit.GetWindowTextW(str);
	double m = _wtof(str);

	kEdit.GetWindowTextW(str);
	double k = _wtof(str);

	uEdit.GetWindowTextW(str);
	double u = _wtof(str);

	yEdit.GetWindowTextW(str);
	double y = _wtof(str);

	wEdit.GetWindowTextW(str);
	double w = _wtof(str);

	dtEdit.GetWindowTextW(str);
	double dt = _wtof(str);

	REdit.GetWindowTextW(str);
	int r = _wtoi(str);

	GEdit.GetWindowTextW(str);
	int g = _wtoi(str);

	BEdit.GetWindowTextW(str);
	int b = _wtoi(str);

	rg->color = Gdiplus::Color(r, g, b);
	rg->SetParams(l, m, k, u, dt);
	rg->SetStartParams(y, w);


}



void SetRGDataDialog::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetModelName NameDialog;
	NameDialog.DoModal();
	if (!NameDialog.isOK)return;
	if (NameDialog.out.IsEmpty())return;

	vector<RG*>& data = *mdata;
	int id = ModelsList.InsertString(-1, NameDialog.out);
	RG* temp = new RG;
	data.push_back(temp);
	ModelsList.SetItemDataPtr(id, temp);
	temp->name = NameDialog.out;
}


void SetRGDataDialog::OnDblclkModelsList()
{
	// TODO: добавьте свой код обработчика уведомлений
	int id = ModelsList.GetCurSel();
	if (id == LB_ERR)return;
	RG* rg = (RG*)ModelsList.GetItemDataPtr(id);

	CString str;

	str.Format(L"%.7lf", rg->l);
	lEdit.SetWindowTextW(str);

	str.Format(L"%.7lf", rg->m);
	mEdit.SetWindowTextW(str);

	str.Format(L"%.7lf", rg->k);
	kEdit.SetWindowTextW(str);

	str.Format(L"%.7lf", rg->u);
	uEdit.SetWindowTextW(str);

	str.Format(L"%.7lf", rg->y0);
	yEdit.SetWindowTextW(str);

	str.Format(L"%.7lf", rg->w0);
	wEdit.SetWindowTextW(str);

	str.Format(L"%d", rg->color.GetR());
	REdit.SetWindowTextW(str);

	str.Format(L"%d", rg->color.GetG());
	GEdit.SetWindowTextW(str);

	str.Format(L"%d", rg->color.GetB());
	BEdit.SetWindowTextW(str);

	str.Format(L"%lf", rg->dt);
	dtEdit.SetWindowTextW(str);
}


BOOL SetRGDataDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	LoadNames();
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


void SetRGDataDialog::OnBnClickedDeleteButton()
{
	// TODO: добавьте свой код обработчика уведомлений
	int id = ModelsList.GetCurSel();
	RG* rg = (RG*)ModelsList.GetItemDataPtr(id);

	vector<RG*>& data = *mdata;
	data.erase(find(data.begin(), data.end(), rg));
	delete rg;
	ModelsList.DeleteString(id);
}


void SetRGDataDialog::OnBnClickedButtonReset()
{
	// TODO: добавьте свой код обработчика уведомлений
	vector<RG*>& data = *mdata;

	for (int i = 0; i < data.size(); i++)
	{
		delete data[i];
	}
	data.clear();
	ModelsList.ResetContent();
}


void SetRGDataDialog::OnClose()
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	CString str;
	dtEdit.GetWindowTextW(str);
	if (str.IsEmpty())
	{
		CDialogEx::OnClose();
		return;
	}
	double dt = _wtof(str);
	vector<RG*>& data = *mdata;

	for (int i = 0; i < data.size(); i++)
	{
		data[i]->SetDt(dt);
	}
	CDialogEx::OnClose();
}


void SetRGDataDialog::LoadNames()
{
	vector<RG*>& data = *mdata;
	if (data.empty())return;

	for (auto& rg : data)
	{
		int id = ModelsList.InsertString(-1, rg->name);
		ModelsList.SetItemDataPtr(id, rg);
	}
}
