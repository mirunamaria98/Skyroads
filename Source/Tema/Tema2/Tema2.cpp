#include "Tema2.h"

#include <vector>
#include <string>
#include <iostream>

#include <Core/Engine.h>
#include <Laboratoare\Laborator5\Transform2D.h>

using namespace std;

Tema2::Tema2()
{
}

Tema2::~Tema2()
{
}


void Tema2::Init()
{
	renderCameraTarget = false;
	//pozitii initiale camera
	camera = new Laborator::Camera();
	camera->Set(glm::vec3(pozitionare_x, pozitionare_y, pozitionare_z), glm::vec3(orientare_x, orientare_y, orientare_z),
				glm::vec3(0, 1, 0));
	{
		Mesh* mesh = new Mesh("box");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "box.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	{
		Mesh* mesh = new Mesh("sphere");
		mesh->LoadMesh(RESOURCE_PATH::MODELS + "Primitives", "sphere.obj");
		meshes[mesh->GetMeshID()] = mesh;
	}

	projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

	// Create a shader program for drawing face polygon with the color of the normal
	{
		//calea catre shadere
		Shader* shader1 = new Shader("ShaderSphereDef");
		shader1->AddShader("Source/Laboratoare/Laborator5/Shaders/VertexShaderDeformare.glsl", GL_VERTEX_SHADER);
		shader1->AddShader("Source/Laboratoare/Laborator5/Shaders/FragmentShaderDeformare.glsl", GL_FRAGMENT_SHADER);
		shader1->CreateAndLink();
		shaders[shader1->GetName()] = shader1;

		Shader* shader = new Shader("ShaderSphere");
		shader->AddShader("Source/Laboratoare/Laborator5/Shaders/VertexShader.glsl", GL_VERTEX_SHADER);
		shader->AddShader("Source/Laboratoare/Laborator5/Shaders/FragmentShader.glsl", GL_FRAGMENT_SHADER);
		shader->CreateAndLink();
		shaders[shader->GetName()] = shader;
		sphere_name = "ShaderSphere";

		Shader* shaderBox = new Shader("ShaderBox");
		shaderBox->AddShader("Source/Laboratoare/Laborator5/Shaders/VertexShaderBox.glsl", GL_VERTEX_SHADER);
		shaderBox->AddShader("Source/Laboratoare/Laborator5/Shaders/FragmentShaderBox.glsl", GL_FRAGMENT_SHADER);
		shaderBox->CreateAndLink();
		shaders[shaderBox->GetName()] = shaderBox;
	}
	//initializare structura de placi
	int col;
	for (int i = 0; i < 10; i++) {
		int x = 0;
		for (int k = 0; k < 3; k++) {
			placi[i][k].x = 2 + x;
			placi[i][k].y = 0;
			placi[i][k].z = -8 * i;
			if (i < 5) {
				placi[i][k].show = 1;
			}
			else{
				placi[i][k].show = rand() % 2;
			}
			placi[i][k].x_sup = placi[i][k].x + 0.9;
			placi[i][k].x_inf = placi[i][k].x - 0.9;
			//initial placile vor fi initializate cu 3 culori : verde, galben si albastru
			col = rand ()% 3;
			if (col == 0) {
				placi[i][k].culoare = culoare2;
			}
			else if (col== 1) {
				placi[i][k].culoare = culoare4;
			}
			else if (col == 2) {
				placi[i][k].culoare = culoare5;
			}
			x += 3;
		}
		if (placi[i][0].show == 0 && placi[i][2].show == 0) {
			placi[i][1].show = 1;
		}
	}

	Mesh* power_bar;
	Mesh* power_bar2;
	Generate_Objects* object = new Generate_Objects();
	power_bar = object->Generate_Power_Bar("power white",culoare_power_bar_white);
	power_bar2 = object->Generate_Power_Bar("power color",culoare_power);
	AddMeshToList(power_bar);
	AddMeshToList(power_bar2);
}

