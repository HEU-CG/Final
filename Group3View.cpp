
// Group3View.cpp: CGroup3View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "Group3.h"
#endif

#include "Group3Doc.h"
#include "Group3View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGroup3View

IMPLEMENT_DYNCREATE(CGroup3View, CView)

BEGIN_MESSAGE_MAP(CGroup3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGroup3View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_DRAW, &CGroup3View::OnMyDraw)
	ON_COMMAND(ID_FILL, &CGroup3View::OnMyFill)
	ON_COMMAND(ID_CLEAR, &CGroup3View::OnClear)
END_MESSAGE_MAP()

// CGroup3View 构造/析构

CGroup3View::CGroup3View() noexcept
{
	// TODO: 在此处添加构造代码
	TR = 1000, d = 1000, Phi = 90, Theta = 60;

	Tk[1] = sin(PI * Theta / 180);
	Tk[2] = sin(PI * Phi / 180);
	Tk[3] = cos(PI * Theta / 180);
	Tk[4] = cos(PI * Phi / 180);
	Tk[5] = Tk[2] * Tk[3];
	Tk[6] = Tk[2] * Tk[1];
	Tk[7] = Tk[4] * Tk[3];
	Tk[8] = Tk[4] * Tk[1];
	ViewPointx = TR * Tk[6];
	ViewPointy = TR * Tk[4];
	ViewPointz = TR * Tk[5];

	Ia = 255;
	Ip = 255;
	ka = 0.2;
	kd = 0.6;
	ks = 0.7;
	ns = 50;
	LPx = TR * sin(PI * 45 / 180) * sin(PI * 45 / 180);
	LPy = TR * cos(PI * 45 / 180);
	LPz = TR * sin(PI * 45 / 180) * cos(PI * 45 / 180);

}

CGroup3View::~CGroup3View()
{
}

