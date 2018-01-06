#pragma once

#include "QuadtreeRect.h"

#include <list>
#include <memory>
#include <vector>


class IQuadtreeObject;
class Quadtree;
class QuadtreePoint;

class QuadtreeNode
{
public:
	QuadtreeNode(Quadtree* tree, const QuadtreeRect& rect, size_t level);
	~QuadtreeNode();

	const QuadtreeRect& rect() const { return rect_; }
	void insert(std::unique_ptr<IQuadtreeObject> object);
	std::vector<const IQuadtreeObject*> findByPoint(const QuadtreePoint& point) const;
	std::vector<const IQuadtreeObject*> findByRect(const QuadtreeRect& rect) const;
	size_t getTotalObjectCount() const;

private:
	QuadtreeNode* getChildNode(const QuadtreeRect& rect) const;
	void split();

	Quadtree* tree_;
	QuadtreeRect rect_;
	size_t level_;
	std::list<std::unique_ptr<IQuadtreeObject>> objects_;
	std::unique_ptr<QuadtreeNode> topright_;
	std::unique_ptr<QuadtreeNode> topleft_;
	std::unique_ptr<QuadtreeNode> bottomleft_;
	std::unique_ptr<QuadtreeNode> bottomright_;
	size_t totalObjectCount_;
};


