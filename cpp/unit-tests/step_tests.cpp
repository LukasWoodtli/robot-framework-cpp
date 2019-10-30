

#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>

#define ROBOT_CPP_CONFIG_MAIN
#include "../include/robotframeworkcpp.h"



ROBOT_CPP_STEP(diff) {
    return "2";
}



TEST_CASE("Test Div Step") {

    robotframeworkcpp::registerStepDef("add", [](std::vector<std::string> args){return std::to_string(std::stoi(args[0]) + std::stoi(args[1]));});

    SECTION("4/2=2")
    {
        std::vector<std::string> args{"4", "2"};
        const auto res = robotframeworkcpp::searchAndRunStep("diff", args);

        REQUIRE(res == "2");
    }
}


