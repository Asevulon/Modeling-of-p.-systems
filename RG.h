#pragma once

#include<vector>
#include<gdiplus.h>
//Рунге-кутта (вся математика модели)
# define PI           3.14159265358979323846  /* pi */

class RG
{
	friend class SetRGDataDialog;
private:
	double y = 0;
	double w = 0;
	double t = 0;

	//устанавливаемые параметры
	double l = 0;
	double m = 0;
	double k = 0;
	double u = 0;

	double dt = 0;

	double y0, w0;

	CString name;
	//константы
	const double g = 9.8;


	bool FixT = true;
	double T = 0;
	double tprev = 0;
protected:
	inline double f(double c, double s);
public:
	std::vector<double> path;
	std::vector<double> speed;
	double minPath = 0;
	double maxPath = 0;
	double minSpeed = 0;
	double maxSpeed = 0;
	Gdiplus::Color color = Gdiplus::Color::Black;

	void SetParams(double L, double M, double K, double u, double DT);
	void SetStartParams(double Y, double W);
	void SetDt(double DT);

	void DoRungeKuttaStep();
	double GetDt();
	double GetL();
	double GetT();
	double GetY0();
	void reset();
};