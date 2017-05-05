#include "SubdivisionCurveEvaluator.h"
#include "modelerapp.h"
#include "modelerui.h"
#include <string>
#include <vector>

vector<string> split(const string& input, char splitter, bool handle_double_quote);

void SubdivisionCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap, float m_fTension) const
{
	vector<double> averageMask;
	string amstr(ModelerApplication::getPUI()->averageMask->value());
	auto amstr_s = split(amstr, ' ', false);
	for (auto& s : amstr_s)
	{
		try
		{
			averageMask.push_back(stod(s));
		}
		catch (exception& e)
		{

		}
	}

	int depth = ModelerApplication::getPUI()->m_pDepthSlider->value();

	vector<Point> ctrlpts = ptvCtrlPts;
	vector<Point> PtsAfterSplitting;
	for (int n = 0; n < depth; ++n)
	{
		PtsAfterSplitting.clear();
		// splitting part
		for (int i = ctrlpts.size() - averageMask.size(); i < ctrlpts.size(); ++i)
		{
			PtsAfterSplitting.push_back(Point(ctrlpts[i].x - fAniLength, ctrlpts[i].y));
			// Add midpoint
			if (i == ctrlpts.size() - 1)PtsAfterSplitting.push_back(Point((ctrlpts[i].x - fAniLength + ctrlpts[0].x) / 2, (ctrlpts[i].y + ctrlpts[0].y) / 2));
			else PtsAfterSplitting.push_back(Point((ctrlpts[i].x - fAniLength + ctrlpts[i + 1].x - fAniLength) / 2, (ctrlpts[i].y + ctrlpts[i + 1].y) / 2));
		}

		for (int i = 0; i < ctrlpts.size() - 1; ++i)
		{
			PtsAfterSplitting.push_back(ctrlpts[i]);
			PtsAfterSplitting.push_back(Point((ctrlpts[i].x + ctrlpts[i + 1].x) / 2, (ctrlpts[i].y + ctrlpts[i + 1].y) / 2));
		}
		PtsAfterSplitting.push_back(ctrlpts[ctrlpts.size() - 1]);

		for (int i = 0; i < averageMask.size(); ++i)
		{
			//Add midpoint
			if (i == 0)PtsAfterSplitting.push_back(Point((ctrlpts[i].x + fAniLength + ctrlpts[ctrlpts.size() - 1].x) / 2, (ctrlpts[i].y + ctrlpts[ctrlpts.size() - 1].y) / 2));
			else PtsAfterSplitting.push_back(Point((ctrlpts[i].x + fAniLength + ctrlpts[i - 1].x + fAniLength) / 2, (ctrlpts[i].y + ctrlpts[i - 1].y) / 2));
			PtsAfterSplitting.push_back(Point(ctrlpts[i].x + fAniLength, ctrlpts[i].y));
		}

		//average part
		ctrlpts.clear();
		for (int i = 0; i < PtsAfterSplitting.size() - averageMask.size() + 1; ++i)
		{
			Point tmp;
			for (int j = 0; j < averageMask.size(); ++j)
			{
				tmp.x += PtsAfterSplitting[i + j].x*averageMask[j];
				tmp.y += PtsAfterSplitting[i + j].y*averageMask[j];
			}
			if (tmp.x >= 0 && tmp.x <= fAniLength)ctrlpts.push_back(tmp);
		}
	}
	ptvEvaluatedCurvePts = ctrlpts;
	ptvEvaluatedCurvePts.push_back(Point(0, ctrlpts[0].y));
	ptvEvaluatedCurvePts.push_back(Point(fAniLength, ctrlpts[ctrlpts.size() - 1].y));
}

vector<string> split(const string& input, char splitter, bool handle_double_quote)
{
	vector<string> result;
	string::size_type length = input.length();
	bool in_quote = false;

	string* curr = new string;
	char c;
	for (unsigned int current = 0; current < length; ++current)
	{
		c = input[current];
		if (c == splitter)
		{
			// if we are in a quoted area,
			// the splitters shall be preserved
			if (handle_double_quote && in_quote)
			{
				curr->push_back(c);
				continue;
			}

			// add the string to result, and start a new one
			if (curr->length() > 0)
			{
				result.push_back(*curr);
				delete curr;
				curr = new string;
			}
		}
		else
		{
			if (handle_double_quote && c == '"')
			{
				// WARNING: Short-cut evaluation used
				// DO NOT change the operant sequence of ||'s

				// if we are in quote, the way out is
				// either the quote is the last character,
				// or it is followed by a splitter
				if (in_quote && (current == length - 1 || input[current + 1] == splitter))
				{
					in_quote = false;

					// when we are getting out of a quote, we should also end the string
					if (curr->length() > 0)
					{
						result.push_back(*curr);
						delete curr;
						curr = new string;
					}
					continue;
				}
				// a start of the quote shall be
				// either at the beginning of the string,
				// or right after a splitter
				else if (!in_quote && (current == 0 || input[current - 1] == splitter))
				{
					in_quote = true;
					continue;
				}
			}

			curr->push_back(c);
		}
	}

	if (curr->length() > 0)
		result.push_back(*curr);
	delete curr;

	return result;
}