BOOL CGroup3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CGroup3View 绘图
void CGroup3View::FillSphere(CDC* pDC, double R) {
	CPoint rgnpoints[4];
	COLORREF fillColor;
	CBrush brush;
	CPen NewPen;
	double sx[4], sy[4];

	double x[4], y[4], z[4];
	double x1[4], y1[4], z1[4];
	int a1, a2, a3, b1, b2;
	double xn, yn, zn, vn, xv, yv, zv, costheta, mv;
	double Ie, Id, Is, costhita1, cosphi, hx, hy, hz, lx, ly, lz;
	int C = 0;

	double px, py, ph, pH, pp;
	px = sqrt(2 * R * R * (1 - cos(72 * PI / 180)));
	ph = sqrt(R * R * (1 - 2 * cos(72 * PI / 180)));
	py = px / (2 + sqrt(2 - 2 * cos(108 * PI / 180)));
	pp = py * sin(36 * PI / 180);
	pH = sqrt(pow(px * sqrt(3) / 2, 2) + pp * pp) / 2;

	Ie = Ia * ka;

	//floor-1
	a1 = 0; a3 = 72;
	for (int ii = 1; ii <= 5; ++ii)
	{
		x[0] = R * cos(a1 * PI / 180);
		y[0] = pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = 0;
		y[1] = pH + ph;
		z[1] = 0;
		x[2] = R * cos(a3 * PI / 180);
		y[2] = pH;
		z[2] = R * sin(a3 * PI / 180);

		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		if (costheta >= 0)
		{
			lx = LPx - x[0]; ly = LPy - y[0]; lz = LPz - z[0];
			mv = sqrt(lx * lx + ly * ly + lz * lz);
			lx = lx / mv; ly = ly / mv; lz = lz / mv;

			costhita1 = xn * lx + yn * ly + zn * lz;
			hx = lx + xv; hy = ly + yv; hz = lz + zv;
			mv = sqrt(hx * hx + hy * hy + hz * hz);
			hx = hx / mv; hy = hy / mv; hz = hz / mv;
			cosphi = xn * hx + yn * hy + zn * hz;
			if (costhita1 >= 0)
				Id = Ip * kd * costhita1;
			else
				Id = 0;
			if (cosphi >= 0)
				Is = Ip * ks * pow(cosphi, ns);
			else
				Is = 0;

			C = (int)(Ie + Id + Is);
			if (C > 255) C = 255;

			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;

				rgnpoints[k].x = x1[k];
				rgnpoints[k].y = y1[k];
			}
			fillColor = RGB(C, C, C);
			if (!brush.CreateSolidBrush(fillColor)) return;
			if (!NewPen.CreatePen(PS_SOLID, 1, fillColor)) return;
			pDC->SelectObject(&NewPen);
			pDC->SelectObject(&brush);
			pDC->Polygon(rgnpoints, 4);
			brush.DeleteObject();
			NewPen.DeleteObject();
		}

		a1 = (a1 + 72) % 360; a3 = (a3 + 72) % 360;
	}

	//floor-2
	a1 = 0; a2 = 36; a3 = 72;
	for (int i = 1; i <= 5; ++i)
	{
		x[0] = R * cos(a1 * PI / 180);
		y[0] = pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a3 * PI / 180);
		y[1] = pH;
		z[1] = R * sin(a3 * PI / 180);
		x[2] = R * cos(a2 * PI / 180);
		y[2] = -pH;
		z[2] = R * sin(a2 * PI / 180);

		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		if (costheta >= 0)
		{
			lx = LPx - x[0]; ly = LPy - y[0]; lz = LPz - z[0];
			mv = sqrt(lx * lx + ly * ly + lz * lz);
			lx = lx / mv; ly = ly / mv; lz = lz / mv;

			costhita1 = xn * lx + yn * ly + zn * lz;
			hx = lx + xv; hy = ly + yv; hz = lz + zv;
			mv = sqrt(hx * hx + hy * hy + hz * hz);
			hx = hx / mv; hy = hy / mv; hz = hz / mv;
			cosphi = xn * hx + yn * hy + zn * hz;
			if (costhita1 >= 0)
				Id = Ip * kd * costhita1;
			else
				Id = 0;
			if (cosphi >= 0)
				Is = Ip * ks * pow(cosphi, ns);
			else
				Is = 0;

			C = (int)(Ie + Id + Is);
			if (C > 255) C = 255;

			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;

				rgnpoints[k].x = x1[k];
				rgnpoints[k].y = y1[k];
			}
			fillColor = RGB(C, C, C);
			if (!brush.CreateSolidBrush(fillColor)) return;
			if (!NewPen.CreatePen(PS_SOLID, 1, fillColor)) return;
			pDC->SelectObject(&NewPen);
			pDC->SelectObject(&brush);
			pDC->Polygon(rgnpoints, 4);
			brush.DeleteObject();
			NewPen.DeleteObject();
		}

		a1 = (a1 + 72) % 360; a2 = (a2 + 72) % 360; a3 = (a3 + 72) % 360;
	}

	//floor-3
	a1 = 36; a2 = 72; a3 = 108;
	for (int j = 1; j <= 5; ++j)
	{
		x[0] = R * cos(a1 * PI / 180);
		y[0] = -pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a2 * PI / 180);
		y[1] = pH;
		z[1] = R * sin(a2 * PI / 180);
		x[2] = R * cos(a3 * PI / 180);
		y[2] = -pH;
		z[2] = R * sin(a3 * PI / 180);

		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		if (costheta >= 0)
		{
			lx = LPx - x[0]; ly = LPy - y[0]; lz = LPz - z[0];
			mv = sqrt(lx * lx + ly * ly + lz * lz);
			lx = lx / mv; ly = ly / mv; lz = lz / mv;

			costhita1 = xn * lx + yn * ly + zn * lz;
			hx = lx + xv; hy = ly + yv; hz = lz + zv;
			mv = sqrt(hx * hx + hy * hy + hz * hz);
			hx = hx / mv; hy = hy / mv; hz = hz / mv;
			cosphi = xn * hx + yn * hy + zn * hz;
			if (costhita1 >= 0)
				Id = Ip * kd * costhita1;
			else
				Id = 0;
			if (cosphi >= 0)
				Is = Ip * ks * pow(cosphi, ns);
			else
				Is = 0;

			C = (int)(Ie + Id + Is);
			if (C > 255) C = 255;

			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;

				rgnpoints[k].x = x1[k];
				rgnpoints[k].y = y1[k];
			}
			fillColor = RGB(C, C, C);
			if (!brush.CreateSolidBrush(fillColor)) return;
			if (!NewPen.CreatePen(PS_SOLID, 1, fillColor)) return;
			pDC->SelectObject(&NewPen);
			pDC->SelectObject(&brush);
			pDC->Polygon(rgnpoints, 4);
			brush.DeleteObject();
			NewPen.DeleteObject();
		}

		a1 = (a1 + 72) % 360; a2 = (a2 + 72) % 360; a3 = (a3 + 72) % 360;
	}

	//floor-4
	a1 = 36; a3 = 108;
	for (int jj = 1; jj <= 5; ++jj)
	{
		x[0] = R * cos(a1 * PI / 180);
		y[0] = -pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a3 * PI / 180);
		y[1] = -pH;
		z[1] = R * sin(a3 * PI / 180);
		x[2] = 0;
		y[2] = -pH - ph;
		z[2] = 0;

		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		if (costheta >= 0)
		{
			lx = LPx - x[0]; ly = LPy - y[0]; lz = LPz - z[0];
			mv = sqrt(lx * lx + ly * ly + lz * lz);
			lx = lx / mv; ly = ly / mv; lz = lz / mv;

			costhita1 = xn * lx + yn * ly + zn * lz;
			hx = lx + xv; hy = ly + yv; hz = lz + zv;
			mv = sqrt(hx * hx + hy * hy + hz * hz);
			hx = hx / mv; hy = hy / mv; hz = hz / mv;
			cosphi = xn * hx + yn * hy + zn * hz;
			if (costhita1 >= 0)
				Id = Ip * kd * costhita1;
			else
				Id = 0;
			if (cosphi >= 0)
				Is = Ip * ks * pow(cosphi, ns);
			else
				Is = 0;

			C = (int)(Ie + Id + Is);
			if (C > 255) C = 255;

			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;

				rgnpoints[k].x = x1[k];
				rgnpoints[k].y = y1[k];
			}
			fillColor = RGB(C, C, C);
			if (!brush.CreateSolidBrush(fillColor)) return;
			if (!NewPen.CreatePen(PS_SOLID, 1, fillColor)) return;
			pDC->SelectObject(&NewPen);
			pDC->SelectObject(&brush);
			pDC->Polygon(rgnpoints, 4);
			brush.DeleteObject();
			NewPen.DeleteObject();
		}

		a1 = (a1 + 72) % 360; a3 = (a3 + 72) % 360;
	}
}

