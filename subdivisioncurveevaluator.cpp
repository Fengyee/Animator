#include "SubdivisionCurveEvaluator.h"
#include "modelerapp.h"
#include "modelerui.h"
#include <string>
#include <vector>

vector<string> split(const string& input, char splitter, bool handle_double_quote);

void SubdivisionCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap, float m_fTension, bool m_bInner, std::vector<Point>& m_ptvCtrlPtsInner) const
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
			// pass
		}
	}

	int depth = ModelerApplication::getPUI()->m_pDepthSlider->value();

	vector<Point> control_points = ptvCtrlPts;
	vector<Point> points_splitted;
	for (int n = 0; n < depth; ++n)
	{
		points_splitted.clear();
		// splitting part
		for (int i = control_points.size() - averageMask.size(); i < control_points.size(); ++i)
		{
			points_splitted.push_back(Point(control_points[i].x - fAniLength, control_points[i].y));
			// Add midpoint
			if (i == control_points.size() - 1)points_splitted.push_back(Point((control_points[i].x - fAniLength + control_points[0].x) / 2, (control_points[i].y + control_points[0].y) / 2));
			else points_splitted.push_back(Point((control_points[i].x - fAniLength + control_points[i + 1].x - fAniLength) / 2, (control_points[i].y + control_points[i + 1].y) / 2));
		}

		for (int i = 0; i < control_points.size() - 1; ++i)
		{
			points_splitted.push_back(control_points[i]);
			points_splitted.push_back(Point((control_points[i].x + control_points[i + 1].x) / 2, (control_points[i].y + control_points[i + 1].y) / 2));
		}
		points_splitted.push_back(control_points[control_points.size() - 1]);

		for (int i = 0; i < averageMask.size(); ++i)
		{
			//Add midpoint
			if (i == 0)points_splitted.push_back(Point((control_points[i].x + fAniLength + control_points[control_points.size() - 1].x) / 2, (control_points[i].y + control_points[control_points.size() - 1].y) / 2));
			else points_splitted.push_back(Point((control_points[i].x + fAniLength + control_points[i - 1].x + fAniLength) / 2, (control_points[i].y + control_points[i - 1].y) / 2));
			points_splitted.push_back(Point(control_points[i].x + fAniLength, control_points[i].y));
		}

		//average part
		control_points.clear();
		for (int i = 0; i < points_splitted.size() - averageMask.size() + 1; ++i)
		{
			Point tmp;
			for (int j = 0; j < averageMask.size(); ++j)
			{
				tmp.x += points_splitted[i + j].x*averageMask[j];
				tmp.y += points_splitted[i + j].y*averageMask[j];
			}
			if (tmp.x >= 0 && tmp.x <= fAniLength)control_points.push_back(tmp);
		}
	}
	ptvEvaluatedCurvePts = control_points;
	ptvEvaluatedCurvePts.push_back(Point(0, control_points[0].y));
	ptvEvaluatedCurvePts.push_back(Point(fAniLength, control_points[control_points.size() - 1].y));
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

				if (in_quote && (current == length - 1 || input[current + 1] == splitter))
				{
					in_quote = false;

					if (curr->length() > 0)
					{
						result.push_back(*curr);
						delete curr;
						curr = new string;
					}
					continue;
				}

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