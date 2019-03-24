#ifndef FILECONTROL_H
#define FILECONTROL_H

#include "utils.h"

#ifdef __cplusplus
extern "C" {
#endif

#define FILE	0x1
#define FOLDER	0x2

int Create(const char *const path, int flags);

int Delete(const char *const path, int flags);

#ifdef __cplusplus
}
#endif

#endif // FILECONTROL_H
