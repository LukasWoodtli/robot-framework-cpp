#pragma once

#include <string>
#include <sstream>
#include <algorithm>
#include <map>
#include <vector>
#include <functional>
#include <cstdlib>
#include <iostream>
#include <array>

#define ROBOT_CPP_STEP(name, ...) \
    std::string _robotframeworkcpp_##name##_function(); \
    robotframeworkcpp::stepDef _robotframeworkcpp_##name##_lambda = [](std::vector<std::string>) -> std::string { \
        return _robotframeworkcpp_##name##_function(); \
    }; \
    struct _robotframeworkcpp_##name##_static_init { \
        _robotframeworkcpp_##name##_static_init() {robotframeworkcpp::registerStepDef(#name, _robotframeworkcpp_##name##_lambda);} \
    }; \
    _robotframeworkcpp_##name##_static_init _robotframeworkcpp_##name##_static_init_obj; \
    std::string _robotframeworkcpp_##name##_function() // here comes the

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


    using stepDef = std::function<std::string(std::vector<std::string>)>;
    void registerStepDef(std::string name, stepDef step);
    std::string searchAndRunStep(std::string stepName, std::vector<std::string> args);
}


#ifdef ROBOT_CPP_CONFIG_MAIN

#ifdef _WIN32
#define ROBOT_CPP_EXPORT_SYMBOL __declspec(dllexport)
#else
#define ROBOT_CPP_EXPORT_SYMBOL __attribute__((visibility("default")))
#endif


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


namespace robotframeworkcpp {
        std::map<std::string, stepDef> _stepDefDict;

void registerStepDef(std::string name, stepDef step) {
    _stepDefDict[name] = step;
}

std::string searchAndRunStep(std::string stepName, std::vector<std::string> args) {
    // check if step is available in map
    return _stepDefDict[stepName](args);
}

}

extern "C" {

ROBOT_CPP_EXPORT_SYMBOL
char const * get_keyword_names() {
    return "add";
}

}
#endif /* CATCH_CONFIG_MAIN */
