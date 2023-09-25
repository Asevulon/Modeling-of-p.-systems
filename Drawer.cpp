// Drawer.cpp: файл реализации
//

#include "pch.h"
#include "Modeling of p. systems.h"
#include "Drawer.h"
using namespace std;
using namespace Gdiplus;

// Drawer

IMPLEMENT_DYNAMIC(Drawer, CStatic)

Drawer::Drawer()
{
	GdiplusStartupInput si;
	GdiplusStartup(&token, &si, NULL);
}

Drawer::~Drawer()
{
	GdiplusShutdown(token);
}


BEGIN_MESSAGE_MAP(Drawer, CStatic)
//	ON_WM_PAINT()
END_MESSAGE_MAP()



// Обработчики сообщений Drawer



void Drawer::SetData(std::vector<RG*>* d)
{
	mdata = d;
	init = true;
}

void Drawer::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	Graphics finalgr(lpDrawItemStruct->hDC);

	int width = lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left;
	int height = lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top;

	Bitmap bmp(width, height);
	Graphics gr(&bmp);
	gr.Clear(Color::White);
	if (!init)
	{
		finalgr.DrawImage(&bmp, 0, 0);
		return;
	}
	gr.SetSmoothingMode(SmoothingModeAntiAlias);
	

	if (DrwMode == DrawMode::DrawGraph) DrawGraph(gr, width, height, lpDrawItemStruct);
	else
		if (DrwMode == DrawMode::DrawPhasePortret)DrawPhasePortret(gr, width, height, lpDrawItemStruct);
		else
			if (DrwMode == DrawMode::DrawMainAnimation)DrawMainAnimation(gr, width, height, lpDrawItemStruct);
			else
				if (DrwMode == DrawMode::DrawTotA)DrawTotA(gr, width, height, lpDrawItemStruct);
	finalgr.DrawImage(&bmp, 0, 0);
}


double Drawer::CalcStringLen(HDC hDC, CString str)
{
	double numlen = 0;
	int size = str.GetLength() + 1;
	for (int i = 0; i < size; i++)
	{
		ABCFLOAT abc;
		GetCharABCWidthsFloatW(hDC, str[i], str[i], &abc);
		numlen += abc.abcfA + abc.abcfB + abc.abcfC;
	}
	return numlen;
}



