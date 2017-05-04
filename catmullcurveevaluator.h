#ifndef INCLUDED_CATMULL_CURVE_EVALUATOR_H
#define INCLUDED_CATMULL_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class CatmullCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	Point calculateCatmull(float t, const Point& p1, const Point& p2, const Point& p3, const Point& p4, float tension) const;
};

#endif#
