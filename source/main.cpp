/*
*   main.cpp
*   Main code for PA:FBLA Trivia game
*   Copyright (c) 2012 Robert MacGregor
*/

#include <stdafx.h>
#include <CEventReceiver.h>

#define BUTTON_MOD 30
#define BUTTON_RATE 5 // Per sec

irr::s32 main(irr::s32 argc, irr::c8 *argv[])
{
    Game::CGameResources *resources = Game::CGameResources::getPointer();
    // Create the logger
    Game::CLogManager *log_manager = Game::CLogManager::getPointer();
    if (!log_manager)
    {
        #ifdef WIN32
        // Window code here
        #endif
        return 1;
    }
	// Create the log handle in global space so other portions of the application can write to it
    resources->log_handle = log_manager->createLogPointer("main","log.txt");

	// Write the initial Copyright text to the log file
    resources->log_handle->write("FBLA Trivia Game");
    resources->log_handle->write("Programming Copyright (c) 2012 Robert MacGregor");
    resources->log_handle->write("Design and Art Copyright (c) 2012 Alexandrea Hutfles");
    resources->log_handle->write(" ");

	// Attempt to load up the Configuration XML file
    resources->log_handle->write("Loading game preferences ...");
    Game::CGamePreferences *game_preferences = new Game::CGamePreferences("media/Config.xml");
    if (!game_preferences) // If for some reason we were unable to allocate the memory for the GamePreferences
    {
        resources->log_handle->write("Fatal: Unable to load game preferences. (Not enough memory)");
        log_manager->destroy(); // Destroy the log manager and exit
        return 2;
    }
    resources->log_handle->write("Game preference loading complete.");

	// We use 32 bits for depth buffers by default
    irr::u32 depth_bits = 32;
    if (game_preferences->getWindowBitDepth16Usage()) // But if the game preferences call for it otherwise
    {
        resources->log_handle->write("Warning: Using 16 bit depth buffer.");
        depth_bits = 16; // Then we use the 16bit depth buffer, but pretty much any machine should be using 32bit
    }

    resources->log_handle->write("Attempting to create Irrlicht device ...");

	// Attempt to select a video driver from the game preferences
    irr::video::E_DRIVER_TYPE video_driver_type = game_preferences->getDeviceDriver();
    #ifdef LINUX
    if (video_driver_type == irr::video::EDT_DIRECT3D8 || video_driver_type == irr::video::EDT_DIRECT3D9)
    {
        resources->log_handle->write("Warning: Configuration file called for non-Linux display driver, will use OpenGL instead.");
        video_driver_type = irr::video::EDT_OPENGL;
    }
    #endif

	// Make sure they don't try to run with some weird video driver
    if (video_driver_type < 0 || video_driver_type > irr::video::EDT_COUNT)
    {
        resources->log_handle->write("Warning: Configuration file called for invalid display driver, will use OpenGL instead.");
        video_driver_type = irr::video::EDT_OPENGL;
    }
	// Warn if a software video driver is being used
    if (video_driver_type == irr::video::EDT_SOFTWARE || video_driver_type == irr::video::EDT_BURNINGSVIDEO)
        resources->log_handle->write("Warning: Using software display driver. The picture may not be correct nor will you get the best FPS!");

	// Create the event receiver and retrieve window dimensions
    Game::CEventReceiver *event_receiver = new Game::CEventReceiver;
	resources->window_width = game_preferences->getWindowWidth();
	resources->window_height = game_preferences->getWindowHeight();

	// Calculate the Window center coordinates
	resources->center_x = resources->window_width/2;
	resources->center_y = resources->window_height/2;

	// Create the Irrlicht device with our specified game preference settings
    resources->irrlicht_device = irr::createDevice(video_driver_type,
                                                             irr::core::dimension2d<irr::u32>(resources->window_width, resources->window_height),
                                                             depth_bits,
                                                             game_preferences->getWindowFullscreen(),
                                                             game_preferences->getDeviceStencilBuffer(), // Used in shadow calculations
                                                             game_preferences->getWindowVSync(), // Vertical sync prevents tearing
                                                             event_receiver);

	// Assign all of the commonly used objects to global memory space
    resources->video_driver = resources->irrlicht_device->getVideoDriver();
    resources->gui_env = resources->irrlicht_device->getGUIEnvironment();
    resources->scene_manager = resources->irrlicht_device->getSceneManager();
    resources->cursor_control = resources->irrlicht_device->getCursorControl();

    resources->log_handle->write("Created Irrlicht device.");
    resources->irrlicht_device->setWindowCaption(L"FBLA Trivia"); // Set the Window title, though not even visible by default

    if (!resources->irrlicht_device)
    {
        resources->log_handle->write("Fatal: Unable to initialise Irrlicht. (Not enough memory / Driver failure)");
        log_manager->destroy();
        return 3;
    }
    resources->log_handle->write("Irrlicht initialised.");

    resources->log_handle->write("Attempting to initialise irrKlang ...");

	// Attempt to select an audio driver
    irrklang::E_SOUND_OUTPUT_DRIVER audio_driver = game_preferences->getAudioDriver();
    if (audio_driver != irrklang::ESOD_AUTO_DETECT)
        resources->log_handle->write("Warning: Will attempt to use user-set audio driver!");

	// Warn and select autodetect if we attempt to use a non-Windows sound driver
    #ifdef WIN32
    if (audio_driver == irrklang::ESOD_ALSA || audio_driver == irrklang::ESOD_CORE_AUDIO)
    {
        resources->log_handle->write("Warning: Configuration file called for non-Windows sound driver, will autodetect instead.");
        audio_driver = irrklang::ESOD_AUTO_DETECT;
    }
    #endif
	// Warn and select autodetect if we attempt to use a non-Linux sound driver
    #ifdef LINUX
    if (audio_driver == irrklang::ESOD_CORE_AUDIO || audio_driver == irrklang::ESOD_DIRECT_SOUND_8 || audio_driver == irrklang::ESOD_DIRECT_SOUND || audio_driver == irrklang::ESOD_WIN_MM)
    {
        resources->log_handle->write("Warning: Configuration file called for non-Linux sound driver, will autodetect instead.");
        audio_driver = irrklang::ESOD_AUTO_DETECT;
    }
    #endif

	// Make sure we're not trying to use an invalid sound driver
    if (audio_driver > irrklang::ESOD_COUNT || audio_driver < 0)
    {
        resources->log_handle->write("Warning: Invalid sound driver in configuration, will autodetect instead.");
        audio_driver = irrklang::ESOD_AUTO_DETECT;
    }

	// Warn if we're using a NULL sound driver
    if (audio_driver == irrklang::ESOD_NULL)
        resources->log_handle->write("Warning: Not using a sound driver, your game will be silent!");

    bool emulate_3d_sound = game_preferences->emulates3DSound();
    bool use_linear_rolloff = game_preferences->soundUsesLinearRolloff();
    irr::u32 irrklang_options = irrklang::ESEO_LOAD_PLUGINS | irrklang::ESEO_MULTI_THREADED | irrklang::ESEO_LOAD_PLUGINS | irrklang::ESEO_PRINT_DEBUG_INFO_TO_DEBUGGER | irrklang::ESEO_PRINT_DEBUG_INFO_TO_STDOUT;

    if (emulate_3d_sound)
        resources->log_handle->write("Warning: Will emulate 3D sound instead of using hardware acceleration.");
    else
        irrklang_options = irrklang_options | irrklang::ESEO_USE_3D_BUFFERS;
    if (use_linear_rolloff)
    {
        resources->log_handle->write("Warning: Sound will be using linear rolloff.");
        irrklang_options = irrklang_options | irrklang::ESEO_LINEAR_ROLLOFF;
    }

    resources->irrklang_device = irrklang::createIrrKlangDevice(audio_driver, irrklang_options);
    if (!resources->irrklang_device)
        resources->log_handle->write("Warning: Unable to initialise irrKlang. This may be a system error and the program will attempt to execute without sound.");

    // --- Load Resources
    resources->log_handle->write("Loading game assets ...");
    resources->dress_question_data = new Game::CQuestionLoader("media/questions/dress.txt");
	resources->competition_question_data = new Game::CQuestionLoader("media/questions/competitions.txt");
	resources->misc_question_data = new Game::CQuestionLoader("media/questions/misc.txt");
	resources->history_question_data = new Game::CQuestionLoader("media/questions/history.txt");
	resources->officer_question_data = new Game::CQuestionLoader("media/questions/officers.txt");

    resources->cube_mesh = resources->scene_manager->getMesh("media/models/cube.obj");
	// Loads all of our game textures into memory
    resources->game_textures[NODE_BUTTON_NULL] = resources->video_driver->getTexture("media/textures/title.png");
    resources->game_textures[NODE_BUTTON_EXIT] = resources->video_driver->getTexture("media/textures/exit.png");
    resources->game_textures[NODE_BUTTON_ONEP] = resources->video_driver->getTexture("media/textures/one_player.png");
    resources->game_textures[NODE_BUTTON_TWOP] = resources->video_driver->getTexture("media/textures/two_player.png");
    resources->game_textures[NODE_BUTTON_RED] = resources->video_driver->getTexture("media/textures/red.png");
	resources->game_textures[NODE_BUTTON_LIGHTBULB] = resources->video_driver->getTexture("media/textures/lightbulb.png");
	resources->game_textures[NODE_BUTTON_INSTRUCTIONS] = resources->video_driver->getTexture("media/textures/instructions.png");

    // --- Initialise the "main scene"
    resources->log_handle->write("Initializing game assets ...");
    Game::CGameTime *game_time = Game::CGameTime::getPointer();

    resources->camera = resources->scene_manager->addCameraSceneNode();
    resources->camera->setID(0);

    // Configures the exit button
    resources->exit_button = resources->scene_manager->addMeshSceneNode(resources->cube_mesh); // Creates the node with a cube mesh
    resources->exit_button->setPosition(*resources->exit_start); // Sets the initial position
    resources->exit_button->setScale(irr::core::vector3d<irr::f32>(1.2f, 2.0f, 1.0f));
    resources->exit_button->setRotation(irr::core::vector3d<irr::f32>(1.0f, 0.0f, 270.0f));
    resources->exit_button->setMaterialFlag(irr::video::EMF_LIGHTING, false); // Notifies Irrlicht we don't want any lighting calculations performed
    resources->exit_button->setMaterialTexture(0, resources->game_textures[NODE_BUTTON_EXIT]);
    resources->exit_button->setID(NODE_BUTTON_EXIT); // Specifies the ID so that CEventReceiver.cpp knows what to do

    // Configures the one-player button
    resources->onep_button = resources->scene_manager->addMeshSceneNode(resources->cube_mesh);
    resources->onep_button->setPosition(*resources->onep_start);
    resources->onep_button->setScale(irr::core::vector3d<irr::f32>(1.2f, 2.0f, 1.0f));
    resources->onep_button->setRotation(irr::core::vector3d<irr::f32>(1.0f, 0.0f, 270.0f));
    resources->onep_button->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    resources->onep_button->setMaterialTexture(0, resources->game_textures[NODE_BUTTON_ONEP]);
    resources->onep_button->setID(NODE_BUTTON_ONEP);

    // Configures the two-player button
    resources->twop_button = resources->scene_manager->addMeshSceneNode(resources->cube_mesh);
    resources->twop_button->setPosition(*resources->twop_start);
    resources->twop_button->setScale(irr::core::vector3d<irr::f32>(1.2f, 2.0f, 1.0f));
    resources->twop_button->setRotation(irr::core::vector3d<irr::f32>(1.0f, 0.0f, 270.0f));
    resources->twop_button->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    resources->twop_button->setMaterialTexture(0, resources->game_textures[NODE_BUTTON_TWOP]);
    resources->twop_button->setID(NODE_BUTTON_TWOP);

    // Configures the title node
    resources->title_node = resources->scene_manager->addMeshSceneNode(resources->cube_mesh);
    resources->title_node->setScale(irr::core::vector3d<irr::f32>(1.2f,2.0f,1.0f));
    resources->title_node->setPosition(*resources->title_start);
    resources->title_node->setRotation(irr::core::vector3d<irr::f32>(1,0,270));
    resources->title_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    resources->title_node->setMaterialTexture(0, resources->game_textures[NODE_BUTTON_NULL]);
    resources->title_node->setID(0);

	  // Configures the title node
    resources->bulb_node = resources->scene_manager->addMeshSceneNode(resources->cube_mesh);
    resources->bulb_node->setScale(irr::core::vector3d<irr::f32>(1.2f,2.0f,1.0f));
    resources->bulb_node->setPosition(*resources->bulb_start);
    resources->bulb_node->setRotation(irr::core::vector3d<irr::f32>(1,0,270));
    resources->bulb_node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    resources->bulb_node->setMaterialTexture(0, resources->game_textures[NODE_BUTTON_LIGHTBULB]);
    resources->bulb_node->setID(0);

	// Configures the instructions
    resources->instructions_button = resources->scene_manager->addMeshSceneNode(resources->cube_mesh);
    resources->instructions_button->setScale(irr::core::vector3d<irr::f32>(1.2f,2.0f,1.0f));
    resources->instructions_button->setPosition(*resources->instructions_start);
    resources->instructions_button->setRotation(irr::core::vector3d<irr::f32>(1,0,270));
    resources->instructions_button->setMaterialFlag(irr::video::EMF_LIGHTING, false);
    resources->instructions_button->setMaterialTexture(0, resources->game_textures[NODE_BUTTON_INSTRUCTIONS]);
    resources->instructions_button->setID(NODE_BUTTON_INSTRUCTIONS);

	// Seeds the random number generator
	resources->generator->init_genrand(time(NULL));

	// Load up a different font file as the default one is HORRIBLY hard to read
	irr::gui::IGUISkin* skin = resources->gui_env->getSkin();
	irr::gui::IGUIFont* font = resources->gui_env->getFont("media/textures/fontlucida.png");
	if (font)
		skin->setFont(font);
	skin->setFont(resources->gui_env->getBuiltInFont(), irr::gui::EGDF_WINDOW);

	// Define the BUTTON_MOD and BUTTON_RATE flags as floating point values for precision correctness
    irr::f32 button_modifier = BUTTON_MOD;
    irr::f32 button_rate = BUTTON_RATE;

    irr::f32 rotationTick = 0.0f; // The current rotation tick

    resources->log_handle->write("Info: Game initialisation completed successfully.");
    resources->game_ready = true;
    while (resources->irrlicht_device->run())
    {
		// Wrap the rotation tick around 360 degrees, could use modulus (%) but compilers don't like you using it with non-integer data types.
        if (rotationTick >= 360)
            rotationTick = 360 - rotationTick;

		irr::core::vector3d<irr::f32> onep_position = resources->onep_button->getPosition();
		irr::core::vector3d<irr::f32> twop_position = resources->twop_button->getPosition();

		// Queue Irrlicht to begin a scene
        resources->video_driver->beginScene(true, true, irr::video::SColor(255, 255-(onep_position.X*3), 180-(twop_position.X*9), 0));
        resources->scene_manager->drawAll(); // Draw the 3D scene
        resources->gui_env->drawAll();		  // Draw the GUI
        resources->video_driver->endScene(); // End the scene

        rotationTick += BUTTON_RATE*game_time->getDelta(); // Bump up the rotationTick

		irr::f32 rotation = -(sin(rotationTick)*BUTTON_MOD);
		// Update all of the dynamic node positions
        resources->exit_button->setRotation(irr::core::vector3d<irr::f32>(rotation,0.0f,270.0f));
        resources->onep_button->setRotation(irr::core::vector3d<irr::f32>(rotation,0.0f,270.0f));
        resources->twop_button->setRotation(irr::core::vector3d<irr::f32>(rotation,0.0f,270.0f));
		resources->instructions_button->setRotation(irr::core::vector3d<irr::f32>(rotation,0.0f,270.0f));

        // The debug preprocessor declaration has the trailing underscore due to some issues with Irrlicht if I had just defined _DEBUG
        #ifdef _DEBUG_
        irr::core::stringw str = L"FBLA Trivia [";
        str += resources->video_driver->getName();
        str += "] FPS:";
        str += resources->video_driver->getFPS();
		// Reassigns the title to the window to display Frames per Second
        resources->irrlicht_device->setWindowCaption(str.c_str());
        #endif

		// Used to update the game type so that game_time->getdelta() returns the proper CPU delta time
        game_time->update();
    }

    delete event_receiver;
    Game::CGameTime::destroy();

	// Delete all our data
	Game::CGameResources::destroy();

	#ifdef _DEBUG_
    resources->log_handle->write("Game closed successfully without forceful explosive decompression.");
    #else
    resources->log_handle->write("Game closed successfully.");
    #endif
	Game::CLogManager::destroy();

	exit(0); // Forces the application to close
    return 0;
}
