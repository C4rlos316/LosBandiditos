#pragma once

// Std. Includes
#include <vector>

// GL Includes
#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement
enum Camera_Movement
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Tipos de cámara
enum Camera_Type
{
	FIRST_PERSON,
	THIRD_PERSON,
	FREE_CAMERA
};

// Default camera values
const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 6.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;
const GLfloat THIRD_PERSON_DISTANCE = 5.0f; // Distancia detrás del personaje

class Camera
{
public:
	// Camera Attributes
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 worldUp;

	// Attributes for Third Person / Target
	glm::vec3 targetPosition; // La posición del personaje (Alex)

	// Eular Angles
	GLfloat yaw;
	GLfloat pitch;

	// Camera options
	GLfloat movementSpeed;
	GLfloat mouseSensitivity;
	GLfloat zoom;

	// State
	Camera_Type cameraType;
	GLfloat thirdPersonDistance;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM),
		cameraType(FIRST_PERSON), thirdPersonDistance(THIRD_PERSON_DISTANCE)
	{
		this->position = position;
		this->targetPosition = position; // Inicialmente el target es la misma posición
		this->worldUp = up;
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
		: front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVTY), zoom(ZOOM),
		cameraType(FIRST_PERSON), thirdPersonDistance(THIRD_PERSON_DISTANCE)
	{
		this->position = glm::vec3(posX, posY, posZ);
		this->targetPosition = this->position;
		this->worldUp = glm::vec3(upX, upY, upZ);
		this->yaw = yaw;
		this->pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		if (cameraType == FIRST_PERSON)
		{
			// La cámara está en los ojos del personaje
			return glm::lookAt(this->position, this->position + this->front, this->up);
		}
		else if (cameraType == THIRD_PERSON)
		{
			// Calculamos la posición de la cámara detrás del target
			glm::vec3 cameraPos = targetPosition - front * thirdPersonDistance;
			cameraPos.y += 1.5f; // Un poco hacia arriba para ver sobre el hombro
			return glm::lookAt(cameraPos, targetPosition, this->up);
		}
		else // FREE_CAMERA
		{
			// Comportamiento estándar libre (vuelo)
			return glm::lookAt(this->position, this->position + this->front, this->up);
		}
	}

	// Processes input received from any keyboard-like input system
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->movementSpeed * deltaTime;

		// Vectores para movimiento en el plano (ignoran Y para caminar, usados en 1ra y 3ra)
		glm::vec3 frontFlat = glm::normalize(glm::vec3(this->front.x, 0.0f, this->front.z));
		glm::vec3 rightFlat = glm::normalize(glm::vec3(this->right.x, 0.0f, this->right.z));

		if (cameraType == FIRST_PERSON)
		{
			// Movemos la posición (ojos) y sincronizamos el target
			if (direction == FORWARD)  this->position += frontFlat * velocity;
			if (direction == BACKWARD) this->position -= frontFlat * velocity;
			if (direction == LEFT)     this->position -= rightFlat * velocity;
			if (direction == RIGHT)    this->position += rightFlat * velocity;

			this->targetPosition = this->position;
		}
		else if (cameraType == THIRD_PERSON)
		{
			// Movemos al personaje (target), la cámara lo seguirá automáticamente en GetViewMatrix
			if (direction == FORWARD)  this->targetPosition += frontFlat * velocity;
			if (direction == BACKWARD) this->targetPosition -= frontFlat * velocity;
			if (direction == LEFT)     this->targetPosition -= rightFlat * velocity;
			if (direction == RIGHT)    this->targetPosition += rightFlat * velocity;

			// Actualizamos 'position' solo como referencia
			this->position = this->targetPosition;
		}
		else // FREE_CAMERA
		{
			// Movimiento libre "vuelo" (usa el vector front real, incluyendo Y)
			// NO movemos targetPosition, el personaje se queda quieto donde lo dejamos
			if (direction == FORWARD)  this->position += this->front * velocity;
			if (direction == BACKWARD) this->position -= this->front * velocity;
			if (direction == LEFT)     this->position -= this->right * velocity;
			if (direction == RIGHT)    this->position += this->right * velocity;
		}
	}

	// Processes input received from a mouse input system
	void ProcessMouseMovement(GLfloat xOffset, GLfloat yOffset, GLboolean constrainPitch = true)
	{
		xOffset *= this->mouseSensitivity;
		yOffset *= this->mouseSensitivity;

		this->yaw += xOffset;
		this->pitch += yOffset;

		if (constrainPitch)
		{
			if (this->pitch > 89.0f) this->pitch = 89.0f;
			if (this->pitch < -89.0f) this->pitch = -89.0f;
		}

		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event
	void ProcessMouseScroll(GLfloat yOffset)
	{
		// Podrías usar esto para alejar/acercar la cámara en 3ra persona
		/*
		if (this->zoom >= 1.0f && this->zoom <= 45.0f)
			this->zoom -= yOffset;
		if (this->zoom <= 1.0f)
			this->zoom = 1.0f;
		if (this->zoom >= 45.0f)
			this->zoom = 45.0f;
		*/
	}

	// Cambia cíclicamente entre los 3 tipos de cámara
	void ToggleCameraType()
	{
		if (cameraType == FIRST_PERSON)
		{
			cameraType = THIRD_PERSON;
			// Al pasar a 3ra, aseguramos que el target es donde estábamos
			targetPosition = position;
		}
		else if (cameraType == THIRD_PERSON)
		{
			cameraType = FREE_CAMERA;
			// Al pasar a libre, calculamos donde estaba la cámara físicamente en 3ra persona
			// para iniciar el vuelo desde ahí y evitar un salto brusco.
			glm::vec3 cameraPos3rd = targetPosition - front * thirdPersonDistance;
			cameraPos3rd.y += 1.5f;
			position = cameraPos3rd;
		}
		else // De FREE_CAMERA vuelve a FIRST_PERSON
		{
			cameraType = FIRST_PERSON;
			// Al volver a 1ra, nos teletransportamos al cuerpo del personaje
			position = targetPosition;
		}
	}

	GLfloat GetZoom()
	{
		return this->zoom;
	}

	// Obtiene la posición real de la cámara para efectos de iluminación/viewPos
	glm::vec3 GetPosition()
	{
		if (cameraType == THIRD_PERSON)
		{
			glm::vec3 cameraPos = targetPosition - front * thirdPersonDistance;
			cameraPos.y += 1.5f;
			return cameraPos;
		}
		return this->position;
	}

	glm::vec3 GetFront()
	{
		return this->front;
	}

	Camera_Type GetCameraType()
	{
		return this->cameraType;
	}

private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		front.y = sin(glm::radians(this->pitch));
		front.z = sin(glm::radians(this->yaw)) * cos(glm::radians(this->pitch));
		this->front = glm::normalize(front);
		this->right = glm::normalize(glm::cross(this->front, this->worldUp));
		this->up = glm::normalize(glm::cross(this->right, this->front));
	}
};