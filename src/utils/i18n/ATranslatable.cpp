/*
ATranslatable.cpp
Author : BAKFR
File under GNU GPL v3.0 license
*/
#include "./ATranslatable.hpp"

#include "./Translator.hpp"

namespace Utils {
    namespace I18n {

        ATranslatable::ATranslatable() {
            Translator::getInstance().subscribe(this);
        }

        ATranslatable::~ATranslatable() {
            Translator::getInstance().unsubscribe(this);
        }

    } // namespace I18n
} // namespace Utils
