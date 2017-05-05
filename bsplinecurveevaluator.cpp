#include "BsplineCurveEvaluator.h"
#include <assert.h>
#include "math.h"
#include "vec.h"

Point BsplineCurveEvaluator::calculateBspline(float t, const Point& p1, const Point& p2, const Point& p3, const Point& p4) const
{
	Vec4f Bx(p1.x, p2.x, p3.x, p4.x);
	Vec4f By(p1.y, p2.y, p3.y, p4.y);
	Vec4f Mb[4] = { Vec4f(1, 4, 1, 0),
		Vec4f(0, 4, 2, 0),
		Vec4f(0, 2, 4, 0),
		Vec4f(0, 1, 4, 1) };
	Vec4f Gx(Mb[0]*Bx /6, Mb[1] * Bx / 6, Mb[2] * Bx / 6, Mb[3] * Bx / 6);
	Vec4f Gy(Mb[0] * By / 6, Mb[1] * By / 6, Mb[2] * By / 6, Mb[3] * By / 6);
	Point result;
	Vec4f T(t*t*t, t*t, t, 1);
	Vec4f M[4] = { Vec4f(-1, 3, -3, 1),
		Vec4f(3, -6, 3, 0),
		Vec4f(-3, 3, 0, 0),
		Vec4f(1, 0, 0, 0) };
	result.x = Vec4f(T*M[0], T*M[1], T*M[2], T*M[3])*Gx;
	result.y = Vec4f(T*M[0], T*M[1], T*M[2], T*M[3])*Gy;
	return result;
}

void BsplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap, float m_fTension) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();

	if (bWrap) {
		for (int i = 0; i < iCtrlPtCount; i++)
		{

			if (i + 1 == iCtrlPtCount) {
				Point ptvEnd1(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
				Point ptvEnd2(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y);
				Point ptvEnd3;
				if (iCtrlPtCount > 2) ptvEnd3 = Point(ptvCtrlPts[2].x + fAniLength, ptvCtrlPts[2].y);
				else ptvEnd3 = Point(ptvCtrlPts[0].x + fAniLength * 2, ptvCtrlPts[0].y);
				for (int j = 0; j < 100; j++) {
					Point evalPt = calculateBspline(j * 0.01, ptvCtrlPts[i], ptvEnd1, ptvEnd2, ptvEnd3);
					if (evalPt.x < fAniLength) ptvEvaluatedCurvePts.push_back(evalPt);
					else ptvEvaluatedCurvePts.push_back(Point(evalPt.x - fAniLength, evalPt.y));
				}
			}
			else if (i + 2 == iCtrlPtCount) {
				Point ptvEnd1(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
				Point ptvEnd2(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y);
				for (int j = 0; j < 100; j++) {
					Point evalPt = calculateBspline(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i+1], ptvEnd1, ptvEnd2);
					if (evalPt.x < fAniLength) ptvEvaluatedCurvePts.push_back(evalPt);
					else ptvEvaluatedCurvePts.push_back(Point(evalPt.x - fAniLength, evalPt.y));
				}
			}
			else if (i + 3 == iCtrlPtCount) {
				Point ptvEnd1(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
				for (int j = 0; j < 100; j++) {
					Point evalPt = calculateBspline(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvEnd1);
					if (evalPt.x < fAniLength) ptvEvaluatedCurvePts.push_back(evalPt);
					else ptvEvaluatedCurvePts.push_back(Point(evalPt.x - fAniLength, evalPt.y));
				}
			}
			else {
				for (int j = 0; j < 100; j++) {
					ptvEvaluatedCurvePts.push_back(calculateBspline(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3]));
				}
			}
		}
	}

	else {
		for (int i = 0; i < iCtrlPtCount - 3; i++)
		{
			for (int j = 0; j < 100; j++) {
				ptvEvaluatedCurvePts.push_back(calculateBspline(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3]));
			}
		}
		ptvEvaluatedCurvePts.push_back(Point(0.0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[0]);
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[iCtrlPtCount - 1]);
	}


}
