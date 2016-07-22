/*
*` CGameResources.cpp
*  Singleton class containing various resources for the game
*  Copyright (c) 2012 Robert MacGregor
*/

#include <stdafx.h>
#include <CGameResources.h>

namespace Game
{
    CGameResources *game_singleton = NULL;
    //! Default Constructor
    CGameResources::CGameResources(void)
    {
        // Irrlicht Resources
        irr::scene::IMesh *cube_mesh = NULL;
       // irr::video::ITexture *game_textures[10];
		game_textures = new irr::video::ITexture*[10];

        // irrlicht Objects
        irrlicht_device = NULL;
        video_driver = NULL;
        scene_manager = NULL;
        gui_env = NULL;
        cursor_control = NULL; // Used for retrieving the current position of the mouse cursor

        // Irrlicht Nodes
        exit_button = NULL; // Used for the node stating "exit"
        onep_button = NULL; // Used for the node stating "one player"
        twop_button = NULL; // Used for the node stating "two player"
        title_node = NULL; // Used for the node with the game title on it
        bulb_node = NULL; // Used for the node with a lightbulb on it
        instructions_button = NULL; // Used to store the Instructions button
        last_selected_node = NULL; // The last selected 3D Node
        camera = NULL; // The Camera placed in our scene

        // Question Resources
        dress_question_data = NULL;
        competition_question_data = NULL;
        officer_question_data = NULL;
        misc_question_data = NULL;
        history_question_data = NULL;

        // irrKlang Objects
        irrklang_device = NULL;

        // Game Flags
        game_ready = false;
        onep_game = false;
        onep_turn = true;
        onep_name = L"John Doe";
		twop_name = L"Jane Doe";
        window_width = 640;
        window_height = 480;
        center_x = 0;
        center_y = 0;
        onep_score = 0;
        twop_score = 0;
        question_id = 0;
        question_count = 0;

        Game::CLogHandle *log_handle = NULL;

        generator = new MersenneTwister;

        current_category = GAME_CATEGORY_RANDOM; // Random

		// Default Position Data
        exit_start = new irr::core::vector3d<irr::f32>(0,-3.5,10);
        onep_start = new irr::core::vector3d<irr::f32>(-6,-3.5,10);
        twop_start = new irr::core::vector3d<irr::f32>(6,-3.5,10);
        title_start = new irr::core::vector3d<irr::f32>(0,5,10);
        bulb_start = new irr::core::vector3d<irr::f32>(0,3,10);
        instructions_start = new irr::core::vector3d<irr::f32>(-5.5,4.5,10);
    }
    //! Default Destructor
    CGameResources::~CGameResources(void)
    {

    }

    //! Create the singleton pointer
    CGameResources *CGameResources::getPointer(void)
    {
        if (!game_singleton)
            game_singleton = new CGameResources;
        return game_singleton;
    }

    //! Destroy the singleton pointer
    void CGameResources::destroy(void)
    {
        if (game_singleton)
		{
			delete game_singleton->generator;
			delete game_singleton->exit_start;
			delete game_singleton->onep_start;
			delete game_singleton->twop_start;
			delete game_singleton->title_start;
			delete game_singleton->bulb_start;
			delete game_singleton->instructions_start;
		}
    }
} // End Namespace Game
