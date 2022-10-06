//---------------------------------------------------------
// file:	gamestate_logo.c
// author:	Daunte Williamson
// email:	daunte.williamson@digipen.edu
//
// brief:	Main entry point for the sample project
//			of the CProcessing library
//
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include "gamestate_logo.h"
#include "gamestate_menu.h"

CP_Image logo;
float dot_list[10][3];
int dot_size = sizeof(dot_list);
int dot_col_size = sizeof(dot_list[0]);
int dot_row_size = sizeof(dot_list) / sizeof(dot_list[0]);
float dot_alpha = 0.f;


void dot_list_fill(int row, int column) 
{
  for (int i = 0; i < row; i++) 
  {
    for (int j = 0; j < column; j++) 
    {
      switch (j) 
      {
      case 0:
        dot_list[i][j] = (screen.center.horz + CP_Random_RangeFloat(-300, 300));
        if (i > 0) 
        {
          float currentX = dot_list[i][j];
          float previousX = dot_list[i - 1][j];
          if (currentX > previousX - 300.0f && currentX < previousX) 
          {
            dot_list[i][j] -= 300.f;
          }
          else if (currentX < previousX + 300.f && currentX > previousX)
          {
            dot_list[i][j] += 300.f;
          }
        }
        break;
      case 1:
        dot_list[i][j] = (screen.center.vert + CP_Random_RangeFloat(-100, 100));
        break;
      default:
        dot_list[i][j] = 0;
        break;
      }
    }
  }
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void logo_init(void)
{
  CP_System_ShowCursor(FALSE);
  CP_System_SetWindowTitle("RPS: Awakening");
  CP_Settings_RectMode(CP_POSITION_CENTER);
  CP_Settings_ImageMode(CP_POSITION_CENTER);
  // initialize variables and CProcessing settings for this gamestate
  logo = CP_Image_Load("./Assets/DigiPen_BLACK.png");
  scroll = CP_Image_Load("./Assets/rps_ScrollBG.png");
  cursorSheet = CP_Image_Load("./Assets/CURSOR_Sheet_Animated.png");
  screen.width = (float)CP_System_GetWindowWidth();
  screen.height = (float)CP_System_GetWindowHeight();
  screen.center.horz = screen.width / 2;
  screen.center.vert = screen.height / 2;
  dot_list_fill(dot_row_size, dot_col_size);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void logo_update(void)
{
  int frame_count = CP_System_GetFrameCount();
  float seconds = CP_System_GetSeconds();
  float image_width = (float)CP_Image_GetWidth(logo);
  float image_height = (float)CP_Image_GetHeight(logo);
  /*screen.width = (float)CP_System_GetWindowWidth();
  screen.height = (float)CP_System_GetWindowHeight();
  screen.center.horz = screen.width / 2;
  screen.center.vert = screen.height / 2;*/
  CP_Color black = CP_Color_Create(125, 75, 75, 255);

  CP_Graphics_ClearBackground(black);
  CP_Image_Draw(logo, screen.center.horz, screen.center.vert, image_width / 2, image_height / 2, CP_Math_ClampInt(frame_count * 2, 0, 255));

  if (seconds > 3) 
  {
    if (dot_alpha < 255) 
    {
      dot_alpha += 2;
    }
    for (int i = 0; i < dot_row_size; i++) 
    {
      float r_scale = CP_Math_LerpFloat(dot_alpha, CP_Math_ClampFloat(dot_alpha * 3 + CP_Random_RangeInt(1, 4), 0, 3000), 10.0f);
      int r_alpha = CP_Math_ClampInt((int)dot_list[i][2] + (CP_Random_RangeInt(0, 5) + (int)dot_alpha), 0, 255);
      CP_Color dot_color = CP_Color_Create(125, 75, 75, r_alpha);

      CP_Settings_NoStroke();
      CP_Settings_Fill(dot_color);
      CP_Graphics_DrawCircle(dot_list[i][0], dot_list[i][1], r_scale);
    }
  }

  if (seconds > 5) {
    CP_Engine_SetNextGameState(menu_init, menu_update, menu_exit);
  }
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void logo_exit(void)
{
  // shut down the gamestate and cleanup any dynamic memory
}