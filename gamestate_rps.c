//---------------------------------------------------------
// file:	gamestate_rps.c
// author:	Daunte Williamson
// email:	daunte.williamson@digipen.edu
//
// brief:	RPS: Awakening main file.
// Project for GAM100 Coding Assignment 2.
// Rock Paper Scissors game with bluff mechanic.
// 
// documentation link:
// https://github.com/DigiPen-Faculty/CProcessing/wiki
//
// Copyright © 2020 DigiPen, All rights reserved.
//---------------------------------------------------------

#include "cprocessing.h"
#include <stdlib.h>
#include <stdio.h>
#include "gamestate_rps.h"
#include "collision.h"
#include "gamestate_menu.h"
#include "gamestate_tutorial.h"
#include "anim_controller.h"

int cpuBluff;
int cpuHand;
int playerBluff;
int playerHand;
int results;
int playerWins;
int cpuWins;
int draws;
int roundOver;
int bluffCooldown;
int currentBluffSFX;
char textBuffer[50] = { 0 };

CP_Image bgRPS;
CP_Sound bluff1;
CP_Sound bluff2;
CP_Sound bluff3;
CP_Sound bluff4;

void ResetAnimation(int anim)
{
	switch (anim)
	{
		case IDLE:
			cursor_Idle.startRow = 0;
			cursor_Idle.startFrame = 0;
			cursor_Idle.currentRow = 0;
			cursor_Idle.currentFrame = 0;
			cursor_Idle.endRow = 0;
			cursor_Idle.endFrame = 2;
			cursor_Idle.frameWidth = 96;
			cursor_Idle.frameHeight = 96;
			cursor_Idle.frameRate = 9;
			cursor_Idle.currentStep = cursor_Idle.frameRate;
			cursor_Idle.loop = 1;
			cursor_Idle.opacity = 255;
			break;
		case CLICKED:
			cursor_Click.startRow = 1;
			cursor_Click.startFrame = 0;
			cursor_Click.currentRow = 1;
			cursor_Click.currentFrame = 0;
			cursor_Click.endRow = 1;
			cursor_Click.endFrame = 1;
			cursor_Click.frameWidth = 96;
			cursor_Click.frameHeight = 96;
			cursor_Click.frameRate = 6;
			cursor_Click.currentStep = cursor_Click.frameRate;
			cursor_Click.loop = 1;
			cursor_Click.opacity = 255;
			break;
		case 2:
			rock_Idle.startRow = 1;
			rock_Idle.startFrame = 0;
			rock_Idle.currentRow = 1;
			rock_Idle.currentFrame = 0;
			rock_Idle.endRow = 1;
			rock_Idle.endFrame = 2;
			rock_Idle.frameWidth = 192;
			rock_Idle.frameHeight = 192;
			rock_Idle.frameRate = 9;
			rock_Idle.currentStep = rock_Idle.frameRate;
			rock_Idle.loop = 1;
			rock_Idle.opacity = 255;
			break;
		case 3:
			paper_Idle.startRow = 2;
			paper_Idle.startFrame = 0;
			paper_Idle.currentRow = 2;
			paper_Idle.currentFrame = 0;
			paper_Idle.endRow = 2;
			paper_Idle.endFrame = 2;
			paper_Idle.frameWidth = 192;
			paper_Idle.frameHeight = 192;
			paper_Idle.frameRate = 9;
			paper_Idle.currentStep = paper_Idle.frameRate;
			paper_Idle.loop = 1;
			paper_Idle.opacity = 255;
			break;
		case 4:
			scissors_Idle.startRow = 0;
			scissors_Idle.startFrame = 0;
			scissors_Idle.currentRow = 0;
			scissors_Idle.currentFrame = 0;
			scissors_Idle.endRow = 0;
			scissors_Idle.endFrame = 2;
			scissors_Idle.frameWidth = 192;
			scissors_Idle.frameHeight = 192;
			scissors_Idle.frameRate = 9;
			scissors_Idle.currentStep = scissors_Idle.frameRate;
			scissors_Idle.loop = 1;
			scissors_Idle.opacity = 255;
			break;
	}
}

