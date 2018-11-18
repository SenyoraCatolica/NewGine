#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#include "CameraComponent.h"


#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") 

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl.h"
#include "Imgui\imgui_impl_sdl_gl3.h"
#include "Imgui\ImGuizmo.h"


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "renderer";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	LOG("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	GLenum gl_init = glewInit();

	if (ret == true)
	{
		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);

		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.5f, 0.5f, 0.5f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();

		GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		lights[0].Active(true);
		glEnable(GL_LIGHTING);
		glEnable(GL_COLOR_MATERIAL);
	}

	// Projection matrix for
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT - 200 ,60);

	ImGui_ImplSdlGL3_Init(App->window->window);
	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	float4x4 matrix = App->camera->GetCurrentCam()->GetProjectionMatrix();
	SetCurrentCamView(matrix);


	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(*App->camera->GetViewMatrix().v);

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	//ImGuizmo::SetDrawlist();


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate()
{
	std::list<GameObject*>::iterator it = App->go_manager->all_gameobjects.begin();
	while (it != App->go_manager->all_gameobjects.end())
	{
		DrawGameObject(*it);
		it++;
	}

	ImGui::Render();


	SDL_GL_SwapWindow(App->window->window);


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(int width, int height, float fovy)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Calculate perspective
	float4x4 perspective;
	float _near = 0.125f;
	float _far = 512.0f;

	perspective.SetIdentity();
	float tan_theta_over2 = tan(fovy * pi / 360.0f);

	perspective[0][0] = 1.0f / tan_theta_over2;
	perspective[1][1] = ((float)width / (float)height) / tan_theta_over2;
	perspective[2][2] = (_near + _far) / (_near - _far);
	perspective[3][2] = 2 * _near * _far / (_near - _far);
	perspective[2][3] = -1;
	perspective[3][3] = 0;

	ProjectionMatrix = perspective;
	glLoadMatrixf(*ProjectionMatrix.v);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::SetCurrentCamView(const float4x4& matrix)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = matrix;
	glLoadMatrixf(*ProjectionMatrix.v);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DrawMesh(MyMesh m)
{
	/*if (m.num_vertices > 0 && m.num_indices > 0)
	{
		//wireframe should be enabled here
		if (draw_wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (m.texture_coords == nullptr)
		{
			glColor4f(1, 1, 1, 1);
		}

		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		//Texture should be drawn here
		if (enable_textures)
		{
			if (m.id_texture_coords > 0)
			{
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, 0);

				
				for (int i = 0; i < m.textures.size(); i++)
				{
					glBindTexture(GL_TEXTURE_2D, m.textures[i]->id);
				}

				glEnableClientState(GL_TEXTURE_COORD_ARRAY);
				glBindBuffer(GL_ARRAY_BUFFER, m.id_texture_coords);
				glTexCoordPointer(2, GL_FLOAT, 0, NULL);
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, m.id_vertices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m.id_indices);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawElements(GL_TRIANGLES, m.num_indices, GL_UNSIGNED_INT, NULL);
		glBindTexture(GL_TEXTURE_2D, 0);


		//normals should be drwan here
		if (draw_normals)
		{
			if (m.normals > 0)
			{
				glEnableClientState(GL_NORMAL_ARRAY);
				//Setting Normals
				glBindBuffer(GL_ARRAY_BUFFER, m.id_normals);
				glNormalPointer(GL_FLOAT, 0, NULL);
			}
		}

		glDisableClientState(GL_VERTEX_ARRAY);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);


		//wireframe should be disabled here
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}*/
}

void ModuleRenderer3D::DrawGameObject(GameObject* go)
{
	if (go->IsActive())
	{
		if (go->components.size() > 1) //to check if any object has more than a transform component
		{
			std::list<MeshComponent*> meshes;
			for (std::list<Component*>::iterator it = go->components.begin(); it != go->components.end(); it++)
			{
				if ((*it)->type == COMPONENT_MESH)
					meshes.push_back((MeshComponent*)(*it));
			}

			TransformComponent* transform = (TransformComponent*)go->GetComponent(COMPONENT_TRANSFORM);
			if (transform != nullptr)
			{
				glPushMatrix();
				glMultMatrixf(*transform->GetTransformationMatrix().v);
			}

			//draw every mesh of the go
			for (std::list<MeshComponent*>::iterator mesh = meshes.begin(); mesh != meshes.end(); mesh++)
			{
				if ((*mesh)->mesh != nullptr)
				{
					if ((*mesh)->mesh->mesh->num_vertices > 0 && (*mesh)->mesh->mesh->num_indices > 0)
					{
						glEnableClientState(GL_VERTEX_ARRAY);
						glEnableClientState(GL_NORMAL_ARRAY);
						glEnableClientState(GL_ELEMENT_ARRAY_BUFFER);

						//Set Wireframe
						if (draw_wireframe)
						{
							glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						}

						if (enable_color_material)
						{
							//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
						}

						//draw texture
						if (enable_textures)
						{
							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							glEnable(GL_TEXTURE_2D);
							glBindTexture(GL_TEXTURE_2D, 0);


							std::list<MaterialComponent*> materials;
							for (std::list<Component*>::iterator it = go->components.begin(); it != go->components.end(); it++)
							{
								if ((*it)->type == COMPONENT_MATERIAL)
									materials.push_back((MaterialComponent*)(*it));
							}

							for (std::list<MaterialComponent*>::iterator mat = materials.begin(); mat != materials.end(); mat++)
							{
								if ((*mat)->material != nullptr && (*mat)->material->texture != nullptr)
									glBindTexture(GL_TEXTURE_2D, (*mat)->material->texture->id);
							}

							glEnableClientState(GL_TEXTURE_COORD_ARRAY);
							glBindBuffer(GL_ARRAY_BUFFER, (*mesh)->mesh->mesh->id_texture_coords);
							glTexCoordPointer(2, GL_FLOAT, 0, NULL);
						}

						//vertices
						glBindBuffer(GL_ARRAY_BUFFER, (*mesh)->mesh->mesh->id_vertices);
						glVertexPointer(3, GL_FLOAT, 0, NULL);


						//indices
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, (*mesh)->mesh->mesh->id_indices);
						glDrawElements(GL_TRIANGLES, (*mesh)->mesh->mesh->num_indices, GL_UNSIGNED_INT, NULL);



						// NORMALS ----------------------------------
						if ((*mesh)->mesh->mesh->num_normals > 0)
						{
							glBindBuffer(GL_ARRAY_BUFFER, (*mesh)->mesh->mesh->id_normals);
							glVertexPointer(3, GL_FLOAT, sizeof(float3), NULL);
							glDrawArrays(GL_LINES, 0, (*mesh)->mesh->mesh->num_vertices * 2);
							glBindBuffer(GL_ARRAY_BUFFER, 0);
						}

						glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

						glBindBuffer(GL_ARRAY_BUFFER, 0);
						glBindTexture(GL_TEXTURE_2D, 0);
						glDisable(GL_TEXTURE_2D);

						if (draw_wireframe)
						{
							glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
						}
						glDisableClientState(GL_VERTEX_ARRAY);
						glDisableClientState(GL_NORMAL_ARRAY);
						glDisableClientState(GL_ELEMENT_ARRAY_BUFFER);
						glDisableClientState(GL_TEXTURE_COORD_ARRAY);
					}
				}

				//if (go->boundingbox_active)
				{
					DrawBox((*mesh)->local_box, Yellow);
				}
			}

			if (transform != nullptr)
				glPopMatrix();
		}

		CameraComponent* cam = (CameraComponent*)go->GetComponent(COMPONENT_CAMERA);
		if(cam != nullptr && cam != App->camera->GetEditorCam())
			DrawFrustrum(cam->frustum);
	}
}



