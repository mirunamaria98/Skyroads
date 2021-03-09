#pragma once
#include <Component/SimpleScene.h>
#include "LabCamera.h"
#include "Generate_Objects.h"

class Tema2 : public SimpleScene
{
	public:
		Tema2();
		~Tema2();

		Mesh* Generate_Power_Bar();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color);

		void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix) override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;

	protected:
		Laborator::Camera *camera;
		glm::mat4 projectionMatrix;
		glm::mat3 modelMatrix2;
		bool renderCameraTarget;

		struct Road
		{
			float x_sup;
			float x_inf;
			float x_fata;
			float x_spate;
			int show;
			float x, y, z;
			glm::vec3 culoare;
		};

		GLfloat bottom = 0.01f;
		GLfloat fov = 40.f;
		GLfloat lleft = 0.01f;
		GLfloat rright = 10.f;
		GLfloat top = 10.f;
		GLfloat zfar = 200.f;
		GLfloat znear = 0.01f;
		float pi = 3.14f;
		bool perspectia = true;
		float sphereMovement = 3;
		float sphereCoordX = 5;
		float sphereCoordY = 0.5;
		float sphereCoordZ = 0;
		int vec_poz[3][10];
		int game_over = 0;
		float jump = false;
		float down = false;
		Road placi[10][3];
		int j = 0;
		int plath1, plath2, plath3 = 1;
		float last = -72;
		float translate_z = 0;
		glm::vec3 culoare1 = glm::vec3(1.000, 0.000, 0.000);
		glm::vec3 culoare2 = glm::vec3(1.000, 1.000, 0.000);
		glm::vec3 culoare3 = glm::vec3(1.000, 0.549, 0.000);
		glm::vec3 culoare4 = glm::vec3(0.000, 1.000, 0.000);
		glm::vec3 culoare5 = glm::vec3(0.000, 0.000, 0.804);
		glm::vec3 culoare6 = glm::vec3(0.545, 0.000, 0.545);
		glm::vec3 culoare_power_bar_white = glm::vec3(1.000, 1.000, 1.000);
		glm::vec3 culoare_power = glm::vec3(1.000, 0.078, 0.576);
		int ggame = 1;
		float power_up = 0;
		bool between = false;
		float power_bar_x = 1;
		bool orange = false;
		int orange_counter = 0;
		float last_power_up = 0;
		bool perspecive = false;
		float translate_power_bar_x = -2.3;
		float translate_power_bar_y = 2.1;

		float pozitionare_x = 5;
		float pozitionare_y = 3.5;
		float pozitionare_z = 7.5f;

		float orientare_x = 5;
		float orientare_y = 1;
		float orientare_z = 0;
		string sphere_name;
		float constant_power = 7;
		float max_time_power = 170;
		float over_because_gas = 0.0001;
		float decreases_gas = 0.0001;
		float decreases_on_yellow = 0.05;
		float increase_on_green = 0.1;
		float initial_sphere_on_y = 0.5;
		float max_height_on_y = 3;
};
