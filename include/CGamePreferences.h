/*
*   CGamePreferences.h
*   Game preference code for PA:FBLA Trivia
*   Copyright (c) 2012 Robert MacGregor
*/

#ifndef _INCLUDE_CGAMEPREFERENCES_H_
#define _INCLUDE_CGAMEPREFERENCES_H_

#include "CXMLLoader.h"
#include "CLogManager.h"
#include <irrlicht.h>
#include <irrKlang.h>

//! Root Kiaro Namespace
namespace Game
{
    //! Game Preference loading class
    class CGamePreferences : public CXMLLoader
    {
        public:
        //! Default Constructor
        CGamePreferences(const irr::c8 *target);
        //! Default Destructor
        ~CGamePreferences(void);

        //! Gets the window width
        irr::u32 getWindowWidth(void);
        //! Gets the window height
        irr::u32 getWindowHeight(void);
        //! Gets the window bit depth
        bool getWindowBitDepth16Usage(void);
        //! Returns the full screen status
        bool getWindowFullscreen(void);
        //! Returns the window VSync status
        bool getWindowVSync(void);
        //! Returns the stencil buffer usage
        bool getDeviceStencilBuffer(void);
        //! Returns the device driver
        irr::video::E_DRIVER_TYPE getDeviceDriver(void);
        //! Returns if we're emulating 3D sound
        bool emulates3DSound(void);
        //! Returns if we're using linear rolloff
        bool soundUsesLinearRolloff(void);

        //! Returns the audio driver
        irrklang::E_SOUND_OUTPUT_DRIVER getAudioDriver(void);
        private:
        irr::u32 window_width, window_height;
        irr::video::E_DRIVER_TYPE device_driver;
        bool window_bitdepth16, window_fullscreen, window_vSync, device_usestencilbuffer;

        irrklang::E_SOUND_OUTPUT_DRIVER audio_driver;
        bool emulate_3d_sound;
        bool use_linear_rolloff;
    };
} // End namespace Game
#endif // _INCLUDE_CGAMEPREFERENCES_H_
