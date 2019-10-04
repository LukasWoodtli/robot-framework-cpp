
#define ROBOT_CPP_CONFIG_MAIN
#include "include/robotframeworkcpp.h"


ROBOT_CPP_STEP(add, args, namedArgs) {
    return args[0] + args[1];
}
