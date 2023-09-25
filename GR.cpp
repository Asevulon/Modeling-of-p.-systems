#include"pch.h"
#include"RG.h"

void RG::SetParams(double L, double M, double K, double U, double DT)
{
	l = L;
	m = M;
	k = K;
	u = U;
	dt = DT;
}
void RG::SetStartParams(double Y, double W)
{
	y = Y / 180. * PI;
	w = W;
	minPath = y;
	maxPath = y;
	y0 = Y;
	w0 = W;

	minSpeed = w;
	maxSpeed = w;
}
double RG::GetDt()
{
	return dt;
}
void RG::SetDt(double DT)
{
	dt = DT;
}
double RG::GetL()
{
	return l;
}
void RG::reset()
{
	SetStartParams(y0, w0);
	t = 0;
	speed.clear();
	path.clear();
	tprev = 0;
	T = 0;
	FixT = true;
}
double RG::GetT()
{
	return T;
}
double RG::GetY0()
{
	return y0;
}
double RG::f(double yy, double s)
{
	double Mmg = -g * sin(yy) / l;
	double Mstr = -u * s;
	double Mvtr = -k * s * s * l / m;
	if (s < 0)Mvtr = -Mvtr;
	return Mmg + Mstr + Mvtr;
}


void RG::DoRungeKuttaStep()
{
	double kk[4] = { 0,0,0,0 };
	double cy(y), cw(w);

	kk[0] = f(cy, cw) * dt;
	kk[1] = f(cy + cw * dt / 2., cw + kk[0] / 2.) * dt;
	kk[2] = f(cy + cw * dt / 2. + kk[0] * dt / 4., cw + kk[1] / 2.) * dt;
	kk[3] = f(cy + cw * dt + kk[1] * dt / 2., cw + kk[2]) * dt;

	y = cy + w * dt + (1. / 6.) * (kk[0] + kk[1] + kk[2]) * dt;
	w = cw + (1. / 6.) * (kk[0] + 2 * kk[1] + 2 * kk[2] + kk[3]);

	t += dt;

	path.push_back(y);
	speed.push_back(w);
	if (minPath > y)minPath = y;
	if (maxPath < y)maxPath = y;

	if (minSpeed > w)minSpeed = w;
	if (maxSpeed < w)maxSpeed = w;

	if (w * cw < 0)
	{
		T = 2 * (t - tprev);
		tprev = t;
	}
}

