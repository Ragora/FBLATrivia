/*
*   CLogHandle.h
&   Log handle code for PA:FBLA Trivia's logger manager
*   Copyright (c) 2012 Robert MacGregor
*/

#ifndef _INCLUDE_CLOGHANDLE_H_
#define _INCLUDE_CLOGHANDLE_H_

#include <irrlicht.h>
#include <fstream>

//! Root Game Namespace
namespace Game
{
    //! Log Handle Class
    class CLogHandle
    {
        public:
        //! Default Constructor
        CLogHandle(const irr::c8 *name, const irr::c8 *target);
        //! Default Destructor
        ~CLogHandle(void);

        //! Returns the ready status
        bool ready(void);
        //! Write data to the log
        bool write(const irr::c8 *data);
        //! Return the name of this log
        const irr::c8 *name(void);

        private:
        bool is_ready;
        const irr::c8 *log_name;
        const irr::c8 *target_file;
    }; // End class CLogHandle
} // End Namespace Game
#endif // _INCLUDE_CLOGHANDLE_H_
