#ifndef MESH_ANIM_H
#define MESH_ANIM_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/types.h>

#include "Shader.h"
#include "Mesh.h"  // INCLUIR Mesh.h para usar struct Texture

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

typedef unsigned int uint;
#define NUM_BONES_PER_VERTEX 4

// ========================================================================
// ESTRUCTURA VERTEXANIM (diferente a Vertex de Mesh.h)
// ========================================================================
struct VertexAnim
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

//  ELIMINAR: struct Texture (ya está en Mesh.h)

// ========================================================================
// ESTRUCTURA BONEMATRIX
// ========================================================================
struct BoneMatrix
{
	aiMatrix4x4 offset_matrix;
	aiMatrix4x4 final_world_transform;
};

// ========================================================================
// ESTRUCTURA VERTEXBONEDATA
// ========================================================================
struct VertexBoneData
{
	uint ids[NUM_BONES_PER_VERTEX];
	float weights[NUM_BONES_PER_VERTEX];

	VertexBoneData()
	{
		memset(ids, 0, sizeof(ids));
		memset(weights, 0, sizeof(weights));
	}

	void addBoneData(uint bone_id, float weight)
	{
		for (uint i = 0; i < NUM_BONES_PER_VERTEX; i++)
		{
			if (weights[i] == 0.0)
			{
				ids[i] = bone_id;
				weights[i] = weight;
				return;
			}
		}
	}
};

// ========================================================================
// CLASE MESHANIM
// ========================================================================
class MeshAnim {
public:
	/*  Mesh Data  */
	vector<VertexAnim> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;  //  Usa Texture de Mesh.h
	vector<VertexBoneData> bones_id_weights_for_each_vertex;
	unsigned int VAO;

	/*  Functions  */
	// Constructor sin huesos
	MeshAnim(vector<VertexAnim> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->bones_id_weights_for_each_vertex.resize(vertices.size());
		setupMesh();
	}

	// Constructor con huesos
	MeshAnim(vector<VertexAnim> vertices, vector<unsigned int> indices, vector<Texture> textures, vector<VertexBoneData> bone_id_weights)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;
		this->bones_id_weights_for_each_vertex = bone_id_weights;
		setupMesh();
	}

	void Draw(Shader shader)
	{
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;

		for (unsigned int i = 0; i < textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			string number;
			string name = textures[i].type;

			if (name == "texture_diffuse")
				number = std::to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = std::to_string(specularNr++);
			else if (name == "texture_normal")
				number = std::to_string(normalNr++);
			else if (name == "texture_height")
				number = std::to_string(heightNr++);

			shader.setInt((name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);  //  Ahora compila correctamente
		}

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

private:
	unsigned int VBO, EBO, VBO_bones;

	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenBuffers(1, &VBO_bones);

		glBindVertexArray(VAO);

		// Vértices
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexAnim), &vertices[0], GL_STATIC_DRAW);

		// Huesos
		glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);
		glBufferData(GL_ARRAY_BUFFER, bones_id_weights_for_each_vertex.size() * sizeof(VertexBoneData),
			&bones_id_weights_for_each_vertex[0], GL_STATIC_DRAW);

		// Índices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

		// Atributos de vértices
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		// Location 0: Posiciones
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)0);

		// Location 1: Normales
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, Normal));

		// Location 2: TexCoords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, TexCoords));

		// Location 3: Tangentes
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, Tangent));

		// Location 4: Bitangentes
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexAnim), (void*)offsetof(VertexAnim, Bitangent));

		// Atributos de huesos
		glBindBuffer(GL_ARRAY_BUFFER, VBO_bones);

		// Location 5: Bone IDs
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(VertexBoneData), (void*)0);

		// Location 6: Bone Weights
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (void*)offsetof(VertexBoneData, weights));

		glBindVertexArray(0);
	}
};

#endif