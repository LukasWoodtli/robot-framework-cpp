#pragma once

#include <string>
#include <sstream>
#include <algorithm>

namespace robot_framework_cpp {

namespace {
        // inspired by:
        // https://gist.github.com/rodamber/2558e25d4d8f6b9f2ffdf7bd49471340
        inline std::string convert_to_lower_snake_case(std::string &&camelCase) {
            std::stringstream strstream;
            strstream << tolower(camelCase[0]);

            // underscores between lower and upper case letters
            for (auto it = camelCase.begin() + 1; it != camelCase.end(); ++it) {
                if (isupper(*it) && *(it-1) != '_' && islower(*(it-1))) {
                    strstream << "_";
                }
                strstream << *it;
            }

            auto str = strstream.str();
            std::transform(str.begin(), str.end(), str.begin(), ::tolower);

            return str;
        }
}
}

#ifdef ROBOT_CPP_CONFIG_MAIN

#ifdef _WIN32
#define ROBOT_CPP_EXPORT_SYMBOL __declspec(dllexport)
#else
#define ROBOT_CPP_EXPORT_SYMBOL __attribute__((visibility("default")))
#endif

extern "C" {

ROBOT_CPP_EXPORT_SYMBOL
char const * get_keyword_names() {
    return "add";
}


}
#endif /* CATCH_CONFIG_MAIN */
