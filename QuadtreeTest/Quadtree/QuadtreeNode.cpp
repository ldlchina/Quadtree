// author: ldlchina
#include "../stdafx.h"
#include "QuadtreeNode.h"

#include "IQuadtreeObject.h"
#include "Quadtree.h"


QuadtreeNode::QuadtreeNode(Quadtree* tree, const QuadtreeRect& rect, size_t level) :
	tree_(tree),
	rect_(rect),
	level_(level),
	totalObjectCount_(0)
{
}

QuadtreeNode::~QuadtreeNode()
{

}

QuadtreeNode* QuadtreeNode::getChildNode(const QuadtreeRect& rect) const
{
	if (!rect.notEmpty())
		return nullptr;

	if (!topright_)
		return nullptr;

	if (rect.isIn(topright_->rect()))
	{
		return const_cast<QuadtreeNode*>(topright_.get());
	}
	else if (rect.isIn(topleft_->rect()))
	{
		return const_cast<QuadtreeNode*>(topleft_.get());
	}
	else if (rect.isIn(bottomleft_->rect()))
	{
		return const_cast<QuadtreeNode*>(bottomleft_.get());
	}
	else if (rect.isIn(bottomright_->rect()))
	{
		return const_cast<QuadtreeNode*>(bottomright_.get());
	}

	return nullptr;
}

void QuadtreeNode::split()
{
	if (topright_)
		return;

	double minX = rect_.minX();
	double minY = rect_.minY();
	double maxX = rect_.maxX();
	double maxY = rect_.maxY();
	double centerX = (minX + maxX) / 2;
	double centerY = (minY + maxY) / 2;

	QuadtreeRect topright(centerX, centerY, maxX, maxY);
	QuadtreeRect topleft(minX, centerY, centerX, maxY);
	QuadtreeRect bottomleft(minX, minY, centerX, centerY);
	QuadtreeRect bottomright(centerX, minY, maxX, centerY);

	topright_.reset(new QuadtreeNode(tree_, topright, level_ + 1));
	topleft_.reset(new QuadtreeNode(tree_, topleft, level_ + 1));
	bottomleft_.reset(new QuadtreeNode(tree_, bottomleft, level_ + 1));
	bottomright_.reset(new QuadtreeNode(tree_, bottomright, level_ + 1));
}

void QuadtreeNode::insert(std::unique_ptr<IQuadtreeObject> object)
{
	if (!object)
		return;

	// 如果该节点下存在子节点
	QuadtreeNode* child = getChildNode(object->rect());
	if (child) {
		child->insert(std::move(object));
		totalObjectCount_++;
		return;
	}

	objects_.emplace_back(std::move(object));
	totalObjectCount_++;

	// 如果当前节点存储的数量超过了最大数
	if (!topright_ && objects_.size() > tree_->maxObjects() && level_ < tree_->maxLevels() - 1)
	{
		split();

		for (auto it = objects_.begin(); it != objects_.end();)
		{
			QuadtreeNode* pChildNode = getChildNode((*it)->rect());
			if (pChildNode)
			{
				pChildNode->insert(std::move(*it));
				it = objects_.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
}

std::vector<const IQuadtreeObject*> QuadtreeNode::findByPoint(const QuadtreePoint& point) const
{
	std::vector<const IQuadtreeObject*> res;

	if (!rect_.contain(point))
		return res;

	if (topright_)
	{
		if (topright_->rect().contain(point))
		{
			res = topright_->findByPoint(point);
		}
		else if (topleft_->rect().contain(point))
		{
			res = topleft_->findByPoint(point);
		}
		else if (bottomleft_->rect().contain(point))
		{
			res = bottomleft_->findByPoint(point);
		}
		else if (bottomright_->rect().contain(point))
		{
			res = bottomright_->findByPoint(point);
		}
	}

	for (const auto& object : objects_)
	{
		res.emplace_back(object.get());
	}

	return res;
}

std::vector<const IQuadtreeObject*> QuadtreeNode::findByRect(const QuadtreeRect& rect) const
{
	std::vector<const IQuadtreeObject*> res;

	if (!rect.notEmpty())
		return res;

	if (!rect_.intersect(rect))
		return res;

	if (topright_)
	{
		auto toprightObjs = topright_->findByRect(rect);
		res.insert(res.end(), toprightObjs.begin(), toprightObjs.end());

		auto topleftObjs = topleft_->findByRect(rect);
		res.insert(res.end(), topleftObjs.begin(), topleftObjs.end());

		auto bottomleftObjs = bottomleft_->findByRect(rect);
		res.insert(res.end(), bottomleftObjs.begin(), bottomleftObjs.end());

		auto bottomrightObjs = bottomright_->findByRect(rect);
		res.insert(res.end(), bottomrightObjs.begin(), bottomrightObjs.end());
	}

	for (const auto& object : objects_)
	{
		res.emplace_back(object.get());
	}

	return res;
}

size_t QuadtreeNode::getTotalObjectCount() const
{
	return totalObjectCount_;
}
