

#include "ModuleFileSystem.h"
#include "Application.h"

#include "SDL\include\SDL.h"

#include "PhysFS\include\physfs.h"
#pragma comment (lib, "PhysFS/libx86/physfs.lib")

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	if (PHYSFS_isInit() == 0)
	{
		char *base_path = SDL_GetBasePath();
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
	LOG("Loading File System");


	bool ret = true;

	char *write_path = SDL_GetPrefPath(App->GetName(), App->GetOrder());

	if (PHYSFS_setWriteDir(write_path) == 0)
	{
		LOG("File System error while creating write dir: %s", PHYSFS_getLastError());
		ret = false;
	}
	else
	{
		LOG("Writing directory is %s", write_path);
		AddPath(write_path, GetSaveDirectory());
	}

	SDL_free(write_path);

	// Generate IO interfaces
	CreateAssimpIO();

	return ret;
}

bool ModuleFileSystem::CleanUp()
{
	char **paths;
	for (paths = PHYSFS_getSearchPath(); *paths != NULL; paths++)
	{
		PHYSFS_removeFromSearchPath(*paths);
	}

	delete AssimpIO;

	return true;
}

bool ModuleFileSystem::AddPath(const char *path_or_zip, const char *mount_point)
{
	bool ret = true;

	if (PHYSFS_mount(path_or_zip, mount_point, 1) == 0)
	{
		LOG("Failure on adding path : %s", path_or_zip);
		ret = false;
	}

	return ret;
}

/*bool ModuleFileSystem::RemoveAllSearchPaths()
{
	bool ret = false;

	char **paths;
	for (paths = PHYSFS_getSearchPath(); *paths != NULL; paths++)
	{
		PHYSFS_removeFromSearchPath(*paths);
	}

	if (*(paths = PHYSFS_getSearchPath()) == NULL)
		ret = true;

	PHYSFS_freeList(paths);

	return ret;
}

bool ModuleFileSystem::RemovePath(const char *path_or_zip)
{
	bool ret = true;

	if (PHYSFS_removeFromSearchPath(path_or_zip) == 0)
	{
		DEBUG("%s %s", "Failure on removing directory or file on search path", path_or_zip);
		DEBUG("%s", "Error:", PHYSFS_getLastError());
		ret = false;
	}

	return ret;
}*/

uint ModuleFileSystem::Load(const char* file, char **buffer) const
{
	uint ret = 0;

	PHYSFS_file* file_tmp = PHYSFS_openRead(file);

	if (file_tmp != NULL)
	{
		PHYSFS_sint64 size = PHYSFS_fileLength(file_tmp);

		if (size > 0)
		{
			*buffer = new char[(uint)size];
			PHYSFS_sint64 mem_read = PHYSFS_read(file_tmp, *buffer, 1, size);

			if (mem_read != size)
			{
				LOG("File system error while reading from file %s: %s", file, PHYSFS_getLastError());

				if (buffer)
					delete buffer;
			}
			else
				ret = (uint)mem_read;
		}

		if (PHYSFS_close(file_tmp) == 0)
			LOG("Error while closing file  %s . Error: %s", file, PHYSFS_getLastError());
	}

	else
	{
		LOG("Error while loading file %s . Error: %", file, PHYSFS_getLastError());
	}

	return ret;
}

SDL_RWops *ModuleFileSystem::Load(const char *file) const
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

int close_sdl_rwops(SDL_RWops *rw)
{
	if (rw->hidden.mem.base != nullptr)
		delete rw->hidden.mem.base;

	SDL_FreeRW(rw);
	return 0;
}

uint ModuleFileSystem::Save(const char *file, const char *buffer, uint size) const
{
	uint ret = 0;

	PHYSFS_file *file_tmp = PHYSFS_openWrite(file);

	if (file_tmp != NULL)
	{
		PHYSFS_sint64 mem_written = PHYSFS_write(file_tmp, (const void*)buffer, 1, size);

		if (mem_written != size)
		{
			LOG("Failure on writing in %s. Error: %s", file, PHYSFS_getLastError());
		}
		else
			ret = (uint)mem_written;

		if (PHYSFS_close(file_tmp) == 0)
			LOG("Error while closing file %s. Error: %s", file, PHYSFS_getLastError());
	}
	else
		LOG("Error while loading file %s . Error: %", file, PHYSFS_getLastError());

	return ret;
}

bool ModuleFileSystem::IsDirectory(const char *file) const
{
	return PHYSFS_isDirectory(file) != 0;
}

bool ModuleFileSystem::Exists(const char *file) const
{
	return PHYSFS_exists(file) != 0;
}

const char *ModuleFileSystem::GetSaveDirectory() const
{
	return "save/";
}

// -----------------------------------------------------
// ASSIMP IO
// -----------------------------------------------------

size_t AssimpWrite(aiFile* file, const char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_write((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if (ret == -1)
		//LOG("File System error while WRITE via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpRead(aiFile* file, char* data, size_t size, size_t chunks)
{
	PHYSFS_sint64 ret = PHYSFS_read((PHYSFS_File*)file->UserData, (void*)data, size, chunks);
	if (ret == -1)
		LOG("File System error while READ via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpTell(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_tell((PHYSFS_File*)file->UserData);
	if (ret == -1)
		LOG("File System error while TELL via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

size_t AssimpSize(aiFile* file)
{
	PHYSFS_sint64 ret = PHYSFS_fileLength((PHYSFS_File*)file->UserData);
	if (ret == -1)
		LOG("File System error while SIZE via assimp: %s", PHYSFS_getLastError());

	return (size_t)ret;
}

void AssimpFlush(aiFile* file)
{
	if (PHYSFS_flush((PHYSFS_File*)file->UserData) == 0)
		LOG("File System error while FLUSH via assimp: %s", PHYSFS_getLastError());
}

aiReturn AssimpSeek(aiFile* file, size_t pos, aiOrigin from)
{
	int res = 0;

	switch (from)
	{
	case aiOrigin_SET:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, pos);
		break;
	case aiOrigin_CUR:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_tell((PHYSFS_File*)file->UserData) + pos);
		break;
	case aiOrigin_END:
		res = PHYSFS_seek((PHYSFS_File*)file->UserData, PHYSFS_fileLength((PHYSFS_File*)file->UserData) + pos);
		break;
	}

	if (res == 0)
		LOG("File System error while SEEK via assimp: %s", PHYSFS_getLastError());

	return (res != 0) ? aiReturn_SUCCESS : aiReturn_FAILURE;
}

aiFile* AssimpOpen(aiFileIO* io, const char* name, const char* format)
{
	static aiFile file;

	file.UserData = (char*)PHYSFS_openRead(name);
	file.ReadProc = AssimpRead;
	file.WriteProc = AssimpWrite;
	file.TellProc = AssimpTell;
	file.FileSizeProc = AssimpSize;
	file.FlushProc = AssimpFlush;
	file.SeekProc = AssimpSeek;

	return &file;
}

void AssimpClose(aiFileIO* io, aiFile* file)
{
	if (PHYSFS_close((PHYSFS_File*)file->UserData) == 0)
		LOG("File System error while CLOSE via assimp: %s", PHYSFS_getLastError());
}

void ModuleFileSystem::CreateAssimpIO()
{
	if (AssimpIO)
	{
		delete AssimpIO;
		AssimpIO = nullptr;
	}

	AssimpIO = new aiFileIO;
	AssimpIO->OpenProc = AssimpOpen;
	AssimpIO->CloseProc = AssimpClose;
}

aiFileIO * ModuleFileSystem::GetAssimpIO()
{
	return AssimpIO;
}
