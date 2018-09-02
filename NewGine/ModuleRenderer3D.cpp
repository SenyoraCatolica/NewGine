#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "Glew\include\glew.h"
#include "SDL\include\SDL_opengl.h"

#include "ModuleEditor.h"
#include <gl/GL.h>
#include <gl/GLU.h>

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib, "Glew/libx86/glew32.lib") 

#include "Imgui\imgui.h"
#include "Imgui\imgui_impl_sdl_gl3.h"


ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
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
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
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
	OnResize(SCREEN_WIDTH, SCREEN_HEIGHT, 60);

	ImGui_ImplSdlGL3_Init(App->window->window);
	App->camera->Look(vec(100.0f, 100.0f, 50.0f), vec(0.0f, 0.0f, 0.0f));

	LOG("OpenGL version: %s", glGetString(GL_VERSION));
	LOG("Glew version: %s", glewGetString(GLEW_VERSION));

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());

	// light 0 on cam pos
	lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();


	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	ImGui::Render();
	SDL_GL_SwapWindow(App->window->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	ImGui_ImplSdlGL3_Shutdown();
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


void ModuleRenderer3D::DrawMesh(MyMesh m)
{
	if (m.num_vertices > 0 && m.num_indices > 0)
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
				
				//bind textures
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
	}
}