void CGroup3View::HideSphere(CDC* pDC, double R) {
	double x[4], y[4], z[4];
	double x1[4], y1[4], z1[4];
	int a1, a2, a3;
	double sx[4], sy[4];
	double xn, yn, zn, vn, xv, yv, zv, costheta, mv;
	Tk[1] = sin(PI * Theta / 180);
	Tk[2] = sin(PI * Phi / 180);
	Tk[3] = cos(PI * Theta / 180);
	Tk[4] = cos(PI * Phi / 180);
	Tk[5] = Tk[2] * Tk[3];
	Tk[6] = Tk[2] * Tk[1];
	Tk[7] = Tk[4] * Tk[3];
	Tk[8] = Tk[4] * Tk[1];

	double px, py, ph, pH, pp;
	px = sqrt(2 * R * R * (1 - cos(72 * PI / 180)));
	ph = sqrt(R * R * (1 - 2 * cos(72 * PI / 180)));
	py = px / (2 + sqrt(2 - 2 * cos(108 * PI / 180)));
	pp = py * sin(36 * PI / 180);
	pH = sqrt(pow(px * sqrt(3) / 2, 2) + pp * pp) / 2;


	//floor-1
	a1 = 0; a3 = 72;
	for (int ii = 1; ii <= 5; ++ii)
	{
		x[0] = R * cos(a1 * PI / 180);
		y[0] = pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = 0;
		y[1] = pH + ph;
		z[1] = 0;
		x[2] = R * cos(a3 * PI / 180);
		y[2] = pH;
		z[2] = R * sin(a3 * PI / 180);

		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		if (costheta >= 0)
		{
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
				sx[k] = x1[k];
				sy[k] = y1[k];
			}
			pDC->MoveTo((int)sx[0], (int)sy[0]);
			pDC->LineTo((int)sx[1], (int)sy[1]);
			pDC->LineTo((int)sx[2], (int)sy[2]);
			pDC->LineTo((int)sx[0], (int)sy[0]);
		}
		a1 = (a1 + 72) % 360; a3 = (a3 + 72) % 360;
	}

	//floor-2
	a1 = 0; a2 = 36; a3 = 72;
	for (int i = 1; i <= 5; ++i)
	{
		x[0] = R * cos(a1 * PI / 180);
		y[0] = pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a3 * PI / 180);
		y[1] = pH;
		z[1] = R * sin(a3 * PI / 180);
		x[2] = R * cos(a2 * PI / 180);
		y[2] = -pH;
		z[2] = R * sin(a2 * PI / 180);

		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		if (costheta >= 0)
		{
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
				sx[k] = x1[k];
				sy[k] = y1[k];
			}
			pDC->MoveTo((int)sx[0], (int)sy[0]);
			pDC->LineTo((int)sx[1], (int)sy[1]);
			pDC->LineTo((int)sx[2], (int)sy[2]);
			pDC->LineTo((int)sx[0], (int)sy[0]);
		}
		a1 = (a1 + 72) % 360; a2 = (a2 + 72) % 360; a3 = (a3 + 72) % 360;
	}

	//floor-3
	a1 = 36; a2 = 72; a3 = 108;
	for (int j = 1; j <= 5; ++j)
	{
		x[0] = R * cos(a1 * PI / 180);
		y[0] = -pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a2 * PI / 180);
		y[1] = pH;
		z[1] = R * sin(a2 * PI / 180);
		x[2] = R * cos(a3 * PI / 180);
		y[2] = -pH;
		z[2] = R * sin(a3 * PI / 180);

		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		if (costheta >= 0)
		{
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
				sx[k] = x1[k];
				sy[k] = y1[k];
			}
			pDC->MoveTo((int)sx[0], (int)sy[0]);
			pDC->LineTo((int)sx[1], (int)sy[1]);
			pDC->LineTo((int)sx[2], (int)sy[2]);
			pDC->LineTo((int)sx[0], (int)sy[0]);
		}
		a1 = (a1 + 72) % 360; a2 = (a2 + 72) % 360; a3 = (a3 + 72) % 360;
	}

	//floor-4
	a1 = 36; a3 = 108;
	for (int jj = 1; jj <= 5; ++jj)
	{
		x[0] = R * cos(a1 * PI / 180);
		y[0] = -pH;
		z[0] = R * sin(a1 * PI / 180);
		x[1] = R * cos(a3 * PI / 180);
		y[1] = -pH;
		z[1] = R * sin(a3 * PI / 180);
		x[2] = 0;
		y[2] = -pH - ph;
		z[2] = 0;


		xn = (y[1] - y[0]) * (z[2] - z[0]) - (y[2] - y[0]) * (z[1] - z[0]);
		yn = (x[2] - x[0]) * (z[1] - z[0]) - (x[1] - x[0]) * (z[2] - z[0]);
		zn = (x[1] - x[0]) * (y[2] - y[0]) - (x[2] - x[0]) * (y[1] - y[0]);
		vn = sqrt(xn * xn + yn * yn + zn * zn);
		xn = xn / vn;
		yn = yn / vn;
		zn = zn / vn;
		xv = ViewPointx - x[0];
		yv = ViewPointy - y[0];
		zv = ViewPointz - z[0];
		mv = sqrt(xv * xv + yv * yv + zv * zv);
		xv = xv / mv;
		yv = yv / mv;
		zv = zv / mv;
		costheta = xn * xv + yn * yv + zn * zv;
		if (costheta >= 0)
		{
			for (int k = 0; k < 3; k++)
			{
				x1[k] = x[k] * Tk[3] - z[k] * Tk[1];
				y1[k] = -x[k] * Tk[8] + y[k] * Tk[2] - z[k] * Tk[7];
				z1[k] = -x[k] * Tk[6] - y[k] * Tk[4] - z[k] * Tk[5] + TR;
				sx[k] = x1[k];
				sy[k] = y1[k];
			}
			pDC->MoveTo((int)sx[0], (int)sy[0]);
			pDC->LineTo((int)sx[1], (int)sy[1]);
			pDC->LineTo((int)sx[2], (int)sy[2]);
			pDC->LineTo((int)sx[0], (int)sy[0]);
		}
		a1 = (a1 + 72) % 360; a3 = (a3 + 72) % 360;
	}
}

