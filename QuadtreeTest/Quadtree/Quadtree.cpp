// author: ldlchina
#include "../stdafx.h"
#include "Quadtree.h"

#include "IQuadtreeObject.h"
#include "QuadtreeNode.h"


Quadtree::Quadtree( const QuadtreeRect& rootRect, size_t maxObjects /*= 128*/, size_t maxLevels /*= 5*/ ) :
    maxObjects_(maxObjects),
    maxLevels_(maxLevels)
{
    root_.reset(new QuadtreeNode(this, rootRect, 0));
}

Quadtree::~Quadtree()
{

}

Quadtree* Quadtree::create( const QuadtreeRect& rootRect, size_t maxObjects /*= 128*/, size_t maxLevels /*= 5*/ )
{
	if (!rootRect.notEmpty())
		return nullptr;

    return new Quadtree(rootRect, maxObjects, maxLevels);
}

void Quadtree::insert(std::unique_ptr<IQuadtreeObject> object )
{
    root_->insert(std::move(object));
}

std::vector<const IQuadtreeObject*> Quadtree::findByPoint(const QuadtreePoint& point) const
{
    return root_->findByPoint(point);
}

std::vector<const IQuadtreeObject*> Quadtree::findByRect( const QuadtreeRect& rect ) const
{
    return root_->findByRect(rect);
}

size_t Quadtree::getTotalObjectCount() const
{
    return root_->getTotalObjectCount();
}
