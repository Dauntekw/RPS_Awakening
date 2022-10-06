#include "cprocessing.h"
#include "gamestate_menu.h"
#include "anim_controller.h"
#include "collision.h"


float yPos;
float yPos2;
void menu_init(void)
{
  CP_System_SetWindowTitle("RPS: Awakening");
  bg = CP_Image_Load("./Assets/rps_GradientBG1.png");
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  CP_Settings_RectMode(CP_POSITION_CENTER);
  yPos = 360;
  yPos2 = 1200;
  cursor.state = IDLE;
  cursor.x = 0;
  cursor.y = 0;
  rpsSheet = CP_Image_Load("./Assets/RPS_Sheet_Animated.png");
  menuBoxSheet = CP_Image_Load("./Assets/MenuBox_Sheet.png");
  rpsLogo = CP_Image_Load("./Assets/Logo_Full.png");
  cardSheet = CP_Image_Load("./Assets/RPS_Cards_Sheet.png");
  bluffSheet = CP_Image_Load("./Assets/RPS_Bluff_Sheet.png");
  resetSheet = CP_Image_Load("./Assets/RPS_Retry_Sheet.png");
  cursorSheet = CP_Image_Load("./Assets/CURSOR_Sheet_Animated.png");
  cursor_Idle.file = cursorSheet;
  cursor_Click.file = cursorSheet;
  rock_Idle.file = rpsSheet;
  paper_Idle.file = rpsSheet;
  scissors_Idle.file = rpsSheet;
  onHover = CP_Sound_Load("./Assets/rpsMenu_OnHover.wav");
  onClick = CP_Sound_Load("./Assets/rpsMenu_Accept.wav");
  unacceptable = CP_Sound_Load("./Assets/rpsMenu_Cancel.wav");
  ResetAnimation(IDLE);
  ResetAnimation(CLICKED);
  ResetAnimation(2);
  ResetAnimation(3);
  ResetAnimation(4);
  onHover = CP_Sound_Load("./Assets/rpsMenu_OnHover.wav");
}

void menu_update(void)
{
  yPos = (yPos < -360) ? 1320: yPos - 1;
  yPos2 = (yPos2 < -360) ? 1320: yPos2 - 1;
  CP_Graphics_ClearBackground(CP_Color_Create(75, 75, 125, 255));
  CP_Settings_TextSize(20);
  CP_Settings_Fill(CP_Color_Create(255, 255, 255, 200));
  CP_Image_Draw(bg, screen.center.horz, screen.center.vert, (float)CP_Image_GetWidth(bg), (float)CP_Image_GetHeight(bg), 255);
  CP_Image_Draw(scroll, screen.center.horz, yPos, (float)CP_Image_GetWidth(scroll), (float)CP_Image_GetHeight(scroll), 255);
  CP_Image_Draw(scroll, screen.center.horz, yPos2, (float)CP_Image_GetWidth(scroll), (float)CP_Image_GetHeight(scroll), 255);
  CP_Font_DrawText("Developed by Daunte Williamson", screen.center.horz, screen.height - 70);
  CP_Font_DrawText("GAM100 - Professor Andy Ellinger", screen.center.horz, screen.height - 50);
  CP_Font_DrawText("Copyright 2022 DigiPen, All rights reserved.", screen.center.horz, screen.height - 30);
  CP_Settings_TextSize(100);
  CP_Image_Draw(rpsLogo, screen.center.horz, (screen.center.vert - screen.center.vert / 2) - 20, (float)CP_Image_GetWidth(rpsLogo), (float)CP_Image_GetHeight(rpsLogo), 255);
  CP_Settings_TextSize(36);
  GetCursorPosition();
  DrawCard(screen.center.horz, screen.center.vert - 20, PLAY);
  DrawCard(screen.center.horz, (screen.center.vert + screen.center.vert / 4) + 10, HOW_TO_PLAY);
  DrawCard(screen.center.horz, (screen.center.vert + screen.center.vert / 2) + 40, EXIT);
  DrawCursor(cursor.x, cursor.y, cursor.state);
  CursorClickManager();
}

void menu_exit(void)
{

}