void Drawer::DrawGraph(Graphics& gr, int width, int height, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	double lPadding = 10;
	double rPadding = 5;
	double tPadding = 22;
	double bPadding = 22;

	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);

	vector<RG*>& data = *mdata;
	if (data.empty())
	{
		LeaveCriticalSection(&cs);
		DeleteCriticalSection(&cs);
		return;
	}

	double left = 0;
	double right = (data[0]->path.size() - 1) * data[0]->GetDt();
	double top = maxPath;
	double bot = minPath;


	double actWidth(right - left);
	double actHeight(top - bot);
	double actLP = actWidth * lPadding / 100.;
	double actRP = actWidth * rPadding / 100.;
	double actTP = actHeight * tPadding / 100.;
	double actBP = actHeight * bPadding / 100.;

	double actTop = top + actTP;
	double actBot = bot - actBP;
	double actLeft = left - actLP;
	double actRight = right + actRP;

	actWidth = actRight - actLeft;
	actHeight = actTop - actBot;


	double xScale = width / actWidth;
	double yScale = -height / actHeight;

	Pen BackGroundPen(Color(200, 200, 200));

	Matrix matr;
	matr.Scale(xScale, yScale);
	matr.Translate(actLP - left, -actTop);


	float steplenY = (top - bot) / 10.;
	float steplenX = (right - left) / 10.;

	FontFamily FF(L"Arial");
	Gdiplus::Font font(&FF, 12, FontStyle::FontStyleRegular, UnitPixel);
	SolidBrush brush(Color::Black);
	for (int i = 0; i < 11; i++)
	{
		PointF p1, p2;
		p1.X = left;
		p1.Y = top - i * steplenY;

		p2.X = right;
		p2.Y = top - i * steplenY;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		p1.X = right - i * steplenX;
		p1.Y = top;

		p2.X = right - i * steplenX;
		p2.Y = bot;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		CString str;
		str.Format(L"%.2f", top - i * steplenY);

		PointF strPoint;
		strPoint.X = left - actWidth / 100 - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale;
		strPoint.Y = top - i * steplenY - 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		str.Format(L"%.2f", right - i * steplenX);

		strPoint.X = right - i * steplenX - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale / 2.;
		strPoint.Y = bot + 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	Pen pen(Color::Black, 1.1F);
	PointF p1(left, top), p2(left, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);

	p1 = PointF(left, bot);
	p2 = PointF(right, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);


	CString title = L"";
	if (!title.IsEmpty())
	{

		p1.X = actLeft + actWidth / 2 - CalcStringLen(lpDrawItemStruct->hDC, title) / 2 / xScale;
		p1.Y = (actTop + top) / 2 - 6 / yScale;
		matr.TransformPoints(&p1);
		gr.DrawString(title, title.GetLength() + 1, &font, p1, &brush);
	}


	for (int k = 0; k < data.size(); k++)
	{
		if (data[k]->path.empty())continue;

		p1 = PointF(0, data[k]->path[0]);
		matr.TransformPoints(&p1);
		Pen DataPen(data[k]->color, 2.0F);

		int deltaT = data[0]->path.size() - data[k]->path.size();
		for (int i = 1; i < data[k]->path.size(); i++)
		{
			p2 = PointF((i + deltaT) * data[k]->GetDt(), data[k]->path[i]);
			matr.TransformPoints(&p2);
			gr.DrawLine(&DataPen, p1, p2);
			p1 = p2;
		}
	}
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}
void Drawer::DrawPhasePortret(Gdiplus::Graphics& gr, int width, int height, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	double lPadding = 10;
	double rPadding = 5;
	double tPadding = 22;
	double bPadding = 22;

	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);

	vector<RG*>& data = *mdata;
	if (data.empty())
	{
		LeaveCriticalSection(&cs);
		DeleteCriticalSection(&cs);
		return;
	}

	double left = minPath;
	double right = maxPath;
	double top = maxSpeed;
	double bot = minSpeed;


	double actWidth(right - left);
	double actHeight(top - bot);
	double actLP = actWidth * lPadding / 100.;
	double actRP = actWidth * rPadding / 100.;
	double actTP = actHeight * tPadding / 100.;
	double actBP = actHeight * bPadding / 100.;

	double actTop = top + actTP;
	double actBot = bot - actBP;
	double actLeft = left - actLP;
	double actRight = right + actRP;

	actWidth = actRight - actLeft;
	actHeight = actTop - actBot;


	double xScale = width / actWidth;
	double yScale = -height / actHeight;

	Pen BackGroundPen(Color(200, 200, 200));

	Matrix matr;
	matr.Scale(xScale, yScale);
	matr.Translate(actLP - left, -actTop);


	float steplenY = (top - bot) / 10.;
	float steplenX = (right - left) / 10.;

	FontFamily FF(L"Arial");
	Gdiplus::Font font(&FF, 12, FontStyle::FontStyleRegular, UnitPixel);
	SolidBrush brush(Color::Black);
	for (int i = 0; i < 11; i++)
	{
		PointF p1, p2;
		p1.X = left;
		p1.Y = top - i * steplenY;

		p2.X = right;
		p2.Y = top - i * steplenY;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		p1.X = right - i * steplenX;
		p1.Y = top;

		p2.X = right - i * steplenX;
		p2.Y = bot;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		CString str;
		str.Format(L"%.2f", top - i * steplenY);

		PointF strPoint;
		strPoint.X = left - actWidth / 100 - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale;
		strPoint.Y = top - i * steplenY - 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		str.Format(L"%.2f", right - i * steplenX);

		strPoint.X = right - i * steplenX - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale / 2.;
		strPoint.Y = bot + 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	Pen pen(Color::Black, 1.1F);
	PointF p1(left, top), p2(left, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);

	p1 = PointF(left, bot);
	p2 = PointF(right, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);


	CString title = L"";
	if (!title.IsEmpty())
	{

		p1.X = actLeft + actWidth / 2 - CalcStringLen(lpDrawItemStruct->hDC, title) / 2 / xScale;
		p1.Y = (actTop + top) / 2 - 6 / yScale;
		matr.TransformPoints(&p1);
		gr.DrawString(title, title.GetLength() + 1, &font, p1, &brush);
	}


	for (int k = 0; k < data.size(); k++)
	{
		if (data[k]->path.empty())continue;
		if (data[k]->speed.empty())continue;

		p1 = PointF(data[k]->path[0], data[k]->speed[0]);
		matr.TransformPoints(&p1);
		Pen DataPen(data[k]->color, 2.0F);


		for (int i = 1; i < data[k]->path.size(); i++)
		{
			p2 = PointF(data[k]->path[i], data[k]->speed[i]);
			matr.TransformPoints(&p2);
			gr.DrawLine(&DataPen, p1, p2);
			p1 = p2;
		}
	}
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}



