#pragma once

#include <string>
#include <Core/GPU/Mesh.h>
#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include <Component/SimpleScene.h>
using namespace std;
class Generate_Objects
{
public:
	Generate_Objects();
	~Generate_Objects();
	Mesh* Generate_Power_Bar(string name, glm::vec3 culoare);
};