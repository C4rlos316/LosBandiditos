#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

//Skybox
#include "Texture.h"

//Mixamo
#include "modelAnim.h"

// Funciones prototipo para callbacks
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// =================================================================================
// 	CONFIGURACIÓN INICIAL Y VARIABLES GLOBALES
// =================================================================================
//Configurar funciones para repetir textura de piso
void ConfigurarVAO(GLuint& VAO, GLuint& VBO, float* vertices, size_t size);
void ConfigurarTexturaRepetible(GLuint textureID);
void DibujarPiso(GLuint textureID, glm::vec3 posicion, glm::vec3 escala, GLuint VAO_Cubo, GLint modelLoc);



// Propiedades de la ventana
const GLuint WIDTH = 1000, HEIGHT = 800;
int SCREEN_WIDTH, SCREEN_HEIGHT;


// Configuración de la cámara
Camera  camera(glm::vec3(0.0f, 0.0f, 21.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

bool keys[1024];
bool firstMouse = true;
// Posiciones de las luces
glm::vec3 lightPos(1.5f, 2.0f, -2.5f);
glm::vec3 lightPos2(-1.5f, 2.0f, -2.5f);
bool active;

// Posiciones para luces puntuales 
glm::vec3 pointLightPositions[] = {
	glm::vec3(2.0f, 0.2f,  2.0f),  // Esquina 1 (Positivo X, Positivo Z)
	glm::vec3(-2.0f, 0.2f,  2.0f),  // Esquina 2 (Negativo X, Positivo Z)
	glm::vec3(2.0f, 0.2f, -2.0f),  // Esquina 3 (Positivo X, Negativo Z)
	glm::vec3(-2.0f, 0.2f, -2.0f)   // Esquina 4 (Negativo X, Negativo Z)
};



// =================================================================================
// 						ANIMACIÓN Y POSICIONES BASE DE ANIMALES
// =================================================================================

// -----------------------------------------
//  PERSONAJE MIXAMO (CENTRO)
// -----------------------------------------

float personajeMixamoScale = 0.005f;
float rotPersonajeMixamo = 180.0f;
glm::vec3 personajeMixamoPos = glm::vec3(0.0f, -0.5f, 5.0f);

//		Pinguino (Cuadrante X, -Z)
float PinAlaIzq = 0.0f;
float PinAlaDer = 0.0f;
float PinScale = 0.70f;
glm::vec3 PinguinoPos = glm::vec3(10.0f, -0.1f, -4.0f);

bool animarPinguino = false;
float startTimePinguino = 0.0f;
bool teclaC_presionada = false;

//		Foca (Cuadrante X, -Z)
float rotFocaMedio = 0.0f;
float rotFocaCola = 0.0f;
float FocaScale = 1.6f; // Ajusta la escala general de la foca si es necesario
glm::vec3 focaPosBase = glm::vec3(5.0f, -0.17f, -9.0f); // Posición base
glm::vec3 focaPosActual = focaPosBase; // Posición que se animará
float focaRotY = -90.0f; //Rotación inicial (mirando a +Z)

float rotFocaPataDer = 0.0f; // Rotación Aleta Derecha (Eje X)
float rotFocaPataIzq = 0.0f; // Rotación Aleta Izquierda (Eje X)

bool animarFoca = false;
float startTimeFoca = 0.0f;
bool teclaB_presionada = false; // Usaremos 'B' para la foca

//delfin (Cuadrante X, -Z)
glm::vec3 delfinPosBase = glm::vec3(2.8f, -0.9f, -10.7f); // Posición inicial
glm::vec3 delfinPosActual = delfinPosBase; // Posición que se actualizará
float delfinRotY = -90.0f; // Inicia mirando a +X
float delfinRotX = 0.0f; // Cabeceo (Pitch)
float DelfinScale = 0.8f; // Escala del delfín

bool animarDelfin = false;
float startTimeDelfin = 0.0f;
bool teclaD_presionada = false; // Usaremos 'X' para activarlo


// -----------------------------------------
//  SELVA (X,Z)
// -----------------------------------------

// CAPIBARA ( X, Z)
float capibaraScale = 1.0f;
float rotCapibara = 180.0f;
float capibaraCabezaRot = 0.0f;
float capibaraNaranjaRot = 0.0f;
float capibaraPataDelDer = 0.0f;
float capibaraPataDelIzq = 0.0f;
float capibaraPataTrasDer = 0.0f;
float capibaraPataTrasIzq = 0.0f;
glm::vec3 capibaraPos = glm::vec3(11.0f, 0.0f, 8.0f);
bool animarCapibara = false;
float startTimeCapibara = 0.0f;
bool teclaP_presionada = false;

//  MONO ( X, Z)
float monoScale = 1.0f;
float rotMono = 180.0f;
float monoColaRot = 0.0f;
float monoPataDelDer = 0.0f;
float monoPataDelIzq = 0.0f;
float monoPataTrasDer = 0.0f;
float monoPataTrasIzq = 0.0f;
glm::vec3 monoPos = glm::vec3(11.0f, 0.0f, 11.0f);
bool animarMono = false;
float startTimeMono = 0.0f;
bool teclaN_presionada = false;

// GUACAMAYA ( X, Z)
float guacamayaScale = 1.0f;
float rotGuacamaya = 270.0f;
float guacamayaAlaDer = 0.0f;
float guacamayaAlaIzq = 0.0f;
glm::vec3 guacamayaPos = glm::vec3(11.1f, 1.55f, 6.5f);
bool animarGuacamaya = false;
float startTimeGuacamaya = 0.0f;
bool teclaO_presionada = false;

// -----------------------------------------
//  SABANA (-X,-Z)
// -----------------------------------------


// -----------------------------------------
//  DESIERTO (-X,Z)
// -----------------------------------------

// CAMELLO ( -X, Z)
float rotCamel = 180.0f;
float camelLegFL = 0.0f;
float camelLegFR = 0.0f;
float camelLegBL = 0.0f;
float camelLegBR = 0.0f;
float camelHead = 0.0f;
float camelTail = 0.0f;
float camelScale = 0.65f;
glm::vec3 camelPos = glm::vec3(-4.0f, -0.5f, 10.0f);
bool animarCamello = false;
float startTimeCamello = 0.0f;
bool teclaM_presionada = false;


// TORTUGA ( -X, Z)
float rotTortuga = 0.0f;
float tortugaLegFL = 0.0f;
float tortugaLegFR = 0.0f;
float tortugaScale = 0.20f;
glm::vec3 tortugaPos = glm::vec3(-7.8f, -0.18f, 9.5f);
bool animarTortuga = false;
float startTimeTortuga = 0.0f;
bool teclaX_presionada = false;

//  CÓNDOR (Cuadrante -X, Z)
float rotCondor = 90.0f;
float condorHead = 0.0f;
float condorAlaIzq = 0.0f;
float condorAlaDer = 0.0f;
float condorScale = 0.70f;
glm::vec3 condorPos = glm::vec3(-6.7f, 0.5f, 6.0f);
bool animarCondor = false;
bool teclaQ_presionada = false;


// -----------------------------------------
//  Habitat Pandas
// -----------------------------------------

// PANDA 1 
float panda1Scale = 0.005f;
float rotPanda1 = 180.0f;
glm::vec3 panda1Pos = glm::vec3(0.0f, -0.5f, 15.0f);



// Vértices para el piso
float vertices[] = {
	// Posiciones           // Normales           // Coordenadas de Textura (U, V)
	// Cara Trasera (-Z)
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	// Cara Frontal (+Z)
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f, 0.0f,
	// Cara Izquierda (-X)
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	// Cara Derecha (+X)
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	 // Cara Inferior (-Y) *** MODIFICADA PARA REPETIR TEXTURA ***
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 10.0f,
	  0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  10.0f, 10.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  10.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  10.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 10.0f,
	 // Cara Superior (+Y) *** MODIFICADA PARA REPETIR TEXTURA ***
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 10.0f,
	  0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  10.0f, 10.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  10.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  10.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 10.0f
};


// Vértices para PAREDES 
float verticesPared[] = {
	//  Cara Trasera (-Z)
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   15.0f, 0.0f,  
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   15.0f, 1.0f,  
	 0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   15.0f, 1.0f,  
	-0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f,   0.0f, 0.0f,




	//  Cara Frontal (+Z)
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   11.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   11.0f, 1.0f, 
	 0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   11.0f, 1.0f, 
	-0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f,   0.0f, 0.0f,


	// Cara Izquierda (-X) 
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   20.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   20.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f,   20.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f,   0.0f, 1.0f,



	// Cara Derecha (+X)
	0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   20.0f, 1.0f,
	0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   20.0f, 0.0f,
	0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f,   20.0f, 0.0f,
	0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f,   0.0f, 1.0f,



	 // Cara Inferior (-Y)
	 -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 5.0f,
	  0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   5.0f, 5.0f,
	  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   5.0f, 0.0f,
	  0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   5.0f, 0.0f,
	 -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 0.0f,
	 -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f,   0.0f, 5.0f,

	 // Cara Superior (+Y)
	 -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 5.0f,
	  0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   5.0f, 5.0f,
	  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   5.0f, 0.0f,
	  0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   5.0f, 0.0f,
	 -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 0.0f,
	 -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f,   0.0f, 5.0f
};

//glm::vec3 lampColors[] = {
//	glm::vec3(1.0f, 0.0f, 0.0f), // Rojo
//	glm::vec3(0.0f, 1.0f, 0.0f), // Verde
//	glm::vec3(0.0f, 0.0f, 1.0f), // Azul
//	glm::vec3(1.0f, 1.0f, 1.0f)  // Blanco
//};

glm::vec3 Light1 = glm::vec3(0);


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame



void pataDraw(glm::mat4 modelo, glm::vec3 escala, glm::vec3 traslado, GLint uniformModel, GLuint VAO, GLuint texturaID)
{
	// 1. Configurar la matriz del modelo para esta parte
	modelo = glm::mat4(1);
	modelo = glm::scale(modelo, escala); // tamaño
	modelo = glm::translate(modelo, traslado);// posición
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelo));

	// 2. Activar y enlazar la textura específica para esta parte
	//    Asumimos que el shader ya tiene los uniforms de sampler (material.diffuse = 0, material.specular = 1)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturaID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturaID); // Usamos la misma textura para difuso y especular

	// 3. Enlazar el VAO del cubo
	glBindVertexArray(VAO);

	// 4. *** CRÍTICO *** Habilitar TODOS los atributos que usa lightingShader
	// (El lampShader los deshabilita, así que hay que volver a habilitarlos)
	glEnableVertexAttribArray(0); // Posición
	glEnableVertexAttribArray(1); // Normal
	glEnableVertexAttribArray(2); // TexCoords

	// 5. Dibujar el cubo
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// 6. Desvincular (buena práctica)
	glBindVertexArray(0);


}

void cuboDraw(glm::mat4 modelo, glm::vec3 escala, glm::vec3 traslado, GLint uniformModel, GLuint VAO, GLuint texturaID, float rotacion)
{
	// 1. Configurar la matriz del modelo para esta parte
	modelo = glm::mat4(1);
	modelo = glm::translate(modelo, traslado);// primero traslaci�n
	modelo = glm::rotate(modelo, rotacion, glm::vec3(0.0f, 1.0f, 0.0f)); // luego rotaci�n
	modelo = glm::scale(modelo, escala); // finalmente escala
	glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(modelo));

	// 2. Activar y enlazar la textura
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturaID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturaID);

	glBindVertexArray(VAO);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}