void Drawer::DrawMainAnimation(Gdiplus::Graphics& gr, int width, int height, LPDRAWITEMSTRUCT lpDrawItemStruct)
{

	vector<RG*>& data = *mdata;
	if (data.empty())
	{
		return;
	}

	
	double xScale = width / (2.2 * lmax);
	double yScale = height / (2.2 * lmax);
	Matrix matr;

	if (width > height)
	{
		matr.Translate((width - height) / 2., 0);
		matr.Scale(yScale, yScale);
	}
	else
	{
		matr.Translate(0, (height - width)/2.);
		matr.Scale(xScale, xScale);
	}
	matr.Translate(1.1 * lmax, 1.1 *lmax);


	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);

	PointF p0(0, 0);
	matr.TransformPoints(&p0);
	PointF p1, p2;
	for (int k = 0; k < data.size(); k++)
	{
		if (data[k]->path.empty())continue;

		p1 = PointF(sin(data[k]->path[data[k]->path.size() - 1]) * data[k]->GetL(), cos(data[k]->path[data[k]->path.size() - 1]) * data[k]->GetL());
		matr.TransformPoints(&p1);
		Pen DataPen(data[k]->color, 2.0F);

		gr.DrawLine(&DataPen, p0, p1);
		p2 = PointF(sin(data[k]->path[data[k]->path.size() - 1]) * data[k]->GetL() - 0.025 * data[k]->GetL(), cos(data[k]->path[data[k]->path.size() - 1]) * data[k]->GetL() - 0.025 * data[k]->GetL());
		p1 = PointF(sin(data[k]->path[data[k]->path.size() - 1]) * data[k]->GetL() + 0.025 * data[k]->GetL(), cos(data[k]->path[data[k]->path.size() - 1]) * data[k]->GetL() + 0.025 * data[k]->GetL());
		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		SolidBrush brush(data[k]->color);
		gr.FillEllipse(&brush, p2.X, p2.Y, p1.X - p2.X, p1.Y - p2.Y);
	}
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);

	SolidBrush brush(Color::Black);
	p0.X = -0.04 * lmax;
	p0.Y = -0.04 * lmax;
	p1.X = 0.04 * lmax;
	p1.Y = 0.04 * lmax;
	matr.TransformPoints(&p0);
	matr.TransformPoints(&p1);
	gr.FillEllipse(&brush, p0.X, p0.Y, p1.X - p0.X, p1.Y - p0.Y);
}



