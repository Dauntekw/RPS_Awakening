//---------------------------------------------------------
// file:	main.c
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
#include "gamestate_logo.h"
#include <stdlib.h>
#include <stdio.h>

// main() the starting point for the program
int main(void)
{
	CP_System_SetWindowSize(1280, 720);
	CP_System_SetFrameRate(60);
	CP_System_FullscreenAdvanced(1280,720);
	CP_Engine_SetNextGameState(logo_init, logo_update, logo_exit);
	CP_Engine_Run();
	return 0;
}