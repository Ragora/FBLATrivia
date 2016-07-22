/*
*   CLogHandle.cpp
*   Log handle code for PA:FBLA Trivia's logger manager
*   Copyright (c) 2012 Robert MacGregor
*/

#include <CLogHandle.h>

//! Root Game Namespace
namespace Game
{
    //! Default Constructor
    CLogHandle::CLogHandle(const irr::c8 *name, const irr::c8 *target) : is_ready(false),
                                                                         log_name(name),
                                                                         target_file(target)
    {
        // This log should both have a name and a target (not NULL)
        if (!name || !target)
            return;

        // This is a test to see if we may even be able to wrtie to the target directory
        std::ofstream out_stream;
        out_stream.open(target, std::ios::out);
        if (out_stream.is_open())
        {
            out_stream.close();
            is_ready = true;
        }
    } // End Default Constructor

    //! Default Destructor
    CLogHandle::~CLogHandle(void)
    {

    } // End Default Destructor

    //! Returns the ready status
    bool CLogHandle::ready(void)
    {
        return is_ready;
    }

    //! Writes data to the log
    bool CLogHandle::write(const irr::c8 *data)
    {
        // If the constructor determined that we cannot write to the target file, then we can't do anything
        if (!is_ready || !data)
            return false;

        std::ofstream out_stream;
        out_stream.open(target_file, std::ios::out | std::ios::app);
        // Probably just paranoia on my part since if the file was written to at application start, it shouldn't decide to not be able to be written to after
        if (!out_stream.is_open())
        {
            is_ready = false;
            return false;
        }
        out_stream << data << "\n";
        out_stream.close();
        return true;
    }

    //! Return the name of this log
    const irr::c8 *CLogHandle::name(void)
    {
        return log_name;
    }
} // End Namespace Game
