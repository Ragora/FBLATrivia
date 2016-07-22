/*
*   CQuestionLoader.h
*   Question loading code for PA:FBLA Trivia
*   Copyright (c) 2012 Robert MacGregor
*/

#ifndef _INCLUDE_CQUESTIONLOADER_H_
#define _INCLUDE_CQUESTIONLOADER_H_

#include <fstream>

#include <irrlicht.h>

#include "CReference.h"

//! Root Game Namespace
namespace Game
{
    //! Question Class
    class CQuestion
    {
        public:
        //! Default Constructor
        CQuestion(void) { }

        char display_text[256];
        char correct_answer_text[256];
        char wrong_answer_first[256];
        char wrong_answer_second[256];
        char wrong_answer_third[256];
    };
    //! Question Loading class
    class CQuestionLoader : public CReference
    {
        public:
        //! Default Constructor
        CQuestionLoader(const irr::c8 *target);
        //! Default Destructor
        ~CQuestionLoader(void);

        //! Returns the question list
        irr::core::array<CQuestion*> getQuestionList(void);
        private:

        irr::core::array<CQuestion*> question_list;
    }; // End Class CQuestionLoader
} // End namespace Game
#endif // _INCLUDE_CQUESTIONLOADER_H_
