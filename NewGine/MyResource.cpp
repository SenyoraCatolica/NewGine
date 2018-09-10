#include "MyResource.h"

MyResource::MyResource(uint uuid, R_TYPE type, R_STATE state)
{

}

MyResource::~MyResource()
{

}

//getters
MyResource::R_STATE MyResource::GetState()
{
	return state;
}

MyResource::R_TYPE MyResource::GetType()
{
	return type;
}

uint64_t MyResource::GetUUID() const
{
	return uuid;
}

//setters
void MyResource::SetState(R_STATE state)
{
	this->state = state;
}
void MyResource::SetType(R_TYPE type)
{
	this->type = type;
}