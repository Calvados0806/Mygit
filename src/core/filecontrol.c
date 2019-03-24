#include "filecontrol.h"

#define FPERMS 0644 // -rw-r--r--
#define DPERMS 0755 // drwxr-xr-x

int Create(const char *const path, int flags)
{
	if (flags ^ (CREATE_FILE | CREATE_FOLDER)) {
#if defined(_WIN32) || defined(_WIN64)
		if (flags & CREATE_FILE)
			return CreateFile(
				path, 
				GENERIC_READ | GENERIC_WRITE, 
				0, NULL, 
				CREATE_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL, NULL
			) != INVALID_HANDLE_VALUE;
		else if (flags & CREATE_FOLDER)
			return CreateDirectory(path, NULL);
		else
			return 0;
#elif defined(__linux__) || defined(__unix__)
		if (flags & CREATE_FILE)
			return creat(path, FPERMS) != -1;
		else if (flags & CREATE_FOLDER)
			return mkdir(path, DPERMS) != -1;
		else
			return 0;
#endif // _WIN32, _WIN64
	}
	// CREATE_FILE and CREATE_FOLDER was set at the same time
	// Error
	return 0;
}
