/*
*   CLogManager.cpp
*   Log manager code for PA:FBLA Trivia
*   Copyright (c) 2012 Robert MacGregor
*/

#include <CLogManager.h>

//! Root Game Namespace
namespace Game
{
    // Store a singleton instance of the log manager to be manipulating
    CLogManager *log_manager_instance = NULL;

    //! Default Constructor
    CLogManager::CLogManager(void)
    {
		log_data = new irr::core::array<CLogHandle*>;
    } // End Default Constructor

    //! Default Destructor
    CLogManager::~CLogManager(void)
    {
		irr::core::array<CLogHandle*> log = *log_data;
        for (irr::u32 i = 0; i < log.size(); i++)
        {
            CLogHandle *handle = log[i];
            delete handle;
        }
        log.clear();

		delete log_data;
    } // End Default Destructor

    //! Returns the instance of the log manager
    CLogManager *CLogManager::getPointer(void)
    {
        if (log_manager_instance == NULL)
            log_manager_instance = new CLogManager();
        return log_manager_instance;
    }

    //! Destroys the instance of the log manager
    void CLogManager::destroy(void)
    {
        if (log_manager_instance != NULL)
        {
            delete log_manager_instance;
            log_manager_instance = NULL;
        }
    }

    //! Destroys a log by name. Returns true if the log actually existed
    bool CLogManager::destroyLog(const irr::c8 *name)
    {
        CLogHandle *handle = getLogPointer(name);
		irr::core::array<CLogHandle*> log = *log_data;
        if (handle != NULL)
            for (irr::u32 i = 0; i < log.size(); i++)
            {
                CLogHandle *handle = log[i];
                if (strcmp(handle->name(), name) == 0)
                {
                    delete handle;
                    log.erase(i);
                    return true;
                }
            }
        return false;
    }

    //! Creates a log from the log manager and return a pointer thereof. Will not create the log if name is the same as on another log
    CLogHandle *CLogManager::createLogPointer(const irr::c8 *name, const irr::c8 *file)
    {
        // If our log name actually points to something, we can't create a log with that name
        if (!name || !file || getLogPointer(name) != NULL)
            return NULL;

		irr::core::array<CLogHandle*> log = *log_data;
		// If our handle doesn't already exist then create a new one so we can return it
        CLogHandle *handle = new CLogHandle(name, file);
        log.push_back(handle);
        return handle;
    }


    //! Returns a stored log as a pointer if name matches up with one.
    CLogHandle *CLogManager::getLogPointer(const irr::c8 *name)
    {
        // The input name should always be valid
        if (!name)
            return NULL;

		irr::core::array<CLogHandle*> log = *log_data;
        for (irr::u32 i = 0; i < log.size(); i++)
        {
            CLogHandle *handle = log[i];
            if (strcmp(handle->name(), name) == 0)
                return handle;
        }
        return NULL;
    }
} // End Namespace Game
