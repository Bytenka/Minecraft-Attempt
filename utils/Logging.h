#pragma once
#include <iostream>

#define LOG_DEBUG1(x) std::cout << "[DEBUG1]:\t" << x << std::endl
#define LOG_INFO(x) std::cout << "[INFO]:\t" << x << std::endl
#define LOG_WARN(x) std::cout << "[WARN]:\t" << x << std::endl
#define LOG_ERROR(x) std::cout << "[ERROR]:\t" << x << std::endl; std::cout << "Press enter to continue ..."; std::cin.get()
#define LOG_CRITICAL_ERROR(x) std::cout << "[CRITICAL]:\t" << x << std::endl; std::cout << "Press enter to stop the application ..."; std::cin.get(); exit(-1)