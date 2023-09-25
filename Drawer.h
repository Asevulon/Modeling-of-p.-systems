#pragma once


// Drawer
#include"RG.h"
#include<vector>


enum DrawMode
{
	none,
	DrawGraph,
	DrawPhasePortret,
	DrawMainAnimation,
	DrawTotA
};

struct vectorpair
{
	double key = 0;
	double val = 0;
	vectorpair() {}
	vectorpair(double key, double val) :key(key), val(val) {}
};
class Drawer : public CStatic
{
	DECLARE_DYNAMIC(Drawer)

public:
	Drawer();
	virtual ~Drawer();
	std::vector<RG*>* mdata = nullptr;
	bool init = false;
	double maxPath = 0;
	double minPath = 0;
	double maxSpeed = 0;
	double minSpeed = 0;
	double lmax = 0;

	std::vector<vectorpair>T;
	double Tmax = 0;
	double Tmin = 0;

	ULONG_PTR token;
	CRITICAL_SECTION cs;
protected:
	double Drawer::CalcStringLen(HDC hDC, CString str);
	void Drawer::DrawGraph(Gdiplus::Graphics&gr, int width, int height, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawPhasePortret(Gdiplus::Graphics& gr, int width, int height, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawMainAnimation(Gdiplus::Graphics& gr, int width, int height, LPDRAWITEMSTRUCT lpDrawItemStruct);
	void DrawTotA(Gdiplus::Graphics& gr, int width, int height, LPDRAWITEMSTRUCT lpDrawItemStruct);
	DECLARE_MESSAGE_MAP()
public:
	void SetData(std::vector<RG*>* d);
	DrawMode DrwMode;

	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
};


