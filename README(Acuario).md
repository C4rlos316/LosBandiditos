# Documentación del Acuario

## Texturas
- `pisoAcuarioTextureID`: Textura de nieve para el piso del acuario
  ```cpp
  GLuint pisoAcuarioTextureID = TextureFromFile("images/textnieve.jpg", ".");
  ```

## Modelos Cargados
- Pinguino:
  ```cpp
  Model cuerpoPin((char*)"Models/pinguino/cuerpo.obj");
  Model arm1((char*)"Models/pinguino/arm1.obj");
  Model arm2((char*)"Models/pinguino/aletIzq.obj");
  ```

- Foca:
  ```cpp
  Model Foca1((char*)"Models/foca/cuerpoFoca.obj");
  Model FocaCo((char*)"Models/foca/medio.obj");
  Model FocaMe((char*)"Models/foca/cola.obj");
  Model FocaDer((char*)"Models/foca/FocaDer.obj");
  Model FocaIzq((char*)"Models/foca/FocaIzq.obj");
  ```

- Delfín:
  ```cpp
  Model Delfin((char*)"Models/delfin.obj");
  ```

- Elementos decorativos:
  ```cpp
  Model iglu((char*)"Models/iglu/iglu3d.obj");
  Model huevo((char*)"Models/huevo/huevo.obj");
  Model pino((char*)"Models/pino/nievepino.obj");
  Model Agua((char*)"Models/agua/agua.obj");
  ```

## Variables de Animación

### Pingüino
```cpp
float PinAlaIzq = 0.0f;        // Rotación ala izquierda
float PinAlaDer = 0.0f;        // Rotación ala derecha
float PinScale = 0.70f;        // Escala del pingüino
glm::vec3 PinguinoPos = glm::vec3(10.0f, -0.1f, -4.0f);  // Posición
bool animarPinguino = false;    // Control de animación
float startTimePinguino = 0.0f; // Tiempo inicio animación
```

### Foca
```cpp
float rotFocaMedio = 0.0f;     // Rotación parte media
float rotFocaCola = 0.0f;      // Rotación cola
float FocaScale = 1.6f;        // Escala de la foca
float focaRotY = -90.0f;       // Rotación inicial en Y
float rotFocaPataDer = 0.0f;   // Rotación aleta derecha
float rotFocaPataIzq = 0.0f;   // Rotación aleta izquierda
bool animarFoca = false;       // Control de animación
float startTimeFoca = 0.0f;    // Tiempo inicio animación
```

### Delfín
```cpp
glm::vec3 delfinPosBase = glm::vec3(2.8f, -0.9f, -10.7f);  // Posición inicial
float delfinRotY = -90.0f;     // Rotación inicial en Y
float delfinRotX = 0.0f;       // Cabeceo (Pitch)
float DelfinScale = 0.8f;      // Escala del delfín
bool animarDelfin = false;     // Control de animación
float startTimeDelfin = 0.0f;  // Tiempo inicio animación
```

## Controles de Animación

### Teclas
- **Pingüino**:
  - `C`: Iniciar animación
  - `V`: Detener animación

- **Foca**:
  - `B`: Iniciar animación
  - `N`: Detener animación

- **Delfín**:
  - `X`: Iniciar animación
  - `Z`: Detener animación