void ModuleRenderer3D::DebugDrawQuadtree(Quadtree* quadtree, QuadNode root)
{
	if (quadtree->GetRoot().GetChilds().empty())
		return;

	float3 corners[8];
	root.GetBox().GetCornerPoints(corners);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	DrawAABB(corners, Blue);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	std::vector<QuadNode> childs = root.GetChilds();
	std::vector<QuadNode>::iterator it = childs.begin();
	while (it != childs.end())
	{
		DebugDrawQuadtree(quadtree, (*it));
		it++;
	}
}

void ModuleRenderer3D::DrawBox(const vec* corners, float4 color)
{
	glColor4f(color.x, color.y, color.z, color.w);

	glBegin(GL_LINES);

	glVertex3fv(corners[0].ptr()); glVertex3fv(corners[1].ptr());
	glVertex3fv(corners[0].ptr()); glVertex3fv(corners[2].ptr());
	glVertex3fv(corners[0].ptr()); glVertex3fv(corners[4].ptr());
	glVertex3fv(corners[3].ptr()); glVertex3fv(corners[1].ptr());
	glVertex3fv(corners[3].ptr()); glVertex3fv(corners[2].ptr());
	glVertex3fv(corners[3].ptr()); glVertex3fv(corners[7].ptr());
	glVertex3fv(corners[5].ptr()); glVertex3fv(corners[1].ptr());
	glVertex3fv(corners[5].ptr()); glVertex3fv(corners[4].ptr());
	glVertex3fv(corners[5].ptr()); glVertex3fv(corners[7].ptr());
	glVertex3fv(corners[6].ptr()); glVertex3fv(corners[2].ptr());
	glVertex3fv(corners[6].ptr()); glVertex3fv(corners[4].ptr());
	glVertex3fv(corners[6].ptr()); glVertex3fv(corners[7].ptr());

	glEnd();
}

