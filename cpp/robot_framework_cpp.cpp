
#define ROBOT_CPP_CONFIG_MAIN
#include "include/robotframeworkcpp.h"


ROBOT_CPP_STEP(add) {
    const auto ret = args[0] + args[1];
    return ret.c_str();
}
