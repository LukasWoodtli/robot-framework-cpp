

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#define ROBOT_CPP_CONFIG_MAIN
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




TEST_CASE("Basic tests") {
    robotframeworkcpp::detail::input_data input;
    
    
    const auto stepResult1 = robotframeworkcpp::detail::runStepAndReturnString(testStep1, input);

    REQUIRE(stepResult1 == "hello");


    const auto stepResult2 = robotframeworkcpp::detail::runStepAndReturnString(testStep2, input);

    REQUIRE(stepResult2 == "3");


    const auto stepResult3 = robotframeworkcpp::detail::runStepAndReturnString(testStep3, input);

    REQUIRE(stepResult3 == "");
}


TEST_CASE("Test Add Step") {

    robotframeworkcpp::registerStepDef("add", [](std::vector<std::string> args){return std::to_string(std::stoi(args[0]) + std::stoi(args[1]));});

    SECTION("1+2=3")
    {
        std::vector<std::string> args{"1", "2"};
        const auto res = robotframeworkcpp::searchAndRunStep("add", args);

        REQUIRE(res == "3");
    }
}

TEST_CASE("Test Mul Step") {

    robotframeworkcpp::registerStepDef("mul", [](std::vector<std::string> args){return std::to_string(std::stoi(args[0]) * std::stoi(args[1]));});

    std::vector<std::string> args{"3", "2"};
    const auto res = robotframeworkcpp::searchAndRunStep("mul", args);

    REQUIRE(res == "6");
}