void Drawer::DrawTotA(Gdiplus::Graphics& gr, int width, int height, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	double lPadding = 10;
	double rPadding = 5;
	double tPadding = 22;
	double bPadding = 22;

	InitializeCriticalSection(&cs);
	EnterCriticalSection(&cs);

	vector<RG*>& data = *mdata;
	if (T.size() < 2)
	{
		LeaveCriticalSection(&cs);
		DeleteCriticalSection(&cs);
		return;
	}
	
	double left = T[0].key;
	double right = T[T.size() - 1].key;
	double top = Tmax;
	double bot = Tmin;
	if (Tmax == Tmin)
	{
		if (Tmax > 0)
		{
			top = 1.1 * Tmax;
			bot = 0.9 * Tmin;
		}
		else
		{
			top = 0.9 * Tmax;
			bot = 1.1 * Tmin;
		}
	}

	double actWidth(right - left);
	double actHeight(top - bot);
	double actLP = actWidth * lPadding / 100.;
	double actRP = actWidth * rPadding / 100.;
	double actTP = actHeight * tPadding / 100.;
	double actBP = actHeight * bPadding / 100.;

	double actTop = top + actTP;
	double actBot = bot - actBP;
	double actLeft = left - actLP;
	double actRight = right + actRP;

	actWidth = actRight - actLeft;
	actHeight = actTop - actBot;


	double xScale = width / actWidth;
	double yScale = -height / actHeight;

	Pen BackGroundPen(Color(200, 200, 200));

	Matrix matr;
	matr.Scale(xScale, yScale);
	matr.Translate(actLP - left, -actTop);


	float steplenY = (top - bot) / 10.;
	float steplenX = (right - left) / 10.;

	FontFamily FF(L"Arial");
	Gdiplus::Font font(&FF, 12, FontStyle::FontStyleRegular, UnitPixel);
	SolidBrush brush(Color::Black);
	for (int i = 0; i < 11; i++)
	{
		PointF p1, p2;
		p1.X = left;
		p1.Y = top - i * steplenY;

		p2.X = right;
		p2.Y = top - i * steplenY;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		p1.X = right - i * steplenX;
		p1.Y = top;

		p2.X = right - i * steplenX;
		p2.Y = bot;

		matr.TransformPoints(&p1);
		matr.TransformPoints(&p2);

		gr.DrawLine(&BackGroundPen, p1, p2);


		CString str;
		str.Format(L"%.2f", top - i * steplenY);

		PointF strPoint;
		strPoint.X = left - actWidth / 100 - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale;
		strPoint.Y = top - i * steplenY - 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);

		str.Format(L"%.2f", right - i * steplenX);

		strPoint.X = right - i * steplenX - CalcStringLen(lpDrawItemStruct->hDC, str) / xScale / 2.;
		strPoint.Y = bot + 6 / yScale;
		matr.TransformPoints(&strPoint);
		gr.DrawString(str, str.GetLength() + 1, &font, strPoint, &brush);
	}

	Pen pen(Color::Black, 1.1F);
	PointF p1(left, top), p2(left, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);

	p1 = PointF(left, bot);
	p2 = PointF(right, bot);
	matr.TransformPoints(&p1);
	matr.TransformPoints(&p2);
	gr.DrawLine(&pen, p1, p2);


	CString title = L"";
	if (!title.IsEmpty())
	{

		p1.X = actLeft + actWidth / 2 - CalcStringLen(lpDrawItemStruct->hDC, title) / 2 / xScale;
		p1.Y = (actTop + top) / 2 - 6 / yScale;
		matr.TransformPoints(&p1);
		gr.DrawString(title, title.GetLength() + 1, &font, p1, &brush);
	}



	p1 = PointF(T[0].key, T[0].val);
	matr.TransformPoints(&p1);
	Pen DataPen(Color::Orange, 2.0F);

	for (int i = 1; i < T.size(); i++)
	{
		p2 = PointF(T[i].key, T[i].val);
		matr.TransformPoints(&p2);
		gr.DrawLine(&DataPen, p1, p2);
		p1 = p2;
	}
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}