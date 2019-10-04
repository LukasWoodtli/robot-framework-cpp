

#include "../include/robotframeworkcpp.h"


    namespace {
        robotframeworkcpp::SetpDefinition<std::string> testStep1 = [](std::string args,
                                                             std::map<std::string, std::string> namedArgs) {
            return "hello";
        };

        robotframeworkcpp::SetpDefinition<int> testStep2 = [](std::string args, std::map<std::string, std::string> namedArgs) {
            return 3;
        };

        robotframeworkcpp::SetpDefinition<void> testStep3 = [](std::string args, std::map<std::string, std::string> namedArgs) {
            // no return
        };

    }




int main() {
    robotframeworkcpp::detail::input_data input;
    const auto stepResult1 = robotframeworkcpp::detail::runStepAndReturnString(testStep1, input);

    if (stepResult1 != "hello")
        return EXIT_FAILURE;


    const auto stepResult2 = robotframeworkcpp::detail::runStepAndReturnString(testStep2, input);

    if (stepResult2 != "3")
        return EXIT_FAILURE;


    const auto stepResult3 = robotframeworkcpp::detail::runStepAndReturnString(testStep3, input);

    if (stepResult3 != "")
        return EXIT_FAILURE;
}