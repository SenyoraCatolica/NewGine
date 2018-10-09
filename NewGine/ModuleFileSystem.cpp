#include "ModuleFileSystem.h"
#include "SDL\include\SDL.h"

#include "PhysFS\include\physfs.h"
#pragma comment (lib, "PhysFS/libx86/physfs.lib")


ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled = true) : Module(app, start_enabled) 
{
	name = "file_system";

	if (PHYSFS_isInit() == 0)
	{
		char* base_path = SDL_GetBasePath();
		PHYSFS_init(base_path);
		SDL_free(base_path);

		AddPath(".\Game");
	}
}

ModuleFileSystem::~ModuleFileSystem() 
{
	PHYSFS_deinit();
}

bool ModuleFileSystem::Init()
{

}
bool ModuleFileSystem::CleanUp()
{

}

bool ModuleFileSystem::AddPath(const char* path, const char* mount_point = NULL)
{
	bool ret = false;

	if (PHYSFS_mount(path, mount_point, 1) == 0)
	{
		LOG("File System error while adding a path or zip(%s): %s\n", path, PHYSFS_getLastError());
	}
	else
	{
		LOG("Added new path to the File System: %s\n", path);
		ret = true;
	}

	return ret;
}

bool ModuleFileSystem::Exists(const char* file) const
{
	return PHYSFS_exists(file) != 0;
}

bool ModuleFileSystem::IsDirectory(const char* file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

bool ModuleFileSystem::CreateDir(const char* dir)
{
	if (!PHYSFS_mkdir(dir))
	{
		LOG("Could not create directory: %s", PHYSFS_getLastError());
		return false;
	}

	LOG("New directory created: %s", dir);
	return true;
}

bool ModuleFileSystem::GetFilesFromPath(const char* path, std::vector<std::string>& output_files)
{
	char** ef = PHYSFS_enumerateFiles(path);

	for (char** i = ef; *i != NULL; i++)
	{
		output_files.push_back(*i);
	}

	PHYSFS_freeList(ef);

	return (ef != NULL) ? true : false;
}


std::string ModuleFileSystem::GetNameFromDirectory(std::string file)
{
	size_t final = file.find_last_of("\\");
	file = file.substr(final + 1);
	return file;
}




// Open for Read/Write
uint ModuleFileSystem::Load(const char* file, char** buffer) const
{
	uint ret = 0;

	PHYSFS_file* open_file = PHYSFS_openRead(file);
	if (open_file != nullptr)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(open_file);
		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 read_file = PHYSFS_read(open_file, *buffer, 1, (PHYSFS_sint32)size);

			if(read_file > 0)
				ret = (uint)read_file;

			else
			{
				RELEASE(buffer);
				LOG("WARNING! error while reading file %s: %s\n", file, PHYSFS_getLastError());
			}

			PHYSFS_close(open_file);
		}
	}

	else
		LOG("WARNING! error while opening file %s: %s\n", file, PHYSFS_getLastError());

}

SDL_RWops* ModuleFileSystem::Load(const char* file) const
{
	SDL_RWops *ret = NULL;
	char *buffer;
	uint size = Load(file, &buffer);

	if (size > 0)
	{
		ret = SDL_RWFromConstMem(buffer, size);
		if (ret != NULL)
			ret->close = close_sdl_rwops;
	}

	return ret;
}

uint ModuleFileSystem::Save(const char* file, const char* buffer, uint size) const
{
	uint ret = 0;

	PHYSFS_file* open_file = PHYSFS_openWrite(file);

	if (open_file != nullptr)
	{
		PHYSFS_sint64 write_file = PHYSFS_write(open_file, (const void*)buffer, 1, size);
		if (write_file != size)
		{
			LOG("WRNING! error while writing file %s: %s\n", file, PHYSFS_getLastError());
		}
		else
			ret = (uint)write_file;

		PHYSFS_close(open_file);
	}
	else
		LOG("WARNIN! error while opening file %s: %s\n", file, PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::Save(const char* file, const char* buffer, uint size, const char* folder, const char* extension)
{
	bool ret = false;

	char complete_name[100];
	sprintf_s(complete_name, 100, "%s.%s", file, extension);


	std::vector<std::string> files;
	GetFilesFromPath(folder, files);
	std::vector<std::string>::iterator it = files.begin();

	uint copies = 0;

	while (it != files.end())
	{
		if ((*it).compare(complete_name) == 0)
		{
			copies++;
			//Add the number of the copy to the original name to distinguix
			sprintf_s(complete_name, 100, "%s%d.%s", file, copies, extension);
			break;
		}
	}

	char final_name[500];
	sprintf_s(final_name, 500, "%s%s", folder, complete_name);

	ret = Save(final_name, buffer, size);

	return ret;
}
