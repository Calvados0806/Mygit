#ifndef UTILS_H
#define UTILS_H

#if defined(_WIN32) || defined(_WIN64)

#include <Windows.h>
#include <direct.h>
#define MAX_PATH_LEN MAX_PATH
#define __getcwd _getcwd

#elif defined(__linux__) || defined(__unix__)

#include <dirent.h>
#include <linux/limits.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define MAX_PATH_LEN _POSIX_PATH_MAX 
#define __getcwd getcwd

#else

#error "Not compliant platform"

#endif // _WIN32, _WIN64

#ifdef __cplusplus

#define MYGIT_HEADER_BEGIN extern "C" {
#define MYGIT_HEADER_END }

#else

#define MYGIT_HEADER_BEGIN
#define MYGIT_HEADER_END

#endif // __cplusplus

#endif // UTILS_H