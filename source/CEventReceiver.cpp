/*
*	CEventReceiver.cpp
*	Event receiver code for FBLA Trivia
*	Copyright (c) 2012 Robert MacGregor
*/

#include <stdafx.h>
#include <CEventReceiver.h>

//! Game Namespace
namespace Game
{
    //! Default Constructor
    CEventReceiver::CEventReceiver(void)
    {

    }

    //! Function called by Irrlicht to pass specific events to my game code
    bool CEventReceiver::OnEvent(const irr::SEvent& event)
    {
        Game::CGameResources *resources = Game::CGameResources::getPointer();
		// This if prevents a crash from the event receiver responding before the game code is even ready for any of it; it's an Irrlicht thing
        if (!resources->game_ready)
            return true;

		// We're grabbing a scene node that the mouse is currently hovering over
        irr::scene::ISceneNode *picked_scene_node = resources->scene_manager->getSceneCollisionManager()->getSceneNodeFromScreenCoordinatesBB(resources->cursor_control->getPosition(),                                                                                                                             NODE_BUTTON_EXIT | NODE_BUTTON_ONEP | NODE_BUTTON_TWOP );
        if (picked_scene_node)
		{
            if (picked_scene_node != resources->last_selected_node)
            {
				// If the last selected node is not the same as our current, then we basically swap them out so that the textures are fixed later on
                if (resources->last_selected_node)
                    resources->last_selected_node->setMaterialTexture(0, resources->game_textures[resources->last_selected_node->getID()]);
                resources->last_selected_node = picked_scene_node;
                resources->last_selected_node->setMaterialTexture(0,  resources->game_textures[NODE_BUTTON_RED]);
            }
        }
        else
        {
            // If no nodes are selected but we still have a last selected node, reset the texture on said node
            if (resources->last_selected_node)
                resources->last_selected_node->setMaterialTexture(0, resources->game_textures[resources->last_selected_node->getID()]);
            resources->last_selected_node = NULL;
        }

        // Used to check for keyboard events, in this instance it's just to close the Irrlicht Device
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
		{
			switch (event.KeyInput.Key)
			{
				case irr::KEY_ESCAPE:
                    // Signal Irrlicht to wrap it up since we received this event
					resources->irrlicht_device->closeDevice();
                break;
			}
		}
		// An If used for checking mouse click events so that the 3D GUI works
        else if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
        {
            switch(event.MouseInput.Event)
            {
                // Switch-case for left-mouse button actions
                case irr::EMIE_LMOUSE_PRESSED_DOWN:
                    if (picked_scene_node)
                    {
                        // Switch for specific nodes assigned from over in main.cpp
                        switch (picked_scene_node->getID())
                        {
                            // Switch-case for if the mouse happened to click on the exit button
                            case NODE_BUTTON_EXIT:
                                resources->irrlicht_device->closeDevice();
                                break;

                            // 3D GUI Element SwitchCases ---------------------------------------
							case NODE_BUTTON_INSTRUCTIONS:
							{
							    // Destroy the current GUI contents and load up the instructions GUI
								resources->gui_env->clear();
                                resources->gui_env->loadGUI("media/gui/instructions.xml");

                                // Centers the instruction Window
                                adjustWindow();
                                // Animates the 3D GUI Elements to float off screen
								menuAnimate(false);
								break;
							}
                            case NODE_BUTTON_ONEP:
							{
							    // Load the one player menu
                                resources->gui_env->loadGUI("media/gui/one_game.xml");
								// Centers the one player window
                                adjustWindow();
                                // Animates all of the 3D GUI Elements to float off screen
								menuAnimate(false);
                                break;
							}
                            case NODE_BUTTON_TWOP:
							{
                                // Load the two player name entry window
                                resources->gui_env->loadGUI("media/gui/two_game.xml");
                                // Centers the two player window
								adjustWindow();
								// Animates the 3D GUI elements to go and float off screen
								menuAnimate(false);
                                break;
							}
                            default:
                                resources->log_handle->write("Warning: Received unknown button identification in Mouse event.");
                                break;
                        }
                    }
                    break; // Breaks case irr::EMIE_LMOUSE_PRESSED_DOWN:

                // Unused Mouse events
                case irr::EMIE_LMOUSE_LEFT_UP:
                    break;
                case irr::EMIE_RMOUSE_PRESSED_DOWN:
                    break;
                case irr::EMIE_RMOUSE_LEFT_UP:
                    break;
                case irr::EMIE_MMOUSE_PRESSED_DOWN:
                    break;
                case irr::EMIE_MMOUSE_LEFT_UP:
                    break;
                case irr::EMIE_MOUSE_MOVED:
                    break;
                default:
                    break;
            }
        }
        // An if for when there's GUI Elements (from Irrlicht's native GUI System)
        else if (event.EventType == irr::EET_GUI_EVENT)
        {
            irr::s32 id = event.GUIEvent.Caller->getID();
            switch(event.GUIEvent.EventType)
            {
                case irr::gui::EGET_BUTTON_CLICKED:
                    switch(id)
                    {
                        // Non-gameplay GUI Events ----------------------------
                        case GUI_BUTTON_CANCEL:
                        {
                            // Nukes any contents of the GUI
                            resources->gui_env->clear();
                            // Animates the 3D GUI Elements so that they float back on screen
                            menuAnimate(true);
                            break;
                        }
                        case GUI_BUTTON_ONEP_CONTINUE:
                        {
                            // Locates the text input for the player's name
                            irr::gui::IGUIElement *root = resources->gui_env->getRootGUIElement();
							irr::gui::IGUIElement *onep_entry = root->getElementFromId(GUI_ELEMENT_ONEPTEXT, true);
							// Assigns the player's name from input so it may be used during gameplay
							resources->onep_name = onep_entry->getText();

                            // Sets the one player game flag so no turn logic isused
							resources->onep_game = true;

                            // Clears the GUI and loads up the catogory Window
                            resources->gui_env->clear();
                            resources->gui_env->loadGUI("media/gui/category_set.xml");
                            // Centers the catgory selection Window on screen
                            adjustWindow();
                            break;
                        }
                        case GUI_BUTTON_TWOP_CONTINUE:
                        {
                            // Locate player one's name and assign it to global memory
                            irr::gui::IGUIElement *root = resources->gui_env->getRootGUIElement();
							irr::gui::IGUIElement *onep_entry = root->getElementFromId(GUI_ELEMENT_ONEPTEXT, true);
							resources->onep_name = onep_entry->getText();
							// Locate player two's name and assign it to global memory
							irr::gui::IGUIElement *twop_entry = root->getElementFromId(GUI_ELEMENT_TWOPTEXT, true);
							resources->twop_name = twop_entry->getText();

                            // Sets the one player game state to false so that turn logic is enabled
							resources->onep_game = false;

                            // Wrecks any GUI elements on screen and loads the category Window
                            resources->gui_env->clear();
                            resources->gui_env->loadGUI("media/gui/category_set.xml");
                            // Automatically center the currently loaded window
							adjustWindow();
                            break;
                        }

                        // Category Selection GUI Events ---------------------
						case GUI_BUTTON_DRESS:
							selectCategory(GAME_CATEGORY_DRESS);
                            break;
						case GUI_BUTTON_HISTORY:
                            selectCategory(GAME_CATEGORY_HISTORY);
						    break;
						case GUI_BUTTON_MISC:
							selectCategory(GAME_CATEGORY_MISC);
						    break;
						case GUI_BUTTON_OFFICERS:
                            selectCategory(GAME_CATEGORY_OFFICERS);
						    break;
						case GUI_BUTTON_COMPETITIONS:
							selectCategory(GAME_CATEGORY_COMPETITIONS);
						    break;
                        case GUI_BUTTON_RANDOM:
                            selectCategory(GAME_CATEGORY_RANDOM);
                            break;

						// Gameplay GUI Events ------------------------------------
						case GUI_BUTTON_WRONG:
						{
						    // Nukes the GUI and loads up a role shift window
							resources->gui_env->clear();
							resources->gui_env->loadGUI("media/gui/role_shift.xml");

							// Set the appropriate text values
							irr::gui::IGUIElement *root = resources->gui_env->getRootGUIElement();
							irr::gui::IGUIElement *role_window = root->getElementFromId(GUI_ELEMENT_WINDOW);
							irr::gui::IGUIElement *role_text = role_window->getElementFromId(GUI_ELEMENT_ROLESHIFTTEXT);
							irr::gui::IGUIElement *role_continue = role_window->getElementFromId(GUI_BUTTON_ROLECONTINUE);
							role_window->setText(L"Incorrect answer.");

                            // Centers the loaded role shift window
							adjustWindow();
							// Now discard the question
							questionAnswered(false);
							break;
						}
						case GUI_BUTTON_CORRECT:
						{
							resources->gui_env->clear();
							resources->gui_env->loadGUI("media/gui/role_shift.xml");

							// Set the appropriate text values
							irr::gui::IGUIElement *root = resources->gui_env->getRootGUIElement();
							irr::gui::IGUIElement *role_window = root->getElementFromId(GUI_ELEMENT_WINDOW);
							irr::gui::IGUIElement *role_text = role_window->getElementFromId(GUI_ELEMENT_ROLESHIFTTEXT);
							irr::gui::IGUIElement *role_continue = role_window->getElementFromId(GUI_BUTTON_ROLECONTINUE);
							role_window->setText(L"Correct answer");

							adjustWindow();
							questionAnswered(true);
							break;
						}
						case GUI_BUTTON_ROLECONTINUE:
						{
						    // Destroys the GUI contents and reloads the question display window
							resources->gui_env->clear();
							resources->gui_env->loadGUI("media/gui/question_display.xml");

                            // An array used for storing question data
							irr::core::array<Game::CQuestion*> questions;

							// If our globally loaded array has no elements, then we load up some more
							if (resources->game_questions.size() == 0)
								switch (resources->current_category)
								{
									case GAME_CATEGORY_DRESS:
										resources->game_questions = resources->dress_question_data->getQuestionList();
										break;
									case GAME_CATEGORY_HISTORY:
										resources->game_questions = resources->history_question_data->getQuestionList();
										break;
									case GAME_CATEGORY_MISC:
										resources->game_questions = resources->misc_question_data->getQuestionList();
										break;
									case GAME_CATEGORY_OFFICERS:
										resources->game_questions = resources->officer_question_data->getQuestionList();
										break;
									case GAME_CATEGORY_COMPETITIONS:
										resources->game_questions = resources->competition_question_data->getQuestionList();
										break;
									case GAME_CATEGORY_RANDOM:
									{
									    // Grabs the dress question data
										irr::core::array<Game::CQuestion*> data = resources->dress_question_data->getQuestionList();
										// Iterates throuh all questions the dress category has and assigns it to our globally defined question array
										for (irr::u32 i = 0; i < data.size(); i++)
											resources->game_questions.push_back(data[i]);

                                        // These use the same process
										data = resources->competition_question_data->getQuestionList();
										for (irr::u32 i = 0; i < data.size(); i++)
											resources->game_questions.push_back(data[i]);

										data = resources->misc_question_data->getQuestionList();
										for (irr::u32 i = 0; i < data.size(); i++)
											resources->game_questions.push_back(data[i]);

										data = resources->history_question_data->getQuestionList();
										for (irr::u32 i = 0; i < data.size(); i++)
											resources->game_questions.push_back(data[i]);

										data = resources->officer_question_data->getQuestionList();
										for (irr::u32 i = 0; i < data.size(); i++)
											resources->game_questions.push_back(data[i]);
										break;
									}
									default:
										resources->log_handle->write("Error: Failed to read category type.");
										break;
								}

							questions = resources->game_questions;

							// Pick a question in the set
							resources->question_id = resources->generator->genrand_int32() % questions.size();

							irr::core::array<Game::CQuestion *> question_data = questions;
							Game::CQuestion *question = question_data[resources->question_id];

                            // Grabs any relevant pointers
							irr::gui::IGUIElement *root = resources->gui_env->getRootGUIElement();
							irr::gui::IGUIElement *question_text = root->getElementFromId(GUI_ELEMENT_QUESTIONDISPLAY, true);
							irr::gui::IGUIElement *window = root->getElementFromId(GUI_ELEMENT_WINDOW);

                            // Centers the created question Window
							adjustWindow();

                            // Sets up and assigns the question display data to the question window
							irr::core::stringw question_display_data = question->display_text;
							question_text->setText(question_display_data.c_str());

                            // Finds all of the buttons on the loaded question window
							irr::gui::IGUIElement *top_left = root->getElementFromId(GUI_BUTTON_ANSWER_TOPLEFT, true);
							irr::gui::IGUIElement *top_right = root->getElementFromId(GUI_BUTTON_ANSWER_TOPRIGHT, true);
							irr::gui::IGUIElement *bottom_left = root->getElementFromId(GUI_BUTTON_ANSWER_BOTTOMLEFT, true);
							irr::gui::IGUIElement *bottom_right = root->getElementFromId(GUI_BUTTON_ANSWER_BOTTOMRIGHT, true);

                            // Sets up the answer data
							irr::core::stringw correct_data = question->correct_answer_text;
							irr::core::stringw wrong1_data = question->wrong_answer_first;
							irr::core::stringw wrong2_data = question->wrong_answer_second;
							irr::core::stringw wrong3_data = question->wrong_answer_third;

							// Small hack to detect true and false questions
							if (wrong2_data == L"<NULL>" && wrong3_data == L"<NULL>")
							{
                                // If this is a true/false question, hide the bottom two buttons
								bottom_left->setVisible(false);
								bottom_right->setVisible(false);

								// We then change the top buttons' text to yes/no
								top_left->setText(L"Yes");
								top_right->setText(L"No");

								// Assigns the button ID's based on correct answer Text
								if (correct_data == L"Yes")
								{
									top_left->setID(GUI_BUTTON_CORRECT);
									top_right->setID(GUI_BUTTON_WRONG);
								}
								else
								{
									top_right->setID(GUI_BUTTON_CORRECT);
									top_left->setID(GUI_BUTTON_WRONG);
								}
								break;
							}
							else
							{
							    // Picks a random number between 0 and 3 to randomize question choices
                                irr::u32 id = resources->generator->genrand_int32() % 4;
                                switch (id)
                                {
                                    case 0: // Where the top left button is the correct answer
                                        top_left->setText(correct_data.c_str());
                                        top_left->setID(GUI_BUTTON_CORRECT);
                                        top_right->setText(wrong1_data.c_str());
                                        top_right->setID(GUI_BUTTON_WRONG);
                                        bottom_left->setText(wrong2_data.c_str());
                                        bottom_left->setID(GUI_BUTTON_WRONG);
                                        bottom_right->setText(wrong3_data.c_str());
                                        bottom_right->setID(GUI_BUTTON_WRONG);
                                        break;
                                    case 1: // Where the top right button is the correct answer
                                        top_left->setText(wrong1_data.c_str());
                                        top_left->setID(GUI_BUTTON_WRONG);
                                        top_right->setText(correct_data.c_str());
                                        top_right->setID(GUI_BUTTON_CORRECT);
                                        bottom_left->setText(wrong2_data.c_str());
                                        bottom_left->setID(GUI_BUTTON_WRONG);
                                        bottom_right->setText(wrong3_data.c_str());
                                        bottom_right->setID(GUI_BUTTON_WRONG);
                                        break;
                                    case 2: // Where the bottom left button is the correct answer
                                        top_left->setText(wrong1_data.c_str());
                                        top_left->setID(GUI_BUTTON_WRONG);
                                        top_right->setText(wrong2_data.c_str());
                                        top_right->setID(GUI_BUTTON_WRONG);
                                        bottom_left->setText(correct_data.c_str());
                                        bottom_left->setID(GUI_BUTTON_CORRECT);
                                        bottom_right->setText(wrong3_data.c_str());
                                        bottom_right->setID(GUI_BUTTON_WRONG);
                                        break;
                                    case 3: // Where the bottom right button is the correct answer
                                        top_left->setText(wrong1_data.c_str());
                                        top_left->setID(GUI_BUTTON_WRONG);
                                        top_right->setText(wrong2_data.c_str());
                                        top_right->setID(GUI_BUTTON_WRONG);
                                        bottom_left->setText(wrong3_data.c_str());
                                        bottom_left->setID(GUI_BUTTON_WRONG);
                                        bottom_right->setText(correct_data.c_str());
                                        bottom_right->setID(GUI_BUTTON_CORRECT);
                                        break;
                                }
							}
							break;
						}
                }
            }
		}
		return false;
	}

