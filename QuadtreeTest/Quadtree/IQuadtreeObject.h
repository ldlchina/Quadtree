// author: ldlchina
#pragma once

#include "QuadtreeRect.h"


// 四叉树节点所存放的对象接口，用于派生。
class IQuadtreeObject
{
public:
	virtual ~IQuadtreeObject() {};

	virtual QuadtreeRect rect() const = 0;
};


