#pragma once

#include "Module.h"
#include "Globals.h"
#include <vector>

struct SDL_RWops;
int close_sdl_rwops(SDL_RWops *rw);

class ModuleFileSystem : public Module
{
public:

	ModuleFileSystem(Application* app, bool start_enabled = true);
	~ModuleFileSystem();

	bool Init();
	bool CleanUp();

	bool AddPath(const char* path, const char* mount_point = NULL);
	bool Exists(const char* file) const;
	bool IsDirectory(const char* file) const;
	bool CreateDir(const char* dir);
	bool CopyFileToDir(const char * from, const char * to);


	//to check the files of a folder
	bool GetFilesFromPath(const char* path, std::vector<std::string>& output_files);
	std::string GetNameFromDirectory(std::string file);
	std::string ChangeExtension(std::string path, const char* extension);
	std::string DeleteExtensionFromName(std::string file);
	std::vector<std::string> GetFileNamesFromDirectory(const char* dir);

	// Open for Read/Write
	uint Load(const char* file, char** buffer) const;
	SDL_RWops* Load(const char* file) const;

	uint Save(const char* file, const char* buffer, uint size) const;
	bool Save(const char* file, const char* buffer, uint size, const char* folder, const char* extension);
};
