#include <string>

#include "init.hpp"
#include "core/file/info.h"
#include "core/file/control.h"


bool Initialize()
{
	std::string cwd = GetCurrWorkDir();
	bool prevStatus = true;

	prevStatus = prevStatus ? Create((cwd += "/.mygit").c_str(), FOLDER) : false;
	prevStatus = prevStatus ? Create((cwd + "/objects").c_str(), FOLDER) : false;
	prevStatus = prevStatus ? Create((cwd + "/refs").c_str(), FOLDER) : false;
	prevStatus = prevStatus ? Create((cwd + "/HEAD").c_str(), FILE) : false;
	prevStatus = prevStatus ? Create((cwd + "/index").c_str(), FILE) : false;

	return prevStatus;
}
