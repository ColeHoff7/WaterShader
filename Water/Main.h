#pragma once

#include "BasicIncludesAndDefines.h"

#include "MyScene.h"




static void RenderSceneCB();


static void ResizeCB(int width, int height);


static void KeyboardCB(unsigned char key, int x, int y);

static void SpecialKeysCB(int key, int x, int y);

static void IdleCB();


static void mainMenuCB(int value);