int main()
{
	// =================================================================================
	// INICIALIZACIÓN DE GLFW, GLEW Y VENTANA
	// =================================================================================

	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Proyecto Final", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	
	/*
	================================================================================
	CARGA DE SHADERS Y MODELOS 3D
	================================================================================

	SHADERS:
	- lightingShader: Shader principal con modelo de iluminación Phong
	- lampShader: Shader simplificado para objetos emisores de luz
	- skyboxShader: Shader especializado para cubemap ambiental
	*/

	// Cargar shaders
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	Shader skyboxShader("Shader/skybox.vs", "Shader/skybox.frag");

	// Animaciones Mixamo
	Shader animShader("Shader/anim.vs", "Shader/anim.fs");


	// Vértices de cubo del skybox
	float skyboxVertices[] = {
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// =================================================================================
	// 						CARGA DE MODELO que sean de FBX hay que usar estas funciones
	//								sino no funcionan las animaciones
	//								ModelAnim y .initShaders(animShader.Program);
	// =================================================================================
	ModelAnim animacionPersonaje((char*)"Models/Personaje1/Arm.dae");
	animacionPersonaje.initShaders(animShader.Program);

	// =================================================================================
	// 						CARGA DE MODELO para Habitat de  PANDAS si es FBX 
	// =================================================================================

	std::cout << "Cargando modelos habitat pandas..." << std::endl;

	// ====== ESCENARIO ======
	Model PinoBosque((char*)"Models/arbolSelva/arbolSelva.obj");
	glm::vec3 pinoBosquePos(11.0f, -0.5f, 3.1f);
	glm::vec3 pinoBosqueScale(0.2f, 0.2f, 0.2f);
	float pinoBosqueRot = 0.0f;

	// ====== PANDA1 ======
	ModelAnim animacionPanda1((char*)"Models/Personaje1/Arm.dae");
	animacionPanda1.initShaders(animShader.Program);

	//// ====== PANDA2 ======
	//ModelAnim animacionPanda2((char*)"Models/Personaje1/Arm.fbx");
	//animacionPanda2.initShaders(animShader.Program);



	std::cout << "Modelos cargados habitat pandas!" << std::endl;


	// =================================================================================
	// 						CARGA DE MODELOS - Acuario (X,-Z)
	// =================================================================================

	Model Piso((char*)"Models/piso.obj");
	Model Delfin((char*)"Models/delfin.obj");
	Model cuerpoPin((char*)"Models/pinguino/cuerpo.obj");
	Model arm1((char*)"Models/pinguino/arm1.obj");
	Model arm2((char*)"Models/pinguino/aletIzq.obj");
	Model Foca1((char*)"Models/foca/cuerpoFoca.obj");
	Model FocaCo((char*)"Models/foca/medio.obj");
	Model FocaMe((char*)"Models/foca/cola.obj");
	Model Agua((char*)"Models/agua/agua.obj");
	Model FocaDer((char*)"Models/foca/FocaDer.obj");
	Model FocaIzq((char*)"Models/foca/FocaIzq.obj");
	Model iglu((char*)"Models/iglu/iglu3d.obj");
	Model huevo((char*)"Models/huevo/huevo.obj");
	Model pino((char*)"Models/pino/nievepino.obj");

	// Carga textura
	GLuint armTextureID = TextureFromFile("images/madera.jpg", ".");
	GLuint greenTextureID = TextureFromFile("images/textverde.jpg", ".");
	GLuint amarilloTextureID = TextureFromFile("images/textamarillo.jpg", ".");
	GLuint cafeTextureID = TextureFromFile("images/cafe.jpg", ".");


	// =================================================================================
	// 						CARGA DE MODELOS - Selva (X,Z)
	// =================================================================================

	std::cout << "Cargando modelos selva..." << std::endl;

	// ====== ESCENARIO ======
	Model ArbolSelva((char*)"Models/arbolSelva/arbolSelva.obj");
	glm::vec3 arbolSelvaPos(11.0f, -0.5f, 3.1f);
	glm::vec3 arbolSelvaScale(0.2f, 0.2f, 0.2f);
	float arbolSelvaRot = 0.0f;

	Model Sandia((char*)"Models/sandia/sandia.obj");
	glm::vec3 sandiaPos(3.0f, -0.2f, 8.0f);
	glm::vec3 sandiaScale(1.0f, 1.0f, 1.0f);
	float sandiaRot = 180.0f;

	Model TroncoSelva1((char*)"Models/troncoSelva/troncoSelva.obj");
	glm::vec3 troncoSelva1Pos(10.0f, -0.2f, 11.0f);
	glm::vec3 troncoSelva1Scale(1.0f, 0.7f, 1.0f);
	float troncoSelva1Rot = 0.0f;

	Model TroncoSelva2((char*)"Models/troncoSelva/troncoSelva.obj");
	glm::vec3 troncoSelva2Pos(5.5f, -0.2f, 11.0f);
	glm::vec3 troncoSelva2Scale(1.0f, 0.5f, 1.0f);
	float troncoSelva2Rot = 0.0f;

	Model Pelota((char*)"Models/pelota/pelota.obj");
	glm::vec3 pelotaPos(5.5f, -0.2f, 11.0f);
	glm::vec3 pelotaScale(0.5f, 0.5f, 0.5f);
	float pelotaRot = 0.0f;

	Model Platano1((char*)"Models/platano/platano.obj");
	glm::vec3 platano1Pos(11.0f, -0.4f, 11.8f);
	glm::vec3 platano1Scale(5.0f, 5.0f, 5.0f);
	float platano1Rot = 0.0f;

	Model Platano2((char*)"Models/platano/platano.obj");
	glm::vec3 platano2Pos(7.6f, -0.4f, 9.8f);
	glm::vec3 platano2Scale(5.0f, 5.0f, 5.0f);
	float platano2Rot = 0.0f;

	Model Gato((char*)"Models/platano/gato.obj");
	glm::vec3 gatoPos(3.2f, 0.0f, 11.9f);
	glm::vec3 gatoScale(0.8f, 0.8f, 0.8f);
	float gatoRot = 90.0f;

	Model ArbolRama((char*)"Models/arbolRama/arbolRama.obj");
	glm::vec3 arbolRamaPos(11.0f, 0.6f, 6.0f);
	glm::vec3 arbolRamaScale(2.5f, 2.5f, 2.5f);
	float arbolRamaRot = 0.0f;

	Model RamaSelva((char*)"Models/ramaSelva/ramaSelva.obj");
	glm::vec3 ramaSelvaPos(3.0f, 0.15f, 6.0f);
	glm::vec3 ramaSelvaScale(2.5f, 2.5f, 4.0f);
	float ramaSelvaRot = 0.0f;

	Model PlantaSelva1((char*)"Models/plantaSelva/planta_selva.obj");
	glm::vec3 plantaSelva1Pos(8.5f, -0.3f, 3.1f);
	glm::vec3 plantaSelva1Scale(0.4f, 0.4f, 0.4f);
	float plantaSelva1Rot = 0.0f;
	glm::vec3 plantaSelva3Pos(-3.8f, -0.3f, -3.1f);

	Model PlantaSelva2((char*)"Models/plantaSelva/planta_selva.obj");
	glm::vec3 plantaSelva2Pos(11.0f, -0.3f, 9.5f);
	glm::vec3 plantaSelva2Scale(0.4f, 0.4f, 0.4f);
	float plantaSelva2Rot = 0.0f;

	Model Loto1((char*)"Models/loto/loto.obj");
	glm::vec3 loto1Pos(6.5f, -0.21f, 3.1f);
	glm::vec3 loto1Scale(1.0f, 1.0f, 1.0f);
	float loto1Rot = 0.0f;

	Model Loto2((char*)"Models/loto/loto.obj");
	glm::vec3 loto2Pos(5.0f, -0.21f, 3.1f);
	glm::vec3 loto2Scale(1.0f, 1.0f, 1.0f);
	float loto2Rot = 0.0f;

	Model Loto3((char*)"Models/loto/loto.obj");
	glm::vec3 loto3Pos(3.5f, -0.21f, 3.1f);
	glm::vec3 loto3Scale(1.0f, 1.0f, 1.0f);
	float loto3Rot = 0.0f;

	// ====== CAPIBARA ======
	Model Capibara_Cabeza((char*)"Models/capibara/cabezaCapi.obj");
	Model Capibara_Cuerpo((char*)"Models/capibara/cuerpoCapi.obj");
	Model Capibara_Naranja((char*)"Models/capibara/naranjaCapi.obj");
	Model Capibara_PataDelDer((char*)"Models/capibara/pataDelDerCapi.obj");
	Model Capibara_PataDelIzq((char*)"Models/capibara/pataDelIzqCapi.obj");
	Model Capibara_PataTrasDer((char*)"Models/capibara/pataTrasDerCapi.obj");
	Model Capibara_PataTrasIzq((char*)"Models/capibara/pataTrasIzqCapi.obj");

	// ====== MONO ======
	Model Mono_Cuerpo((char*)"Models/mono/cuerpoMono.obj");
	Model Mono_Cola((char*)"Models/mono/colaMono.obj");
	Model Mono_PataDelDer((char*)"Models/mono/pataDelDerMono.obj");
	Model Mono_PataDelIzq((char*)"Models/mono/pataDelIzqMono.obj");
	Model Mono_PataTrasDer((char*)"Models/mono/pataTasDerMono.obj");
	Model Mono_PataTrasIzq((char*)"Models/mono/pataTrasIzqMono.obj");

	// ====== GUACAMAYA ======
	Model Ave_Cuerpo((char*)"Models/aveSelva/cuerpoAve.obj");
	Model Ave_AlaDer((char*)"Models/aveSelva/alaDerAve.obj");
	Model Ave_AlaIzq((char*)"Models/aveSelva/alaIzqAve.obj");

	std::cout << "Modelos cargados selva!" << std::endl;

	// =================================================================================
	// 						CARGA DE MODELOS - DESIERTO (-X,Z)
	// =================================================================================

	std::cout << "Cargando modelos desierto..." << std::endl;
	// ====== ESCENARIO ======
	Model Oasis((char*)"Models/oasis/oasis.obj");
	glm::vec3 oasisPos(-9.5f, -0.64f, 9.5f);
	glm::vec3 oasisScale(20.0f, 20.0f, 20.0f);
	float oasisRot = 270.0f;

	Model Huesos((char*)"Models/huesos/huesos.obj");
	glm::vec3 huesosPos(-8.5f, -0.6f, 4.0f);
	glm::vec3 huesosScale(0.3f, 0.25f, 0.25f);
	float huesosRot = 90.0f;

	Model Tronco((char*)"Models/tronco/tronco.obj");
	glm::vec3 troncoPos(-6.8f, -0.5f, 6.0f);
	glm::vec3 troncoScale(0.7f, 0.7f, 0.7f);
	float troncoRot = 0.0f;


	Model Cactus((char*)"Models/cactus/Cactus.obj");
	glm::vec3 cactusPos(-4.0f, -0.5f, 3.7f);
	glm::vec3 cactusScale(0.04f, 0.04f, 0.04f);
	float cactusRot = 0.0f;

	// ====== CAMELLO ======
	Model CamelBody((char*)"Models/camello/CamelBody.obj");
	Model CamelHead((char*)"Models/camello/CamelCabeza.obj");
	Model CamelLeg_FL((char*)"Models/camello/CamelPataizqEnfr.obj");
	Model CamelLeg_FR((char*)"Models/camello/CamelPataEnfreDer.obj");
	Model CamelLeg_BL((char*)"Models/camello/CamelPataizqAtras.obj");
	Model CamelLeg_BR((char*)"Models/camello/CamelPataAtrasDer.obj");

	// ====== TORTUGA ======
	Model TortugaBody1((char*)"Models/tortuga/tortuga_cuerpo.obj");
	Model TortugaLeg_FL1((char*)"Models/tortuga/tortuga_pata_izq.obj");
	Model TortugaLeg_FR1((char*)"Models/tortuga/tortuga_pata_der.obj");


	// ====== CÓNDOR ======
	Model CondorBody((char*)"Models/condor/condor_cuerpo.obj");
	Model CondorHead((char*)"Models/condor/condor_cabeza.obj");
	Model CondorAla_Der((char*)"Models/condor/condor_ala_der.obj");
	Model CondorAla_Izq((char*)"Models/condor/condor_ala_izq.obj");

	std::cout << "Modelos cargados desierto!" << std::endl;

	// =================================================================================
	// 						CARGA DE MODELOS - Sabana (-X,-Z)
	// =================================================================================



	// =================================================================================
	// 						Carga de Texturas para los pisos
	// =================================================================================


	// ***TEXTURA GENERAL PARA EL PISO ***
	GLuint pisoTextureID = TextureFromFile("images/ladrillo.png", ".");
	ConfigurarTexturaRepetible(pisoTextureID);

	// ***TEXTURA  PARA EL PISO ENTRADA ***
	GLuint pisoEntradaID = TextureFromFile("images/pasto.jpg", ".");
	ConfigurarTexturaRepetible(pisoEntradaID);

	// *** TEXTURA PARA LAS PAREDES ***
	GLuint paredTextureID = TextureFromFile("images/muro.jpg", ".");
	ConfigurarTexturaRepetible(paredTextureID);
	//Altura de la pared
	float alturaPared = 3.0f;
	// Escala general del área
	float tamanoBase = 25.0f;

	// *** TEXTURA PARA EL PISO ACUARIO ***
	GLuint pisoAcuarioTextureID = TextureFromFile("images/textnieve.jpg", ".");
	ConfigurarTexturaRepetible(pisoAcuarioTextureID);

	// *** TEXTURA PARA EL PISO SELVA ***
	GLuint pisoSelvaTextureID = TextureFromFile("images/selva.png", ".");
	ConfigurarTexturaRepetible(pisoSelvaTextureID);

	// *** TEXTURA PARA EL PISO SABANA ***
	GLuint pisoSabanaTextureID = TextureFromFile("images/sabana.jpg", ".");
	ConfigurarTexturaRepetible(pisoSabanaTextureID);

	// *** TEXTURA PARA EL PISO DESIERTO ***
	GLuint pisoArenaTextureID = TextureFromFile("images/sand.jpg", ".");
	ConfigurarTexturaRepetible(pisoArenaTextureID);

	// *** TEXTURA PARA EL HÁBITAT DE PANDAS ***
	GLuint pisoPandasTextureID = TextureFromFile("images/bosque.png", ".");
	ConfigurarTexturaRepetible(pisoPandasTextureID);


	// =================================================================================
	// 					CONFIGURACIÓN DE VÉRTICES PARA PRIMITIVAS
	// =================================================================================

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

	// =================================================================================
	// 		CONFIGURACIÓN DE VÉRTICES PARA PISO GENERAL
	// =================================================================================
	GLuint VBO_Cubo, VAO_Cubo;
	ConfigurarVAO(VAO_Cubo, VBO_Cubo, vertices, sizeof(vertices));

	// =================================================================================
	// 		CONFIGURACIÓN DE VÉRTICES PARA PISO DE ENTRADA
	// =================================================================================
	GLuint VBO_Entrada, VAO_Entrada;
	ConfigurarVAO(VAO_Entrada, VBO_Entrada, vertices, sizeof(vertices));


	// =================================================================================
	// 		CONFIGURACIÓN DE VÉRTICES PARA PAREDES
	// =================================================================================
	GLuint VBO_Pared, VAO_Pared;
	ConfigurarVAO(VAO_Pared, VBO_Pared, verticesPared, sizeof(verticesPared));


	//SKYBOX
	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

	// Cargar las 6 caras del cubo del skybox
	vector <const GLchar*> faces;
	faces.push_back("images/skybox/right.jpg");
	faces.push_back("images/skybox/left.jpg");
	faces.push_back("images/skybox/top.jpg");
	faces.push_back("images/skybox/bottom.jpg");
	faces.push_back("images/skybox/back.jpg");
	faces.push_back("images/skybox/front.jpg");
	GLuint cubeMapTexture = TextureLoading::LoadCubemap(faces);


	// =================================================================================
	// 								CICLO DE RENDERIZADO (GAME LOOP)
	// =================================================================================

	while (!glfwWindowShouldClose(window))
	{

		// Delta time, Eventos, Limpieza
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();

		// Clear the colorbuffer
		glClearColor(0.6f, 0.7f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);



		//Load Model


		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Luz Direccional (dirLight)
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.6f, 0.6f, 0.6f);

		// Luces Puntuales (pointLights) - Usando los colores de las lámparas
		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);

		//
		//// Point light 0 (Rojo)
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient")/*, lampColors[0].r * 0.1f, lampColors[0].g * 0.1f, lampColors[0].b * 0.1f*/); // Ambiente Rojo tenue
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse")/*, lampColors[0].r, lampColors[0].g, lampColors[0].b*/);       // Difuso Rojo
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular")/*, lampColors[0].r, lampColors[0].g, lampColors[0].b*/);      // Especular Rojo
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f); // Atenuación específica
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f); // Atenuación específica

		//// Point light 1 (Verde)
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), lampColors[1].r * 0.1f, lampColors[1].g * 0.1f, lampColors[1].b * 0.1f); // Ambiente Verde tenue
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), lampColors[1].r, lampColors[1].g, lampColors[1].b);       // Difuso Verde
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), lampColors[1].r, lampColors[1].g, lampColors[1].b);      // Especular Verde
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f); // Atenuación ejemplo
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f); // Atenuación ejemplo

		//// Point light 2 (Azul)
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), lampColors[2].r * 0.1f, lampColors[2].g * 0.1f, lampColors[2].b * 0.1f); // Ambiente Azul tenue
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), lampColors[2].r, lampColors[2].g, lampColors[2].b);       // Difuso Azul
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), lampColors[2].r, lampColors[2].g, lampColors[2].b);      // Especular Azul
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.09f); // Atenuación ejemplo
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.032f); // Atenuación ejemplo

		//// Point light 3 (Blanco)
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), lampColors[3].r * 0.1f, lampColors[3].g * 0.1f, lampColors[3].b * 0.1f); // Ambiente Blanco tenue
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), lampColors[3].r, lampColors[3].g, lampColors[3].b);       // Difuso Blanco
		//glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), lampColors[3].r, lampColors[3].g, lampColors[3].b);      // Especular Blanco
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.09f); // Atenuación ejemplo
		//glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.032f); // Atenuación ejemplo


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);



		// Point light 2
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), 0.05f, 0.05f, 0.05f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.0f);

		// Point light 3
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.0f);

		// Point light 4
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.0f);
		// SpotLight  //una luz tipo linterna en la cámara
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.8f, 0.8f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 1.0f, 1.0f, 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(6.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(10.0f)));

		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 2.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		GLint lampColorLoc = glGetUniformLocation(lampShader.Program, "lampColor");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 modelTemp = glm::mat4(1.0f);

		// ---------------------------------------------------------------------------------
		// 							CARGA DE TEXTURAS
		// ---------------------------------------------------------------------------------

		//GLuint texMadera, texMetal, texPlastico;
		//int textureWidth, textureHeight, nrChannels;
		//unsigned char* image;

		//// Función lambda para cargar texturas
		//auto cargarTextura = [&](const char* path, GLuint& tex) {
		//	glGenTextures(1, &tex);
		//	glBindTexture(GL_TEXTURE_2D, tex);
		//	stbi_set_flip_vertically_on_load(true);
		//	image = stbi_load(path, &textureWidth, &textureHeight, &nrChannels, 0);
		//	if (image)
		//	{
		//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		//		glGenerateMipmap(GL_TEXTURE_2D);
		//	}
		//	else
		//	{
		//		std::cout << "Failed to load texture: " << path << std::endl;
		//	}
		//	stbi_image_free(image);

		//	// Parámetros de textura
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//	};

		//// Cargar las tres texturas
		//cargarTextura("images/madera.jpg", texMadera);
		//cargarTextura("images/madera.jpg", texMetal);
		//cargarTextura("images/madera.jpg", texPlastico);
		

		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE ESCENARIOS
		// ---------------------------------------------------------------------------------

		// DIBUJO DEL PISO GENERAL LADRILLO

		lightingShader.Use();
		DibujarPiso(pisoTextureID, glm::vec3(0.0f, -0.5f, 0.0f), glm::vec3(25.0f, 0.1f, 25.0f), VAO_Cubo, modelLoc);

		// DIBUJO DEL PASTO ENTRADA
		DibujarPiso(pisoEntradaID, glm::vec3(0.0f, -0.5f, 17.5f), glm::vec3(25.0f, 0.1f, 10.0f), VAO_Cubo, modelLoc);

		// *** DIBUJO DEL PISO FONDO ***
		DibujarPiso(pisoTextureID, glm::vec3(0.0f, -0.5f, -17.5f), glm::vec3(25.0f, 0.1f, 32.0f), VAO_Cubo, modelLoc);


		// =================================================================================
		// 							DIBUJO DE PAREDES
		// =================================================================================

		//  Pared trasera (Z negativa) - EXTENDIDA HASTA EL HÁBITAT DE PANDAS
		DibujarPiso(paredTextureID, glm::vec3(0.0f, alturaPared / 2 - 0.5f, -33.5f),
			glm::vec3(tamanoBase, alturaPared, 0.2f), VAO_Pared, modelLoc);


		// Pared izquierda (X negativa)
		DibujarPiso(paredTextureID, glm::vec3(-tamanoBase / 2, alturaPared / 2 - 0.5f, -10.25f),
			glm::vec3(0.2f, alturaPared, 46.3f), VAO_Pared, modelLoc);

		// Pared derecha (X positiva)
		DibujarPiso(paredTextureID, glm::vec3(tamanoBase / 2, alturaPared / 2 - 0.5f, -10.25f),
			glm::vec3(0.2f, alturaPared, 46.3), VAO_Pared, modelLoc);

		//// Pared de entrada - Lado IZQUIERDO
		//DibujarPiso(paredTextureID, glm::vec3(-7.15f, alturaPared / 2 - 0.5f, 12.5f),
		//	glm::vec3(10.50f, alturaPared, 0.2f), VAO_Pared, modelLoc);

		//// Pared de entrada - Lado DERECHO
		//DibujarPiso(paredTextureID, glm::vec3(7.15f, alturaPared / 2 - 0.5f, 12.5f),
		//	glm::vec3(10.50f, alturaPared, 0.2f), VAO_Pared, modelLoc);

		// =================================================================================
		// 							DIBUJO DE MODELOS - ENTRADA
		// =================================================================================



		// =================================================================================
		// 				DIBUJO DE PERSONAJES ANIMADOS MIXAMO (TODOS)
		// =================================================================================

		// ========== ACTIVAR SHADER DE ANIMACIÓN es una sola vez para todos los personajes ==========
		animShader.Use();

		// Configurar matrices de proyección y vista
		glm::mat4 projectionAnim = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);
		glm::mat4 viewAnim = camera.GetViewMatrix();

		animShader.setMat4("projection", projectionAnim);
		animShader.setMat4("view", viewAnim);

		// Configurar iluminación para TODOS los personajes Mixamo
		animShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		animShader.setVec3("light.diffuse", glm::vec3(0.7f, 0.7f, 0.7f));
		animShader.setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
		animShader.setVec3("light.direction", glm::vec3(-0.4f, -1.0f, -0.2f));
		animShader.setVec3("viewPos", camera.GetPosition());

		// ========== PERSONAJE 1: BAILARÍN ==========
		glm::mat4 modelAnim1 = glm::mat4(1.0f);
		modelAnim1 = glm::translate(modelAnim1, personajeMixamoPos);
		modelAnim1 = glm::rotate(modelAnim1, glm::radians(rotPersonajeMixamo), glm::vec3(0.0f, 1.0f, 0.0f));
		modelAnim1 = glm::scale(modelAnim1, glm::vec3(personajeMixamoScale));
		animShader.setMat4("model", modelAnim1);
		animacionPersonaje.Draw(animShader); 


		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE PANDAS
		// ---------------------------------------------------------------------------------

		// ========== EJEMPLO PANDA 1: ==========
		//glm::mat4 panda1 = glm::mat4(1.0f);
		//panda1 = glm::translate(panda1, panda1Pos);
		//panda1 = glm::rotate(panda1, glm::radians(rotPanda1), glm::vec3(0.0f, 1.0f, 0.0f));
		//panda1 = glm::scale(panda1, glm::vec3(panda1Scale));
		//animShader.setMat4("model", panda1);
		//animacionPanda1.Draw(animShader); 


		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE CAMELLO
		// ---------------------------------------------------------------------------------




		// ========== VOLVER AL SHADER DE ILUMINACIÓN esto se deja hasta el final siempre ==========
		lightingShader.Use();



		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE ESCENARIO HABITAT PANDAS
		// ---------------------------------------------------------------------------------

		//// DIBUJO PISO HABITAT DE PANDAS ***
		DibujarPiso(pisoPandasTextureID, glm::vec3(0.0f, -0.49f, -21.0f), glm::vec3(9.5f, 0.1f, 9.5f), VAO_Cubo, modelLoc);


	// ---------------------------------------------------------------------------------
	// 							DIBUJO DE ESCENARIO ACUARIO
	// ---------------------------------------------------------------------------------

		// **** DIBUJOS DEL PISO Y ACCESORIOS DE ACUARIO ****
		DibujarPiso(pisoAcuarioTextureID, glm::vec3(7.25f, -0.49f, -7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);

		//modelo Agua
		glm::mat4 textAgua = glm::mat4(1.0f);
		textAgua = glm::translate(textAgua, glm::vec3(7.24f, -0.5f, -10.9f));
		textAgua = glm::rotate(textAgua, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		textAgua = glm::scale(textAgua, glm::vec3(1.6f, 0.05f, 3.3f));
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(textAgua));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Agua.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//modelo IGLU
		glm::mat4 Iglu3d = glm::mat4(1.0f);
		Iglu3d = glm::translate(Iglu3d, glm::vec3(9.9f, 0.5f, -7.0f));
		Iglu3d = glm::rotate(Iglu3d, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		Iglu3d = glm::scale(Iglu3d, glm::vec3(4.0f, 4.0f, 4.0f));
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Iglu3d));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		iglu.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//modelo huevo
		glm::mat4 pingu = glm::mat4(1.0f);
		pingu = glm::translate(pingu, glm::vec3(9.9f, -0.3f, -7.0f));
		pingu = glm::rotate(pingu, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		pingu = glm::scale(pingu, glm::vec3(0.4f, 0.4f, 0.4f));
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pingu));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		huevo.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);

		//modelo pino
		glm::mat4 nievePino = glm::mat4(1.0f);
		nievePino = glm::translate(nievePino, glm::vec3(2.9f, 1.15f, -3.1f));
		nievePino = glm::rotate(nievePino, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		nievePino = glm::scale(nievePino, glm::vec3(3.2f, 3.2f, 3.2f));
		//glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(nievePino));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		pino.Draw(lightingShader);
		//glDisable(GL_BLEND);  //Desactiva el canal alfa 
		glBindVertexArray(0);




		// **** DIBUJO DE ANIMALES ACUARIO ****

		// --- FOCA ANIMADA ---
		if (animarFoca)
		{
			// Obtener el tiempo transcurrido
			float t = glfwGetTime() - startTimeFoca;

			// --- Parámetros de la Animación ---
			float moveDuration = 4.0f; // 6 segundos de avance
			float turnDuration = 1.0f; // 1 segundo para girar
			float totalLoopTime = (moveDuration + turnDuration) * 2.0f; // 14 segundos en total
			float t_loop = fmod(t, totalLoopTime); // Tiempo en el bucle actual (0 a 14)

			float moveDist = 6.0f; // Qué tan lejos avanza (distancia total del recorrido)
			float aleteoVelocidad = 4.0f; // Velocidad de aleteo (patas)
			float colaVelocidad = 2.2f; // Velocidad de cola

			// --- Amplitudes SUTILES (Corregidas) ---
			float ampPata = 8.0f;      // Amplitud Patas (Reducido de 25)
			float ampColaMedio = 2.0f; // Amplitud Media (Como tu código anterior)
			float ampColaFin = 3.0f;   // Amplitud Cola (Un poco más que el medio)


			// --- Máquina de Estados (Corregida) ---
			if (t_loop < moveDuration) // --- Estado 1: Mover a +Z (0 a 6 seg) ---
			{
				float phase = t_loop / moveDuration; // 0.0 a 1.0
				focaPosActual.z = focaPosBase.z + (phase * moveDist); // Avanza hacia -Z (acercándose)
				focaPosActual.x = focaPosBase.x; // X se mantiene
				focaRotY = -90.0f; // Mirando hacia -Z

				// Aleteo Patas (en Eje X)
				rotFocaPataDer = sin(t * aleteoVelocidad) * ampPata;
				rotFocaPataIzq = -sin(t * aleteoVelocidad) * ampPata;
				// Aleteo Cola (en Eje Z - Vertical)
				rotFocaMedio = sin(t * colaVelocidad) * ampColaMedio;
				rotFocaCola = sin(t * colaVelocidad + 0.6f) * ampColaFin;
			}
			else if (t_loop < moveDuration + turnDuration) // --- Estado 2: Giro 1 (6 a 7 seg) ---
			{
				focaPosActual.z = focaPosBase.z + moveDist; // Se queda en el destino
				float phase = (t_loop - moveDuration) / turnDuration; // 0.0 a 1.0
				focaRotY = -90.0f + (phase * 180.0f); // Giro de -90 a +90

				// Detener aleteo
				rotFocaPataDer = 0.0f; rotFocaPataIzq = 0.0f;
				rotFocaMedio = 0.0f; rotFocaCola = 0.0f;
			}
			else if (t_loop < (moveDuration * 2.0f) + turnDuration) // --- Estado 3: Mover a -Z (7 a 13 seg) ---
			{
				float phase = (t_loop - (moveDuration + turnDuration)) / moveDuration; // 0.0 a 1.0
				focaPosActual.z = (focaPosBase.z + moveDist) - (phase * moveDist); // Regresa a pos inicial
				focaPosActual.x = focaPosBase.x;
				focaRotY = 90.0f; // Mirando hacia -Z

				// Aleteo Patas (en Eje X)
				rotFocaPataDer = sin(t * aleteoVelocidad) * ampPata;
				rotFocaPataIzq = -sin(t * aleteoVelocidad) * ampPata;
				// Aleteo Cola (en Eje Z - Vertical)
				rotFocaMedio = sin(t * colaVelocidad) * ampColaMedio;
				rotFocaCola = sin(t * colaVelocidad + 0.6f) * ampColaFin;
			}
			else // --- Estado 4: Giro 2 (13 a 14 seg) ---
			{
				focaPosActual.z = focaPosBase.z; // Se queda en el inicio
				float phase = (t_loop - (moveDuration * 2.0f + turnDuration)) / turnDuration; // 0.0 a 1.0
				focaRotY = 90.0f + (phase * 180.0f); // Giro de 90 a 270 (que es -90)

				// Detener aleteo
				rotFocaPataDer = 0.0f; rotFocaPataIzq = 0.0f;
				rotFocaMedio = 0.0f; rotFocaCola = 0.0f;
			}
		}
		else
		{
			// Resetear a posición inicial si no está animada
			focaPosActual = focaPosBase;
			focaRotY = -90.0f; // Listo para empezar (mirando a +Z)
			rotFocaMedio = 0.0f;
			rotFocaCola = 0.0f;
			rotFocaPataDer = 0.0f;
			rotFocaPataIzq = 0.0f;
		}


		// Jerarquía: Cuerpo -> Medio -> Cola
		//          y: Cuerpo -> AletaDer
		//          y: Cuerpo -> AletaIzq
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);

		// 1. CUERPO (Padre Principal) [Foca1]
		glm::mat4 modelCuerpo = glm::mat4(1.0f);
		modelCuerpo = glm::translate(modelCuerpo, focaPosActual); // Mover a la posición animada
		modelCuerpo = glm::rotate(modelCuerpo, glm::radians(focaRotY), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación animada
		modelCuerpo = glm::scale(modelCuerpo, glm::vec3(FocaScale)); // Escala general
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCuerpo));
		Foca1.Draw(lightingShader); // Dibuja Cuerpo

		// 2. MEDIO (Hijo del Cuerpo) [FocaCo]
		glm::mat4 modelMedio = modelCuerpo; // Hereda
		glm::vec3 pivotMedio = glm::vec3(0.0f, 0.0f, -1.0f); // ¡AJUSTA ESTO!
		modelMedio = glm::translate(modelMedio, pivotMedio);
		modelMedio = glm::rotate(modelMedio, glm::radians(rotFocaMedio), glm::vec3(0.0f, 0.0f, 1.0f)); // <-- CORREGIDO: Rotar en Eje Z (Vertical)
		modelMedio = glm::translate(modelMedio, -pivotMedio);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelMedio));
		FocaCo.Draw(lightingShader); // Dibuja Medio

		// 3. COLA (Hijo del Medio) [FocaMe]
		glm::mat4 modelCola = modelMedio; // Hereda
		glm::vec3 pivotCola = glm::vec3(0.0f, 0.0f, -0.8f);
		modelCola = glm::translate(modelCola, pivotCola);
		modelCola = glm::rotate(modelCola, glm::radians(rotFocaCola), glm::vec3(0.0f, 0.0f, 1.0f)); // <-- CORREGIDO: Rotar en Eje Z (Vertical)
		modelCola = glm::translate(modelCola, -pivotCola);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelCola));
		FocaMe.Draw(lightingShader); // Dibuja Cola

		// 4. ALETA DERECHA (Hijo del Cuerpo) [FocaDer]
		glm::mat4 FocaPataDerecha = modelCuerpo; // Hereda del CUERPO
		glm::vec3 pivotPataDer = glm::vec3(0.3f, 0.1f, -0.2f);
		FocaPataDerecha = glm::translate(FocaPataDerecha, pivotPataDer);
		FocaPataDerecha = glm::rotate(FocaPataDerecha, glm::radians(rotFocaPataDer), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotar en Z (Aleteo)
		FocaPataDerecha = glm::translate(FocaPataDerecha, -pivotPataDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(FocaPataDerecha));
		FocaDer.Draw(lightingShader); // Dibuja Aleta Derecha

		// 5. ALETA IZQUIERDA (Hijo del Cuerpo) [FocaIzq]
		glm::mat4 FocaPataIzq = modelCuerpo; // Hereda del CUERPO
		glm::vec3 pivotPataIzq = glm::vec3(-0.3f, 0.1f, -0.2f);
		FocaPataIzq = glm::translate(FocaPataIzq, pivotPataIzq);
		FocaPataIzq = glm::rotate(FocaPataIzq, glm::radians(rotFocaPataIzq), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotar en Z (Aleteo)
		FocaPataIzq = glm::translate(FocaPataIzq, -pivotPataIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(FocaPataIzq));
		FocaIzq.Draw(lightingShader); // Dibuja Aleta Izquierda

		glBindVertexArray(0); // Desvincular al final


		//fin de foca
		// -------Inicio Delfin ----------

		if (animarDelfin)
		{
			float t = glfwGetTime() - startTimeDelfin;

			// --- Definir parámetros del salto ---
			float jumpDuration = 4.0f; // 4 segundos para el salto de ida
			float turnDuration = 1.0f; // 1 segundo para girar
			float totalLoopTime = (jumpDuration + turnDuration) * 2.0f; // 10 segundos en total
			float t_loop = fmod(t, totalLoopTime); // Tiempo en el bucle actual (0 a 10)

			float jumpDist = 9.0f; // Qué tan lejos salta (en X)
			float jumpHeight = 2.8f; // Qué tan alto salta (en Y)
			float pitchAngle = -20.0f; // Ángulo de "mirar arriba" (rotación en X)

			// --- Máquina de Estados de la Animación ---
			if (t_loop < jumpDuration) // --- Estado 1: Primer Salto (hacia +X) ---
			{
				float phase = t_loop / jumpDuration; // 0.0 a 1.0
				delfinPosActual.x = delfinPosBase.x + (phase * jumpDist); // Mover en X
				delfinPosActual.y = delfinPosBase.y + (sin(phase * 3.14159f) * jumpHeight); // Arco en Y
				delfinPosActual.z = delfinPosBase.z; // Z se mantiene constante
				delfinRotX = pitchAngle; // Mirar arriba
				delfinRotY = 0.0f; // Mirando hacia +X
			}
			else if (t_loop < jumpDuration + turnDuration) // --- Estado 2: Primer Giro (gira a -X) ---
			{
				delfinPosActual.x = delfinPosBase.x + jumpDist; // Se queda en la posición de aterrizaje
				delfinPosActual.y = delfinPosBase.y;
				delfinPosActual.z = delfinPosBase.z;
				delfinRotX = 0.0f; // Se nivela

				float phase = (t_loop - jumpDuration) / turnDuration; // 0.0 a 1.0
				delfinRotY = 0.0f + (phase * 180.0f); // Giro de -90 a +90 grados
			}
			else if (t_loop < jumpDuration * 2.0f + turnDuration) // --- Estado 3: Segundo Salto (hacia -X) ---
			{
				float phase = (t_loop - (jumpDuration + turnDuration)) / jumpDuration; // 0.0 a 1.0
				delfinPosActual.x = (delfinPosBase.x + jumpDist) - (phase * jumpDist); // Mover de regreso en X
				delfinPosActual.y = delfinPosBase.y + (sin(phase * 3.14159f) * jumpHeight); // Arco en Y
				delfinPosActual.z = delfinPosBase.z;
				delfinRotX = pitchAngle; // Mirar arriba
				delfinRotY = -180.0f; // Mirando hacia -X
			}
			else // --- Estado 4: Segundo Giro (gira a +X) ---
			{
				delfinPosActual.x = delfinPosBase.x; // Se queda en la posición inicial
				delfinPosActual.y = delfinPosBase.y;
				delfinPosActual.z = delfinPosBase.z;
				delfinRotX = 0.0f; // Se nivela

				float phase = (t_loop - (jumpDuration * 2.0f + turnDuration)) / turnDuration; // 0.0 a 1.0
				delfinRotY = -180.0f + (phase * 180.0f); // Giro de 90 a 270 grados (que es -90)
			}
		}
		else
		{
			// Resetear a posición inicial si no está animada
			delfinPosActual = delfinPosBase;
			delfinRotX = 0.0f;
			delfinRotY = 0.0f; // Mirando hacia +X (listo para empezar)
		}

		glm::mat4 delfin = glm::mat4(1.0f);
		delfin = glm::translate(delfin, delfinPosActual); // 1. Mover a la posición actual
		delfin = glm::rotate(delfin, glm::radians(delfinRotY), glm::vec3(0.0f, 1.0f, 0.0f)); // 2. Girar (Yaw)
		delfin = glm::rotate(delfin, glm::radians(delfinRotX), glm::vec3(0.0f, 0.0f, 1.0f)); // 3. Cabecear (Pitch)
		delfin = glm::scale(delfin, glm::vec3(DelfinScale)); // 4. Escalar

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(delfin));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Delfin.Draw(lightingShader);
		glBindVertexArray(0);

		// -------Fin Delfin ----------


		// --- INICIO PINGUINO ---
		if (animarPinguino)
		{
			// Obtener el tiempo transcurrido desde que se inició la animación
			float t = glfwGetTime() - startTimePinguino;

			// Definir la animación
			float amplitud = 14.0f; // Qué tanto suben y bajan (en grados)
			float velocidad = 3.0f;  // Qué tan rápido aletea
			float aleteo = sin(t * velocidad) * amplitud;

			PinAlaIzq = aleteo;
			PinAlaDer = -aleteo; // Ala opuesta para movimiento alternado
		}
		// Cuerpo del Pinguino
		glm::mat4 PinguinoCuerpo = glm::mat4(1.0f);
		PinguinoCuerpo = glm::translate(PinguinoCuerpo, PinguinoPos); // Usa la variable de posición
		PinguinoCuerpo = glm::rotate(PinguinoCuerpo, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Rotación base
		PinguinoCuerpo = glm::scale(PinguinoCuerpo, glm::vec3(PinScale)); // Escala
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(PinguinoCuerpo));
		cuerpoPin.Draw(lightingShader);
		glBindVertexArray(0);

		// Ala Derecha (arm1)
		glm::mat4 PinguinoDer = glm::mat4(1.0f);
		PinguinoDer = glm::translate(PinguinoDer, PinguinoPos); // 1. Mover a la posición base
		PinguinoDer = glm::rotate(PinguinoDer, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 2. Rotar base

		// *** APLICAR ANIMACIÓN DE ALATEO (Rotación en Eje Z) ***
		PinguinoDer = glm::rotate(PinguinoDer, glm::radians(PinAlaDer), glm::vec3(1.0f, 0.0f, 0.0f));

		PinguinoDer = glm::scale(PinguinoDer, glm::vec3(PinScale)); // 4. Escalar
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(PinguinoDer));
		arm1.Draw(lightingShader);
		glBindVertexArray(0);

		// Ala Izquierda (arm2)
		// Nota: El código original tenía un error, usaba PinguinoDer como base para PinguinoIzq
		glm::mat4 PinguinoIzq = glm::mat4(1.0f);
		PinguinoIzq = glm::translate(PinguinoIzq, PinguinoPos); // 1. Mover a la posición base
		PinguinoIzq = glm::rotate(PinguinoIzq, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 2. Rotar base

		// *** APLICAR ANIMACIÓN DE ALATEO (Rotación en Eje Z) ***
		PinguinoIzq = glm::rotate(PinguinoIzq, glm::radians(PinAlaIzq), glm::vec3(1.0f, 0.0f, 0.0f));

		PinguinoIzq = glm::scale(PinguinoIzq, glm::vec3(PinScale)); // 4. Escalar
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(PinguinoIzq));
		arm2.Draw(lightingShader);
		glBindVertexArray(0);

		// --- FIN PINGUINO ---


		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS SABANA (x,z)
		// ---------------------------------------------------------------------------------
		
		// **** DIBUJO DEL PISO SELVA Y ACCESORIOS SELVA ****
		DibujarPiso(pisoSelvaTextureID, glm::vec3(7.25f, -0.49f, 7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);
		// --- ÁRBOL ---
		model = glm::mat4(1);
		model = glm::translate(model, arbolSelvaPos);
		model = glm::scale(model, arbolSelvaScale);
		model = glm::rotate(model, glm::radians(arbolSelvaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ArbolSelva.Draw(lightingShader);

		// --- SANDÍA ---
		model = glm::mat4(1);
		model = glm::translate(model, sandiaPos);
		model = glm::scale(model, sandiaScale);
		model = glm::rotate(model, glm::radians(sandiaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Sandia.Draw(lightingShader);

		// --- TRONCO 1 ---
		model = glm::mat4(1);
		model = glm::translate(model, troncoSelva1Pos);
		model = glm::scale(model, troncoSelva1Scale);
		model = glm::rotate(model, glm::radians(troncoSelva1Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TroncoSelva1.Draw(lightingShader);

		// --- PELOTA ---
		model = glm::mat4(1);
		model = glm::translate(model, pelotaPos);
		model = glm::scale(model, pelotaScale);
		model = glm::rotate(model, glm::radians(pelotaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Pelota.Draw(lightingShader);

		// --- PLATANO 1 ---
		model = glm::mat4(1);
		model = glm::translate(model, platano1Pos);
		model = glm::scale(model, platano1Scale);
		model = glm::rotate(model, glm::radians(platano1Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Platano1.Draw(lightingShader);

		// --- PLATANO 2 ---
		model = glm::mat4(1);
		model = glm::translate(model, platano2Pos);
		model = glm::scale(model, platano2Scale);
		model = glm::rotate(model, glm::radians(platano2Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Platano2.Draw(lightingShader);

		// --- GATO ---
		model = glm::mat4(1);
		model = glm::translate(model, gatoPos);
		model = glm::scale(model, gatoScale);
		model = glm::rotate(model, glm::radians(gatoRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Gato.Draw(lightingShader);

		// --- ÁRBOL MUERTO ---
		model = glm::mat4(1);
		model = glm::translate(model, arbolRamaPos);
		model = glm::scale(model, arbolRamaScale);
		model = glm::rotate(model, glm::radians(arbolRamaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		ArbolRama.Draw(lightingShader);

		// --- RAMA ---
		model = glm::mat4(1);
		model = glm::translate(model, ramaSelvaPos);
		model = glm::scale(model, ramaSelvaScale);
		model = glm::rotate(model, glm::radians(ramaSelvaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		RamaSelva.Draw(lightingShader);

		//--- PLANTA 1 ---
		model = glm::mat4(1);
		model = glm::translate(model, plantaSelva1Pos);
		model = glm::scale(model, plantaSelva1Scale);
		model = glm::rotate(model, glm::radians(plantaSelva1Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PlantaSelva1.Draw(lightingShader);

		//--- PLANTA 2 ---
		model = glm::mat4(1);
		model = glm::translate(model, plantaSelva2Pos);
		model = glm::scale(model, plantaSelva2Scale);
		model = glm::rotate(model, glm::radians(plantaSelva2Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		PlantaSelva2.Draw(lightingShader);

		// --- LOTO 1 ---
		model = glm::mat4(1);
		model = glm::translate(model, loto1Pos);
		model = glm::scale(model, loto1Scale);
		model = glm::rotate(model, glm::radians(loto1Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Loto1.Draw(lightingShader);

		// --- LOTO 2 ---
		model = glm::mat4(1);
		model = glm::translate(model, loto2Pos);
		model = glm::scale(model, loto2Scale);
		model = glm::rotate(model, glm::radians(loto2Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Loto2.Draw(lightingShader);

		// --- LOTO 3 ---
		model = glm::mat4(1);
		model = glm::translate(model, loto3Pos);
		model = glm::scale(model, loto3Scale);
		model = glm::rotate(model, glm::radians(loto3Rot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Loto3.Draw(lightingShader);

		// **** DIBUJO DEL CAPIBARA ****
		model = glm::mat4(1);
		model = glm::translate(model, capibaraPos);
		model = glm::rotate(model, glm::radians(rotCapibara), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(capibaraScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_Cuerpo.Draw(lightingShader);

		// Cabeza
		glm::vec3 capibaraPivotCabeza(0.0f, 0.5f, 0.4f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotCabeza);
		model = glm::rotate(model, glm::radians(capibaraCabezaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -capibaraPivotCabeza);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_Cabeza.Draw(lightingShader);

		// Pata delantera derecha
		glm::vec3 capibaraPivotPataDelDer(0.2f, 0.3f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotPataDelDer);
		model = glm::rotate(model, glm::radians(capibaraPataDelDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -capibaraPivotPataDelDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_PataDelDer.Draw(lightingShader);

		// Pata delantera izquierda
		glm::vec3 capibaraPivotPataDelIzq(-0.2f, 0.3f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotPataDelIzq);
		model = glm::rotate(model, glm::radians(capibaraPataDelIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -capibaraPivotPataDelIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_PataDelIzq.Draw(lightingShader);

		// Pata trasera derecha
		glm::vec3 capibaraPivotPataTrasDer(0.2f, 0.3f, -0.3f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotPataTrasDer);
		model = glm::rotate(model, glm::radians(capibaraPataTrasDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -capibaraPivotPataTrasDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_PataTrasDer.Draw(lightingShader);

		// Pata trasera izquierda
		glm::vec3 capibaraPivotPataTrasIzq(-0.2f, 0.3f, -0.3f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotPataTrasIzq);
		model = glm::rotate(model, glm::radians(capibaraPataTrasIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -capibaraPivotPataTrasIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_PataTrasIzq.Draw(lightingShader);

		// Naranja
		glm::vec3 capibaraPivotNaranja(0.0f, 0.0f, 0.0f);
		model = modelTemp;
		model = glm::translate(model, capibaraPivotNaranja);
		model = glm::rotate(model, glm::radians(capibaraNaranjaRot), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, -capibaraPivotNaranja);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Capibara_Naranja.Draw(lightingShader);

		// CAPIBARA - ANIMACIÓN
		if (animarCapibara)
		{
			float t = glfwGetTime() - startTimeCapibara;

			// FASE 1: CAMINANDO (10 segundos)
			if (t < 10.0f)
			{
				float totalDist = 7.0f;
				capibaraPos.x = 11.0f - (t * (totalDist / 10.0f));

				// Movimiento de patas (caminata de cuadrúpedo)
				float paso = sin(t * 6.0f);
				capibaraPataDelDer = paso * 1.5f;
				capibaraPataTrasDer = paso * 1.5f;
				capibaraPataDelIzq = -paso * 1.5f;
				capibaraPataTrasIzq = -paso * 1.5f;

				// Naranja rotando mientras camina
				capibaraNaranjaRot = t * 180.0f;

				capibaraCabezaRot = 0.0f;
				rotCapibara = 180.0f;
			}
			// FASE 2: DETENIDO, COMIENDO (6 segundos)
			else if (t < 16.0f)
			{
				float t2 = t - 10.0f;
				capibaraPos.x = 4.0f;

				// Patas deteniéndose gradualmente
				capibaraPataDelDer = sin(t2 * 0.5f) * 2.0f;
				capibaraPataDelIzq = -capibaraPataDelDer;
				capibaraPataTrasDer = -capibaraPataDelDer;
				capibaraPataTrasIzq = capibaraPataDelDer;

				// Naranja regresa a posición inicial
				capibaraNaranjaRot = 1800.0f - (t2 * 300.0f);

				capibaraCabezaRot = 0.0f;
				rotCapibara = 180.0f;
			}
			// FASE 3: QUIETO
			else
			{
				capibaraPos.x = 4.0f;
				capibaraCabezaRot = 0.0f;
				capibaraPataDelDer = capibaraPataDelIzq = 0.0f;
				capibaraPataTrasDer = capibaraPataTrasIzq = 0.0f;
				capibaraNaranjaRot = 0.0f;
				rotCapibara = 180.0f;
			}
		}

		// **** DIBUJO DEL MONO ****
		model = glm::mat4(1);
		model = glm::translate(model, monoPos);
		model = glm::rotate(model, glm::radians(rotMono), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(monoScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_Cuerpo.Draw(lightingShader);

		// Cola
		glm::vec3 monoPivotCola(0.0f, 0.5f, -0.4f);
		model = modelTemp;
		model = glm::translate(model, monoPivotCola);
		model = glm::rotate(model, glm::radians(monoColaRot), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -monoPivotCola);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_Cola.Draw(lightingShader);

		// Pata delantera derecha
		glm::vec3 monoPivotPataDelDer(0.2f, 0.3f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, monoPivotPataDelDer);
		model = glm::rotate(model, glm::radians(monoPataDelDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -monoPivotPataDelDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_PataDelDer.Draw(lightingShader);

		// Pata delantera izquierda
		glm::vec3 monoPivotPataDelIzq(-0.2f, 0.3f, 0.3f);
		model = modelTemp;
		model = glm::translate(model, monoPivotPataDelIzq);
		model = glm::rotate(model, glm::radians(monoPataDelIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -monoPivotPataDelIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_PataDelIzq.Draw(lightingShader);

		// Pata trasera derecha
		glm::vec3 monoPivotPataTrasDer(0.2f, 0.3f, -0.3f);
		model = modelTemp;
		model = glm::translate(model, monoPivotPataTrasDer);
		model = glm::rotate(model, glm::radians(monoPataTrasDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -monoPivotPataTrasDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_PataTrasDer.Draw(lightingShader);

		// Pata trasera izquierda
		glm::vec3 monoPivotPataTrasIzq(-0.2f, 0.3f, -0.3f);
		model = modelTemp;
		model = glm::translate(model, monoPivotPataTrasIzq);
		model = glm::rotate(model, glm::radians(monoPataTrasIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -monoPivotPataTrasIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Mono_PataTrasIzq.Draw(lightingShader);

		// MONO - ANIMACIÓN
		if (animarMono)
		{
			float t = glfwGetTime() - startTimeMono;

			// FASE 1: PRIMER BRINCO (1.5 segundos)
			if (t < 1.5f)
			{
				float saltoCiclo = abs(sin(t * 2.094f));
				monoPos.y = 0.0f + (saltoCiclo * 1.0f);
				monoPos.x = 11.0f - (t * 1.5f);

				// Patas recogidas durante el salto
				monoPataDelDer = saltoCiclo * 8.0f;
				monoPataDelIzq = saltoCiclo * 8.0f;
				monoPataTrasDer = saltoCiclo * 8.0f;
				monoPataTrasIzq = saltoCiclo * 8.0f;

				// Cola levantada durante el salto
				monoColaRot = 2.0f + (saltoCiclo * 1.5f);
				rotMono = 180.0f;
			}
			// PAUSA 1: ATERRIZA (0.2 segundos)
			else if (t < 1.7f)
			{
				monoPos.y = 0.0f;
				monoPos.x = 8.75f;
				monoPataDelDer = monoPataDelIzq = 0.0f;
				monoPataTrasDer = monoPataTrasIzq = 0.0f;
				monoColaRot = 0.5f;
				rotMono = 180.0f;
			}
			// FASE 2: SEGUNDO BRINCO (1.5 segundos)
			else if (t < 3.2f)
			{
				float t2 = t - 1.7f;
				float saltoCiclo = abs(sin(t2 * 2.094f));
				monoPos.y = 0.0f + (saltoCiclo * 1.0f);
				monoPos.x = 8.75f - (t2 * 1.5f);

				// Patas recogidas durante el salto
				monoPataDelDer = saltoCiclo * 8.0f;
				monoPataDelIzq = saltoCiclo * 8.0f;
				monoPataTrasDer = saltoCiclo * 8.0f;
				monoPataTrasIzq = saltoCiclo * 8.0f;

				// Cola levantada durante el salto
				monoColaRot = 2.0f + (saltoCiclo * 1.5f);
				rotMono = 180.0f;
			}
			// PAUSA 2: ATERRIZA (0.2 segundos)
			else if (t < 3.4f)
			{
				monoPos.y = 0.0f;
				monoPos.x = 6.5f;
				monoPataDelDer = monoPataDelIzq = 0.0f;
				monoPataTrasDer = monoPataTrasIzq = 0.0f;
				monoColaRot = 0.5f;
				rotMono = 180.0f;
			}
			// FASE 3: TERCER BRINCO (1.5 segundos)
			else if (t < 4.9f)
			{
				float t3 = t - 3.4f;
				float saltoCiclo = abs(sin(t3 * 2.094f));
				monoPos.y = 0.0f + (saltoCiclo * 1.0f);
				monoPos.x = 6.5f - (t3 * 1.5f);

				// Patas recogidas durante el salto
				monoPataDelDer = saltoCiclo * 8.0f;
				monoPataDelIzq = saltoCiclo * 8.0f;
				monoPataTrasDer = saltoCiclo * 8.0f;
				monoPataTrasIzq = saltoCiclo * 8.0f;

				// Cola levantada durante el salto
				monoColaRot = 2.0f + (saltoCiclo * 1.5f);
				rotMono = 180.0f;
			}
			// FASE 4: CAMINANDO (1 segundo)
			else if (t < 5.9f)
			{
				float t4 = t - 4.9f;
				monoPos.y = 0.0f;
				monoPos.x = 4.25f - (t4 * 1.0f);

				// Movimiento de patas (caminata de cuadrúpedo)
				float paso = sin(t4 * 6.0f);
				monoPataDelDer = paso * 2.0f;
				monoPataTrasDer = paso * 2.0f;
				monoPataDelIzq = -paso * 2.0f;
				monoPataTrasIzq = -paso * 2.0f;

				// Cola moviéndose mientras camina
				monoColaRot = sin(t4 * 2.0f) * 2.0f;
				rotMono = 180.0f;
			}
			// FASE 5: QUIETO

			else
			{
				monoPos.x = 3.f;
				monoPos.y = 0.0f;
				monoColaRot = 0.0f;
				monoPataDelDer = monoPataDelIzq = 0.0f;
				monoPataTrasDer = monoPataTrasIzq = 0.0f;
				rotMono = 180.0f;
			}
		}

		// **** DIBUJO DE LA GUACAMAYA ****
		model = glm::mat4(1);
		model = glm::translate(model, guacamayaPos);
		model = glm::rotate(model, glm::radians(rotGuacamaya), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(guacamayaScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ave_Cuerpo.Draw(lightingShader);

		// Ala derecha
		glm::vec3 guacamayaPivotAlaDer(0.3f, 0.0f, 0.0f);
		model = modelTemp;
		model = glm::translate(model, guacamayaPivotAlaDer);
		model = glm::rotate(model, glm::radians(guacamayaAlaDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -guacamayaPivotAlaDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ave_AlaDer.Draw(lightingShader);

		// Ala izquierda
		glm::vec3 guacamayaPivotAlaIzq(-0.3f, 0.0f, 0.0f);
		model = modelTemp;
		model = glm::translate(model, guacamayaPivotAlaIzq);
		model = glm::rotate(model, glm::radians(guacamayaAlaIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -guacamayaPivotAlaIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Ave_AlaIzq.Draw(lightingShader);

		// GUACAMAYA - ANIMACIÓN
		if (animarGuacamaya)
		{
			float t = glfwGetTime() - startTimeGuacamaya;

			// FASE 1: Volando (8 segundos)
			if (t < 8.0f)
			{
				// Vuelo en línea recta
				guacamayaPos.x = 11.1f - (t * 1.0f);
				guacamayaPos.y = 1.55f;
				guacamayaPos.z = 6.5f;

				// Aleteo suave
				float aleteo = sin(t * 2.5f);
				guacamayaAlaDer = aleteo * 5.0f;
				guacamayaAlaIzq = -aleteo * 5.0f;
				rotGuacamaya = 270.0f;
			}
			// FASE 2: Descendiendo hacia la rama (4 segundos)
			else if (t < 12.0f)
			{
				float t2 = t - 8.0f;
				guacamayaPos.x = 3.1f - (t2 * 0.025f);
				guacamayaPos.y = 1.55f - (t2 * 0.2625f);
				guacamayaPos.z = 6.5f;

				// Alas casi quietas al planear
				guacamayaAlaDer = 3.0f;
				guacamayaAlaIzq = -3.0f;
				rotGuacamaya = 270.0f;
			}
			// FASE 3: Posada en la rama
			else
			{
				guacamayaPos.x = 3.0f;
				guacamayaPos.y = 0.5f;
				guacamayaPos.z = 6.5f;
				guacamayaAlaDer = 0.0f;
				guacamayaAlaIzq = 0.0f;
				rotGuacamaya = 270.0f;
			}
		}

		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS SABANA (-x,-z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO SABANA Y ACCESORIOS SABANA ****
		DibujarPiso(pisoSabanaTextureID, glm::vec3(-7.25f, -0.49f, -7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);



		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS DESIERTO (-x,z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO DESIERTO  Y COMPONENTES ****

		DibujarPiso(pisoArenaTextureID, glm::vec3(-7.25f, -0.49f, 7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);

		// --- OASIS ---
		model = glm::mat4(1);
		model = glm::translate(model, oasisPos);
		model = glm::scale(model, oasisScale);
		model = glm::rotate(model, glm::radians(oasisRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Oasis.Draw(lightingShader);

		// --- HUESOS ---
		model = glm::mat4(1);
		model = glm::translate(model, huesosPos);
		model = glm::scale(model, huesosScale);
		model = glm::rotate(model, glm::radians(huesosRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Huesos.Draw(lightingShader);

		// --- TRONCO ---
		model = glm::mat4(1);
		model = glm::translate(model, troncoPos);
		model = glm::scale(model, troncoScale);
		model = glm::rotate(model, glm::radians(troncoRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Tronco.Draw(lightingShader);

		// --- CACTUS ---
		model = glm::mat4(1);
		model = glm::translate(model, cactusPos);
		model = glm::scale(model, cactusScale);
		model = glm::rotate(model, glm::radians(cactusRot), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		Cactus.Draw(lightingShader);

		// **** DIBUJO DE ANIMALES DESIERTO ****

		//CAMELLO

		if (animarCamello)
		{
			float t = glfwGetTime() - startTimeCamello;

			// CAMINANDO HACIA EL CACTUS
			if (t < 8.0f)
			{
				// Movimiento de avance 
				float totalDist = 10.0f - 5.0f;
				camelPos.z = 10.0f - (t * (totalDist / 8.0f));

				// Movimiento de patas
				float paso = sin(t * 2.0f);
				camelLegFL = paso * 15.0f;  // pata frontal izq
				camelLegBR = paso * 15.0f;  // pata trasera der
				camelLegFR = -paso * 15.0f; // pata frontal der (opuesta)
				camelLegBL = -paso * 15.0f; // pata trasera izq (opuesta)

				// Cabeza 
				camelHead = sin(t * 0.5f) * 1.3f;
				rotCamel = 180.0f;
			}

			// LLEGA AL CACTUS
			else if (t < 14.0f)
			{
				float t2 = t - 8.0f;
				camelPos.z = 5.0f; // está en el cactus 

				// Detiene patas
				camelLegFL = sin(t2 * 1.0f) * 5.0f;
				camelLegFR = -camelLegFL;
				camelLegBL = -camelLegFR;
				camelLegBR = camelLegFR;

				// Movimiento de cabeza
				camelHead = abs(sin(t2 * 1.5f)) * 2.9f;
				rotCamel = 180.0f;
			}

			else
			{
				camelPos.z = 5.0f;
				camelHead = 0.0f;
				camelLegFL = camelLegFR = camelLegBL = camelLegBR = 0.0f;
				rotCamel = 180.0f;
			}
		}


		// DIBUJO DEL CAMELLO
		model = glm::mat4(1);
		model = glm::translate(model, camelPos);
		model = glm::rotate(model, glm::radians(rotCamel), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(camelScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelBody.Draw(lightingShader);

		// Cabeza
		model = modelTemp;
		model = glm::rotate(model, glm::radians(camelHead), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelHead.Draw(lightingShader);

		// Pierna delantera izquierda
		glm::vec3 camelPivotFL(0.3f, 1.2f, 0.5f);
		model = modelTemp;
		model = glm::translate(model, camelPivotFL);
		model = glm::rotate(model, glm::radians(camelLegFL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -camelPivotFL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelLeg_FL.Draw(lightingShader);

		// Pierna delantera derecha
		glm::vec3 camelPivotFR(-0.3f, 1.2f, 0.5f);
		model = modelTemp;
		model = glm::translate(model, camelPivotFR);
		model = glm::rotate(model, glm::radians(camelLegFR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -camelPivotFR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelLeg_FR.Draw(lightingShader);

		// Pierna trasera izquierda
		glm::vec3 camelPivotBL(0.3f, 1.2f, -0.5f);
		model = modelTemp;
		model = glm::translate(model, camelPivotBL);
		model = glm::rotate(model, glm::radians(camelLegBL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -camelPivotBL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelLeg_BL.Draw(lightingShader);

		// Pierna trasera derecha
		glm::vec3 camelPivotBR(-0.3f, 1.2f, -0.5f);
		model = modelTemp;
		model = glm::translate(model, camelPivotBR);
		model = glm::rotate(model, glm::radians(camelLegBR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -camelPivotBR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CamelLeg_BR.Draw(lightingShader);



		//CONDOR

		if (animarCondor) {
			float t = glfwGetTime();
			condorAlaIzq = sin(t * 10.0f) * 1.5f;
			condorAlaDer = -condorAlaIzq;
			condorHead = sin(t * 8.0f) * 1.0f;
			condorPos.y = 0.7f + sin(t * 0.8f) * 0.15f;
		}

		// Cuerpo condor
		model = glm::mat4(1);
		model = glm::translate(model, condorPos);
		model = glm::rotate(model, glm::radians(rotCondor), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(condorScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CondorBody.Draw(lightingShader);

		// Cabeza
		model = modelTemp;
		model = glm::rotate(model, glm::radians(condorHead), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CondorHead.Draw(lightingShader);

		// Ala izquierda
		glm::vec3 condorPivotAlaIzq(0.5f, 0.5f, 0.0f);
		model = modelTemp;
		model = glm::translate(model, condorPivotAlaIzq);
		model = glm::rotate(model, glm::radians(condorAlaIzq), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -condorPivotAlaIzq);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CondorAla_Izq.Draw(lightingShader);

		// Ala derecha
		glm::vec3 condorPivotAlaDer(-0.5f, 0.5f, 0.0f);
		model = modelTemp;
		model = glm::translate(model, condorPivotAlaDer);
		model = glm::rotate(model, glm::radians(condorAlaDer), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::translate(model, -condorPivotAlaDer);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		CondorAla_Der.Draw(lightingShader);

		//TORTUGA - ANIMACIÓN

		if (animarTortuga)
		{
			float t = glfwGetTime() - startTimeTortuga; // tiempo de animación

			//Camina hacia el agua
			if (t < 2.5f)
			{
				tortugaPos.x = -7.8f - (t * 0.08f);
				tortugaPos.y = -0.18f;
				rotTortuga = 0.0f;
				tortugaScale = 0.20f;
			}
			//Empieza a sumergirse
			else if (t < 6.0f)
			{
				float t2 = t - 2.5f;
				tortugaPos.x = -8.0f - (t2 * 0.4f);    // avanza en el agua 
				tortugaPos.y = -0.18f - (t2 * 0.07f);  // baja más suave
				tortugaScale = 0.20f - (t2 * 0.012f);  // se achica 
				rotTortuga = sin(t2 * 0.5f) * 5.0f;
			}
			//Sale del agua
			else if (t < 9.0f)
			{
				float t3 = t - 6.0f;
				tortugaPos.x = -9.4f - (t3 * 0.7f);
				tortugaPos.y = -0.425f + (t3 * 0.0483f);
				tortugaScale = 0.19f + (t3 * 0.0033f);
				rotTortuga = t3 * 60.0f;
			}
			//gira
			else {
				tortugaPos.x = -11.5f;
				tortugaPos.y = -0.28f;
				tortugaScale = 0.20f;
				rotTortuga = 180.0f;
			}
		}

		//Cuerpo tortuga desierto
		model = glm::mat4(1);
		model = glm::translate(model, tortugaPos);
		model = glm::rotate(model, glm::radians(rotTortuga), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(tortugaScale));
		modelTemp = model;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TortugaBody1.Draw(lightingShader);

		// Pata delantera izquierda
		glm::vec3 tortugaPivotFL(0.2f, 0.0f, 0.2f);
		model = modelTemp;
		model = glm::translate(model, tortugaPivotFL);
		model = glm::rotate(model, glm::radians(tortugaLegFL), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -tortugaPivotFL);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TortugaLeg_FL1.Draw(lightingShader);

		// Pata delantera derecha
		glm::vec3 tortugaPivotFR(-0.2f, 0.0f, 0.2f);
		model = modelTemp;
		model = glm::translate(model, tortugaPivotFR);
		model = glm::rotate(model, glm::radians(tortugaLegFR), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::translate(model, -tortugaPivotFR);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		TortugaLeg_FR1.Draw(lightingShader);


		/*lightingShader.Use();
		GLint objectColorLoc = glGetUniformLocation(lightingShader.Program, "objectColor");*/

		/*glUniform3f(objectColorLoc, 0.0f, 1.0f, 0.0f);*/
		/*pataDraw(model, glm::vec3(0.1f, 1.3f, 0.1f), glm::vec3(14.5f, -0.5f, 9.5f), modelLoc, VAO_Cubo, armTextureID);
		pataDraw(model, glm::vec3(0.1f, 1.3f, 0.1f), glm::vec3(-14.5f, -0.5f, 9.5f), modelLoc, VAO_Cubo, armTextureID);
		pataDraw(model, glm::vec3(0.1f, 1.3f, 0.1f), glm::vec3(-14.5f, -0.5f, -9.5f), modelLoc, VAO_Cubo, armTextureID);
		pataDraw(model, glm::vec3(0.1f, 1.3f, 0.1f), glm::vec3(14.5f, -0.5f, -9.5f), modelLoc, VAO_Cubo, armTextureID);
		pataDraw(model, glm::vec3(0.1f, 0.5f, 0.1f), glm::vec3(0.5f, 0.5f, -0.5f), modelLoc, VAO_Cubo, armTextureID);
		pataDraw(model, glm::vec3(0.5f, 0.1f, 0.5f), glm::vec3(0.5f, 3.0f, -0.5f), modelLoc, VAO_Cubo, armTextureID);
		pataDraw(model, glm::vec3(80.0f, 4.0f, 0.2f), glm::vec3(0.0f, 0.41f, -120.0f), modelLoc, VAO_Cubo, armTextureID);//pared uno
		pataDraw(model, glm::vec3(0.2f, 4.0f, 48.0f), glm::vec3(200.0f, 0.41f, 0.0f), modelLoc, VAO_Cubo, armTextureID);//pared de enfrente
		pataDraw(model, glm::vec3(0.2f, 4.0f, 10.0f), glm::vec3(160.0f, 0.41f, -1.9f), modelLoc, VAO_Cubo, armTextureID);//pared de pinguino derecha
		pataDraw(model, glm::vec3(0.2f, 4.0f, 10.0f), glm::vec3(120.0f, 0.41f, -1.9f), modelLoc, VAO_Cubo, armTextureID);//pared de foca derecha
		pataDraw(model, glm::vec3(0.2f, 4.0f, 10.0f), glm::vec3(77.0f, 0.41f, -1.9f), modelLoc, VAO_Cubo, armTextureID);//pared de delfín derecha*/



		lightingShader.Use(); // shader de iluminación 
		// -------------------------------------------------------------------------------- -
	// 							DIBUJO DE MODELO JERÁRQUICO (BRAZO)
	// ---------------------------------------------------------------------------------
		//// Textura
		//// (Asumimos que la textura difusa va en la unidad 0)
		//glActiveTexture(GL_TEXTURE0);
		//glBindTexture(GL_TEXTURE_2D, armTextureID);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, armTextureID);

		//glBindVertexArray(VAO_Cubo); // Usa el VAO del cubo

		//// Habilitar los atributos (Ubicación 0 y 1 ya están habilitadas por defecto)
		//glEnableVertexAttribArray(0); // Posición
		//glEnableVertexAttribArray(1); // Normal
		//glEnableVertexAttribArray(2); //Habilitar Coordenadas de Textura

		//// Hombro
		//glm::mat4 model_brazo = glm::mat4(1.0f);
		//glm::mat4 modelTemp = glm::mat4(1.0f);
		//// ... (Transformaciones hombro) ...
		//model_brazo = glm::translate(model_brazo, glm::vec3(2.0f, 1.0f, 0.0f));
		//model_brazo = glm::rotate(model_brazo, glm::radians(hombro_rot), glm::vec3(0.0f, 0.0f, 1.0f));
		//modelTemp = model_brazo = glm::translate(model_brazo, glm::vec3(1.0f, 0.0f, 0.0f));
		//model_brazo = glm::scale(model_brazo, glm::vec3(2.0f, 0.4f, 0.4f));
		//glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_brazo));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//// Antebrazo
		//// ... (Transformaciones antebrazo) ...
		//model_brazo = glm::translate(modelTemp, glm::vec3(1.0f, 0.0f, 0.0f));
		//model_brazo = glm::rotate(model_brazo, glm::radians(codo_rot), glm::vec3(0.0f, 0.0f, 1.0f));
		//model_brazo = glm::translate(model_brazo, glm::vec3(0.75f, 0.0f, 0.0f));
		//model_brazo = glm::scale(model_brazo, glm::vec3(1.5f, 0.4f, 0.4f));
		//glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model_brazo));
		//glDrawArrays(GL_TRIANGLES, 0, 36);

		//glBindVertexArray(0); // Desvincular VAO_Cubo




		//// Also draw the lamp object, again binding the appropriate shader
		//lampShader.Use();
		//// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		//modelLoc = glGetUniformLocation(lampShader.Program, "model");
		//viewLoc = glGetUniformLocation(lampShader.Program, "view");
		//projLoc = glGetUniformLocation(lampShader.Program, "projection");

		//// Set matrices
		//glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		//glm::mat4 model = glm::mat4(1);
		//model = glm::translate(model, lightPos);
		//model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));


		// ========================================================================
		//								DIBUJAR SKYBOX
		// ========================================================================

		glDepthFunc(GL_LEQUAL);
		skyboxShader.Use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		//Desenlazar la textura del skybox
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}


// --- Función para configurar VAO/VBO genérico ---
void ConfigurarVAO(GLuint& VAO, GLuint& VBO, float* vertices, size_t size)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	// Atributo de Posición (Location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Atributo de Normal (Location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Atributo de Coordenadas de Textura (Location 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}


// --- Función para configurar los parametros del piso de textura repetible ---
void ConfigurarTexturaRepetible(GLuint textureID)
{
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
}



// --- Función para dibujar pisos con textura ---
void DibujarPiso(GLuint textureID, glm::vec3 posicion, glm::vec3 escala, GLuint VAO_Cubo, GLint modelLoc)
{
	// Activar y enlazar la textura
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Enlazar el VAO del cubo
	glBindVertexArray(VAO_Cubo);

	// Habilitar los atributos necesarios
	glEnableVertexAttribArray(0); // Posición
	glEnableVertexAttribArray(1); // Normal
	glEnableVertexAttribArray(2); // TexCoords

	// Crear matriz de transformación para el piso
	glm::mat4 model_piso = glm::mat4(1.0f);
	model_piso = glm::translate(model_piso, posicion);
	model_piso = glm::scale(model_piso, escala);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model_piso));

	// Dibujar el cubo (piso)
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindVertexArray(0);
}


// Moves/alters the camera positions based on user input
void DoMovement()
{

	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}


	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(1.0f, 1.0f, 0.0f);
		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}


	// ANMALES DESIERTO

	//CAMELLO(Tecla C)
	if (keys[GLFW_KEY_M]) {
		if (!teclaM_presionada) {
			animarCamello = !animarCamello;
			startTimeCamello = glfwGetTime();
			teclaM_presionada = true;
		}
	}
	else {
		teclaM_presionada = false;
	}
	//CONDOR(Tecla Z)
	if (keys[GLFW_KEY_Q]) {
		if (!teclaQ_presionada) {
			animarCondor = !animarCondor;
			teclaQ_presionada = true;
		}
	}
	else {
		teclaQ_presionada = false;
	}
	//TORTUGA(Tecla X)
	if (keys[GLFW_KEY_X]) {
		if (!teclaX_presionada) {
			animarTortuga = !animarTortuga;
			startTimeTortuga = glfwGetTime();
			teclaX_presionada = true;
		}
	}
	else {
		teclaX_presionada = false;
	}

	//CAPIBARA (Tecla P)
	if (keys[GLFW_KEY_P])
	{
		if (!teclaP_presionada) // Reutilizas la variable que ya tienes
		{
			animarCapibara = !animarCapibara;
			startTimeCapibara = glfwGetTime();
			teclaP_presionada = true;
		}
	}
	else
	{
		teclaP_presionada = false;
	}
	 
	//MONO (Tecla N)
	if (keys[GLFW_KEY_N])
	{
		if (!teclaN_presionada)
		{
			animarMono = !animarMono;
			startTimeMono = glfwGetTime();
			teclaN_presionada = true;
		}
	}
	else
	{
		teclaN_presionada = false;
	}

	//GUACAMAYA (Tecla O)
	if (keys[GLFW_KEY_O])
	{
		if (!teclaO_presionada)
		{
			animarGuacamaya = !animarGuacamaya;
			startTimeGuacamaya = glfwGetTime();
			teclaO_presionada = true;
		}
	}
	else
	{
		teclaO_presionada = false;
	}

	// Activa la animación del pinguino con la tecla 'C'
	if (key == GLFW_KEY_C && action == GLFW_PRESS && !teclaC_presionada)
	{
		animarPinguino = true;
		startTimePinguino = glfwGetTime(); // Guarda el tiempo de inicio
		teclaC_presionada = true; // Evita que se reinicie si se deja presionada
	}
	// Opcional: Detener animación con 'V'
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		animarPinguino = false;
		teclaC_presionada = false; // Permite volver a iniciar
		PinAlaIzq = 0.0f; // Resetea posición
		PinAlaDer = 0.0f; // Resetea posición
	}


	if (key == GLFW_KEY_B && action == GLFW_PRESS && !teclaB_presionada)
	{
		animarFoca = true;
		startTimeFoca = glfwGetTime(); // Guarda el tiempo de inicio
		teclaB_presionada = true; // Evita que se reinicie si se deja presionada
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS)
	{
		animarFoca = false;
		teclaB_presionada = false; // Permite volver a iniciar
	}

	// Activa la animación del Delfin con la tecla 'X'
	if (key == GLFW_KEY_X && action == GLFW_PRESS && !teclaD_presionada)
	{
		animarDelfin = true;
		startTimeDelfin = glfwGetTime(); // Guarda el tiempo de inicio
		teclaD_presionada = true; // Evita que se reinicie si se deja presionada
	}
	// Opcional: Detener animación con 'Z'
	if (key == GLFW_KEY_Z && action == GLFW_PRESS)
	{
		animarDelfin = false;
		teclaD_presionada = false; // Permite volver a iniciar
	}

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}
