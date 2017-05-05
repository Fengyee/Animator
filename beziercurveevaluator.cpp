#include "BezierCurveEvaluator.h"
#include <assert.h>
#include "math.h"
#include "vec.h"

Point BezierCurveEvaluator::calculateBezier(float t, const Point& p1, const Point& p2, const Point& p3, const Point& p4) const
{
	Point result;
	Vec4f T(t*t*t, t*t, t, 1);
	Vec4f M[4] = { Vec4f(-1, 3, -3, 1),
		Vec4f(3, -6, 3, 0),
		Vec4f(-3, 3, 0, 0),
		Vec4f(1, 0, 0, 0) };
	Vec4f Gx(p1.x, p2.x, p3.x, p4.x);
	Vec4f Gy(p1.y, p2.y, p3.y, p4.y);

	result.x = T * Vec4f(M[0] * Gx, M[1] * Gx, M[2] * Gx, M[3] * Gx);
	result.y = T * Vec4f(M[0] * Gy, M[1] * Gy, M[2] * Gy, M[3] * Gy);

	return result;
}

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap, float m_fTension, bool m_bInner, std::vector<Point>& m_ptvCtrlPtsInner) const
{
	double flatness = 0.1;
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();

	float y1, y2;

	if (bWrap) {
		// if wrapping is on, interpolate the y value at xmin and
		// xmax so that the slopes of the lines adjacent to the
		// wraparound are equal.

		if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[iCtrlPtCount - 1].x > 0.0f) {
			y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x) +
				ptvCtrlPts[iCtrlPtCount - 1].y * ptvCtrlPts[0].x) /
				(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
		}
		else
			y1 = ptvCtrlPts[0].y;
		
		y2 = y1;
	}
	else {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal.

		y1 = ptvCtrlPts[0].y;
		y2 = ptvCtrlPts[iCtrlPtCount - 1].y;
	}


	if (bWrap) {
		for (int i = 0; i <= iCtrlPtCount; i = i + 3)
		{
			if (i + 3 > iCtrlPtCount) {
				for (int j = i; j < iCtrlPtCount; j++) {
					ptvEvaluatedCurvePts.push_back(ptvCtrlPts[j]);
				}
				continue;
			}
			if (i + 3 == iCtrlPtCount) {
				Point ptvEnd(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
				for (int j = 0; j < 100; j++) {
					Point evalPt = calculateBezier(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvEnd);
					if (evalPt.x < fAniLength) ptvEvaluatedCurvePts.push_back(evalPt);
					else ptvEvaluatedCurvePts.push_back(Point(evalPt.x - fAniLength, evalPt.y));
				}
			}
			else {
				for (int j = 0; j < 100; j++) {
					ptvEvaluatedCurvePts.push_back(calculateBezier(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3]));
				}
			}
		}
		if (iCtrlPtCount % 3 != 0) {
			ptvEvaluatedCurvePts.push_back(Point(0.0, y1));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));
		}
	}

	else {
		for (int i = 0; i < iCtrlPtCount; i = i + 3)
		{
			if (i >= iCtrlPtCount - 3)
			{
				for (int j = i; j < iCtrlPtCount; j++) {
					ptvEvaluatedCurvePts.push_back(ptvCtrlPts[j]);
				}
				continue;
			}
			
			for (int j = 0; j < 100; j++) {
				ptvEvaluatedCurvePts.push_back(calculateBezier(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3]));
			}
		}
		ptvEvaluatedCurvePts.push_back(Point(0.0, y1));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));
	}


}
/*
void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	double flatness = 0.1;
	//int iCtrlPtCount = ptvCtrlPts.size();
	//ptvEvaluatedCurvePts.assign(ptvCtrlPts.begin(), ptvCtrlPts.end());

	int count = 0;
	int sampleRate = 120;
	ptvEvaluatedCurvePts.clear();

	int CtrlPtGroupCount = (ptvCtrlPts.size() - 1) / 3;
	int RemainPtCount = ptvCtrlPts.size() - CtrlPtGroupCount * 3;


	// Normal case
	if (bWrap)
	{
		// when #remain_pts==3,they can form a bezier curve with the first ctrl pt
		if (RemainPtCount == 3)
		{
			Point p0(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
			int pos = ptvCtrlPts.size() - RemainPtCount;
			for (int j = 0; j <= sampleRate; ++j)
			{
				Point tmp = calculateBezier((float)j / sampleRate, ptvCtrlPts[pos], ptvCtrlPts[pos + 1], ptvCtrlPts[pos + 2], p0);
				if (tmp.x >= fAniLength)ptvEvaluatedCurvePts.push_back(Point(tmp.x - fAniLength, tmp.y));
				else ptvEvaluatedCurvePts.push_back(tmp);

			}

		}
		// else do linearly interpolate
		else
		{
			float y1;
			if ((ptvCtrlPts[0].x + fAniLength) - ptvCtrlPts[ptvCtrlPts.size() - 1].x > 0.0f)
			{
				y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x) +
					ptvCtrlPts[ptvCtrlPts.size() - 1].y * ptvCtrlPts[0].x) /
					(ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[ptvCtrlPts.size() - 1].x);
			}
			else
				y1 = ptvCtrlPts[0].y;
			ptvEvaluatedCurvePts.push_back(Point(0, y1));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, y1));
		}
	}
	else
	{
		//handle the line segment before first and after last ctrl pt
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[ptvCtrlPts.size() - 1].y));
	}

	for (int i = 0; i < CtrlPtGroupCount; ++i)
	{
		for (int j = 0; j <= sampleRate; ++j)
		{
			Point tmp = calculateBezier((float)j / sampleRate, ptvCtrlPts[3 * i], ptvCtrlPts[3 * i + 1], ptvCtrlPts[3 * i + 2], ptvCtrlPts[3 * i + 3]);
			if (tmp.x <= fAniLength)ptvEvaluatedCurvePts.push_back(tmp);
		}
	}
	//handle pts which can't form a groud of ctrl pts
	if (RemainPtCount != 3 || !bWrap)
	{
		for (int i = ptvCtrlPts.size() - RemainPtCount; i < ptvCtrlPts.size(); ++i)
		{
			ptvEvaluatedCurvePts.push_back(ptvCtrlPts[i]);
		}
	}
}
*/