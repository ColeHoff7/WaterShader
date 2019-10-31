#pragma once

#include "VisibleObjectOrderedRender.h"

class Mesh : public VisibleObjectOrderedRender
{
public:
	Mesh();
	~Mesh();
	virtual void initialize();
};

