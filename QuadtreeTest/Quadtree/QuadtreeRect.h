// author: ldlchina
#pragma once

#include "QuadtreePoint.h"

static const double s_Tolerance = 0.000001;

class QuadtreeRect
{
public:
	QuadtreeRect(double minX, double minY, double maxX, double maxY) :
		minX_(minX),
		minY_(minY),
		maxX_(maxX),
		maxY_(maxY)
	{
	}

	double minX() const { return minX_; }
	double minY() const { return minY_; }
	double maxX() const { return maxX_; }
	double maxY() const { return maxY_; }

	bool notEmpty() const
	{
		return (minX_ < maxX_ + s_Tolerance) && (minY_ < maxY_ + s_Tolerance);
	}

	bool contain(const QuadtreePoint& point, double tolerance = s_Tolerance) const
	{
		return (point.x() >= minX_ - tolerance
			&& point.x() <= maxX_ + tolerance
			&& point.y() >= minY_ - tolerance
			&& point.y() <= maxY_ + tolerance);
	}

	bool intersect(const QuadtreeRect& rect, double tolerance = s_Tolerance) const
	{
		if (!notEmpty() || !rect.notEmpty())
		{
			return false;
		}

		return (maxX_ >= rect.minX() - tolerance
			&& minX_ <= rect.maxX() + tolerance
			&& maxY_ >= rect.minY() - tolerance
			&& minY_ <= rect.maxY() + tolerance);
	}

	bool isIn(const QuadtreeRect& rect, double tolerance = s_Tolerance) const
	{
		if (!notEmpty())
			return false;

		return minX_ >= rect.minX() - tolerance
			&& maxX_ <= rect.maxX() + tolerance
			&& minY_ >= rect.minY() - tolerance
			&& maxY_ <= rect.maxY() + tolerance;
	}

private:
	double minX_;
	double minY_;
	double maxX_;
	double maxY_;
};
