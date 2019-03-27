#ifndef FILECONTROL_H
#define FILECONTROL_H

#include "../utils.h"

MYGIT_HEADER_BEGIN

#define FILE	0x1
#define FOLDER	0x2

int Create(const char *const path, int flags);

int Delete(const char *const path, int flags);

MYGIT_HEADER_END

#endif // FILECONTROL_H