// Determine what bluff a specified player is using this round
void Bluff(int side, int bluff) {
	int bluffRand = CP_Random_RangeInt(1, 3);
	switch (side) 
	{
		case 1:
			if (bluffRand == 1) 
			{
				cpuBluff = cpuHand;
			}
			else if (bluffRand == 2)
			{
				cpuBluff = (cpuHand > ROCK) ? cpuHand - 1 : SCISSORS;
			}
			else
			{
				cpuBluff = (cpuHand < SCISSORS) ? cpuHand + 1 : ROCK;
			}
			break;
		default:
			bluffCooldown = 2;
			playerBluff = bluff;
			if (bluffRand < 3) {
				cpuHand = (cpuHand < SCISSORS) ? cpuHand + 1 : ROCK;
			}
			break;
	}
	switch (currentBluffSFX)
	{
		case 0:
			CP_Sound_Play(bluff1);
			break;
		case 1:
			CP_Sound_Play(bluff2);
			break;
		case 2:
			CP_Sound_Play(bluff3);
			break;
		case 3:
			CP_Sound_Play(bluff4);
			break;
	}
	currentBluffSFX = CP_Random_RangeInt(0, 3);
}

// Determine what hand a specified player is throwing this round
void ThrowHand(int side, int key) 
{
	switch (side) 
	{
		case 1:
			//CPU
			cpuHand = CP_Random_RangeInt(1, 3);
			break;
		default:
			if (key == KEY_1) 
			{
				//rock
				playerHand = ROCK;
			}
			else if (key == KEY_2) 
			{
				//paper
				playerHand = PAPER;
			}
			else 
			{
				//scissors
				playerHand = SCISSORS;
			}
			break;
	}
}

// Draw specified hand on screen at desired coordinates
void DrawHand(float xPos, float yPos, int hand) 
{
	switch (hand) 
	{
		case ROCK:
			rock_Idle = playAnimation(rock_Idle, xPos, yPos); //rock
			break;
		case PAPER:
			paper_Idle = playAnimation(paper_Idle, xPos, yPos); //paper
			break;
		case SCISSORS:
			scissors_Idle = playAnimation(scissors_Idle, xPos, yPos); //scissors
			break;
	}
}

