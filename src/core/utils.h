#ifndef UTILS_H
#define UTILS_H

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>
#define MAX_PATH_LEN MAX_PATH

#elif defined(__linux__) || defined(__unix__)

#include <dirent.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#define MAX_PATH_LEN _POSIX_PATH_MAX 

#else

#error "Not compliant platform"

#endif // _WIN32, _WIN64

#endif // UTILS_H