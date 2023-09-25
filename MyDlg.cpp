
// MyDlg.cpp: файл реализации
//

#include "pch.h"
#include "framework.h"
#include "Modeling of p. systems.h"
#include "MyDlg.h"
#include "afxdialogex.h"
#include "GraphDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
// Диалоговое окно MyDlg



MyDlg::MyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MODELING_OF_P_SYSTEMS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void MyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MODEL_PICUTURE, drw);
	DDX_Control(pDX, IDC_EDIT1, DelayEdit);
}

BEGIN_MESSAGE_MAP(MyDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &MyDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_MODELS_SETTINGS, &MyDlg::OnBnClickedModelsSettings)
	ON_BN_CLICKED(IDC_GRAPHS, &MyDlg::OnBnClickedGraphs)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &MyDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_PHASE_PORTRAIT, &MyDlg::OnBnClickedButtonPhasePortrait)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &MyDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// Обработчики сообщений MyDlg

BOOL MyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию
	CString str;
	str.Format(L"%d", Delay);
	DelayEdit.SetWindowTextW(str);
	drw.DrwMode = DrawMode::DrawMainAnimation;
	drw.SetData(&models);
	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void MyDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR MyDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




DWORD ThreadFunc(LPVOID ptr)
{
	MyDlg* dlg = (MyDlg*)ptr;
	if (dlg->models.empty())return 0;
	dlg->ContinueProcess = true;

	if (dlg->Delay)
	{
		while (dlg->ContinueProcess)
		{
			Sleep(dlg->Delay);
			for (auto& item : dlg->models)item->DoRungeKuttaStep();
		}
	}
	else
	{
		while (dlg->ContinueProcess)
		{
			for (auto& item : dlg->models)item->DoRungeKuttaStep();
		}
	}
	return 0;
}
void MyDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	TerminateThread(thr, 0);
	
	for (auto& rg : models)rg->reset();
	CString str; 
	DelayEdit.GetWindowTextW(str);
	Delay = _wtoi(str);
	ContinueProcess = true;
	thr = CreateThread(NULL, NULL, ThreadFunc, this, NULL, NULL);
	KillTimer(timerId);
	timerId = SetTimer(1, 16, NULL);
}

double FindMaxRGL(vector<RG*>& data)
{
	double max = data[0]->GetL();
	for (int i = 1; i < data.size(); i++)
	{
		if (data[i]->GetL() > max)max = data[i]->GetL();
	}
	return max;
}
void MyDlg::OnBnClickedModelsSettings()
{
	// TODO: добавьте свой код обработчика уведомлений
	SetRGDataDialog dialog;
	dialog.mdata = &models;
	dialog.DoModal();
	if(!models.empty()) drw.lmax = FindMaxRGL(models);
}


void MyDlg::OnBnClickedGraphs()
{
	// TODO: добавьте свой код обработчика уведомлений
	GraphDialog dlg(DrawMode::DrawGraph);
	dlg.mdata = &models;
	dlg.DoModal();
}


void MyDlg::OnBnClickedButtonStop()
{
	// TODO: добавьте свой код обработчика уведомлений
	ContinueProcess = false;
}


void MyDlg::OnBnClickedButtonPhasePortrait()
{
	// TODO: добавьте свой код обработчика уведомлений
	GraphDialog dlg(DrawMode::DrawPhasePortret);
	dlg.mdata = &models;
	dlg.DoModal();
}


void MyDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	drw.Invalidate();
	CDialogEx::OnTimer(nIDEvent);
}



void MyDlg::OnBnClickedButton1()
{
	// TODO: добавьте свой код обработчика уведомлений
	GraphDialog dlg(DrawMode::DrawTotA);
	dlg.mdata = &models;
	dlg.DoModal();
}
