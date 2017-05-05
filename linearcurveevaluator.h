#ifndef INCLUDED_LINEAR_CURVE_EVALUATOR_H
#define INCLUDED_LINEAR_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class LinearCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap, float m_fTension, bool m_bInner, std::vector<Point>& m_ptvCtrlPtsInner) const;
};

#endif