// Draw specified card on screen at desired coordinates
void DrawCard(float xPos, float yPos, int card) 
{
	struct RectCollider rc;
	int row = 0;
	int col = 0;
	rc.xMin = xPos - (175 / 2);
	rc.xMax = xPos + (175 / 2);
	rc.yMin = yPos - 150;
	rc.yMax = yPos + 150;
	int trueState = CP_Input_MouseClicked();
	int collision;
	CP_Settings_NoStroke();
	switch (card) 
	{
		case ROCK:
			col = 0;
			rc.ID = 1;
			collision = IsRectCollide(cursor.RC, rc);
			if (collision == 1) 
			{
				row = 1;
				if (trueState) 
				{
					CP_Sound_Play(onClick);
					playerHand = ROCK;
				}
			}
			else 
			{
				row = 0;
			}
			DrawSpriteFrame(cardSheet, 175.0f, 300.0f, row, col, xPos, yPos, 255);
			break;
		case PAPER:
			col = 1;
			rc.ID = 2;
			collision = IsRectCollide(cursor.RC, rc);
			if (collision == 1)
			{
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(onClick);
					playerHand = PAPER;
				}
			}
			else
			{
				row = 0;
			}
			DrawSpriteFrame(cardSheet, 175.0f, 300.0f, row, col, xPos, yPos, 255);
			break;
		case SCISSORS:
			col = 2;
			rc.ID = 3;
			collision = IsRectCollide(cursor.RC, rc);
			if (collision == 1)
			{
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(onClick);
					playerHand = SCISSORS;
				}
			}
			else
			{
				row = 0;
			}
			DrawSpriteFrame(cardSheet, 175.0f, 300.0f, row, col, xPos, yPos, 255);
			break;
		case BLUFF_ROCK:
			rc.xMin = xPos - (175 / 2);
			rc.xMax = xPos + (175 / 2);
			rc.yMin = yPos - 25;
			rc.yMax = yPos + 25;
			rc.ID = 4;
			collision = IsRectCollide(cursor.RC, rc);
			col = 0;
			if (collision == 1)
			{
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(onClick);
					Bluff(2, ROCK);
				}
			}
			else
			{
				row = 0;
			}
			DrawSpriteFrame(bluffSheet, 175.0f, 50.0f, row, col, xPos, yPos, 255);
			break;
		case BLUFF_PAPER:
			rc.xMin = xPos - (175 / 2);
			rc.xMax = xPos + (175 / 2);
			rc.yMin = yPos - 25;
			rc.yMax = yPos + 25;
			rc.ID = 5;
			collision = IsRectCollide(cursor.RC, rc);
			col = 1;
			if (collision == 1)
			{
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(onClick);
					Bluff(2, PAPER);
				}
			}
			else
			{
				row = 0;
			}
			DrawSpriteFrame(bluffSheet, 175.0f, 50.0f, row, col, xPos, yPos, 255);
			break;
		case BLUFF_SCISSORS:
			rc.xMin = xPos - (175 / 2);
			rc.xMax = xPos + (175 / 2);
			rc.yMin = yPos - 25;
			rc.yMax = yPos + 25;
			rc.ID = 6;
			collision = IsRectCollide(cursor.RC, rc);
			col = 2;
			if (collision == 1)
			{
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(onClick);
					Bluff(2, SCISSORS);
				}
			}
			else
			{
				row = 0;
			}
			DrawSpriteFrame(bluffSheet, 175.0f, 50.0f, row, col, xPos, yPos, 255);
			break;
		case PLAY:
			rc.xMin = xPos - 150;
			rc.xMax = xPos + 150;
			rc.yMin = yPos - 50;
			rc.yMax = yPos + 50;
			rc.ID = 7;
			collision = IsRectCollide(cursor.RC, rc);
			col = 0;
			if (collision == 1)
			{
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(onClick);
					CP_Engine_SetNextGameState(rps_reset, rps_update, rps_exit);
				}
			}
			else
			{
				row = 0;
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
			}
			CP_Settings_TextSize(36);
			DrawSpriteFrame(menuBoxSheet, 300.0f, 100.0f, row, col, xPos, yPos, 255);
			CP_Font_DrawText("PLAY", xPos, yPos);
			break;
		case HOW_TO_PLAY:
			rc.xMin = xPos - 150;
			rc.xMax = xPos + 150;
			rc.yMin = yPos - 50;
			rc.yMax = yPos + 50;
			rc.ID = 8;
			collision = IsRectCollide(cursor.RC, rc);
			col = 0;
			if (collision == 1)
			{
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(onClick);
					CP_Engine_SetNextGameState(tutorial_init, tutorial_update, tutorial_exit);
				}
			}
			else
			{
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
				row = 0;
			}
			CP_Settings_TextSize(36);
			DrawSpriteFrame(menuBoxSheet, 300.0f, 100.0f, row, col, xPos, yPos, 255);
			CP_Font_DrawText("RULES", xPos, yPos);
			break;
		case RETURN_TO_MENU:
			rc.xMin = xPos - 150;
			rc.xMax = xPos + 150;
			rc.yMin = yPos - 50;
			rc.yMax = yPos + 50;
			rc.ID = 9;
			collision = IsRectCollide(cursor.RC, rc);
			col = 0;
			if (collision == 1)
			{
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(unacceptable);
					CP_Engine_SetNextGameState(menu_init, menu_update, menu_exit);
				}
			}
			else
			{
				row = 0;
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
			}
			CP_Settings_TextSize(36);
			DrawSpriteFrame(menuBoxSheet, 300.0f, 100.0f, row, col, xPos, yPos, 255);
			CP_Font_DrawText("MAIN MENU", xPos, yPos);
			break;
		case EXIT:
			rc.xMin = xPos - 150;
			rc.xMax = xPos + 150;
			rc.yMin = yPos - 50;
			rc.yMax = yPos + 50;
			rc.ID = 10;
			collision = IsRectCollide(cursor.RC, rc);
			col = 0;
			if (collision == 1)
			{
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
				row = 1;
				if (trueState)
				{
					CP_Sound_Play(onClick);
					CP_Engine_Terminate();
				}
			}
			else
			{
				row = 0;
				CP_Settings_Fill(CP_Color_Create(255, 255, 255, 255));
			}
			CP_Settings_TextSize(36);
			DrawSpriteFrame(menuBoxSheet, 300.0f, 100.0f, row, col, xPos, yPos, 255);
			CP_Font_DrawText("EXIT GAME", xPos, yPos);
			break;
		default:
			rc.xMin = xPos - 150;
			rc.xMax = xPos + 150;
			rc.yMin = yPos - 50;
			rc.yMax = yPos + 50;
			rc.ID = 11;
			collision = IsRectCollide(cursor.RC, rc);
			col = 0;
			if (collision == 1)
			{
				row = 1;
				if (trueState)
				{
					playerHand = -1;
					CP_Sound_Play(onClick);
					CP_Engine_SetNextGameStateForced(rps_init, rps_update, rps_exit);
				}
			}
			else
			{
				row = 0;
			}
			CP_Graphics_DrawRect(xPos, yPos, 300, 100);
			DrawSpriteFrame(resetSheet, 300.0f, 100.0f, row, col, xPos, yPos, 255);
			break;
	}
}