void CGroup3View::OnDraw(CDC* pDC)
{
	CGroup3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	DrawXY();
}


// CGroup3View 打印


void CGroup3View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGroup3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CGroup3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CGroup3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CGroup3View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGroup3View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGroup3View 诊断

#ifdef _DEBUG
void CGroup3View::AssertValid() const
{
	CView::AssertValid();
}

void CGroup3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGroup3Doc* CGroup3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGroup3Doc)));
	return (CGroup3Doc*)m_pDocument;
}
#endif //_DEBUG


// CGroup3View 消息处理程序
void CGroup3View::DrawXY() {
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(rect);//获取当前窗口区域大小
	pDC->SetMapMode(MM_ANISOTROPIC); //设置映射模式
	pDC->SetWindowExt(rect.Width(), rect.Height()); //设置窗口范围
	pDC->SetViewportExt(rect.Width(), -rect.Height()); //设置视区范围
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2); //设置视区原点

	// 绘制x轴
	pDC->MoveTo(-rect.Width() / 2, 0);
	pDC->LineTo(rect.Width() / 2, 0);
	pDC->TextOutW(rect.Width() / 2 - 50, -20, L"x");

	// 绘制y轴
	pDC->MoveTo(0, -rect.Height() / 2);
	pDC->LineTo(0, rect.Height() / 2);
	pDC->TextOutW(-20, rect.Height() / 2 - 20, L"y");

	// 标出原点
	CBrush brush(RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(&brush);
	pDC->Ellipse(CRect(-3, -3, 3, 3));
	pDC->SelectObject(oldBrush);

	// 绘制箭头
	int arrowSize = 6;
	pDC->MoveTo(rect.Width() / 2 - arrowSize, arrowSize);
	pDC->LineTo(rect.Width() / 2, 0);
	pDC->LineTo(rect.Width() / 2 - arrowSize, -arrowSize);
	pDC->MoveTo(-arrowSize, rect.Height() / 2 - arrowSize);
	pDC->LineTo(0, rect.Height() / 2);
	pDC->LineTo(arrowSize, rect.Height() / 2 - arrowSize);
	ReleaseDC(pDC);
}

void CGroup3View::OnMyDraw()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	HideSphere(pDC, 100);
	ReleaseDC(pDC);
}


void CGroup3View::OnMyFill()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(rect.Width(), rect.Height());
	pDC->SetViewportExt(rect.Width(), -rect.Height());
	pDC->SetViewportOrg(rect.Width() / 2, rect.Height() / 2);
	FillSphere(pDC, 100);
	ReleaseDC(pDC);
}


void CGroup3View::OnClear()
{
	// TODO: 在此添加命令处理程序代码
	CDC* pDC = GetDC();
	CRect rect;
	GetClientRect(rect);
	CBrush BkBrush;
	BkBrush.CreateSolidBrush(RGB(255, 255, 255));
	pDC->FillRect(rect, &BkBrush);
	DrawXY();
	ReleaseDC(pDC);
}
