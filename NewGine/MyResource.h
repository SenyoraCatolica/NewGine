#pragma once

#include "Globals.h"
#include <string>

class MyResource
{
public:
	enum R_STATE{LOADED, UNLOADED, TO_DELETE, TO_REIMPORT};
	enum R_TYPE{NONE, MESH, MATERIAL, FOLDER};

	MyResource(uint uuid, R_TYPE type, R_STATE state);
	virtual ~MyResource();

	//getters
	R_STATE GetState();
	R_TYPE GetType();

	uint64_t GetUUID() const;

	//setters
	void SetState(R_STATE state);
	void SetType(R_TYPE type);

protected:
	R_TYPE type;
	R_STATE state;
	uint64_t uuid;

public:
	char name[NAME_MAX_LEN];
	std::string path;
};
