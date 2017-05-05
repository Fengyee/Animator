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
		const bool& bWrap, float m_fTension, bool m_bInner, std::vector<Point>& m_ptvCtrlPtsInner) const;
	Point calculateCatmull(int i,int t, const Point& p1, const Point& p2, const Point& p3, const Point& p4, float tension, std::vector<Point>& m_ptvCtrlPtsInner, std::vector<Point> prevInner, bool bInner) const;
};

#endif#
