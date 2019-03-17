#include "utils.h"

#include <stdlib.h>
#include <string.h>

#if defined(_WIN32) || defined(_WIN64)

#define MAX_PATH_LEN MAX_PATH

#elif defined(__linux__) || defined(__unix__)

#include <linux/limits.h>
#include <sys/stat.h>
#define MAX_PATH_LEN _POSIX_PATH_MAX 

#endif // _WIN32, _WIN64

static char **Realloc(char **src, size_t nElems, size_t *outNewNelems)
{
	size_t newSize = (size_t)(nElems * 1.68);
	char **newBuff = (char **)malloc(newSize * sizeof(char *));
	memcpy(newBuff, src, sizeof(char *) * nElems);
	free(src);

	char *newContentBuff = (char *)malloc(MAX_PATH_LEN * newSize * sizeof(char));
	memcpy(newContentBuff, *newBuff, MAX_PATH_LEN * nElems * sizeof(char));
	free(*newBuff);
	*newBuff = newContentBuff;

	for (size_t i = 1; i < newSize; i++) {
		newBuff[i] = newBuff[i - 1] + MAX_PATH_LEN;
	}

	*outNewNelems = newSize;
	return newBuff;
}


char **GetFileNames(const char *const dirName, size_t *outFileCount)
{
	size_t elemCount = 10, fileCounter = 0;
	char **fileNames = (char **)malloc(elemCount * sizeof(char *));
	*fileNames = (char *)malloc(MAX_PATH_LEN * elemCount * sizeof(char));

	for (size_t i = 1; i < elemCount; i++) {
		fileNames[i] = fileNames[i-1] + MAX_PATH_LEN;
	}
#if defined(_WIN32) || defined(_WIN64)
	HANDLE handler;
	WIN32_FIND_DATA fileInfo;

	handler = FindFirstFile(TEXT(dirName), &fileInfo);
	if (handler != INVALID_HANDLE_VALUE) {
		do {
			if (fileCounter == elemCount)
				fileNames = Realloc(fileNames, elemCount, &elemCount);

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

			if (strcmp(".", ent->d_name) != 0 && strcmp("..", ent->d_name) != 0)
				strcpy(fileNames[fileCounter++], ent->d_name);
		}
		closedir(dir);
		*outFileCount = fileCounter;
		return fileNames;
	}
	else {
		closedir(dir);
		return NULL;
	}
#endif // _WIN32, _WIN64
}

int IsDirectory(const char *const path)
{
#if defined(_WIN32) || defined(_WIN64)
	HANDLE handler;
	WIN32_FIND_DATA fileInfo;

	handler = FindFirstFile(TEXT(path), &fileInfo);
	if (handler != INVALID_HANDLE_VALUE)
		return fileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
	return -1;
#elif defined(__linux__) || defined(__unix__)
	struct stat stBuff;
	int status;

	status = stat(path, &stBuff);
	if (status != 0)
		return -1;
	return S_ISDIR(stBuff.st_mode);
#endif
}