/*
*   CXMLLoader.h
*   XML file loading code for PA:FBLA Trivia
*   Copyright (c) 2012 Robert MacGregor
*/

#ifndef _INCLUDE_CXMLLOADER_H_
#define _INCLUDE_CXMLLOADER_H_

#include <irrlicht.h>
#include <irrXML.h>

#include "CReference.h"

//! Root Game Namespace
namespace Game
{
    class XMLAttributeData : public CReference
    {
        public:
        irr::c8 *name;
        irr::c8 *value;
    }; // End Struct XMLAttributeData
    class XMLNode : public CReference
    {
        public:
        //! Return the value of an attribute
        irr::c8 *getAttribute(const irr::c8 *name)
        {
            irr::core::array<XMLAttributeData*> attb_data = *attribute_data;
            for (irr::u32 i = 0; i < attb_data.size(); i++)
            {
                XMLAttributeData *attribute = attb_data[i];
                if (strcmp(attribute->name,name) == 0)
                    return attribute->value;
            }
            return NULL;
        }

        irr::c8 *name;
        irr::core::array<XMLAttributeData*> *attribute_data;
        irr::core::array<XMLNode*> *subnode_data;
        XMLNode *parent;
    }; // End Struct XMLNode

    //! XML Loading Class
    class CXMLLoader : public CReference
    {
        public:
        //! Default Constructor
        CXMLLoader(const irr::c8 *target);
        //! Default Destructor
        ~CXMLLoader(void);

        //! Return the stored node data
        irr::core::array<XMLNode*> getNodeData(void);

        private:
        irr::core::array<XMLNode*> *node_data;
        const irr::c8 *target_file;
    }; // End Class CXMLLoader
} // End Namespace Game
#endif // _INCLUDE_CXMLLOADER_H_
