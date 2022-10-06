#pragma once
#include "cprocessing.h"

struct Animation
{
  CP_Image file;
  float frameWidth;
  float frameHeight;
  int startRow;
  int startFrame;
  int currentRow;
  int currentFrame;
  int endRow;
  int endFrame;
  int frameRate;
  int currentStep;
  int loop;
  int opacity;
};

struct Animation playAnimation(struct Animation anim, float xPos, float yPos);
void DrawSpriteFrame(CP_Image file, float width, float height, int row,
  int col, float xPos, float yPos, int alpha);