void Tema2::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds)
{


		if (perspecive == false) {
			camera->Set(glm::vec3(pozitionare_x, pozitionare_y, pozitionare_z), glm::vec3(orientare_x, orientare_y, orientare_z),
				glm::vec3(0, 1, 0));
		}
		else {
			camera->Set(glm::vec3(sphereCoordX, sphereCoordY + 1, 0.f), glm::vec3(sphereCoordX, sphereCoordY + 1, -1), glm::vec3(0, sphereCoordY, 0));
		}

		if (power_up != 0) {
			if (power_bar_x > 1) {
				power_bar_x = 1;
			}
			power_bar_x -= decreases_gas;
		}


		if (power_bar_x == 0) {
			power_up = 0;
		}
		if (orange == true && power_up != 0) {
			power_up = constant_power;
			orange_counter += 1;
		}
		if (orange_counter == max_time_power) {
			orange = false;
			orange_counter = 0;
			power_up = last_power_up;
			sphere_name = "ShaderSphere";
		}
		if (power_bar_x <= over_because_gas) {
			power_up = 0;
		}

		modelMatrix2 = glm::mat3(1);
		modelMatrix2 *= Transform2D::Translate(translate_power_bar_x, translate_power_bar_y);
		modelMatrix2 *= Transform2D::Scale (power_bar_x, 1);
		RenderMesh2D(meshes["power color"], shaders["VertexColor"], modelMatrix2);
		modelMatrix2 = glm::mat3(1);
		modelMatrix2 *= Transform2D::Translate(translate_power_bar_x, translate_power_bar_y);
		RenderMesh2D(meshes["power white"], shaders["VertexColor"], modelMatrix2);

		for (int i = 0; i < 10; i++) {
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(placi[i][0].x, placi[i][0].y, placi[i][0].z));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(placi[i][0].show * 2, placi[i][0].show * 0.2, placi[i][0].show * 7));
				RenderSimpleMesh(meshes["box"], shaders["ShaderBox"], modelMatrix, placi[i][0].culoare);
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(placi[i][1].x, placi[i][1].y, placi[i][1].z));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(placi[i][1].show * 2, placi[i][1].show * 0.2, placi[i][1].show * 7));
				RenderSimpleMesh(meshes["box"], shaders["ShaderBox"], modelMatrix, placi[i][1].culoare);
			}
			{
				glm::mat4 modelMatrix = glm::mat4(1);
				modelMatrix = glm::translate(modelMatrix, glm::vec3(placi[i][2].x, placi[i][2].y, placi[i][2].z));
				modelMatrix = glm::scale(modelMatrix, glm::vec3(placi[i][2].show * 2, placi[i][2].show * 0.2, placi[i][2].show * 7));
				RenderSimpleMesh(meshes["box"], shaders["ShaderBox"], modelMatrix, placi[i][2].culoare);
			}

			if ((sphereCoordX > placi[i][0].x_sup && sphereCoordX < placi[i][1].x_inf ||
				sphereCoordX > placi[i][1].x_sup && sphereCoordX < placi[i][2].x_inf) && sphereCoordY <= initial_sphere_on_y) {
				sphereCoordY -= 5 *deltaTimeSeconds;
				perspecive = false;
				power_up = 0;
			}
			for (int k = 0; k < 3; k++) {
				if (sphereCoordX > placi[i][k].x_inf && sphereCoordX < placi[i][k].x_sup
					&& placi[i][k].z - 4 >= -7.5 && placi[i][k].z <= 4 && sphereCoordY <= initial_sphere_on_y && placi[i][k].show == 1) {
					//daca pica pe rosu jocul se termina
					if (placi[i][k].culoare == culoare1) {
						power_up = 0;
					}
					else 
					{	//daca pica pe galben se scade o valoare mai mare din combustibil
						if (placi[i][k].culoare == culoare2 && power_up != 0) {
							power_bar_x -= decreases_on_yellow;
						}
						//daca pica pe portocaliu va ave o viteza constatnta un anumit numar de secunde
						if (placi[i][k].culoare == culoare3) {
							orange = true;
							orange_counter = 0;
							sphere_name = "ShaderSphereDef";

						}
						//daca pica pe verde va creste combustibilul
						if (placi[i][k].culoare == culoare4 && power_up != 0) {
							if(power_bar_x <= 1)
								power_bar_x += increase_on_green;
							else {
								power_bar_x = 1;
							}
						}
						//pe orice placa ar pica aceasta va deveni mov
						placi[i][k].culoare = culoare6;
					}
				}
				//verific daca sfera sa afla intre placi
				//daca aceasta se afla intre placi jocul se termina
				if (sphereCoordX > placi[i][k].x_inf && sphereCoordX < placi[i][k].x_sup
					&& placi[i][k].z - 4 >= -6 && placi[i][k].z <= 4 && sphereCoordY <= initial_sphere_on_y && placi[i][k].show == 0) {
					sphereCoordY -= 5 * deltaTimeSeconds;
					perspecive = false;
					power_up = 0;
				}

			}


		}
		int col;
		//se realizeaza miscarea placilor
		for (int i = 0; i < 10; i++) {
			placi[i][0].z += deltaTimeSeconds * power_up;
			placi[i][1].z += deltaTimeSeconds * power_up;
			placi[i][2].z += deltaTimeSeconds * power_up;
			//daca placile ies din ecran acestea vor fi translatate pe ultima pozitie din vectorul de placi
			if (placi[i][0].z + translate_z >= 8 || placi[i][1].z + translate_z >= 8 || placi[i][2].z + translate_z >= 8) {
				//culorile acestora se vor alege random
				//cea mai frecventa culoare este albastru
				for (int k = 0; k < 3; k++) {
					placi[i][k].z = last;
					placi[i][k].show = rand() % 2;
					col = rand() % 10;
					if (col == 0 || col == 1) {
						placi[i][k].culoare = culoare2;
					}
					else if (col == 2 || col == 3) {
						placi[i][k].culoare = culoare3;
					}
					else if (col == 4 || col == 5) {
						placi[i][k].culoare = culoare4;
					}
					else if (col > 5) {
						placi[i][k].culoare = culoare5;
					}
				}
				//daca nu se genereaza nicio placa pe un rand atunci va aparea o placa pe mojloc
				if (placi[i][0].show == 0 && placi[i][2].show == 0) {
					placi[i][1].show = 1;
				}
				//daca exista 3 placi pe un rand atnci una cel putin una poate sa fie portocalie
				if (placi[i][0].show == 1 && placi[i][2].show == 1 && placi[i][1].show == 1) {
					int pl = rand() % 3;
					placi[i][pl].culoare = culoare1;
				}
			}
		}
		//verificam daca sfera se afla intre placi fara spate
		for (int i = 0; i < 10; i++) {
			for (int k = 0; k < 3; k++) {
				if (i == 10) {
					//in cazul ultimei placi aceasta va fi comparata cu prime deoarece la translatie 
					//prima placa va deveni placa din spatele celei de-a 10-a placa
					if (sphereCoordZ < placi[10][k].z - 4 && sphereCoordZ > placi[0][k].z + 3 &&
						sphereCoordY <= 0.5) {
						sphereCoordY -= 5 *deltaTimeSeconds;
						perspecive = false;
						between = true;
					}
				}
				//verific pentru toate celelalte placi
				if (sphereCoordZ < placi[i][k].z - 3.5 && sphereCoordZ > placi[i + 1][k].z + 3.5 &&
					sphereCoordY <= initial_sphere_on_y) {
					sphereCoordY -= 5 * deltaTimeSeconds;
					perspecive = false;
					between = true;
				}
				

			}
		}

		if (between == true)
		{
			sphereCoordY -= deltaTimeSeconds * 5;
			perspecive = false;
			power_up = 0;
		}
		//voi genera bila doar in 3rd person
		if (perspecive == false) {
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, glm::vec3(sphereCoordX, sphereCoordY + 0.2, sphereCoordZ));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
			RenderMesh(meshes["sphere"], shaders[sphere_name], modelMatrix);
		}
		//verific daca placa a depasit prima si a 3-a placa
		//in acest caz jocul se termina
		if (sphereCoordX > 9 || sphereCoordX < 1) {
			game_over = 1;
			power_up = 0;
		}

		if (game_over == 1) {
			sphereCoordY -= deltaTimeSeconds;
		}
		//saritura sferei
		if (jump == true) {
			if (sphereCoordY < max_height_on_y && down == false) {
				sphereCoordY += deltaTimeSeconds * 3;
				down = false;
			}
			else {
				down = true;
				sphereCoordY -= deltaTimeSeconds * 3;
			}
			if (sphereCoordY < initial_sphere_on_y) {
				jump = false;
				down = false;
			}
		}

		if (renderCameraTarget)
		{
			glm::mat4 modelMatrix = glm::mat4(1);
			modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
			modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
			RenderMesh(meshes["sphere"], shaders["VertexNormal"], modelMatrix);
		}
	}


