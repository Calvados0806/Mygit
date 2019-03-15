#include "utils.h"

#include <stdlib.h>
#include <string.h>


char **GetFileNames(const char *const dirName, size_t *outFileCount)
{
	size_t startCount = 10, fileCounter = 0;
	char **fileNames = (char **)malloc(startCount * sizeof(char *));
	*fileNames = (char *)malloc(MAX_PATH * startCount * sizeof(char));

	for (int i = 1; i < startCount; i++) {
		fileNames[i] = fileNames[i-1] + MAX_PATH;
	}
#if defined(_WIN32) || defined(_WIN64)
	HANDLE handler;
	WIN32_FIND_DATA fileInfo;

	handler = FindFirstFile(TEXT(dirName), &fileInfo);
	if (handler != INVALID_HANDLE_VALUE) {
		do {
			if (fileCounter == startCount) {
				char **newBuff = (char**)malloc((size_t)(startCount * 1.68) * sizeof(char*));
				memcpy(fileNames, newBuff, sizeof(char*) * startCount);
				free(fileNames);
				fileNames = newBuff;
				startCount = (size_t)(startCount * 1.68);
			}
			if (strcmp(".", fileInfo.cFileName) != 0 && strcmp("..", fileInfo.cFileName) != 0)
				strcpy_s(fileNames[fileCounter++], MAX_PATH, fileInfo.cFileName);
		} while (FindNextFile(handler, &fileInfo));

		*outFileCount = fileCounter;
		return fileNames;
	}
	else
		return NULL;
#elif defined(__linux__) || defined(__unix__)



#endif // _WIN32, _WIN64
}