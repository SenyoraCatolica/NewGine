#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"

#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "camera";

}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	return true;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

void ModuleCamera3D::CleanCameras()
{
	if (editor_cam_go != nullptr) delete editor_cam_go;
	if (editor_cam != nullptr)editor_cam = nullptr;
	if (current_cam != nullptr)current_cam = nullptr;
	if (game_cam != nullptr)game_cam = nullptr;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update()
{
	UpdateEditorCam();

	UpdateGameCam();

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec &Position, const vec &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference);
	Z.Normalize();
	X = vec(0.0f, 1.0f, 0.0f).Cross(Z);
	X.Normalize();
	Y = Z.Cross(X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec &Spot)
{
	GetCurrentCam()->LookAt(Spot);
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}
// ------------------------------------------------------------------
void ModuleCamera3D::Move(Direction d, float speed)
{
	vec newPos(0, 0, 0);
	switch (d)
	{
	case GO_RIGHT:
		newPos += X * speed;
		break;
	case GO_LEFT:
		newPos -= X * speed;
		break;
	case GO_UP:
		newPos.y += speed;
		break;
	case GO_DOWN:
		newPos.y -= speed;
		break;
	default:
		break;
	}

	Position += newPos;
	Reference += newPos;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float4x4 ModuleCamera3D::GetViewMatrix()
{
	return current_cam->GetViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -(X.Dot( Position)), -(Y.Dot(Position)), -(Z.Dot(Position)), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

void ModuleCamera3D::Rotate(float x, float y)
{
	int dx = x;
	int dy = y;

	float3 rotation_point = current_cam->frustum.Pos() + current_cam->frustum.Front() * 30.0f;
	LookAt(rotation_point);
}
// -----------------------------------------------------------------

void ModuleCamera3D::From3Dto2D(vec point, int& x, int& y)
{
	//Calculate perspective
	float4x4 perspective;
	float fov = 60.0f;
	float _near = 0.125f;
	float _far = 512.0f;

	perspective.SetIdentity();
	float tan_theta_over2 = tan(fov * pi / 360.0f);

	perspective[0][0] = 1.0f / tan_theta_over2;
	perspective[1][1] = ((float)SCREEN_WIDTH / (float)SCREEN_HEIGHT) / tan_theta_over2;
	perspective[2][2] = (_near + _far) / (_near - _far);
	perspective[3][2] = 2 * _near * _far / (_near - _far);
	perspective[2][3] = -1;
	perspective[3][3] = 0;

	float4x4 projection = perspective;

	float4 screen_mat = ViewMatrix * float4(point, 1);
	vec screen = screen_mat.xyz();
	screen = float4(projection * screen_mat).xyz();

	screen.x /= screen.z;
	screen.y /= screen.z;

	x = (screen.x +1) * (SCREEN_WIDTH /2);
	y = (screen.y + 1) * (SCREEN_HEIGHT /2);
}

// -----------------------------------------------------------------

vec ModuleCamera3D::GetPosition()
{
	TransformComponent* t = (TransformComponent*)editor_cam_go->GetComponent(COMPONENT_TRANSFORM);
	return t->position;
}

vec ModuleCamera3D::GetReference()
{
	return Reference;
}

void ModuleCamera3D::SetPosition(float3 pos)
{
	TransformComponent* t = (TransformComponent*)editor_cam_go->GetComponent(COMPONENT_TRANSFORM);
	t->SetTranslation(pos);
}


void ModuleCamera3D::UpdateEditorCam()
{
	if (current_cam == editor_cam)
	{	
		float2 pos(App->input->GetMouseX(), App->input->GetMouseY());
		if (pos.x > 350 && pos.x < 1050 && pos.y > 25 && pos.y < 600)
		{
			//Keys Movement   --------------------------------------------------------------------------------
			TransformComponent* t = (TransformComponent*)current_cam->parent->GetComponent(COMPONENT_TRANSFORM);

			float3 new_pos = float3::zero;

			float3 world_y = t->GetGlobalTranform().WorldY();
			float3 world_z = t->GetGlobalTranform().WorldZ();
			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
				new_pos += world_z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
				new_pos -= world_z * speed;

			float3 world_x = t->GetGlobalTranform().WorldX();
			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
				new_pos += world_x * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
				new_pos -= world_x * speed;


			// Mouse motion ---------------------------------------------------------
			if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
			{
				float Sensitivity = 0.5f;

				int dx = -App->input->GetMouseXMotion();
				int dy = -App->input->GetMouseYMotion();

				Rotate(dx, dy);

				float delta_x = (float)dx * Sensitivity;
				float delta_y = (float)dy * Sensitivity;

				Quat quatX, quatY;
				quatX = Quat(world_y, DegToRad(delta_x));
				if (delta_y != 0)
					quatY = Quat(world_x, DegToRad(-delta_y));
				else
					quatY = Quat::identity;
				TransformComponent* t = (TransformComponent*)current_cam->parent->GetComponent(COMPONENT_TRANSFORM);
				Quat current_rot = t->GetQuatRotation();
				Quat rot = quatX * quatY;
				if (current_rot.x != 0 && current_rot.y != 0 && current_rot.z != 0)
					rot = rot * current_rot;

				t->SetRotation(rot);
			}

			//Mouse wheel --------------------------------------------------------------
			float wheel_speed = 2.0f;

			if (App->input->GetMouseZ() > 0) new_pos += world_z * speed * wheel_speed;
			if (App->input->GetMouseZ() < 0) new_pos -= world_z * speed * wheel_speed;


			//Middle mouse button movement
			if (App->input->GetMouseButton(SDL_BUTTON_MIDDLE) == KEY_REPEAT)
			{
				int dx = App->input->GetMouseXMotion();
				int dy = App->input->GetMouseYMotion();

				new_pos += world_x * speed * dx;
				new_pos += world_y * speed * dy;
			}

			if (new_pos.x != 0 || new_pos.y != 0 || new_pos.z != 0)
			{
				//Set final position;
				float3 pos = t->GetTranslation();
				pos += new_pos;
				t->SetTranslation(pos);
			}
		}
	}		
}

void ModuleCamera3D::UpdateGameCam()
{
	std::list<CameraComponent*>::iterator it = App->go_manager->camera_objects.begin();
	while (it != App->go_manager->camera_objects.end())
	{
		if ((*it)->main_camera == true)
		{
			SetGameCam(*it);
			break;
		}
		it++;
	}
}


void  ModuleCamera3D::ChangeCurrentCam(CameraComponent* cam) 
{
	if (cam != nullptr)
	{
		current_cam = cam;
	}
}


void ModuleCamera3D::CreateEditorCam()
{
	editor_cam_go = App->go_manager->CreateCamera("EditorCam", true);
	editor_cam = (CameraComponent*)editor_cam_go->GetComponent(COMPONENT_CAMERA);
	editor_cam->culling = false;
	editor_cam->main_camera = false;
	editor_cam->SetFar(200);
	current_cam = editor_cam;
}

//----------------------------------------------------------------------------------------
CameraComponent* ModuleCamera3D::GetEditorCam()
{
	return editor_cam;
}

CameraComponent* ModuleCamera3D::GetCurrentCam()
{
	if (current_cam == nullptr)
	{
		current_cam = editor_cam;
		return editor_cam;
	}

	else
		return current_cam;
}

CameraComponent* ModuleCamera3D::GetGameCam()
{
	return game_cam;
}

//----------------------------------------------------------------------------------------

void ModuleCamera3D::SetEditorCam(CameraComponent* cam)
{
	if (editor_cam_go == nullptr)
		CreateEditorCam();

	editor_cam = cam;
}

void ModuleCamera3D::SetEditorCam(GameObject* cam)
{
	editor_cam_go = cam;
	editor_cam = (CameraComponent*)editor_cam_go->GetComponent(COMPONENT_CAMERA);
}

void ModuleCamera3D::SetCurrentCam(CameraComponent* cam)
{
	current_cam = cam;
}
void ModuleCamera3D::SetGameCam(CameraComponent* cam)
{
	game_cam = cam;
}

//--------------------------------------------------------------------
bool ModuleCamera3D::LoadConfig(JSON_Object* data)
{
	bool ret = true;

	JSON_Object* pos = json_object_get_object(data, "pos");
	JSON_Object* reference = json_object_get_object(data, "reference");

	Position.x = json_object_get_number(pos, "x");
	Position.y = json_object_get_number(pos, "y");
	Position.z = json_object_get_number(pos, "z");
	Reference.x = json_object_get_number(reference, "x");
	Reference.y = json_object_get_number(reference, "y");
	Reference.z = json_object_get_number(reference, "z");
	speed = json_object_get_number(data, "speed");

	return ret;
}

bool ModuleCamera3D::SaveConfig(JSON_Object* data) const
{
	bool ret = true;

	JSON_Object* pos = json_object_get_object(data, "pos");
	JSON_Object* reference = json_object_get_object(data, "reference");

	json_object_set_number(pos, "x", Position.x);
	json_object_set_number(pos, "y", Position.y);
	json_object_set_number(pos, "z", Position.z);
	json_object_set_number(reference, "x", Reference.x);
	json_object_set_number(reference, "y", Reference.y);
	json_object_set_number(reference, "z", Reference.z);
	json_object_set_number(data, "speed", speed);

	return ret;
}
