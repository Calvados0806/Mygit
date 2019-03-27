#ifndef FILEINFO_H
#define FILEINFO_H

#include "../utils.h"

MYGIT_HEADER_BEGIN

struct DirInfo {
	int Amount;
	int Capacity;
	char (*Names)[MAX_PATH_LEN];
};

void FreeDirInfo(struct DirInfo *obj);

const char *FileName(struct DirInfo *obj, int index);

struct DirInfo *GetFileNames(const char *const path);

int IsDirectory(const char *const path);

MYGIT_HEADER_END

#endif // FILEINFO_H