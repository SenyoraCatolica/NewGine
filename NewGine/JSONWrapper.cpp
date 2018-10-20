#include "JSONWrapper.h"

JSONWrapper::JSONWrapper()
{
	root_value = json_value_init_object();
	root = json_value_get_object(root_value);
}
JSONWrapper::JSONWrapper(JSON_Object* obj) : root(root)
{

}

JSONWrapper::~JSONWrapper()
{
	json_value_free(root_value);
}

//Write===========================================================================

bool JSONWrapper::WriteInt(const char* name, int value)
{
	return json_object_set_number(root, name, value) == JSONSuccess;
}
bool JSONWrapper::WriteUInt(const char* name, unsigned int value)
{
	return json_object_set_number(root, name, (double)value) == JSONSuccess;
}

bool JSONWrapper::WriteFloat(const char* name, float value)
{
	return json_object_set_number(root, name, (double)value) == JSONSuccess;
}

bool JSONWrapper::WriteFloat3(const char* name, const float* value)
{
	JSON_Value* json_value = json_value_init_array();
	JSON_Array* array = json_value_get_array(json_value);

	for (int i = 0; i < 3; i++)
		json_array_append_number(array, value[i]);

	return json_object_set_value(root, name, json_value) == JSONSuccess;
}

bool JSONWrapper::WriteBool(const char* name, bool value)
{
	return json_object_set_boolean(root, name, value) == JSONSuccess;
}

bool JSONWrapper::WriteString(const char* name, const char* string)
{
	return json_object_set_string(root, name, string) == JSONSuccess;
}

bool JSONWrapper::WriteArray(const char* name)
{
	JSON_Value* value = json_value_init_array();
	array = json_value_get_array(value);
	return json_object_set_value(root, name, value) == JSONSuccess;
}

bool JSONWrapper::WriteArrayValue(const JSONWrapper& data)
{
	if (array)
		return json_array_append_value(array, json_value_deep_copy(data.root_value)) == JSONSuccess;
	else
		return false;
}

JSONWrapper JSONWrapper::WriteJSONObject(const char* name)
{
	json_object_set_value(root, name, json_value_init_object());
	return ReadJSONObject(name);
}



//Read=====================================================================================

int JSONWrapper::ReadInt(const char* name)const
{
	return (int)json_object_get_number(root, name);
}

unsigned int JSONWrapper::ReadUInt(const char* name)const
{
	return (unsigned int)json_object_get_number(root, name);
}

float JSONWrapper::ReadeFloat(const char* name)const
{
	return (float)json_object_get_number(root, name);
}

float3 JSONWrapper::ReadFloat3(const char* name)const
{
	JSON_Array* json_array = json_object_get_array(root, name);
	if (json_array)
	{
		float3 ret((float)json_array_get_number(json_array, 0),(float)json_array_get_number(json_array, 1),(float)json_array_get_number(json_array, 2));
		return ret;
	}
	return float3::zero;
}

bool JSONWrapper::ReadBool(const char* name)const
{
	return json_object_get_boolean(root, name);
}

std::string JSONWrapper::ReadString(const char* name)const
{
	return json_object_get_string(root, name);
}

JSONWrapper JSONWrapper::ReadArray(const char* name, unsigned int index)const
{
	JSON_Array* json_array = json_object_get_array(root, name);
	if (json_array)
		return JSONWrapper(json_array_get_object(json_array, index));
	return JSONWrapper((JSON_Object*) nullptr);
}

JSONWrapper JSONWrapper::ReadJSONObject(const char* name)const
{
	return JSONWrapper(json_object_get_object(root, name));
}