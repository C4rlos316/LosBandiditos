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


// Funciones prototipo para callbacks
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();


// =================================================================================
// 	CONFIGURACIÓN INICIAL Y VARIABLES GLOBALES
// =================================================================================
//Configurar funciones para repetir textura de piso
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


// -----------------------------------------
//  SABANA (-X,-Z)
// -----------------------------------------


// -----------------------------------------
//  DESIERTO (-X,Z)
// -----------------------------------------



// Vértices del cubo CON COORDENADAS DE TEXTURA
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



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

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


	// PARA ESTA PARTE YA HAY UNA FUNCION LA CUAL ES ConfigurarTexturaRepetible para que la usen
	// y no tengan que poner todo el codigo repetido ConfigurarTexturaRepetible(variablePiso);

	// ***TEXTURA GENERAL PARA EL PISO ***
	GLuint pisoTextureID = TextureFromFile("images/ladrillo.png", ".");
	ConfigurarTexturaRepetible(pisoTextureID);

	GLuint pisoEntradaID = TextureFromFile("images/pasto.jpg", ".");
	ConfigurarTexturaRepetible(pisoEntradaID);

	// *** TEXTURA PARA EL PISO ACUARIO ***
	GLuint pisoAcuarioTextureID = TextureFromFile("images/textnieve.jpg", ".");
	ConfigurarTexturaRepetible(pisoAcuarioTextureID);

	// *** TEXTURA PARA EL PISO SELVA ***
	GLuint pisoSelvaTextureID = TextureFromFile("images/selva.jpg", ".");
	ConfigurarTexturaRepetible(pisoSelvaTextureID);

	// *** TEXTURA PARA EL PISO SABANA ***
	GLuint pisoSabanaTextureID = TextureFromFile("images/sabana.jpg", ".");
	ConfigurarTexturaRepetible(pisoSabanaTextureID);

	// *** TEXTURA PARA EL PISO DESIERTO ***
	GLuint pisoArenaTextureID = TextureFromFile("images/sand.jpg", ".");
	ConfigurarTexturaRepetible(pisoArenaTextureID);


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
		// 					CONFIGURACIÓN DE VÉRTICES PARA PRIMITIVAS (CUBO)
		// =================================================================================
	GLuint VBO_Cubo, VAO_Cubo;
	glGenVertexArrays(1, &VAO_Cubo);
	glGenBuffers(1, &VBO_Cubo);
	glBindVertexArray(VAO_Cubo);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cubo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(VAO_Cubo); // Enlazar el VAO

	// Atributo de Posición (Location 0)
	// El Stride (paso) ahora es de 8 floats
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);

	// Atributo de Normal (Location 1)
	// El Stride es 8, el Offset (desplazamiento) es después de los 3 floats de posición
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// *** NUEVO: Atributo de Coordenadas de Textura (Location 2) ***
	// El Stride es 8, el Offset es después de 3 (pos) + 3 (norm) = 6 floats
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Desvincular el VAO


	// =================================================================================
	// 		CONFIGURACIÓN DE VÉRTICES PARA PISO DE ENTRADA
	// =================================================================================
	GLuint VBO_Entrada, VAO_Entrada;
	glGenVertexArrays(1, &VAO_Entrada);
	glGenBuffers(1, &VBO_Entrada);
	glBindVertexArray(VAO_Entrada);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_Entrada);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Atributo de Posición (Location 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Atributo de Normal (Location 1)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Atributo de Coordenadas de Textura (Location 2)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Desvincular el VAO


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


		// **** DIBUJO DEL PISO SELVA Y ACCESORIOS SELVA ****
		DibujarPiso(pisoSelvaTextureID, glm::vec3(7.25f, -0.49f, 7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);




		// **** DIBUJO DE ANIMALES SELVA ****



		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS SABANA (-x,-z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO SABANA Y ACCESORIOS SABANA ****
		DibujarPiso(pisoSabanaTextureID, glm::vec3(-7.25f, -0.49f, -7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);



		// **** DIBUJO DE ANIMALES SABANA ****



		// ---------------------------------------------------------------------------------
		// 							DIBUJO DE MODELOS DESIERTO (-x,z)
		// ---------------------------------------------------------------------------------

		// **** DIBUJO DEL PISO DESIERTO  Y COMPONENTES ****

		DibujarPiso(pisoArenaTextureID, glm::vec3(-7.25f, -0.49f, 7.25f), glm::vec3(10.5f, 0.1f, 10.5f), VAO_Cubo, modelLoc);

		// **** DIBUJO DE ANIMALES DESIERTO ****


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


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}


	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
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