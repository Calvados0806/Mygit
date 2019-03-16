#include "utils.h"

#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)

#define MAX_PATH_LEN MAX_PATH

#elif defined(__linux__) || defined(__unix__)

#include <linux/limits.h>
#define MAX_PATH_LEN _POSIX_PATH_MAX 

#endif // _WIN32, _WIN64

static size_t Realloc(char **src, size_t nElems)
{
	char **newBuff = (char **)malloc((size_t)(nElems * 1.68) * sizeof(char *));
	memcpy(src, newBuff, sizeof(char *) * nElems);
	free(src);
	src = newBuff;
	return (size_t)(nElems * 1.68);
}


char **GetFileNames(const char *const dirName, size_t *outFileCount)
{
	size_t elemCount = 10, fileCounter = 0;
	char **fileNames = (char **)malloc(elemCount * sizeof(char *));
	*fileNames = (char *)malloc(MAX_PATH_LEN * elemCount * sizeof(char));

	for (size_t i = 1; i < elemCount; i++) {
		fileNames[i] = fileNames[i-1] + MAX_PATH;
	}
#if defined(_WIN32) || defined(_WIN64)
	HANDLE handler;
	WIN32_FIND_DATA fileInfo;

	handler = FindFirstFile(TEXT(dirName), &fileInfo);
	if (handler != INVALID_HANDLE_VALUE) {
		do {
			if (fileCounter == elemCount)
				elemCount = Realloc(fileNames, elemCount);

			if (strcmp(".", fileInfo.cFileName) != 0 && strcmp("..", fileInfo.cFileName) != 0)
				strcpy_s(fileNames[fileCounter++], MAX_PATH, fileInfo.cFileName);
		} while (FindNextFile(handler, &fileInfo));

		*outFileCount = fileCounter;
		FindClose(handler);
		return fileNames;
	}
	else {
		FindClose(handler);
		return NULL;
	}
#elif defined(__linux__) || defined(__unix__)
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir(dirName)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (fileCounter == elemCount)
				elemCount = Realloc(fileNames, elemCount);

			if (strcmp(".", fileInfo.cFileName) != 0 && strcmp("..", fileInfo.cFileName) != 0)
				strcpy(fileNames[fileCounter++], ent->d_name);
		}
		closedir(dir);
		return NULL;
	}
	else {
		closedir(dir);
		return NULL;
	}
#endif // _WIN32, _WIN64
}