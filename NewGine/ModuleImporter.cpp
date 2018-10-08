#include "ModuleImporter.h"

ModuleImporter::ModuleImporter(Application* app, bool start_enabled) : Module(app, start_enabled) {}

ModuleImporter::~ModuleImporter()
{

}


bool ModuleImporter::Init()
{
	mesh_importer = new MeshImporter(App);
	mat_importer = new MaterialImporter(App);
}


bool ModuleImporter::Import(const char* file, FILE_TYPE state)
{
	bool ret = false;

	switch (FILE_TYPE)
	{
	case MESH:
		mesh_importer->Import(file)
		break;

	case MATERIAL:
		mat_importer->Import(file);
		break;

	default:
		break;
	}
}