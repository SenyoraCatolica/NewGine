#include "Application.h"

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleImporter::~ModuleImporter()
{

}


bool ModuleImporter::Init()
{
	mesh_importer = new MeshImporter(App);
	mat_importer = new MaterialImporter(App);

	mat_importer->Init();

	return true;
}


bool ModuleImporter::Import(const char* file, FILE_TYPE type)
{
	bool ret = false;

	switch (type)
	{
		case MESH:
			ret = mesh_importer->Import(file);
			break;

		case MATERIAL:
			ret = mat_importer->Import(file);
			break;

		default:
			break;
	}

	return ret;
}