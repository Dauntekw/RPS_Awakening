#include "cprocessing.h"
#include "collision.h"
#include "gamestate_menu.h"
#include "gamestate_tutorial.h"
#include "gamestate_rps.h"

CP_Image bgTutorial;
void tutorial_init(void)
{
  CP_System_SetWindowTitle("RPS: Awakening");
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  CP_Settings_RectMode(CP_POSITION_CENTER);
  bgTutorial = CP_Image_Load("./Assets/rps_GradientBG2.png");
  cursor.state = IDLE;
  cursor.x = 0;
  cursor.y = 0;
  cursorSheet = CP_Image_Load("./Assets/CURSOR_Sheet_Animated.png");
  rpsSheet = CP_Image_Load("./Assets/RPS_Sheet_Animated.png");
  rock_Idle.file = rpsSheet;
  paper_Idle.file = rpsSheet;
  scissors_Idle.file = rpsSheet;
  ResetAnimation(2);
  ResetAnimation(3);
  ResetAnimation(4);
}

void tutorial_update(void)
{
  CP_Graphics_ClearBackground(CP_Color_Create(75, 125, 75, 255));
  GetCursorPosition();
  CP_Image_Draw(bgTutorial, screen.center.horz, screen.center.vert, (float)CP_Image_GetWidth(bgTutorial), (float)CP_Image_GetHeight(bgTutorial), 255);
  CP_Settings_TextSize(100);
  CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
  CP_Font_DrawText("HOW TO PLAY", screen.center.horz, (screen.center.vert - screen.center.vert / 2));
  CP_Settings_TextSize(36);
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
  CP_Font_DrawText("Rock VS Scissors: Rock Wins", 60, screen.center.vert - screen.center.vert / 4);
  DrawHand(screen.center.horz - 80, screen.center.vert - screen.center.vert / 4, ROCK);
  CP_Font_DrawText("Paper VS Rock: Paper Wins", 60, screen.center.vert + 12);
  DrawHand(screen.center.horz, screen.center.vert, PAPER);
  CP_Font_DrawText("Scissors VS Paper: Scissors Wins", 60, screen.center.vert + screen.center.vert / 3);
  DrawHand(screen.center.horz + 80, (screen.center.vert + screen.center.vert / 3) - 20, SCISSORS);
  CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
  DrawCard(screen.center.horz, screen.height - 120, RETURN_TO_MENU);
  CursorClickManager();
  DrawCursor(cursor.x, cursor.y, cursor.state);
}

void tutorial_exit(void)
{

}