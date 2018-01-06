 #pragma once

#include <memory>
#include <vector>

class IQuadtreeObject;
class QuadtreeNode;
class QuadtreePoint;
class QuadtreeRect;

// Quadtree类用于生成四叉树，插入新节点，查找节点。
class Quadtree
{
	Quadtree(const QuadtreeRect& rootRect, size_t maxObjects = 128, size_t maxLevels = 5);
public:
	~Quadtree();

	static Quadtree* create(const QuadtreeRect& rootRect, size_t maxObjects = 128, size_t maxLevels = 5);

	void insert(std::unique_ptr<IQuadtreeObject> object);
	std::vector<const IQuadtreeObject*> findByPoint(const QuadtreePoint& point) const;
	std::vector<const IQuadtreeObject*> findByRect(const QuadtreeRect& rect) const;
	size_t getTotalObjectCount() const;

	size_t maxObjects() const { return maxObjects_; }
	size_t maxLevels() const { return maxLevels_; }

private:
	size_t maxObjects_; //每个节点可放置的最大对象数。
	size_t maxLevels_; //四叉树最大层次。
	std::unique_ptr<QuadtreeNode> root_; //四叉树根节点。
};


