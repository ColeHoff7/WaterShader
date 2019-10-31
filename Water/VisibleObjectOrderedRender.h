#pragma once
#include "VisibleObject.h"
class VisibleObjectOrderedRender :
	public VisibleObject
{
public:
	VisibleObjectOrderedRender();
	~VisibleObjectOrderedRender();

	virtual void draw();
};

