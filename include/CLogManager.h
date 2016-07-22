/*
*   CLogManager.h
*   Log managing code for PA:FBLA Trivia
*   Copyright (c) 2012 Robert MacGregor
*/

#ifndef _INCLUDE_CLOGMANAGER_H_
#define _INCLUDE_CLOGMANAGER_H_

#include "CLogHandle.h"
#include <irrlicht.h>

//! Root Game Namespace
namespace Game
{
    //! Log manager class
    class CLogManager
    {
        public:
        //! Returns the instance of the log manager as a pointer
        static CLogManager *getPointer(void);
        //! Destroys the instance of the log manager
        static void destroy(void);

        //! Creates a log from the log manager and return a pointer thereof. Will not create the log if name is the same as on another log
        CLogHandle *createLogPointer(const irr::c8 *name, const irr::c8 *file);
        //! Returns a stored log as a pointer if name matches up with one.
        CLogHandle *getLogPointer(const irr::c8 *name);
        //! Destroys a log by name. Returns true if the log actually existed
        bool destroyLog(const irr::c8 *name);
        private:
        //! Default Constructor
        CLogManager(void);
        //! Default Destructor
        ~CLogManager(void);

        irr::core::array<CLogHandle*> *log_data;
    }; // End Class CLogManager
} // End namespace Game
#endif // _INCLUDE_CLOGMANAGER_H_
