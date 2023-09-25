// GraphDialog.cpp: файл реализации
//

#include "pch.h"
#include "Modeling of p. systems.h"
#include "afxdialogex.h"
#include "GraphDialog.h"

using namespace std;
// Диалоговое окно GraphDialog

IMPLEMENT_DYNAMIC(GraphDialog, CDialogEx)

GraphDialog::GraphDialog(DrawMode drwMode, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{
	drwM = drwMode;
}

GraphDialog::~GraphDialog()
{
}

void GraphDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, drw);
}


BEGIN_MESSAGE_MAP(GraphDialog, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// Обработчики сообщений GraphDialog


BOOL GraphDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Добавить дополнительную инициализацию
	SetTimer(1, TREDRAW, NULL);
	drw.DrwMode = drwM;
	drw.SetData(mdata);
	if (drwM == DrawMode::DrawPhasePortret)SetWindowTextW(L"w(t)");
	if (drwM == DrawMode::DrawTotA)SetWindowTextW(L"T(A)");
	return TRUE;  // return TRUE unless you set the focus to a control
	// Исключение: страница свойств OCX должна возвращать значение FALSE
}


double FindRGMax(vector<RG*>& models)
{
	double max = models[0]->maxPath;
	for (int i = 1; i < models.size(); i++)
	{
		if (max < models[i]->maxPath)max = models[i]->maxPath;
	}
	return max;
}
double FindRGMin(vector<RG*>& models)
{
	double min = models[0]->minPath;
	for (int i = 1; i < models.size(); i++)
	{
		if (min > models[i]->minPath)min = models[i]->minPath;
	}
	return min;
}
double FindRGSpeedMax(vector<RG*>& models)
{
	double max = models[0]->maxSpeed;
	for (int i = 1; i < models.size(); i++)
	{
		if (max < models[i]->maxSpeed)max = models[i]->maxSpeed;
	}
	return max;
}
double FindRGSpeedMin(vector<RG*>& models)
{
	double min = models[0]->minSpeed;
	for (int i = 1; i < models.size(); i++)
	{
		if (min > models[i]->minSpeed)min = models[i]->minSpeed;
	}
	return min;
}

double FindRGTMax(vector<RG*>& models)
{
	double max = models[0]->GetT();
	for (int i = 1; i < models.size(); i++)
	{
		if (max < models[i]->GetT())max = models[i]->GetT();
	}
	return max;
}
double FindRGTMin(vector<RG*>& models)
{
	double min = models[0]->GetT();
	for (int i = 1; i < models.size(); i++)
	{
		if (min > models[i]->GetT())min = models[i]->GetT();
	}
	return min;
}

#include <algorithm>
bool indatasort(vectorpair left, vectorpair right)
{
	return left.key < right.key;
}
void GraphDialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	vector<RG*>& data = *mdata;
	if (!data.empty())
	{
		if (drw.DrwMode == DrawMode::DrawPhasePortret)
		{
			drw.maxPath = FindRGMax(data);
			drw.minPath = FindRGMin(data);
			drw.minSpeed = FindRGSpeedMin(data);
			drw.maxSpeed = FindRGSpeedMax(data);
		}
		else
		{
			if (drw.DrwMode == DrawMode::DrawGraph)
			{
				drw.maxPath = FindRGMax(data);
				drw.minPath = FindRGMin(data);
			}
			else
			{
				if (drw.DrwMode = DrawMode::DrawTotA)
				{
					if (data.size() > 1)
					{
						drw.Tmax = FindRGTMax(data);
						drw.Tmin = FindRGTMin(data);
						vector<vectorpair> indata;
						for (int i = 0; i < data.size(); i++)
						{
							indata.push_back(vectorpair(data[i]->GetY0(), data[i]->GetT()));
						}
						sort(indata.begin(), indata.end(), indatasort);
						drw.T = indata;
					}
				}
			}
		}
		drw.Invalidate();
	}
	CDialogEx::OnTimer(nIDEvent);
}
