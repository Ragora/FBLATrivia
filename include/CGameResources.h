/*
*` CGameResources.h
*  Singleton class containing various resources for the game
*  Copyright (c) 2012 Robert MacGregor
*/

#ifndef _INCLUDE_CGAMERESOURCES_H_
#define _INCLUDE_CGAMERESOURCES_H_

#include <mt.h>
#include <stdafx.h>

namespace Game
{
    class CGameResources
    {
        public:

        //! Create the singleton pointer
        static CGameResources *getPointer(void);
        //! Destroy the singleton pointer
        static void destroy(void);

        // Irrlicht Resources
        irr::scene::IMesh *cube_mesh;
		irr::video::ITexture **game_textures; 

        // Starting Position Data
        irr::core::vector3d<irr::f32> *exit_start;
        irr::core::vector3d<irr::f32> *onep_start;
        irr::core::vector3d<irr::f32> *twop_start;
        irr::core::vector3d<irr::f32> *title_start;
        irr::core::vector3d<irr::f32> *bulb_start;
        irr::core::vector3d<irr::f32> *instructions_start;

        // Irrlicht objects
        irr::IrrlichtDevice *irrlicht_device;
        irr::video::IVideoDriver *video_driver;
        irr::scene::ISceneManager *scene_manager;
        irr::gui::IGUIEnvironment *gui_env;
        irr::gui::ICursorControl *cursor_control;

        // Irrlicht Nodes
        irr::scene::ISceneNode *exit_button;
        irr::scene::ISceneNode *onep_button;
        irr::scene::ISceneNode *twop_button;
        irr::scene::ISceneNode *title_node;
        irr::scene::ISceneNode *bulb_node;
        irr::scene::ISceneNode *instructions_button;
        irr::scene::ICameraSceneNode *camera;
        irr::scene::ISceneNode *last_selected_node;

        // Question Resources
        Game::CQuestionLoader *dress_question_data;
        Game::CQuestionLoader *competition_question_data;
        Game::CQuestionLoader *officer_question_data;
        Game::CQuestionLoader *misc_question_data;
        Game::CQuestionLoader *history_question_data;

        // irrKlang Objects
        irrklang::ISoundEngine *irrklang_device;

        // Game Flags
        bool game_ready;
        bool onep_game;
        bool player_turn;
        irr::u32 window_width;
        irr::u32 window_height;

        irr::core::stringw onep_name;
        irr::core::stringw twop_name;
        irr::scene::ISceneNode *moused_node;
        Game::CLogHandle *log_handle;

        MersenneTwister *generator;
        irr::core::array<Game::CQuestion*> game_questions;

        bool onep_turn;
        irr::s32 onep_score;
        irr::s32 twop_score;
        irr::u32 question_id;
        irr::u32 question_count;

        irr::u32 center_x;
        irr::u32 center_y;

        GAME_CATEGORY_NAME current_category;

        private:
        //! Default Constructor
        CGameResources(void);
        //! Default Destructor
        ~CGameResources(void);

    }; // End Class CGameResources
}
#endif // _INCLUDE_CGAMERESOURCES_H_