// Draw Cursor sprite to screen
void DrawCursor(float xPos, float yPos, int state) 
{
	switch (state) 
	{
	case IDLE:
		//CP_Image_DrawSubImage(cursorSheet, xPos, yPos, 96, 96, 0, 0, 96, 96, 255); //Idle
		cursor_Idle = playAnimation(cursor_Idle, xPos, yPos);
		
		break;
	case CLICKED:
		cursor_Click = playAnimation(cursor_Click, xPos, yPos); //Clicked
		break;
	}
}

// Draw visual representation of RectColliders on screen
void DrawCollision(struct RectCollider _rc, float xPos, float yPos) 
{
	CP_Settings_NoStroke();
	CP_Graphics_DrawRect(xPos, yPos, (_rc.xMax - _rc.xMin), (_rc.yMax - _rc.yMin));
}

// Track cursor position and store results in Cursor struct
void GetCursorPosition() 
{
	cursor.x = CP_Input_GetMouseX();
	cursor.y = CP_Input_GetMouseY();
	cursor.RC.xMin = cursor.x - 2;
	cursor.RC.xMax = cursor.x + 2;
	cursor.RC.yMin = cursor.y - 2;
	cursor.RC.yMax = cursor.y + 2;
}

// Determine results of round
void GetResults(void) 
{
	if (playerHand == cpuHand) 
	{
		//draw
		results = DRAW;
		draws += (roundOver) ? 0 : 1;
	}
	else if ((playerHand == ROCK && cpuHand == PAPER) || (playerHand == PAPER && cpuHand == SCISSORS) || 
		(playerHand == SCISSORS && cpuHand == ROCK)) 
	{
		//player lose
		results = LOSE;
		cpuWins += (roundOver) ? 0 : 1;
	}
	else 
	{
		//player win
		results = WIN;
		playerWins += (roundOver) ? 0 : 1;
	}
	roundOver = TRUE;
}

// Display results of round
void ShowResults(int result) 
{
	CP_Settings_TextSize(100);
	switch (result) 
	{
		case DRAW:
			CP_Font_DrawText("DRAW", screen.center.horz, screen.center.vert);
			break;
		case WIN:
			CP_Font_DrawText("YOU WIN", screen.center.horz, screen.center.vert);
			break;
		case LOSE:
			CP_Font_DrawText("CPU WIN", screen.center.horz, screen.center.vert);
			break;
	}
}

// Force reset using game_reset as init function
void TrueReset(void) 
{
	CP_Engine_SetNextGameStateForced(rps_reset, rps_update, rps_exit);
}

