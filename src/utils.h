#ifndef UTILS_H
#define UTILS_H

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

#elif defined(__linux__) || defined(__unix__)

#include <dirent.h>

#else

#error "Not compliant platform"

#endif // _WIN32, _WIN64

#endif // UTILS_H