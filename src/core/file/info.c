#include "info.h"

#include <stdlib.h>
#include <string.h>


static void InitDirInfo(struct DirInfo *obj)
{
	obj->Amount = 0;
	obj->Capacity = 10;
	obj->Names = (char(*)[MAX_PATH_LEN])malloc(sizeof(char[MAX_PATH_LEN]) * obj->Capacity);
}


static void Realloc(struct DirInfo *obj)
{
	int newSize = (int)(obj->Capacity * 1.68);
	char (*newBuff)[MAX_PATH_LEN] = (char(*)[MAX_PATH_LEN])malloc(sizeof(char[MAX_PATH_LEN]) * newSize);
	memcpy(newBuff, obj->Names, sizeof(char[MAX_PATH_LEN])*obj->Amount);
	free(obj->Names);
	obj->Names = newBuff;
	obj->Capacity = newSize;
}


void FreeDirInfo(struct DirInfo *obj)
{
	free(obj->Names);
	free(obj);
}


const char *FileName(struct DirInfo *obj, int index)
{
	if (index >= 0 && index < obj->Amount)
		return obj->Names[index];
	return NULL;
}


struct DirInfo *GetFileNames(const char *const path)
{
	struct DirInfo *info = (struct DirInfo *)malloc(sizeof(struct DirInfo));
	InitDirInfo(info);

#if defined(_WIN32) || defined(_WIN64)
	HANDLE handler;
	WIN32_FIND_DATA fileInfo;

	handler = FindFirstFile(TEXT(path), &fileInfo);
	if (handler != INVALID_HANDLE_VALUE) {
		do {
			if (info->Amount == info->Capacity)
				Realloc(info);

			if (strcmp(".", fileInfo.cFileName) != 0 && strcmp("..", fileInfo.cFileName) != 0)
				strcpy(info->Names[info->Amount++], fileInfo.cFileName);
		} while (FindNextFile(handler, &fileInfo));

		FindClose(handler);
		return info;
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
			if (info->Amount == info->Capacity)
				Realloc(info);

			if (strcmp(".", ent->d_name) != 0 && strcmp("..", ent->d_name) != 0)
				strcpy(info->Names[info->Amount++], ent->d_name);
		}
		closedir(dir);
		return info;
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
	if (status == 0)
		return S_ISDIR(stBuff.st_mode);
	return -1;
#endif
}

const char *GetCurrWorkDir()
{
	static char cwd[MAX_PATH_LEN];
	return __getcwd(cwd, MAX_PATH_LEN);
}
