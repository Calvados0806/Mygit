#include "filecontrol.h"

#define FPERMS 0644 // -rw-r--r--
#define DPERMS 0755 // drwxr-xr-x

int Create(const char *const path, int flags)
{
	if (flags ^ (FILE | FOLDER)) {
#if defined(_WIN32) || defined(_WIN64)
		if (flags & FILE)
			return CreateFile(
				path, 
				GENERIC_READ | GENERIC_WRITE, 
				0, NULL, 
				CREATE_ALWAYS, 
				FILE_ATTRIBUTE_NORMAL, NULL
			) != INVALID_HANDLE_VALUE;
		else if (flags & FOLDER)
			return CreateDirectory(path, NULL);
		else
			return 0;
#elif defined(__linux__) || defined(__unix__)
		if (flags & FILE)
			return creat(path, FPERMS) != -1;
		else if (flags & FOLDER)
			return mkdir(path, DPERMS) != -1;
		else
			return 0;
#endif // _WIN32, _WIN64
	}
	// FILE and FOLDER was set at the same time
	// Error
	return 0;
}

int Delete(const char *const path, int flags)
{
	if (flags ^ (FILE | FOLDER)) {
#if defined(_WIN32) || defined(_WIN64)
		if (flags & FILE)
			return DeleteFile(path);
		else if (flags & FOLDER)
			return RemoveDirectory(path);
		else
			return 0;
#elif defined(__linux__) || defined(__unix__)
		if (flags & FILE)
			return unlink(path) != -1;
		else if (flags & FOLDER)
			return rmdir(path) != -1;
		else
			return 0;
#endif
	}

	return 0;
}