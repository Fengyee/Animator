#include "CatmullCurveEvaluator.h"
#include <assert.h>
#include "math.h"
#include "vec.h"

Point CatmullCurveEvaluator::calculateCatmull(float t, const Point& p1, const Point& p2, const Point& p3, const Point& p4, float tension) const
{
	Vec4f Bx(p1.x, p2.x, p3.x, p4.x);
	Vec4f By(p1.y, p2.y, p3.y, p4.y);
	Vec4f Mb[4] = { Vec4f(1, 4, 1, 0),
		Vec4f(tension/3.0, 2, tension/3.0, 0),
		Vec4f(0, tension / 3.0, 2, tension / 3.0),
		Vec4f(0, 0, 2, 0) };
	Vec4f Gx(Mb[0] * Bx, Mb[1] * Bx, Mb[2] * Bx, Mb[3] * Bx);
	Vec4f Gy(Mb[0] * By, Mb[1] * By, Mb[2] * By, Mb[3] * By);
	Point result;
	Vec4f T(t*t*t, t*t, t, 1);
	Vec4f M[4] = { Vec4f(-1, 3, -3, 1),
		Vec4f(3, -6, 3, 0),
		Vec4f(-3, 3, 0, 0),
		Vec4f(1, 0, 0, 0) };
	result.x = Vec4f(T*M[0], T*M[1], T*M[2], T*M[3])*Gx;
	result.y = Vec4f(T*M[0], T*M[1], T*M[2], T*M[3])*Gy;
	return result;
	/*
	Point result;
	Vec4f T(t*t*t, t*t, t, 1);
	Vec4f M[4] = { Vec4f(-1, 3, -3, 1),
		Vec4f(2, -5, 4, -1),
		Vec4f(-1, 0, 1, 0),
		Vec4f(0, 2, 0, 0) };
	Vec4f Gx(p1.x, p2.x, p3.x, p4.x);
	Vec4f Gy(p1.y, p2.y, p3.y, p4.y);
	result.x = T * Vec4f(M[0] * Gx / 2, M[1] * Gx / 2, M[2] * Gx / 2, M[3] * Gx / 2);
	result.y = T * Vec4f(M[0] * Gy / 2, M[1] * Gy / 2, M[2] * Gy / 2, M[3] * Gy / 2);
	//result.x = Vec4f(T*M[0]/2, T*M[1]/2, T*M[2]/2, T*M[3]/2)*Gx;
	//result.y = Vec4f(T*M[0]/2, T*M[1]/2, T*M[2]/2, T*M[3]/2)*Gy;
	*/
	return result;
}

void CatmullCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	ptvEvaluatedCurvePts.clear();
	float prev_eval_x = 0.0;
	float tension = 0.5;

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
					Point evalPt = calculateCatmull(j * 0.01, ptvCtrlPts[i], ptvEnd1, ptvEnd2, ptvEnd3, tension);
					if (evalPt.x > fAniLength) {
						evalPt.x = evalPt.x - fAniLength;
						prev_eval_x = 0.0;
					}
					if (evalPt.x > prev_eval_x) {
						ptvEvaluatedCurvePts.push_back(evalPt);
						prev_eval_x = evalPt.x;
					}
						
				}
			}
			else if (i + 2 == iCtrlPtCount) {
				Point ptvEnd1(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
				Point ptvEnd2(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y);
				for (int j = 0; j < 100; j++) {
					Point evalPt = calculateCatmull(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvEnd1, ptvEnd2, tension);
					if (evalPt.x > fAniLength) {
						evalPt.x = evalPt.x - fAniLength;
						prev_eval_x = 0.0;
					}
					if (evalPt.x > prev_eval_x) {
						ptvEvaluatedCurvePts.push_back(evalPt);
						prev_eval_x = evalPt.x;
					}
				}
			}
			else if (i + 3 == iCtrlPtCount) {
				Point ptvEnd1(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
				for (int j = 0; j < 100; j++) {
					Point evalPt = calculateCatmull(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvEnd1, tension);
					if (evalPt.x > fAniLength) {
						evalPt.x = evalPt.x - fAniLength;
						prev_eval_x = 0.0;
					}
					if (evalPt.x > prev_eval_x) {
						ptvEvaluatedCurvePts.push_back(evalPt);
						prev_eval_x = evalPt.x;
					}
				}
			}
			else {
				for (int j = 0; j < 100; j++) {
					Point evalPt = calculateCatmull(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3], tension);
					if (evalPt.x > prev_eval_x) {
						ptvEvaluatedCurvePts.push_back(evalPt);
						prev_eval_x = evalPt.x;
					}
				}
			}
		}
	}

	else {
		for (int i = 0; i < iCtrlPtCount - 3; i++)
		{
			for (int j = 0; j < 100; j++) {
				Point evalPt = calculateCatmull(j * 0.01, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3], tension);
				if (evalPt.x > prev_eval_x) {
					ptvEvaluatedCurvePts.push_back(evalPt);
					prev_eval_x = evalPt.x;
				}
			}
		}
		ptvEvaluatedCurvePts.push_back(Point(0.0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[0]);
		ptvEvaluatedCurvePts.push_back(ptvCtrlPts[iCtrlPtCount - 1]);
	}
}
