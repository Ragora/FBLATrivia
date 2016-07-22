/*
*   stdafx.h
*   Contains some globally used enums in the game code; yes it's not necessarily the actual
*   thing but it they have the same premise.
*   Copyright (c) 2012 Robert MacGregor
*/

#ifndef _INCLUDE_STDAFX_H_
#define _INCLUDE_STDAFX_H_

#include <CLogManager.h>
#include <CGamePreferences.h>
#include <irrlicht/irrlicht.h>
#include <irrklang/irrKlang.h>
#include <CQuestionLoader.h>
#include <CGameTime.h>
#include <CLogManager.h>
#include <math.h>
#include <iostream>
#include <mt.h>

#ifdef WIN32
#include <windows.h>
#endif

//! Definitions for various objects in the game that use textures
enum NODE_BUTTON_TYPE
{
    NODE_BUTTON_NULL = 0,            // Used for the title block
    NODE_BUTTON_EXIT = 1,
    NODE_BUTTON_ONEP = 2,
    NODE_BUTTON_TWOP = 3,
    NODE_BUTTON_RED = 4,             // Not a button, but is used to store the red texture in memory
	NODE_BUTTON_LIGHTBULB = 5,		 // Also not a button but is actually a node
	NODE_BUTTON_INSTRUCTIONS = 6
};

//! Quick definitions for certain GUI elements we'll need to be referring to
enum GUI_ELEMENT_NAME
{
    GUI_ELEMENT_NULL = 0,
    GUI_ELEMENT_ONEPTEXT = 100,
    GUI_ELEMENT_TWOPTEXT = 101,
	GUI_ELEMENT_ROLESHIFTTEXT = 103,
	GUI_ELEMENT_QUESTIONDISPLAY = 104,
	GUI_ELEMENT_WINDOW = 105
};

//! Used for Irrlicht GUI Events
enum GUI_BUTTON_TYPE
{
    GUI_BUTTON_CANCEL = 1,
    GUI_BUTTON_CONTINUE = 2,
    GUI_BUTTON_ONEP_CONTINUE = 3,
    GUI_BUTTON_TWOP_CONTINUE = 4,

    GUI_BUTTON_DRESS = 5,
    GUI_BUTTON_HISTORY = 6,
    GUI_BUTTON_MISC = 7,
    GUI_BUTTON_OFFICERS = 8,
    GUI_BUTTON_COMPETITIONS = 9,
    GUI_BUTTON_RANDOM = 10,
    GUI_BUTTON_ROLECONTINUE = 11,

    GUI_BUTTON_WRONG = 12,
    GUI_BUTTON_CORRECT = 13,

    GUI_BUTTON_ANSWER_TOPLEFT = 14,
    GUI_BUTTON_ANSWER_TOPRIGHT = 15,
    GUI_BUTTON_ANSWER_BOTTOMLEFT = 16,
    GUI_BUTTON_ANSWER_BOTTOMRIGHT = 17
};

//! Used to store the active category name
enum GAME_CATEGORY_NAME
{
	GAME_CATEGORY_NULL = 0,
	GAME_CATEGORY_DRESS = 1,
	GAME_CATEGORY_HISTORY = 2,
	GAME_CATEGORY_MISC = 3,
	GAME_CATEGORY_OFFICERS = 4,
	GAME_CATEGORY_COMPETITIONS = 5,
	GAME_CATEGORY_RANDOM = 6
};
#endif
