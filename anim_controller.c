#include "cprocessing.h"
#include "anim_controller.h"


struct Animation playAnimation(struct Animation anim, float xPos, float yPos)
{
  struct Animation newAnim = anim;

  int startCol = anim.startFrame;
  int currentRow = anim.currentRow;
  int currentCol = anim.currentFrame;
  int endCol = anim.endFrame;

  DrawSpriteFrame(anim.file, anim.frameWidth, anim.frameHeight, currentRow, currentCol, xPos, yPos, anim.opacity);
  
  if (anim.currentStep <= 0)
  {
    currentCol = (currentCol + 1 <= endCol) ? currentCol + 1 : (anim.loop) ? startCol : currentCol;
    newAnim.currentStep = newAnim.frameRate;
    newAnim.currentRow = currentRow;
    newAnim.currentFrame = currentCol;
  }
  else 
  {
    newAnim.currentStep = newAnim.currentStep - 1;
  }
  

  return newAnim;
}

void DrawSpriteFrame(CP_Image file, float width, float height, int row, 
  int col, float xPos, float yPos, int alpha) 
{
  CP_Image_DrawSubImage(file, xPos, yPos, width, height, 0 + (width * col), 
    0 + (height * row), width * (col + 1), height * (row + 1), alpha);
}