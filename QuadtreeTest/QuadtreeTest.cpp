// QuadtreeTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Quadtree/IQuadtreeObject.h"
#include "Quadtree/Quadtree.h"
#include "Quadtree/QuadtreeRect.h"

#include <memory>
#include <random>
#include <vector>

class RectQuadtreeObject : public IQuadtreeObject
{
public:
	RectQuadtreeObject(const QuadtreeRect* rect) : rect_(rect){}

	QuadtreeRect rect() const override { return *rect_; }

	const QuadtreeRect* object() const { return rect_; }

private:
	const QuadtreeRect* rect_;
};

int main()
{
	const int n = 1000000;
	std::vector<std::unique_ptr<QuadtreeRect>> rects;
	rects.reserve(n);

	std::default_random_engine e;
	std::uniform_int_distribution<> dis(0, n);

	// 在0~n之间随机创建n个QuadtreeRect
	for (int i = 0; i < n; i++)
	{
		int minX = dis(e);
		int minY = dis(e);
		int maxX = dis(e);
		int maxY = dis(e);

		if (minX > maxX)
			std::swap(minX, maxX);

		if (minY > maxY)
			std::swap(minY, maxY);

		rects.emplace_back(new QuadtreeRect(minX, minY, maxX, maxY));
	}

	// 用n*n平面范围内随机一点查找包含该点的QuadtreeRect对象。
	QuadtreePoint pt(rand() % n, rand() % n);

	// 不使用八叉树
	std::vector<const QuadtreeRect*> results0;
	results0.reserve(rects.size());
	for (const auto& rect : rects)
	{
		if (rect->contain(pt))
		{
			results0.emplace_back(rect.get());
		}
	}

	// 使用四叉树
	std::unique_ptr<Quadtree> quadtree(Quadtree::create(QuadtreeRect(0, 0, n, n)));
	for (const auto& rect : rects)
	{
		std::unique_ptr<IQuadtreeObject> object(new RectQuadtreeObject(rect.get()));
		quadtree->insert(std::move(object));
	}

	auto objects = quadtree->findByPoint(pt);
	std::vector<const QuadtreeRect*> results1;
	results0.reserve(objects.size());
	for (auto object : objects)
	{
		const RectQuadtreeObject* rectObj = dynamic_cast<const RectQuadtreeObject*>(object);
		if(rectObj->object()->contain(pt))
			results1.emplace_back(rectObj->object());
	}
    return 0;
}

