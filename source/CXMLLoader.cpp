/*
*   CXMLLoader.cpp
*   XML file loading code for PA:FBLA Trivia
*   Copyright (c) 2012 Robert MacGregor
*/

#include <CXMLLoader.h>

// FIXME: MAKE THIS CODE MUCH MUCH MORE READABLE

//! Root Game Namespace
namespace Game
{
    //! Default Constructor
    CXMLLoader::CXMLLoader(const irr::c8 *target) : node_data(new irr::core::array<XMLNode*>),
                                                    target_file(target)
    {
        irr::io::IrrXMLReader *xml = irr::io::createIrrXMLReader(target);
        if (!xml)
            return;

        irr::s32 depth_level = 0;
        XMLNode *current_node = NULL;
        XMLNode *last_node = NULL;
        XMLNode *temp_mem = NULL;

        irr::core::array<const irr::c8*> recurision_depth_tracker;
        while (xml->read())
        {
            switch(xml->getNodeType())
            {
                case irr::io::EXN_ELEMENT:
                {
                    // TODO: Someday figure out why this +2 was required for the depth to work right
                    depth_level += 2;

                    // Grab the node name and copy it over to memory that won't be freed after this
                    const irr::c8 *node_name_temp = xml->getNodeName();
                    irr::u32 node_name_length = strlen(node_name_temp)+1;
                    irr::c8 *node_name = (irr::c8*)malloc(node_name_length);
                    memset(node_name, 0x00, node_name_length);
                    memcpy(node_name, node_name_temp, node_name_length);

                    // Also alloc a bit of more memory to shove into the recursion depth tracker
                    irr::c8 *node_name_recr = (irr::c8*)malloc(node_name_length);
                    memset(node_name_recr, 0x00, node_name_length);
                    memcpy(node_name_recr, node_name_temp, node_name_length);
                    recurision_depth_tracker.push_back(node_name_recr);

                    // Create the XML node instance
                    temp_mem = new XMLNode;

                    // Add the arrays
                    temp_mem->attribute_data = new irr::core::array<XMLAttributeData*>;
                    temp_mem->subnode_data = new irr::core::array<XMLNode*>;

                    if(current_node == NULL)
                        node_data->push_back(temp_mem);
                   else
                        current_node->subnode_data->push_back(temp_mem);

                    current_node = temp_mem;
                    current_node->name = node_name;

                    // Loop through all of the attributes
                    irr::u32 attribute_count = xml->getAttributeCount();
                    for (irr::u32 i = 0; i < attribute_count; i++)
                    {
                        // Get the attribute name and memcpy it over
                        const irr::c8 *attribute_name_temp = xml->getAttributeName(i);
                        irr::u32 attribute_name_size = strlen(attribute_name_temp)+1;
                        irr::c8 *attribute_name = (irr::c8*)malloc(attribute_name_size);
                        memset(attribute_name, 0x00, attribute_name_size);
                        memcpy(attribute_name, attribute_name_temp, attribute_name_size);
                        // Get the attribute value and memcpy it over
                        const irr::c8 *attribute_value_temp = xml->getAttributeValue(i);
                        irr::u32 attribute_value_size = strlen(attribute_value_temp)+1;
                        irr::c8 *attribute_value = (irr::c8*)malloc(attribute_value_size);
                        memset(attribute_value, 0x00, attribute_value_size);
                        memcpy(attribute_value, attribute_value_temp, attribute_value_size);

                        //Create the XML Attribute data
                        XMLAttributeData *attb_data = new XMLAttributeData;
                        attb_data->name = attribute_name;
                        attb_data->value = attribute_value;

                        // Assign the attribute data
                        current_node->attribute_data->push_back(attb_data);
                    }
                }
                case irr::io::EXN_ELEMENT_END:
                {
                    const irr::c8 *node_name = xml->getNodeName();
                    if (strcmp(recurision_depth_tracker.getLast(), node_name) == 0)
                    {
                        depth_level--;
                        // We hit the end of an element, go back up one
                        current_node = last_node;
                        if (current_node != NULL)
                            last_node = current_node->parent;
                    }
                }

                // To quiet compiler warnings
                case irr::io::EXN_NONE:
                    break;
                case irr::io::EXN_TEXT:
                    break;
                case irr::io::EXN_COMMENT:
                    break;
                case irr::io::EXN_CDATA:
                    break;
                case irr::io::EXN_UNKNOWN:
                    break;
            } // End Switch Statement
        } // End While Statement

        recurision_depth_tracker.clear();
        delete xml;
    }

    //! Default Destructor
    CXMLLoader::~CXMLLoader(void)
    {
        // Get a reference to node_data as apparently indexing doesn't work with pointers
        irr::core::array<XMLNode*> node_data_temp = *node_data;

        // Loop through every parent node
        for (irr::u32 i = 0; i < node_data->size(); i++)
        {
            XMLNode *current_node = node_data_temp[i];
            current_node->attribute_data->clear();
            delete current_node->attribute_data;
            current_node->subnode_data->clear();
            delete current_node->subnode_data;
            free(current_node);
        }
        node_data->clear();
        delete node_data;
    }

    //! Return the stored node data
    irr::core::array<XMLNode*> CXMLLoader::getNodeData(void)
    {
        return *node_data;
    }
} // End Namespace Game
