/*
*   CQuestionLoader.cpp
*   Main code for the question loader
*   Copyright (c) 2012 Robert MacGregor
*/

#include <CQuestionLoader.h>
#include <iostream>

//! Root Game Namespace
namespace Game
{
    //! Default Constructor
    CQuestionLoader::CQuestionLoader(const irr::c8 *target) 
    {

        std::ifstream in_data (target);
        irr::u32 line_number = 0;

		// FIXME: This code will read up to 256 bytes of data -- no more
		irr::c8 line_data[256];
        if (in_data.is_open())
        {
			while (in_data.good())
            {
				// Reads up to newline
				in_data.getline(line_data, 256, '\n');
                // We read a line, process it for information
				std::string line_string_data = line_data;

                irr::u32 data_length = line_string_data.length();                                           // Finds the length of our line of data
                irr::u32 question_end = line_string_data.find("\t", 0);                                     // Finds the end of our question on this line
                irr::u32 correct_answer_end = line_string_data.find("\t", question_end+1);                  // Finds the end of the correct answer
                irr::u32 first_wrong_answer_end = line_string_data.find("\t", correct_answer_end+1);        // Finds the end of the 1st wrong answer
                irr::u32 second_wrong_answer_end = line_string_data.find("\t", first_wrong_answer_end+1);   // Finds the end of the 2nd wrong answer

                // Locates the actual text data for our question display, correct answer and wrong answers
                std::string question_display_text = line_string_data.substr(0, question_end);
                std::string question_correct_answer_text = line_string_data.substr(question_end+1, (correct_answer_end-question_end)-1);
                std::string question_first_wrong_answer_text = line_string_data.substr(correct_answer_end+1, (first_wrong_answer_end-correct_answer_end)-1);

				std::string question_second_wrong_answer_text = line_string_data.substr(first_wrong_answer_end+1, (second_wrong_answer_end-first_wrong_answer_end)-1);
				std::string question_last_wrong_answer_text = line_string_data.substr(second_wrong_answer_end+1, data_length);

                CQuestion *question = new CQuestion;

                // Assigns all of the proper data, now
				memset(question->display_text, 0x00, 255);
                memcpy(question->display_text, question_display_text.c_str(), 255);

				memset(question->correct_answer_text, 0x00, 255);
                memcpy(question->correct_answer_text, question_correct_answer_text.c_str(), 255);

				memset(question->wrong_answer_first, 0x00, 255);
                memcpy(question->wrong_answer_first, question_first_wrong_answer_text.c_str(), 255);

				memset(question->wrong_answer_second, 0x00, 255);
                memcpy(question->wrong_answer_second, question_second_wrong_answer_text.c_str(), 255);

				memset(question->wrong_answer_third, 0x00, 255);
                memcpy(question->wrong_answer_third, question_last_wrong_answer_text.c_str(), 255);

                question_list.push_back(question);

                line_number++;
            }
        }
    } // End Default Constructor

    //! Default Destructor
    CQuestionLoader::~CQuestionLoader(void)
    {
        irr::core::array<CQuestion*> list = getQuestionList();
        for (irr::u32 i = 0; i < list.size(); i++)
            delete list[i];
        list.clear();
    } // End Default Destructor

    //! Returns the question list
    irr::core::array<CQuestion*> CQuestionLoader::getQuestionList(void)
    {		
        return question_list;
    }
} // End namespace Game
