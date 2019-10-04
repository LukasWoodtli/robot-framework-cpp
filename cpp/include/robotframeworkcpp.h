#pragma once

#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <functional>
#include <cstdlib>
#include <iostream>

#define ROBOT_CPP_STEP(name, args, namedArgs)

namespace robotframeworkcpp {

        template<typename R>
        using SetpDefinition = std::function<R(std::string, std::map<std::string, std::string>)>;


        // inspired by catch2
        template<typename T>
        struct StringMaker {
            static std::string convert(T const &&value) {
                std::stringstream str;
                str << value;
                return str.str();
            }
        };


        namespace detail {
            struct input_data {
                std::string args;
                std::map<std::string, std::string> namedArgs;
            };

            struct return_data {
                std::string stdOutputAndError;
                std::string returnValue;
                std::string exceptionText;
            };

            class RedirectCoutAndCerr {
            public:
                RedirectCoutAndCerr() {
                    // inspired by
                    // https://stackoverflow.com/a/6211644
                    oldCoutStreamBuf = std::cout.rdbuf();
                    oldCerrStreamBuf = std::cerr.rdbuf();
                    std::cout.rdbuf(strCoutAndCerr.rdbuf());
                    std::cerr.rdbuf(strCoutAndCerr.rdbuf());
                }

                std::string getCoutAndCerrText() const {
                   return strCoutAndCerr.str();
                }

                ~RedirectCoutAndCerr() {
                    std::cout.rdbuf(oldCoutStreamBuf);
                    std::cerr.rdbuf(oldCerrStreamBuf);
                }

            private:
                std::ostringstream strCoutAndCerr;
                std::streambuf* oldCoutStreamBuf;
                std::streambuf* oldCerrStreamBuf;
            };

            template<typename R>
            std::string runStepAndReturnString(SetpDefinition<R> stepDefinition, const input_data &inputData) {
                return StringMaker<R>::convert(stepDefinition(inputData.args, inputData.namedArgs));
            }

            template<>
            std::string runStepAndReturnString<void>(SetpDefinition<void> stepDefinition, const input_data &inputData) {
                stepDefinition(inputData.args, inputData.namedArgs);
                return "";
            }


            template<typename R>
            return_data runStep(std::string args,
                                std::map<std::string, std::string> namedArgs) {
                input_data input;
                input.args = args;
                input.namedArgs = namedArgs;

                return_data ret;

                RedirectCoutAndCerr redirect;

                try {
                    ret.returnValue = runStepAndReturnString<R>(input);
                }
                catch (const std::exception& except) {
                    ret.exceptionText = except.what();
                }
                catch (...) {
                    ret.exceptionText = "robotframeworkcpp: unknown exception occurred";
                    throw;
                }

                ret.stdOutputAndError = redirect.getCoutAndCerrText();

                return ret;
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

namespace {
        // inspired by:
        // https://gist.github.com/rodamber/2558e25d4d8f6b9f2ffdf7bd49471340
        inline std::string convertToLowerCaseSnake(std::string &&camelCase) {
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
#endif /* CATCH_CONFIG_MAIN */