    //! Private function called by the EventReceiver to play animations
    void CEventReceiver::menuAnimate(bool in)
    {
        // Define a reusable pointer for our animation data
        irr::scene::ISceneNodeAnimator *animation = NULL;
        // Define a reusable reference for our position data
        irr::core::vector3d<irr::f32> position;

        Game::CGameResources *resources = Game::CGameResources::getPointer();

        // If all 3D GUI elements must be moved on screen (hence, "inwards")
        if (in)
        {
            // Handle the one player button ------------------------------
            position = resources->onep_button->getPosition(); // Grabs the position of the one player button
            // Creates a simple animation to make the one player button to flat back on screen to its original position
            animation = resources->scene_manager->createFlyStraightAnimator(position, *resources->onep_start, 1000);
            resources->onep_button->addAnimator(animation); // Binds the animation to the one player button
            animation->drop(); // Drops the no longer used animation data

            // Handle the two player button ------------------------------
            position = resources->twop_button->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, *resources->twop_start, 1000);
            resources->twop_button->addAnimator(animation);
            animation->drop();

            // Handle the exit button ------------------------------------
            position = resources->exit_button->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, *resources->exit_start, 1000);
            resources->exit_button->addAnimator(animation);
            animation->drop();

            // Handle the instructions button ----------------------------
            position = resources->instructions_button->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, *resources->instructions_start, 1000);
            resources->instructions_button->addAnimator(animation);
            animation->drop();

            // Handle the title node -------------------------------------
            position = resources->title_node->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, *resources->title_start, 1000);
            resources->title_node->addAnimator(animation);
            animation->drop();

            // Handle the lightbulb node ---------------------------------
            position = resources->bulb_node->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, *resources->bulb_start, 1000);
            resources->bulb_node->addAnimator(animation);
            animation->drop();
        }
        else // Otherwise all 3D GUI elements must be moved off screen
        {
            // Handle the one player button ---------------------------
            position = resources->onep_button->getPosition(); // Grab the position of our single player button
            // Create a simple animator that takes our one player button to the left off screen
            animation = resources->scene_manager->createFlyStraightAnimator(position, irr::core::vector3d<irr::f32>(-20,position.Y,position.Z), 1000);
            resources->onep_button->addAnimator(animation); // Assigns the animation data
            animation->drop(); // Tells Irrlicht that we don't need this animation pointer anymore and drops it

            // Handle the two player button ---------------------------
            position = resources->twop_button->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, irr::core::vector3d<irr::f32>(20,position.Y,position.Z), 1000);
            resources->twop_button->addAnimator(animation);
            animation->drop();

            // Handle the exit button ---------------------------------
            position = resources->exit_button->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, irr::core::vector3d<irr::f32>(position.X,-20,position.Z), 1000);
            resources->exit_button->addAnimator(animation);
            animation->drop();

            // Handle the instructions button -------------------------
            position = resources->instructions_button->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, irr::core::vector3d<irr::f32>(position.X,10,position.Z), 1000);
            resources->instructions_button->addAnimator(animation);
            animation->drop();

            // Handle the title box -----------------------------------
            position = resources->title_node->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, irr::core::vector3d<irr::f32>(position.X,position.Y,300), 1000);
            resources->title_node->addAnimator(animation);
            animation->drop();

            // Handle the light bulb box ------------------------------
            position = resources->bulb_node->getPosition();
            animation = resources->scene_manager->createFlyStraightAnimator(position, irr::core::vector3d<irr::f32>(position.X,-5,position.Z), 1000);
            resources->bulb_node->addAnimator(animation);
            animation->drop();
        }
    }

    //! Private function called by the EventReceiver to adjust on screen windows
    void CEventReceiver::adjustWindow(void)
    {
        Game::CGameResources *resources = Game::CGameResources::getPointer();

        irr::gui::IGUIElement *root = resources->gui_env->getRootGUIElement();
        irr::gui::IGUIElement *window = root->getElementFromId(GUI_ELEMENT_WINDOW);
        irr::core::rect<irr::s32> window_position = window->getRelativePosition();

        // Calc the width and height
        irr::u32 window_width = window_position.LowerRightCorner.X-window_position.UpperLeftCorner.X;
        irr::u32 window_height = window_position.LowerRightCorner.Y-window_position.UpperLeftCorner.Y;
        window->setRelativePosition(irr::core::rect<irr::s32>(resources->center_x-(window_width/2), resources->center_y-(window_height/2),
                                                              resources->center_x+(window_width/2), resources->center_y+(window_height/2)));
    }

    //! Private function called by the EventReceiver when a category is selected
    void CEventReceiver::selectCategory(const GAME_CATEGORY_NAME button)
    {
        Game::CGameResources *resources = Game::CGameResources::getPointer();
        resources->current_category = button;

        // Destroys any GUI contents and loads the role shift window
        resources->gui_env->clear();
        resources->gui_env->loadGUI("media/gui/role_shift.xml");

        // Locate the relevant GUI elements so that we may assign text
        irr::gui::IGUIElement *root = resources->gui_env->getRootGUIElement();
        irr::gui::IGUIElement *role_window = root->getElementFromId(GUI_ELEMENT_WINDOW);
        irr::gui::IGUIElement *role_text = role_window->getElementFromId(GUI_ELEMENT_ROLESHIFTTEXT);

        // Center the currently loaded Window
        adjustWindow();

        // Construct the Window title text
        irr::core::stringw title = L"It is ";
        title += resources->onep_name;
        title += "\'s turn";
        // Assign the window title text
        role_window->setText(title.c_str());

        // Construct the Window content text
        irr::core::stringw text = L"It is your turn, ";
        text += resources->onep_name;
        text += ".";
        // Assign the content of the window
        role_text->setText(text.c_str());
    }

    //! Private function called by the event receiver when a question is answered
    void CEventReceiver::questionAnswered(bool correct)
    {
        Game::CGameResources *resources = Game::CGameResources::getPointer();
        // Discards the current question
        resources->game_questions.erase(resources->question_id,1);

		// Grabs the root GUI Element
		irr::gui::IGUIElement *root = resources->gui_env->getRootGUIElement();

        irr::core::stringw question_status;
        if (correct)
        {
            if (resources->onep_turn && resources->onep_game)
            {
				resources->onep_score++;
                question_status += L"You have gotten the question correct and now have ";
                question_status += resources->onep_score;
                question_status += L" pts.";
            }
            else if (resources->onep_turn && !resources->onep_game)
            {
				resources->onep_score++;
				question_status = resources->onep_name;
                question_status += L" has gotten the question correct and now has ";
                question_status += resources->onep_score;
                question_status += L" pts. ";
                question_status += L"It is now ";
                question_status += resources->twop_name;
                question_status += L"\'s turn who has ";
                question_status += resources->twop_score;
                question_status += L" pts.";

				// Inverts the player tracker so that the next player has their turn
				resources->onep_turn = !resources->onep_turn;
            }
			else if (!resources->onep_turn && !resources->onep_game)
            {
                resources->twop_score++;
                question_status = resources->twop_name;
                question_status += L" has gotten the question correct and now has ";
                question_status += resources->twop_score;
                question_status += L" pts. ";
                question_status += L"It is now ";
                question_status += resources->onep_name;
                question_status += L"\'s turn who has ";
                question_status += resources->onep_score;
                question_status += L" pts.";

				// Inverts the player tracker so that the next player has their turn
				resources->onep_turn = !resources->onep_turn;
            }	
        }
        else  // If the answer is wrong
        {
            if (resources->onep_turn && resources->onep_game)
            {
				resources->onep_score--;
				question_status += L"You have gotten the question incorrect and now have ";
                question_status += resources->onep_score;
                question_status += L" pts.";
			}
            else if (resources->onep_turn && !resources->onep_game)
            {
                resources->onep_score--;
                question_status = resources->onep_name;
				question_status += L" has gotten the question incorrect and now has ";
                question_status += resources->onep_score;
                question_status += L" pts. ";
                question_status += L"It is now ";
                question_status += resources->twop_name;
                question_status += L"\'s turn who has ";
                question_status += resources->twop_score;
				question_status += L" pts.";

				// Inverts the player tracker so that the next player has their turn
				resources->onep_turn = !resources->onep_turn;
			}
            else if (!resources->onep_turn && !resources->onep_game)
            {
                resources->twop_score--;
                question_status = resources->twop_name;
                question_status += L" has gotten the question incorrect and now has ";
                question_status += resources->twop_score;
                question_status += L" pts. ";
                question_status += L"It is now ";
                question_status += resources->onep_name;
                question_status += L"\'s turn who has ";
                question_status += resources->onep_score;
                question_status += L" pts.";

				// Inverts the player tracker so that the next player has their turn
				resources->onep_turn = !resources->onep_turn;
            }
        }

        // Check to see if there are even any questions left to display
        if (resources->game_questions.size() == 0)
        {
			if (resources->onep_game)
			{
				question_status = L"You have finished this category with ";
				question_status += resources->onep_score;
				question_status += L"pts.";

				// Grab the Role button and change it's ID to cancel
				irr::gui::IGUIElement *button = root->getElementFromId(GUI_BUTTON_ROLECONTINUE, true);
				button->setID(GUI_BUTTON_CANCEL);

				resources->twop_score = 0;
				resources->onep_score = 0;
				resources->onep_turn = true;
			}
			else
			{
				// If the first player wins
				if (resources->onep_score > resources->twop_score)
				{
					question_status = resources->onep_name + L" has won the game with ";
					question_status += resources->onep_score;
					question_status += L"pts. ";
					question_status += resources->twop_name + L" had ";
					question_status += resources->twop_score;
					question_status += L" pts.";

					// Grab the Role button and change it's ID to cancel
					irr::gui::IGUIElement *button = root->getElementFromId(GUI_BUTTON_ROLECONTINUE, true);
					button->setID(GUI_BUTTON_CANCEL);

					resources->twop_score = 0;
					resources->onep_score = 0;
					resources->onep_turn = true;
				 }
				// If the second player wins
				else if (resources->onep_score < resources->twop_score)
				{
					question_status = resources->twop_name + L" has won the game with ";
					question_status += resources->twop_score;
					question_status += L"pts. ";
					question_status += resources->onep_name + L" had ";
					question_status += resources->onep_score;
					question_status += L" pts.";

					// Grab the Role button and change it's ID to cancel
					irr::gui::IGUIElement *button = root->getElementFromId(GUI_BUTTON_ROLECONTINUE, true);
					button->setID(GUI_BUTTON_CANCEL);

					resources->twop_score = 0;
					resources->onep_score = 0;
					resources->onep_turn = true;
				}
				// If both players manage to tie
				else
				{
					question_status = L"Both players have tied with a score total of ";
					question_status += resources->onep_score;
					question_status += L" pts.";

					// Grab the Role button and change it's ID to cancel
					irr::gui::IGUIElement *button = root->getElementFromId(GUI_BUTTON_ROLECONTINUE, true);
					button->setID(GUI_BUTTON_CANCEL);

					resources->twop_score = 0;
					resources->onep_score = 0;
					resources->onep_turn = true;
				}
			}
		}

		irr::gui::IGUIElement *role_text = root->getElementFromId(GUI_ELEMENT_ROLESHIFTTEXT, true);
        role_text->setText(question_status.c_str());
    }
} // End Namespace Game
