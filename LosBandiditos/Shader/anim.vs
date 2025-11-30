#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 5) in ivec4 bone_ids;
layout (location = 6) in vec4 weights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 100;
uniform mat4 bones[MAX_BONES];

void main()
{
	// Calcular la transformación de huesos (skinning)
	mat4 bone_transform = bones[bone_ids[0]] * weights[0];
	bone_transform += bones[bone_ids[1]] * weights[1];
	bone_transform += bones[bone_ids[2]] * weights[2];
	bone_transform += bones[bone_ids[3]] * weights[3];
	
	// Aplicar transformación de huesos a la posición del vértice
	vec4 boned_position = bone_transform * vec4(aPos, 1.0);
	
	// Calcular posición en espacio mundo (para iluminación)
	FragPos = vec3(model * boned_position);
	
	// Calcular normal transformada correctamente
	// 1. Transformar por huesos (sin traslación, w=0)
	// 2. Transformar por modelo con matriz normal (inversa transpuesta)
	vec3 boned_normal = mat3(bone_transform) * aNormal;
	Normal = mat3(transpose(inverse(model))) * boned_normal;
	
	// Posición final en espacio de clip
	gl_Position = projection * view * model * boned_position;
	
	// Pasar coordenadas de textura
	TexCoords = aTexCoords;
}