void ModuleRenderer3D::DrawLocator(float4x4 transform, float4 color)
{
	glDisable(GL_LIGHTING);
	
	glPushMatrix();
	glMultMatrixf(transform.ptr());

	glColor4f(color.x, color.y, color.z, color.w);

	glBegin(GL_LINES);

	glVertex3f(1.0f, 0.0f, 0.0f); glVertex3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.0f, 0.0f); glVertex3f(0.0f, -1.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.0f); glVertex3f(0.0f, 0.0f, -1.0f);

	glVertex3f(0.0f, 0.0f, 1.0f); glVertex3f(0.1f, 0.0f, 0.9f);
	glVertex3f(0.0f, 0.0f, 1.0f); glVertex3f(-0.1f, 0.0f, 0.9f);

	glEnd();

	glEnable(GL_LIGHTING);

	glPopMatrix();
}

void ModuleRenderer3D::DrawBox(const AABB &aabb, Color color)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	glDisable(GL_TEXTURE_2D);

	static float3 vertices[8];
	aabb.GetCornerPoints(vertices);

	DrawAABB(vertices, color);


	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void ModuleRenderer3D::DrawAABB(const vec* vertices, Color color)
{
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[3]);

	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);
	glVertex3fv((GLfloat*)&vertices[7]);

	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[1]);
	glVertex3fv((GLfloat*)&vertices[3]);
	glVertex3fv((GLfloat*)&vertices[0]);
	glVertex3fv((GLfloat*)&vertices[2]);

	glVertex3fv((GLfloat*)&vertices[5]);
	glVertex3fv((GLfloat*)&vertices[7]);
	glVertex3fv((GLfloat*)&vertices[4]);
	glVertex3fv((GLfloat*)&vertices[6]);

	glEnd();

	glColor3f(1.0f, 1.0f, 1.0f);
}

void ModuleRenderer3D::DrawFrustrum(const Frustum &frustum)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_CULL_FACE);
	//glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	float3 vertices[8];
	frustum.GetCornerPoints(vertices);

	DrawAABB(vertices, Red);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}


//Getters
bool ModuleRenderer3D::GetEnableTextures() const
{
	return enable_textures;
}
bool ModuleRenderer3D::GetEnableColorMaterial() const
{
	return enable_color_material;
}
bool ModuleRenderer3D::GetEnableDepth() const
{
	return enable_depth;
}
bool ModuleRenderer3D::GetEnableCullFace() const
{
	return enable_face_culling;
}
bool ModuleRenderer3D::GetEnableLight() const
{
	return enable_lighting;
}

//Setters
void ModuleRenderer3D::SetEnableTextures(const bool active)
{
	enable_textures = active;

	if (enable_textures)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
}

void ModuleRenderer3D::SetEnableColorMaterial(const bool active)
{
	enable_color_material = active;

	if (enable_color_material)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
}

void ModuleRenderer3D::SetEnableDepth(const bool active)
{
	enable_depth = active;

	if (enable_depth)
		glEnable(GL_DEPTH);
	else
		glDisable(GL_DEPTH);
}

void ModuleRenderer3D::SetEnableCullFace(const bool active)
{
	enable_face_culling = active;

	if (enable_face_culling)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
}

void ModuleRenderer3D::SetEnableLight(const bool active)
{
	enable_lighting = active;

	if (enable_lighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
}

bool ModuleRenderer3D::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	draw_normals = json_object_get_boolean(data, "draw_normals");
	draw_wireframe = json_object_get_boolean(data, "draw_wireframe");
	draw_meshes = json_object_get_boolean(data, "draw_meshes");
	enable_depth = json_object_get_boolean(data, "enable_depth");
	enable_face_culling = json_object_get_boolean(data, "enable_face_culling");
	enable_lighting = json_object_get_boolean(data, "enable_lighting");
	enable_color_material = json_object_get_boolean(data, "enable_color");
	enable_textures = json_object_get_boolean(data, "enable_textures");

	return ret;
}

bool ModuleRenderer3D::SaveConfig(JSON_Object* data) const
{
	bool ret = true;

	json_object_set_boolean(data, "draw_normals", draw_normals);
	json_object_set_boolean(data, "draw_wireframe", draw_wireframe);
	json_object_set_boolean(data, "draw_meshes", draw_meshes);
	json_object_set_boolean(data, "enable_depth", enable_depth);
	json_object_set_boolean(data, "enable_face_culling", enable_face_culling);
	json_object_set_boolean(data, "enable_lighting", enable_lighting);
	json_object_set_boolean(data, "enable_color", enable_color_material);
	json_object_set_boolean(data, "enable_textures", enable_textures);

	return ret;
}