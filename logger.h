#pragma once

#if CMAKE_BUILD_TYPE == Debug
#include <iostream>
#define LOG(X) std::cout << X << std::endl;
#else
#define LOG(X)
#endif
