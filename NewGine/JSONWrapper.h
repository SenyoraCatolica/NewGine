#pragma once
#include "JSON\parson.h"
#include "MathGeoLib\include\MathGeoLib.h"

class JSONWrapper
{
public:
	JSONWrapper();
	JSONWrapper(JSON_Object* obj);
	~JSONWrapper();


	//Write
	bool WriteInt(const char* name, int value);
	bool WriteUInt(const char* name, unsigned int value);
	bool WriteFloat(const char* name, float value);
	bool WriteFloat3(const char* name, const float* value);
	bool WriteBool(const char* name, bool value);
	bool WriteString(const char* name, const char* string);
	bool WriteArray(const char* name);
	bool WriteArrayValue(const JSONWrapper& data);
	JSONWrapper WriteJSONObject(const char* name);
	bool WriteMatrix(const char * name, const math::float4x4 & matrix);

	//Read
	int ReadInt(const char* name)const;
	unsigned int ReadUInt(const char* name)const;
	float ReadeFloat(const char* name)const;
	float3 ReadFloat3(const char* name)const;
	bool ReadBool(const char* name)const;
	std::string ReadString(const char* name)const;
	JSONWrapper ReadArray(const char* name, unsigned int index)const;
	JSONWrapper ReadJSONObject(const char* name)const;
	float4x4 ReadMatrix(const char * name) const;


private:
	JSON_Object * root = nullptr;
	JSON_Value * root_value = nullptr;
	JSON_Array* array = nullptr;
};