// use CP_Engine_SetNextGameState to specify this function as the initialization function
// this function will be called once at the beginning of the program
void rps_init(void)
{
	// initialize variables and CProcessing settings for this gamestate
	CP_System_SetWindowTitle("RPS: Awakening");
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_System_ShowCursor(FALSE);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	bgRPS = CP_Image_Load("./Assets/rps_GradientBG3.png");
	screen.width = (float)CP_System_GetWindowWidth();
	screen.height = (float)CP_System_GetWindowHeight();
	screen.center.horz = screen.width / 2;
	screen.center.vert = screen.height / 2;
	roundOver = FALSE;
	results = -1;
	cursor.state = IDLE;
	cursor.x = 0;
	cursor.y = 0;
	playerHand = -1;
	playerBluff = -1;
	rpsSheet = CP_Image_Load("./Assets/RPS_Sheet_Animated.png");
	cursorSheet = CP_Image_Load("./Assets/CURSOR_Sheet_Animated.png");
	currentBluffSFX = CP_Random_RangeInt(0, 3);
	cursor_Idle.file = cursorSheet;
	cursor_Click.file = cursorSheet;
	rock_Idle.file = rpsSheet;
	paper_Idle.file = rpsSheet;
	scissors_Idle.file = rpsSheet;
	ResetAnimation(IDLE);
	ResetAnimation(CLICKED);
	ResetAnimation(2);
	ResetAnimation(3);
	ResetAnimation(4);
	int cpuBluffRand = CP_Random_RangeInt(1, 2);
	if (cpuBluffRand == 1) 
	{
		Bluff(1, 0);
	}
	else 
	{
		cpuBluff = -1;
	}
	cpuHand = (int)CP_Random_RangeFloat(1, 3);
	ThrowHand(1, -1);
}

// Copy of game_init() that also resets score keeping variables
void rps_reset(void)
{
	CP_Settings_RectMode(CP_POSITION_CENTER);
	CP_System_ShowCursor(FALSE);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	screen.width = (float)CP_System_GetWindowWidth();
	screen.height = (float)CP_System_GetWindowHeight();
	screen.center.horz = screen.width / 2;
	screen.center.vert = screen.height / 2;
	roundOver = FALSE;
	results = -1;
	playerWins = 0;
	cpuWins = 0;
	draws = 0;
	cursor.state = IDLE;
	cursor.x = 0;
	cursor.y = 0;
	playerHand = -1;
	playerBluff = -1;
	bluffCooldown = 0;
	bluff1 = CP_Sound_Load("./Assets/rps_Bluff1.wav");
	bluff2 = CP_Sound_Load("./Assets/rps_Bluff2.wav");
	bluff3 = CP_Sound_Load("./Assets/rps_Bluff3.wav");
	bluff4 = CP_Sound_Load("./Assets/rps_Bluff4.wav");
	rpsSheet = CP_Image_Load("./Assets/RPS_Sheet.png");
	cursorSheet = CP_Image_Load("./Assets/CURSOR_Sheet.png");
	bgRPS = CP_Image_Load("./Assets/rps_GradientBG3.png");
	int cpuBluffRand = CP_Random_RangeInt(1, 2);
	if (cpuBluffRand == 1)
	{
		Bluff(1, 0);
	}
	else
	{
		cpuBluff = -1;
	}
	cpuHand = (int)CP_Random_RangeFloat(1, 3);
	ThrowHand(1, -1);
}

