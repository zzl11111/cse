#include "cloth.h"
#include <iostream>
Cloth::Cloth() :res_x(10), res_y(10), width(100), length(100) {
	Initialize_Cloth();

}
Cloth::Cloth(int x, int y, int w, int l) :res_x(x), res_y(y), width(w), length(l)
{
	Initialize_Cloth();

}
void Cloth::render(GLFWwindow* window) {

	//Draw Object
	glEnableClientState(GL_VERTEX_ARRAY);

	// Index buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBindVertexArray(VAO);

	glDrawElements(
		GL_TRIANGLES,      // mode
		indices.size(),    // count
		GL_UNSIGNED_INT,   // type
		(void*)0           // element array buffer offset
	);

	glBindVertexArray(0);
}
int Cloth::GetId(int id, int dir) {
	//the west dir then i-1
	if (dir == WEST) { return id - 1; }
	if (dir == EAST) { return id + 1; }
	if (dir == NORTH) { return id + res_x; }
	if (dir == SOUTH) { return id - res_x; }
	//then get the diagnol
	if (dir == NORTH_WEST) { return id - 1 + res_x; }
	if (dir == NORTH_EAST) { return id + res_x + 1; }
	if (dir == SOUTH_WEST) { return id - 1 - res_x; }
	if (dir == SOUTH_EAST) { return id + 1 - res_x; }


}
glm::vec3 Cloth::GetSpringForce(int id1, int id2, int spring_type) {
	//get the  rest length of springtype 
	float spring_rest_length = 0;
	if (spring_type == STRUCTUAL) {
		spring_rest_length = structual_rest_length;

	}
	if (spring_type == SHEAR)
	{
		spring_rest_length = shear_rest_length;

	}
	if (spring_type == BENDING) {
		spring_rest_length = bending_rest_length;
	}
	//
	glm::vec3 delta = vertices[id1] - vertices[id2];
	float delta_length = glm::length(delta);
	return stiffness * delta / delta_length * (delta_length - spring_rest_length);
}
void Cloth::Runge_Kutta(float dt)
{//a K3 RUNGE_KUTTA method
	for (int v = 0; v < vertices.size(); v++)
	{
		glm::vec3 velocity = velocitys[v];
		glm::vec3 acceleration = forces[v] * 1.0f;
		glm::vec3 k1 = velocity;
		glm::vec3 k2 = velocity + 0.5f * dt * acceleration;
		glm::vec3 k3 = velocity + 0.75f * dt * acceleration;
		vertices[v] += 2.0f / 9.0f * dt * k1 + 1.0f / 3.0f * dt * k2 + 4.0f / 9.0f * dt * k3;
		velocitys[v] += acceleration * (dt * 0.45f);
	}
}
void Cloth:: verletIntergration(float dt, int n_iter) {
	//verletIntergration which use the previous position
	for (int v = 0; v < vertices.size(); v++)
	{
		glm::vec3 position_prim = glm::vec3(0);
		glm::vec3 acceleration = forces[v] * 1.0f; // mass
		for (int i = 0; i < n_iter; i++) {
			position_prim = 2.0f * vertices[v] - previous_position[v] + acceleration * dt * dt;
			previous_position[v] = vertices[v];
			vertices[v] = position_prim;
		}

	}
}
void Cloth::Initialize_Cloth() {
	float h = 20.0f;
	//initialize the spring_length
	structual_rest_length = (float)width / (float)res_x;
	shear_rest_length = std::sqrt(2.0f) * structual_rest_length;
	bending_rest_length = structual_rest_length * 2;//
	float offset = (float)width / 2.0f - (float)structual_rest_length / 2.0f;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	for (int y = 0; y < res_y; y += 1) {
		for (int x = 0; x < res_x; x += 1) {
			//push the vertices and initial_vertices
			vertices.push_back(glm::vec3(x * structual_rest_length - offset, h, y * structual_rest_length - offset));//initialize the vertice position
			init_position.push_back(glm::vec3(x * structual_rest_length - offset, h, y * structual_rest_length - offset));
			//push the normal in order to render
			normals.push_back(glm::vec3(0, 1.0, 0));
			//push into uvs
			uvs.push_back(glm::vec2((float)x / (float)width, (float)y / (float)length));
			//propertity
			forces.push_back(glm::vec3(0.0f));
			velocitys.push_back(glm::vec3(0.0f));
			//get the previous position
			previous_position.push_back(glm::vec3(x * structual_rest_length - offset, h, y * structual_rest_length - offset));
			previous_velocitys.push_back(glm::vec3(0));
			previous_forces.push_back(glm::vec3(0.0f));
			//

		}
	}
	//initialize the vertices index
	for (int i = 0; i < res_x - 1; i += 1) {
		for (int j = 0; j < res_y - 1; j += 1) {
			// Triangle 1
			indices.push_back(i + j * res_x);
			indices.push_back((i + 1) + j * res_x);
			indices.push_back(i + (j + 1) * res_x);

			// Triangle 2
			indices.push_back((i + 1) + j * res_x);
			indices.push_back((i + 1) + (j + 1) * res_x);
			indices.push_back((i)+(j + 1) * res_x);

		}
	}
	//bind the vbo
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
	//model normals
	glGenBuffers(1, &NBO);
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float) * 3, &normals[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//UVBO
	glGenBuffers(1, &UVBO);
	glBindBuffer(GL_ARRAY_BUFFER, UVBO);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float) * 2, &uvs[0], GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	//IBO/EBO(draw the elements)
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(int), &indices[0], GL_STATIC_DRAW);


}
void Cloth::calculate_force(GLFWwindow* window) {
	
	for (int v = 0; v < vertices.size(); v++)
	{

		glm::vec3 wind = glm::vec3(0, 0, 0);
		glm::vec3 spring = glm::vec3(0, 0, 0);

		std::vector<glm::vec3> spring_directions;

		if (v % res_x != 0) 
		{ // WEST 
			glm::vec3 force = GetSpringForce(v,v-1,STRUCTUAL);
			spring += force;
			spring_directions.push_back(glm::normalize(vertices[v] - vertices[v-1]));
		}

		if (v < vertices.size() - res_x) {// NORTH 

			if (v % res_x != 0) {
				//north west
				glm::vec3 force = GetSpringForce(v,v+res_x-1,SHEAR);
				spring += force;
				spring_directions.push_back(glm::normalize(vertices[v] - vertices[v+res_x-1]));
			}

			glm::vec3 force = GetSpringForce(v, v+res_x,STRUCTUAL);
			spring += force;
			spring_directions.push_back(glm::normalize(vertices[v] - vertices[v+res_x]));
			//north east
			if ((v + 1) % res_x != 0) {
				glm::vec3 force = GetSpringForce(v,v+res_x+1,SHEAR);
				spring += force;
				spring_directions.push_back(glm::normalize(vertices[v] - vertices[v+res_x+1]));
			}
		}

		if ((v + 1) % res_x != 0) { // EAST 
			glm::vec3 force = GetSpringForce(v, v+1,STRUCTUAL);
			spring += force;
			spring_directions.push_back(glm::normalize(vertices[v] - vertices[v+1]));
		}

		if (v > res_x - 1) {// SOUTH 

			if ((v + 1) % res_x != 0) {
				glm::vec3 force = GetSpringForce(v, v-res_x+1,SHEAR);
				spring += force;
				spring_directions.push_back(glm::normalize(vertices[v] - vertices[v-res_x+1]));
			}

			glm::vec3 force = GetSpringForce(v, v-res_x,STRUCTUAL);
			spring += force;
			spring_directions.push_back(glm::normalize(vertices[v] - vertices[v-res_x]));

			if (v % res_x != 0) {
				glm::vec3 force = GetSpringForce(v, v-res_x-1,SHEAR);
				spring += force;
				spring_directions.push_back(glm::normalize(vertices[v] - vertices[v-res_x-1]));
			}

		}

		// Calculate normals
		glm::vec3 normal = glm::vec3(0);
		for (int i = 1; i < spring_directions.size(); i++) {
			normal += glm::cross(spring_directions[i], spring_directions[i - 1]);
		}
		normals[v] = glm::normalize(normal);
		// Controls
		if (glfwGetKey(window, GLFW_KEY_SPACE)) {
			Play = true;
			begintime = glfwGetTime();
		}
		if (glfwGetKey(window, GLFW_KEY_V)) {
			std::cerr << glfwGetTime()-begintime << std::endl;
		}

		if (glfwGetKey(window, GLFW_KEY_R)) {
			for (int v = 0; v < vertices.size(); v++) {
				vertices[v] = init_position[v];
				velocitys[v] = glm::vec3(0);
				previous_position[v] = vertices[v];
			}
		}

		if (Play) {
			forces[v] =  gravity - spring;
		}


		// Pinned vertices
		if (v > vertices.size() - res_x - 1) {

			if (v % res_x == 0) {
				forces[v] = glm::vec3(0);
			}

			if ((v + 1) % res_x == 0) {
				forces[v] = glm::vec3(0);
			}
		}
	}


}
void Cloth::ForwardEulerIntergration(float dt) {
	//
	for (int v_id = 0; v_id < vertices.size(); v_id++)
	{
		glm::vec3 acceleration = forces[v_id] * 1.f;//mass
		velocitys[v_id] = damping * velocitys[v_id] + acceleration * dt;
		vertices[v_id] = vertices[v_id] + velocitys[v_id] * dt;

	}
}


void Cloth::updateSimulation(float dt, GLFWwindow* window)
{
	//calculate the total force
	calculate_force(window);
	//
	Runge_Kutta(dt);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float) * 3, &vertices[0], GL_DYNAMIC_DRAW);
	//
	glBindBuffer(GL_ARRAY_BUFFER, NBO);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float) * 3, &normals[0], GL_DYNAMIC_DRAW);
}