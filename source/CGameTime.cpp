/*
*   CGameTime.cpp
*   Small singleton class that is used for some time-sensitive operations
*   the trivia game.
*   Copyright (c) 2012 Robert MacGregor
*/

#include <CGameTime.h>

//! Root Game Namespace
namespace Game
{
        // This stores an instance of our CGameTime class that is returned at all times except when it is NULL, for which it is then created.
        CGameTime *time_instance = NULL;

        //! Default Constructor
        CGameTime::CGameTime(void) : time_current_seconds(0),       // The current sim time in seconds
                                         time_last_seconds(0),      // The last stored sim time in seconds
                                         time_delta_seconds(0),     // The delta between the current and last sim times in seconds
                                         time_current_clocks(0),    // The current sim time in clocks
                                         time_last_clocks(0),       // The last sim time in clocks
                                         time_delta_clocks(0)       // The delta between the current and last sim times in clocks
        {

        } // End Default Constructor

        //! Default Destructor
        CGameTime::~CGameTime(void)
        {

        } // End Default Destructor

        //! Gets the stored instance of CGameTime if it already exists
        CGameTime *CGameTime::getPointer(void)
        {
            if (time_instance == NULL)
                time_instance = new CGameTime();
            return time_instance;
        }

        //! Destroys the stored instance of CGameTime
        void CGameTime::destroy(void)
        {
            if (time_instance != NULL)
            {
                delete time_instance;
                time_instance = NULL;
            }
        }

        //! Updates all of the stored times on the instance of CGameTime
        void CGameTime::update(void)
        {
            time_last_clocks = time_current_clocks;
            time_current_clocks = clock();
            time_delta_clocks = time_current_clocks-time_last_clocks;

            time_last_seconds = (irr::f32)time_last_clocks/CLOCKS_PER_SEC;
            time_current_seconds = (irr::f32)time_current_clocks/CLOCKS_PER_SEC;
            time_delta_seconds = (irr::f32)time_delta_clocks/CLOCKS_PER_SEC;
        }

        //! Returns the simtime in seconds
        irr::f32 CGameTime::getSimTime(void)
        {
            return time_current_seconds;
        }

        //! Returns the delta sim time in seconds
        irr::f32 CGameTime::getDelta(void)
        {
           return time_delta_seconds;
        }
} // End Namespace Game
