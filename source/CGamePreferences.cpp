/*
*   CGamePreferences.cpp
*   Game preference code for PA:FBLA Trivia
*   Copyright (c) 2012 Robert MacGregor
*/

#include <CGamePreferences.h>

//! Root Game Namespace
namespace Game
{
    //! Default Constructor
    CGamePreferences::CGamePreferences(const irr::c8 *target) : CXMLLoader(target),                         // The instance of the Irrlicht XML Loader
                                                                window_width(640),                          // Default Window Width
                                                                window_height(480),                         // Default Window Height
                                                                device_driver(irr::video::EDT_OPENGL),      // Default Video Driver
                                                                window_bitdepth16(false),                   // Default bit Depth (32 bit)
                                                                window_fullscreen(false),                   // Default fullscreen status
                                                                window_vSync(false),                        // Default Vertical Sync status
                                                                device_usestencilbuffer(false),             // Default Stencil buffer status
                                                                audio_driver(irrklang::ESOD_AUTO_DETECT),   // Default Audio driver
                                                                emulate_3d_sound(false),                    // Default emulation of 3D sound
                                                                use_linear_rolloff(false)                   // Default usage of Linear Rolloff
    {
        CLogManager *log_manager = CLogManager::getPointer();
        irr::core::array<XMLNode*> xml_data = getNodeData(); // Calls back to CXMLoader.cpp to retrieve any XML nodes in our target

        for (irr::u32 i = 0; i < xml_data.size(); i++)
        {
            XMLNode *current_node = xml_data[i];

            // This is used to recognise just XML nodes that specify settings -> <Setting Type="windowWidth" Value="640"></Setting>
            if (strcmp(current_node->name,"Setting") == 0)
            {
                irr::c8 *type_value = current_node->getAttribute("Type");
                irr::c8 *attribute_value = current_node->getAttribute("Value");

                // The current XML node should have both of these values defined, if they do not then either one will be NULL and could cause a NULL deference
                if (type_value == NULL || attribute_value == NULL)
                    continue;

                irr::u32 attribute_value_real = (irr::u32)atoi(attribute_value);

                // Massive if, else-if switching structure used to assign values to their proper locations
                if (strcmp(type_value,"windowWidth") == 0)
                    window_width = (irr::u32)attribute_value_real;                          // The Window Width
                else if (strcmp(type_value,"windowHeight") == 0)
                    window_height = (irr::u32)attribute_value_real;                         // The Window Height
                else if (strcmp(type_value,"windowBitDepth16") == 0)
                    window_bitdepth16 = attribute_value_real==1;                            // The 16bit depth buffer status
                else if (strcmp(type_value,"windowFullscreen") == 0)
                    window_fullscreen = attribute_value_real==1;                            // The Fullscreen Status
                else if (strcmp(type_value,"windowVSync") == 0)
                    window_vSync = attribute_value_real==1;                                 // Vertical Sync Status
                else if (strcmp(type_value,"deviceUseStencilBuffer") == 0)
                    device_usestencilbuffer = attribute_value_real==1;                      // Stencil Buffer Status
                else if (strcmp(type_value,"deviceDriver") == 0)
                    device_driver = (irr::video::E_DRIVER_TYPE)attribute_value_real;        // Video Driver
                else if (strcmp(type_value,"soundDriver") == 0)
                    audio_driver = (irrklang::E_SOUND_OUTPUT_DRIVER)attribute_value_real;   // Sound Driver
                else if (strcmp(type_value,"soundEmulate3D") == 0)
                    emulate_3d_sound = attribute_value_real == 1;                           // Emulate 3D Status
                else if (strcmp(type_value, "soundUseLinearRollOff") == 0)
                    use_linear_rolloff = attribute_value_real == 1;                         // Linear Rolloff status
            }
        }
    } // End Default Constructor

    //! Default Destructor
    CGamePreferences::~CGamePreferences(void)
    {

    } // End Default Destructor

    //! Gets the window width
    irr::u32 CGamePreferences::getWindowWidth(void)
    {
        return window_width;
    }

    //! Gets the window height
    irr::u32 CGamePreferences::getWindowHeight(void)
    {
        return window_height;
    }

    //! Gets the window bit depth
    bool CGamePreferences::getWindowBitDepth16Usage(void)
    {
        return window_bitdepth16;
    }

    //! Returns the full screen status
    bool CGamePreferences::getWindowFullscreen(void)
    {
        return window_fullscreen;
    }

    //! Returns the window VSync status
    bool CGamePreferences::getWindowVSync(void)
    {
        return window_vSync;
    }

    //! Returns the stencil buffer usage
    bool CGamePreferences::getDeviceStencilBuffer(void)
    {
        return device_usestencilbuffer;
    }

    //! Returns the device driver
    irr::video::E_DRIVER_TYPE CGamePreferences::getDeviceDriver(void)
    {
        return device_driver;
    }

    //! Returns the audio driver
    irrklang::E_SOUND_OUTPUT_DRIVER CGamePreferences::getAudioDriver(void)
    {
        return audio_driver;
    }

    //! Returns if we're emulating 3D sound
    bool CGamePreferences::emulates3DSound(void)
    {
        return emulate_3d_sound;
    }

    //! Returns if we're using linear rolloff
    bool CGamePreferences::soundUsesLinearRolloff(void)
    {
        return use_linear_rolloff;
    }
} // End namespace Game