void Tema2::FrameEnd()
{
	//am ales sa scot fundalul in care apareau axele
	//DrawCoordinatSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix)
{
	if (!mesh || !shader || !shader->program)
		return;

	// render an object using the specified shader and the specified position
	shader->Use();
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	mesh->Render();
}
void Tema2::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color)
{
	if (!mesh || !shader || !shader->GetProgramID())
		return;

	// render an object using the specified shader and the specified position
	glUseProgram(shader->program);


	//glUniform3fv(glGetUniformLocation(shader->program, "object_color"), 1, glm::value_ptr(color));
	GLint locObject = glGetUniformLocation(shader->program, "object_color");
	glUniform3fv(locObject, 1, glm::value_ptr(color));
	// Bind model matrix
	/*GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
	glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));*/
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "View"), 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));


	// Bind view matrix
	//glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
	//int loc_view_matrix = glGetUniformLocation(shader->program, "View");
	//glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));


	//// Bind projection matrix
	//glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
	//int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
	//glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(shader->program, "Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));


	// Draw the object
	glBindVertexArray(mesh->GetBuffers()->VAO);
	glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_SHORT, 0);
}

// Documentation for the input functions can be found in: "/Source/Core/Window/InputController.h" or
// https://github.com/UPB-Graphics/Framework-EGC/blob/master/Source/Core/Window/InputController.h

