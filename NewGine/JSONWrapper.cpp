#include "JSONWrapper.h"

JSONWrapper::JSONWrapper()
{
	root_value = json_value_init_object();
	root = json_value_get_object(root_value);
}
JSONWrapper::JSONWrapper(JSON_Object* obj) : root(root)
{

}

JSONWrapper::JSONWrapper(const char* file)
{
	root_value = json_parse_string(file);
	root = json_value_get_object(root_value);
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

bool JSONWrapper::WriteMatrix(const char * name, const math::float4x4 & matrix)
{
	//only 4x4 matrix
	JSON_Value* value = json_value_init_array();
	JSON_Array* array = json_value_get_array(value);
	const float* matrix_array = *matrix.v;

	for (int i = 0; i < 16; i++)
		json_array_append_number(array, matrix_array[i]);

	return json_object_set_value(root, name, value) == JSONSuccess;
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

float4x4 JSONWrapper::ReadMatrix(const char * name) const
{
	JSON_Array* json_array = json_object_get_array(root, name);
	if (json_array)
	{
		float4x4 ret((float)json_array_get_number(json_array, 0),
			(float)json_array_get_number(json_array, 1),
			(float)json_array_get_number(json_array, 2),
			(float)json_array_get_number(json_array, 3),
			(float)json_array_get_number(json_array, 4),
			(float)json_array_get_number(json_array, 5),
			(float)json_array_get_number(json_array, 6),
			(float)json_array_get_number(json_array, 7),
			(float)json_array_get_number(json_array, 8),
			(float)json_array_get_number(json_array, 9),
			(float)json_array_get_number(json_array, 10),
			(float)json_array_get_number(json_array, 11),
			(float)json_array_get_number(json_array, 12),
			(float)json_array_get_number(json_array, 13),
			(float)json_array_get_number(json_array, 14),
			(float)json_array_get_number(json_array, 15));

		return ret;
	}

	return float4x4::identity;
}

//============================================================================

//small function to get the size of the buffer to save
size_t JSONWrapper::SerializeBuffer(char** buff)
{
	size_t ret = json_serialization_size_pretty(root_value);
	*buff = new char[ret];
	json_serialize_to_buffer_pretty(root_value, *buff, ret);
	return ret;
}

size_t JSONWrapper::GetArraySize(const char* array)
{
	size_t ret = 0;
	JSON_Array* json_array = json_object_get_array(root, array);

	if (json_array != nullptr)
		ret = json_array_get_count(json_array);

	return ret;
}

bool JSONWrapper::IsNull() const
{
	return (root_value == nullptr && root == nullptr && array == nullptr) ? true : false;
}

