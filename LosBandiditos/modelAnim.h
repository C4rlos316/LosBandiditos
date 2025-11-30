#ifndef MODEL_ANIM_H
#define MODEL_ANIM_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "meshAnim.h"
#include "Model.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

// Para timing de animaciones
#include <GLFW/glfw3.h>

using namespace std;

// ❌ ELIMINADO: Declaración duplicada de TextureFromFile
// La función ya está definida en Model.h

class ModelAnim
{
public:
	/*  Model Data */
	vector<Texture> textures_loaded;
	vector<MeshAnim> meshes;
	string directory;
	bool gammaCorrection;

	/* Importacion base */
	Assimp::Importer importer;
	const aiScene* scene;

	/* Huesos */
	static const uint MAX_BONES = 100;

	map<string, uint> m_bone_mapping;
	uint m_num_bones = 0;
	vector<BoneMatrix> m_bone_matrices;
	aiMatrix4x4 m_global_inverse_transform;

	GLuint m_bone_location[MAX_BONES];
	float ticks_per_second = 0.0f;

	/*  Functions   */
	// Constructor
	ModelAnim(string const& path, bool gamma = false) : gammaCorrection(gamma)
	{
		loadModel(path);
	}

	void initShaders(GLuint shader_program)
	{
		for (uint i = 0; i < MAX_BONES; i++)
		{
			string name = "bones[" + to_string(i) + "]";
			m_bone_location[i] = glGetUniformLocation(shader_program, name.c_str());
		}
	}

	// Dibuja el modelo y sus meshes
	void Draw(Shader shader)
	{
		// Calcular transformaciones de huesos
		vector<aiMatrix4x4> transforms;
		boneTransform(glfwGetTime(), transforms);

		// Enviar matrices de huesos al shader
		for (uint i = 0; i < transforms.size(); i++)
		{
			glUniformMatrix4fv(m_bone_location[i], 1, GL_TRUE, (const GLfloat*)&transforms[i]);
		}

		// Dibujar todos los meshes
		for (unsigned int i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

private:

	/*  Functions   */
	void loadModel(string const& path)
	{
		// Leer archivo con ASSIMP
		scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		// Verificar errores
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
			return;
		}

		// Calcular transformación inversa global
		m_global_inverse_transform = scene->mRootNode->mTransformation;
		m_global_inverse_transform.Inverse();

		// Obtener ticks por segundo de la animación
		if (scene->mAnimations && scene->mAnimations[0]->mTicksPerSecond != 0.0)
		{
			ticks_per_second = scene->mAnimations[0]->mTicksPerSecond;
		}
		else
		{
			ticks_per_second = 25.0f;
		}

		// Extraer el directorio del path
		directory = path.substr(0, path.find_last_of('/'));

		// Debug: Imprimir información de animación
		if (scene->HasAnimations())
		{
			cout << "=== INFORMACION DE ANIMACION ===" << endl;
			cout << "Tiene animaciones: " << scene->HasAnimations() << endl;
			cout << "Numero de meshes: " << scene->mNumMeshes << endl;
			cout << "Canales de animacion: " << scene->mAnimations[0]->mNumChannels << endl;
			cout << "Duracion: " << scene->mAnimations[0]->mDuration << endl;
			cout << "Ticks por segundo: " << scene->mAnimations[0]->mTicksPerSecond << endl << endl;

			cout << "Nombres de nodos:" << endl;
			showNodeName(scene->mRootNode);
			cout << endl;
		}

		// Procesar nodos recursivamente
		cout << "Nombres de huesos:" << endl;
		processNode(scene->mRootNode, scene);

		// Imprimir canales de animación
		if (scene->HasAnimations())
		{
			cout << "\nCanales de animacion:" << endl;
			for (uint i = 0; i < scene->mAnimations[0]->mNumChannels; i++)
			{
				cout << "  - " << scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str() << endl;
			}
			cout << endl;
		}
	}

	void showNodeName(aiNode* node)
	{
		cout << "  - " << node->mName.data << endl;
		for (uint i = 0; i < node->mNumChildren; i++)
		{
			showNodeName(node->mChildren[i]);
		}
	}

	void processNode(aiNode* node, const aiScene* scene)
	{
		// Procesar cada mesh en el nodo actual
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}

		// Procesar recursivamente los nodos hijos
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}