bool perspective = true;
void Tema2::OnInputUpdate(float deltaTime, int mods)
{
	//camera->MoveForward(deltaTime * 2);
	//camera->MoveForward(deltaTime);
	// move the camera only if MOUSE_RIGHT button is pressed
	/*if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{*/
		float cameraSpeed = 2.0f;

		if (window->KeyHold(GLFW_KEY_W)) {
			// TODO : translate the camera forward
			//camera->TranslateForward(deltaTime* cameraSpeed);
			if (orange == false) {
				power_up += deltaTime * 3;
				last_power_up = power_up;
			}
	
		}

		if (window->KeyHold(GLFW_KEY_A)) {
			// TODO : translate the camera to the left
			//camera->TranslateRight(-deltaTime * cameraSpeed);
			sphereCoordX -= deltaTime * 5;
		}

		if (window->KeyHold(GLFW_KEY_S)) {
			// TODO : translate the camera backwards
			//camera->TranslateForward(-deltaTime * cameraSpeBed);
			if (orange == false){
				power_up -= deltaTime * 3;
				last_power_up = power_up;
			}
			if (power_up <= 0) {
				power_up = 0;
			}
		}

		if (window->KeyHold(GLFW_KEY_D)) {
			sphereCoordX += deltaTime * 5;
		}
}

void Tema2::OnKeyPress(int key, int mods)
{
	// add key press event
	if (key == GLFW_KEY_SPACE) {
		jump = true;
	}
	if (key == GLFW_KEY_C) {
		perspecive = !perspecive;
	}

}
void Tema2::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event

	if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
	{
		float sensivityOX = 0.001f;
		float sensivityOY = 0.001f;

		if (window->GetSpecialKeyState() == 0) {
			renderCameraTarget = false;
			// TODO : rotate the camera in First-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateFirstPerson_OX(sensivityOX * -deltaY);
			camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
		}

		if (window->GetSpecialKeyState() && GLFW_MOD_CONTROL) {
			renderCameraTarget = true;
			// TODO : rotate the camera in Third-person mode around OX and OY using deltaX and deltaY
			// use the sensitivity variables for setting up the rotation speed
			camera->RotateThirdPerson_OX(sensivityOX * -deltaY);
			camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
		}

	}
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button press event
}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Tema2::OnWindowResize(int width, int height)
{
}
