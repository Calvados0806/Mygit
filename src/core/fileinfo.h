#ifndef FILEINFO_H
#define FILEINFO_H

#include "utils.h"

struct DirInfo {
	int Amount;
	int Capacity;
	char **Names;
};

#ifdef __cplusplus
extern "C" {
#endif

void FreeDirInfo(struct DirInfo *obj);

const char *FileName(struct DirInfo *obj, int index);

struct DirInfo *GetFileNames(const char *const path);

int IsDirectory(const char *const path);

#ifdef __cplusplus
}
#endif

#endif // FILEINFO_H