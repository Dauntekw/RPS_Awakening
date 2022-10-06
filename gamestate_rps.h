#pragma once
#include "collision.h"
#include "anim_controller.h"

#define ROCK 1
#define PAPER 2
#define SCISSORS 3
#define BLUFF_ROCK 4
#define BLUFF_PAPER 5
#define BLUFF_SCISSORS 6
#define PLAY 7
#define HOW_TO_PLAY 8
#define EXIT 9
#define RETURN_TO_MENU 10

void rps_exit(void);
void rps_init(void);
void rps_update(void);
void rps_reset(void);
enum Results { DRAW = 0, WIN, LOSE } ResultEnum;
enum CickStates { IDLE = 0, CLICKED } CursorStateEnum;
CP_Image cursorSheet;
CP_Image rpsSheet;
CP_Image cardSheet;
CP_Image bluffSheet;
CP_Image rpsLogo;
CP_Image awakeningSheet;
CP_Image rpsLogoSheet;
CP_Image menuBoxSheet;
CP_Image resetSheet;
CP_Sound onHover;
CP_Sound onClick;
CP_Sound unacceptable;
CP_Sound onExit;
struct Animation cursor_Idle;
struct Animation cursor_Click;
struct Animation rock_Idle;
struct Animation paper_Idle;
struct Animation scissors_Idle;
struct Screen
{
	float width;
	float height;
	struct Center
	{
		float horz;
		float vert;
	} center;
}screen;

void ResetAnimation(int anim);
int IsRectCollide(struct RectCollider _rc1, struct RectCollider _rc2);
void Bluff(int side, int bluff);
void ThrowHand(int side, int key);
void DrawHand(float xPos, float yPos, int hand);
void DrawCard(float xPos, float yPos, int card);
void DrawCursor(float xPos, float yPos, int state);
void DrawCollision(struct RectCollider _rc, float xPos, float yPos);
void GetCursorPosition();
void GetResults(void);
void ShowResults(int result);
void CursorClickManager(void);