	// ... (líneas 1-187 sin cambios)

	MeshAnim processMesh(aiMesh* mesh, const aiScene* scene)
	{
		cout << "Procesando mesh - Huesos: " << mesh->mNumBones << " Vertices: " << mesh->mNumVertices << endl;

		vector<VertexAnim> vertices;  //  CAMBIO: Vertex → VertexAnim
		vector<unsigned int> indices;
		vector<Texture> textures;
		vector<VertexBoneData> bones_id_weights_for_each_vertex;

		vertices.reserve(mesh->mNumVertices);
		indices.reserve(mesh->mNumFaces * 3);
		bones_id_weights_for_each_vertex.resize(mesh->mNumVertices);

		// Procesar vértices
		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			VertexAnim vertex;  //  CAMBIO: Vertex → VertexAnim
			glm::vec3 vector;

			// Posiciones
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;

			// Normales
			if (mesh->HasNormals())
			{
				vector.x = mesh->mNormals[i].x;
				vector.y = mesh->mNormals[i].y;
				vector.z = mesh->mNormals[i].z;
				vertex.Normal = vector;
			}

			// Coordenadas de textura
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			// Tangentes y Bitangentes  AHORA COMPILA
			if (mesh->HasTangentsAndBitangents())
			{
				vector.x = mesh->mTangents[i].x;
				vector.y = mesh->mTangents[i].y;
				vector.z = mesh->mTangents[i].z;
				vertex.Tangent = vector;  //  VertexAnim SÍ tiene Tangent

				vector.x = mesh->mBitangents[i].x;
				vector.y = mesh->mBitangents[i].y;
				vector.z = mesh->mBitangents[i].z;
				vertex.Bitangent = vector;  //  VertexAnim SÍ tiene Bitangent
			}

			vertices.push_back(vertex);
		}

		// ... (resto del código sin cambios)

		// Procesar índices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// Procesar materiales
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		// Cargar texturas
		vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		// Cargar huesos
		for (uint i = 0; i < mesh->mNumBones; i++)
		{
			uint bone_index = 0;
			string bone_name(mesh->mBones[i]->mName.data);

			cout << "  Hueso: " << bone_name;

			if (m_bone_mapping.find(bone_name) == m_bone_mapping.end())
			{
				// Nuevo hueso
				bone_index = m_num_bones;
				m_num_bones++;
				BoneMatrix bi;
				m_bone_matrices.push_back(bi);

				m_bone_matrices[bone_index].offset_matrix = mesh->mBones[i]->mOffsetMatrix;
				m_bone_mapping[bone_name] = bone_index;

				cout << " (nuevo, index: " << bone_index << ")" << endl;
			}
			else
			{
				bone_index = m_bone_mapping[bone_name];
				cout << " (existente, index: " << bone_index << ")" << endl;
			}

			// Asignar pesos de huesos a vértices
			for (uint j = 0; j < mesh->mBones[i]->mNumWeights; j++)
			{
				uint vertex_id = mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;
				bones_id_weights_for_each_vertex[vertex_id].addBoneData(bone_index, weight);
			}
		}

