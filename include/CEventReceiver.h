/*
*	CEventReceiver.h
*	Event receiver code for FBLA Trivia
*	Copyright (c) 2012 Robert MacGregor
*/

#ifndef _INCLUDE_CEVENTRECEIVER_H_
#define _INCLUDE_CEVENTRECEIVER_H_

#include <stdafx.h>
#include <CGameResources.h>

namespace Game
{
	class CEventReceiver : public irr::IEventReceiver
	{
		public:
            //! Default Constructor
            CEventReceiver(void);
            //! Function called by Irrlicht to pass specific events to my game code
            virtual bool OnEvent(const irr::SEvent& event);
        private:
            irr::scene::ISceneNode *last_hovered_over;
            irr::scene::ISceneNode *moused_node;

            //! Private function called by the EventReceiver to play animations
            void menuAnimate(bool in);
            //! Private function called by the EventReceiver to adjust on screen windows
            void adjustWindow(void);
            //! Private function called by the EventReceiver when a category is selected
            void selectCategory(const GAME_CATEGORY_NAME button);
            //! Private function called by the event receiver when a question is answered
            void questionAnswered(bool correct);
	}; // End Class Game::CEventReceiver
} // End Namespace Game
#endif // _INCLUDE_CEVENTRECEIVER_H_
