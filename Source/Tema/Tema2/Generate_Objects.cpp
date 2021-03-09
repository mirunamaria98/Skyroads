#include "Generate_Objects.h"
Generate_Objects::Generate_Objects()
{
}
Generate_Objects ::~Generate_Objects() {

}
Mesh* Generate_Objects::Generate_Power_Bar(string name,glm::vec3 culoare) {

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(glm::vec3(0,0, 0), culoare),//0
		VertexFormat(glm::vec3(1.5, 0, 0), culoare),//1
		VertexFormat(glm::vec3(0, 0.1, 0),culoare),//2
		VertexFormat(glm::vec3(1.5, 0.1, 0),culoare),//3

	};

	Mesh* power_bar = new Mesh(name);
	std::vector<unsigned short> indices = { 0,1,2
											,1,3,2


	};
	power_bar->InitFromData(vertices, indices);
	return power_bar;
}