		return MeshAnim(vertices, indices, textures, bones_id_weights_for_each_vertex);
	}

	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName)
	{
		vector<Texture> textures;
		for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);

			// Verificar si la textura ya fue cargada
			bool skip = false;
			for (unsigned int j = 0; j < textures_loaded.size(); j++)
			{
				if (std::strcmp(textures_loaded[j].path.C_Str(), str.C_Str()) == 0)
				{
					textures.push_back(textures_loaded[j]);
					skip = true;
					break;
				}
			}

			if (!skip)
			{
				Texture texture;
				// ✅ USAR LA FUNCIÓN DE Model.h (ya incluida con #include "Model.h")
				texture.id = TextureFromFile(str.C_Str(), this->directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				textures_loaded.push_back(texture);
			}
		}
		return textures;
	}

	uint findPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		for (uint i = 0; i < p_node_anim->mNumPositionKeys - 1; i++)
		{
			if (p_animation_time < (float)p_node_anim->mPositionKeys[i + 1].mTime)
			{
				return i;
			}
		}
		assert(0);
		return 0;
	}

	uint findRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		for (uint i = 0; i < p_node_anim->mNumRotationKeys - 1; i++)
		{
			if (p_animation_time < (float)p_node_anim->mRotationKeys[i + 1].mTime)
			{
				return i;
			}
		}
		assert(0);
		return 0;
	}

	uint findScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		for (uint i = 0; i < p_node_anim->mNumScalingKeys - 1; i++)
		{
			if (p_animation_time < (float)p_node_anim->mScalingKeys[i + 1].mTime)
			{
				return i;
			}
		}
		assert(0);
		return 0;
	}

	aiVector3D calcInterpolatedPosition(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumPositionKeys == 1)
		{
			return p_node_anim->mPositionKeys[0].mValue;
		}

		uint position_index = findPosition(p_animation_time, p_node_anim);
		uint next_position_index = position_index + 1;
		assert(next_position_index < p_node_anim->mNumPositionKeys);

		float delta_time = (float)(p_node_anim->mPositionKeys[next_position_index].mTime - p_node_anim->mPositionKeys[position_index].mTime);
		float factor = (p_animation_time - (float)p_node_anim->mPositionKeys[position_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);

		aiVector3D start = p_node_anim->mPositionKeys[position_index].mValue;
		aiVector3D end = p_node_anim->mPositionKeys[next_position_index].mValue;
		aiVector3D delta = end - start;

		return start + factor * delta;
	}

	aiQuaternion calcInterpolatedRotation(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumRotationKeys == 1)
		{
			return p_node_anim->mRotationKeys[0].mValue;
		}

		uint rotation_index = findRotation(p_animation_time, p_node_anim);
		uint next_rotation_index = rotation_index + 1;
		assert(next_rotation_index < p_node_anim->mNumRotationKeys);

		float delta_time = (float)(p_node_anim->mRotationKeys[next_rotation_index].mTime - p_node_anim->mRotationKeys[rotation_index].mTime);
		float factor = (p_animation_time - (float)p_node_anim->mRotationKeys[rotation_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);

		aiQuaternion start_quat = p_node_anim->mRotationKeys[rotation_index].mValue;
		aiQuaternion end_quat = p_node_anim->mRotationKeys[next_rotation_index].mValue;

		return nlerp(start_quat, end_quat, factor);
	}

	aiVector3D calcInterpolatedScaling(float p_animation_time, const aiNodeAnim* p_node_anim)
	{
		if (p_node_anim->mNumScalingKeys == 1)
		{
			return p_node_anim->mScalingKeys[0].mValue;
		}

		uint scaling_index = findScaling(p_animation_time, p_node_anim);
		uint next_scaling_index = scaling_index + 1;
		assert(next_scaling_index < p_node_anim->mNumScalingKeys);

		float delta_time = (float)(p_node_anim->mScalingKeys[next_scaling_index].mTime - p_node_anim->mScalingKeys[scaling_index].mTime);
		float factor = (p_animation_time - (float)p_node_anim->mScalingKeys[scaling_index].mTime) / delta_time;
		assert(factor >= 0.0f && factor <= 1.0f);

		aiVector3D start = p_node_anim->mScalingKeys[scaling_index].mValue;
		aiVector3D end = p_node_anim->mScalingKeys[next_scaling_index].mValue;
		aiVector3D delta = end - start;

		return start + factor * delta;
	}

	const aiNodeAnim* findNodeAnim(const aiAnimation* p_animation, const string p_node_name)
	{
		for (uint i = 0; i < p_animation->mNumChannels; i++)
		{
			const aiNodeAnim* node_anim = p_animation->mChannels[i];
			if (string(node_anim->mNodeName.data) == p_node_name)
			{
				return node_anim;
			}
		}
		return nullptr;
	}

	void readNodeHierarchy(float p_animation_time, const aiNode* p_node, const aiMatrix4x4 parent_transform)
	{
		string node_name(p_node->mName.data);

		const aiAnimation* animation = scene->mAnimations[0];
		aiMatrix4x4 node_transform = p_node->mTransformation;

		const aiNodeAnim* node_anim = findNodeAnim(animation, node_name);

		if (node_anim)
		{
			// Scaling
			aiVector3D scaling_vector = calcInterpolatedScaling(p_animation_time, node_anim);
			aiMatrix4x4 scaling_matr;
			aiMatrix4x4::Scaling(scaling_vector, scaling_matr);

			// Rotation
			aiQuaternion rotate_quat = calcInterpolatedRotation(p_animation_time, node_anim);
			aiMatrix4x4 rotate_matr = aiMatrix4x4(rotate_quat.GetMatrix());

			// Translation
			aiVector3D translate_vector = calcInterpolatedPosition(p_animation_time, node_anim);
			aiMatrix4x4 translate_matr;
			aiMatrix4x4::Translation(translate_vector, translate_matr);

			node_transform = translate_matr * rotate_matr * scaling_matr;
		}

		aiMatrix4x4 global_transform = parent_transform * node_transform;

		if (m_bone_mapping.find(node_name) != m_bone_mapping.end())
		{
			uint bone_index = m_bone_mapping[node_name];
			m_bone_matrices[bone_index].final_world_transform = m_global_inverse_transform * global_transform * m_bone_matrices[bone_index].offset_matrix;
		}

		for (uint i = 0; i < p_node->mNumChildren; i++)
		{
			readNodeHierarchy(p_animation_time, p_node->mChildren[i], global_transform);
		}
	}

	void boneTransform(double time_in_sec, vector<aiMatrix4x4>& transforms)
	{
		aiMatrix4x4 identity_matrix;

		double time_in_ticks = time_in_sec * ticks_per_second;
		float animation_time = fmod(time_in_ticks, scene->mAnimations[0]->mDuration);

		readNodeHierarchy(animation_time, scene->mRootNode, identity_matrix);

		transforms.resize(m_num_bones);

		for (uint i = 0; i < m_num_bones; i++)
		{
			transforms[i] = m_bone_matrices[i].final_world_transform;
		}
	}

	glm::mat4 aiToGlm(aiMatrix4x4 ai_matr)
	{
		glm::mat4 result;
		result[0].x = ai_matr.a1; result[0].y = ai_matr.b1; result[0].z = ai_matr.c1; result[0].w = ai_matr.d1;
		result[1].x = ai_matr.a2; result[1].y = ai_matr.b2; result[1].z = ai_matr.c2; result[1].w = ai_matr.d2;
		result[2].x = ai_matr.a3; result[2].y = ai_matr.b3; result[2].z = ai_matr.c3; result[2].w = ai_matr.d3;
		result[3].x = ai_matr.a4; result[3].y = ai_matr.b4; result[3].z = ai_matr.c4; result[3].w = ai_matr.d4;
		return result;
	}

	aiQuaternion nlerp(aiQuaternion a, aiQuaternion b, float blend)
	{
		a.Normalize();
		b.Normalize();

		aiQuaternion result;
		float dot_product = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		float one_minus_blend = 1.0f - blend;

		if (dot_product < 0.0f)
		{
			result.x = a.x * one_minus_blend + blend * -b.x;
			result.y = a.y * one_minus_blend + blend * -b.y;
			result.z = a.z * one_minus_blend + blend * -b.z;
			result.w = a.w * one_minus_blend + blend * -b.w;
		}
		else
		{
			result.x = a.x * one_minus_blend + blend * b.x;
			result.y = a.y * one_minus_blend + blend * b.y;
			result.z = a.z * one_minus_blend + blend * b.z;
			result.w = a.w * one_minus_blend + blend * b.w;
		}

		return result.Normalize();
	}
};

#endif