// use CP_Engine_SetNextGameState to specify this function as the update function
// this function will be called repeatedly every frame
void rps_update(void)
{
	// check input, update simulation, render etc.
	CP_Graphics_ClearBackground(CP_Color_Create(125, 125, 125, 255));
	CP_Image_Draw(bgRPS, screen.center.horz, screen.center.vert, (float)CP_Image_GetWidth(bgRPS), (float)CP_Image_GetHeight(bgRPS), 255);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
	CP_Settings_TextSize(36);
	sprintf_s(textBuffer, sizeof(textBuffer), "Player Wins: %d", playerWins);
	CP_Font_DrawText(textBuffer, 120, 60);
	sprintf_s(textBuffer, sizeof(textBuffer), "CPU Wins: %d", cpuWins);
	CP_Font_DrawText(textBuffer, screen.width - 120, 60);
	sprintf_s(textBuffer, sizeof(textBuffer), "Draws: %d", draws);
	CP_Font_DrawText(textBuffer, screen.center.horz, 60);
	CP_Font_DrawText("Press [R] to Reset", screen.center.horz, screen.height - 40);
	CP_Font_DrawText("Press [ESC] for Main Menu", screen.center.horz, screen.height - 80);


	GetCursorPosition();

	if (playerHand != -1)
	{
		DrawHand(screen.center.horz / 2, screen.center.vert, playerHand);
		DrawHand(screen.center.horz + (screen.center.horz / 2), screen.center.vert, cpuHand);
		GetResults();
		ShowResults(results);
		DrawCard(screen.center.horz, screen.center.vert + (screen.center.vert / 2), -1);
	}
	else
	{
		if (cpuBluff != -1) 
		{
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_RIGHT, CP_TEXT_ALIGN_V_MIDDLE);
			CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
			switch (cpuBluff) 
			{
			case ROCK:
				CP_Font_DrawText("I'm going ROCK", screen.width - 24, 120);
				break;
			case PAPER:
				CP_Font_DrawText("I'm going PAPER", screen.width - 24, 120);
				break;
			case SCISSORS:
				CP_Font_DrawText("I'm going SCISSORS", screen.width - 24, 120);
				break;
			}
			CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
		}
		if (playerBluff == -1 && (bluffCooldown == 0 || &bluffCooldown == NULL)) 
		{
			DrawCard(screen.width / 3, screen.center.vert, ROCK);
			DrawCard(screen.width / 3, screen.center.vert + 200, BLUFF_ROCK);
			DrawCard(screen.center.horz, screen.center.vert, PAPER);
			DrawCard(screen.center.horz, screen.center.vert + 200, BLUFF_PAPER);
			DrawCard(screen.width - (screen.width / 3), screen.center.vert, SCISSORS);
			DrawCard(screen.width - (screen.width / 3), screen.center.vert + 200, BLUFF_SCISSORS);
		}
		else
		{
			DrawCard(screen.width / 3, screen.center.vert, ROCK);
			DrawCard(screen.center.horz, screen.center.vert, PAPER);
			DrawCard(screen.width - (screen.width / 3), screen.center.vert, SCISSORS);
			if (playerBluff != -1) 
			{
				CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_MIDDLE);
				CP_Settings_Fill(CP_Color_Create(0, 0, 0, 255));
				switch (playerBluff) 
				{
					case ROCK:
						CP_Font_DrawText("I'm going ROCK", 24, 120);
						break;
					case PAPER:
						CP_Font_DrawText("I'm going PAPER", 24, 120);
						break;
					case SCISSORS:
						CP_Font_DrawText("I'm going SCISSORS", 24, 120);
						break;
				}
				CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
			}
		}
	}

	/*CP_Settings_Fill(CP_Color_Create(0, 255, 0, 125));
	DrawCollision(cursor.RC, (float)cursor.x, (float)cursor.y);*/
	CursorClickManager();
	
	DrawCursor(cursor.x, cursor.y, cursor.state);
	if (CP_Input_KeyTriggered(KEY_R)) 
	{
		CP_Sound_Play(unacceptable);
		TrueReset();
	}
	if (CP_Input_KeyTriggered(KEY_ESCAPE))
	{
		CP_Sound_Play(unacceptable);
		CP_Engine_SetNextGameState(menu_init, menu_update, menu_exit);
	}
}

void CursorClickManager(void)
{
	if (CP_Input_MouseDown(MOUSE_BUTTON_LEFT))
	{
		cursor.state = CLICKED;
	}
	else
	{
		cursor.state = IDLE;
	}
}

// use CP_Engine_SetNextGameState to specify this function as the exit function
// this function will be called once just before leaving the current gamestate
void rps_exit(void)
{
	bluffCooldown = (bluffCooldown > 0) ? bluffCooldown - 1 : 0;
}