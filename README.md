# 🦁 Zoológico Virtual Interactivo en OpenGL  

![C++](https://img.shields.io/badge/C%2B%2B-OpenGL%203.3-blue) 
![status](https://img.shields.io/badge/estado-completo-green) 
![version](https://img.shields.io/badge/version-v1.0.0-yellow)
![license](https://img.shields.io/badge/licencia-educativa-lightgrey)

### 🏆 Proyecto Final de **Computación Gráfica – UNAM (FI)**  
**Autores:** Ana Isabel Díaz Bautista, Uriel Benjamin De La Merced Soriano, Carlos Mario Hernández Gutiérrez  
📅 **Fecha de entrega:** 04 Diciembre 2025  
🎓 **Versión:** `v1.0.0`

---
## 👥 Equipo de Desarrollo

| Integrante |
|------------|
| **Ana Isabel Díaz Bautista**<br/>[@anaisabelkitty](https://github.com/anaisabelkitty)  |
| **Uriel Benjamin De La Merced Soriano**<br/>[@DeLaMercedSoriano13](https://github.com/DeLaMercedSoriano13)  |
| **Carlos Mario Hernández Gutiérrez**<br/>[@C4rlos316](https://github.com/C4rlos316) |


---

## 🌍 Descripción General

El **Zoológico Virtual Interactivo** es un entorno 3D desarrollado en **C++ y OpenGL 3.3**, que simula un zoológico con múltiples hábitats temáticos: Acuario, Selva, Sabana, Desierto, Aviario y Hábitat de Pandas. Incluye **animaciones jerárquicas avanzadas**, modelos FBX con skeleton animation, sistema completo de iluminación dinámica, audio ambiental con miniaudio y sistema de cámara triple (Libre/1ra/3ra persona).

Este proyecto combina animación y programación gráfica moderna para crear una experiencia con elementos decorativos temáticos (entrada con personajes, bancas estilo centro comercial, puesto de comida).

---

## ✨ Características Principales

### 🎮 Sistema de Cámara Avanzado
- **Cámara Libre (FREE_CAMERA):** Modo vuelo sin restricciones (WASD + Mouse)  
- **Primera Persona (FIRST_PERSON):** Vista desde los ojos del personaje Alex
- **Tercera Persona (THIRD_PERSON):** Vista orbital con modelo 3D visible de Alex (león)
- Alternancia fluida con tecla **TAB**  
- Movimiento suave e independiente del FPS con `deltaTime`
- Sincronización automática entre posición de cámara y personaje

### 🌅 Renderizado 3D Avanzado
- Motor gráfico basado en **OpenGL 3.3 Core Profile**
- **Modelos 3D** (.obj y .fbx) organizados por hábitat  
- Sistema de **iluminación Phong** con materiales realistas
- **Texturas repetibles** diferenciadas por zona (8 tipos de piso únicos)
- **Skybox 360°** con 6 texturas para ambiente inmersivo  
- **Transparencias y blending** (aviario de vidrio, agua del estanque)
- Modelos decorativos temáticos: Hello Kitty, Naruto, CDMX, Carrusel navideño

### 💡 Sistema de Iluminación Profesional
- **1 luz direccional** (simula luz solar global)
- **4 luces puntuales** configurables con atenuación cuadrática
- **1 spotlight** tipo linterna siguiendo la cámara
- **Luz central animada** con efecto pulsante (activable con ESPACIO)
- Controles de posición en tiempo real (teclas H, U, J)
- Parámetros de ambient, diffuse y specular personalizados

### 🐾 Animaciones Complejas (17 especies + personajes)

#### **Sistema de Animación Jerárquica**
Cada animal implementa transformaciones padre-hijo con pivotes precisos para movimiento realista de extremidades.

| Hábitat | Especies y Controles |
|---------|---------------------|
| **Acuario (X, -Z)** | 🐧 Pingüino (C) - aleteo, 🦭 Foca (B) - nado ondulatorio, 🐬 Delfín (D) - saltos parabólicos, 🦈 Tiburón (I) - nado + salto |
| **Selva (X, Z)** | 🦫 Capibara (P) - caminata + alimentación, 🐒 Mono (N) - saltos triple + caminata, 🦜 Guacamaya (O) - vuelo + aterrizaje |
| **Sabana (-X, -Z)** | 🐘 Elefante (V) - caminata + acostarse lateral, 🦒 Jirafa (J) - caminata + alimentación, 🦓 Cebra (L) - recorrido cuadrado completo |
| **Desierto (-X, Z)** | 🐪 Camello (automático FBX) - caminata + alimentación, 🦅 Cóndor (Q) - vuelo estacionario, 🐢 Tortuga (X) - caminata + inmersión |
| **Aviario (Centro)** | 🦜 Ave enjaulada - aleteo + movimiento de cabeza (automático) |
| **Hábitat Pandas (X, -Z)** | 🐻 Oso (automático FBX), 🦌 Venado (automático FBX) - recorrido ida/vuelta, 🐼 Panda (automático FBX) - recorrido cuadrado |
| **Personajes** | 🦁 Alex (3ra persona visible), 👤 Jake (NPC en puesto) |

#### **Animaciones FBX con Skeleton Animation**
- **Camello:**
- **Oso, Venado, Panda:** Ciclos de caminata con interpolación suave
- **Personajes Mixamo:** Alex y Jake con armature completa

### 🎵 Sistema de Audio
- Implementación con **miniaudio** embebido
- Reproducción en loop de `musica.mp3`
- Activación automática al inicio
- Control de volumen y estado con `ma_engine`

### 🏗️ Escenarios Temáticos

#### **Entrada del Zoológico**
- 🎪 Taquilla
- 🎡 Carrusel con rotación continua (360° loop)
- 🎄 Árbol de Navidad decorativo
- 🐱 Hello Kitty, Monito, Letrero CDMX
- 🍜 Naruto en zona de taquilla

#### **Mobiliario **
- 🪑 4 bancas estilo centro comercial
- 🍔 Puesto de comida rápida con atención (Jake NPC)
- ☂️ 4 mesas en zona de descanso
---

## 🏗️ Estructura del Proyecto
```
ProyectoFinalGrafica/
├── Main.cpp                    #  Archivo principal
├── Camera.h                    # Sistema de cámara triple + targetPosition
├── Shader.h                    # Compilación GLSL con validación
├── Model.h                     # Carga de .obj con ASSIMP
├── modelAnim.h                 # Carga de .fbx con skeleton animation
├── Texture.h                   # Configuración de texturas + cubemaps
├── stb_image.h                 # Decodificación de imágenes
├── miniaudio.h                 # Motor de audio embebido (header-only)
│
├── Shader/
│   ├── lighting.vs             # Vertex shader (modelo Phong completo)
│   ├── lighting.frag           # Fragment shader (4 point lights + dirLight)
│   ├── lamp.vs / lamp.frag     # Shaders para objetos emisores de luz
│   ├── skybox.vs / skybox.frag # Shaders del cubemap ambiental
│   └── anim.vs / anim.fs       # Shaders para modelos FBX con huesos
│
├── Models/                     # 100+ modelos 3D organizados por zona
│   ├── Personaje1/             # Alex (FBX con armature)
│   ├── Talking/                # Jake (FBX conversación)
│   ├── entrada_separar/        # Estructura de entrada
│   ├── kitty/, naruto/, cdmx/  # Decoraciones
│   ├── pinguino/, foca/, delfin/, tiburon/  # Acuario
│   ├── capibara/, mono/, aveSelva/          # Selva
│   ├── elefante/, jirafa/, cebra/           # Sabana
│   ├── Camello1/, condor/, tortuga/         # Desierto
│   ├── Aviario/                # Ave central (6 partes)
│   ├── Oso/, venado/, panda/   # Hábitat pandas (FBX)
│   ├── adornos/, puesto/       # Mobiliario urbano
│   └── [70+ modelos adicionales de decoración]
│
├── images/                     # Texturas y skybox
│   ├── skybox/
│   │   ├── right.jpg, left.jpg
│   │   ├── top.jpg, bottom.jpg
│   │   └── front.jpg, back.jpg
│   ├── ladrillo.png            # Piso general
│   ├── pasto.jpg               # Entrada
│   ├── muro.jpg                # Paredes perimetrales
│   ├── textnieve.jpg           # Acuario
│   ├── selva.png               # Selva
│   ├── sabana.jpg              # Sabana
│   ├── sand.jpg                # Desierto + estanque
│   ├── bosque.png              # Hábitat pandas
│   ├── Agua.jpg                # Agua transparente
│   └── gris.jpg                # Bancas
│
├── musica.mp3                  # Audio ambiental (loop)
└── ProyectoFinalGrafica.sln    # Solución de Visual Studio 2022
```

---

## ⚙️ Requisitos del Sistema

### 🖥️ Hardware Mínimo
- **GPU:** Compatible con OpenGL 3.3 (2010+)
- **RAM:** 4 GB mínimo (8 GB recomendado)
- **Almacenamiento:** 500 MB disponibles
- **Procesador:** Dual-core 2.0 GHz+

### 💻 Software Requerido
- **SO:** Windows 10 / 11 (x64)
- **IDE:** Visual Studio 2022 Community (o superior)
- **Runtimes:** Visual C++ Redistributable 2022

### 📦 Librerías Incluidas (header-only)
- **GLEW** 2.1.0 - Carga de extensiones OpenGL
- **GLFW** 3.3.8 - Gestión de ventanas y eventos
- **GLM** 0.9.9.8 - Matemáticas para gráficos 3D
- **SOIL2** - Carga de texturas legacy
- **stb_image** - Procesamiento de imágenes moderno
- **ASSIMP** - Importación de modelos 3D
- **miniaudio** - Motor de audio embebido

---

## 🔧 Instalación y Compilación

### 1️⃣ Clonar el Repositorio
```bash
git clone https://github.com/C4rlos316/ProyectoFinalGrafica.git
cd ProyectoFinalGrafica
```

### 2️⃣ Configurar Visual Studio
1. Abrir `ProyectoFinalGrafica.sln`
2. Configuración recomendada:
   - **Configuration:** `Release` 
   - **Platform:** `x64` (obligatorio)

---

## 🎮 Controles del Usuario

### 🚶 Movimiento de Cámara
| Acción | Tecla |
|--------|-------|
| Avanzar | W / ↑ |
| Retroceder | S / ↓ |
| Izquierda (strafe) | A / ← |
| Derecha (strafe) | D / → |
| Rotar cámara | **Mouse** (movimiento libre) |
| Cambiar modo de cámara | **TAB** (Libre → 1ra → 3ra → Libre...) |

### 💡 Sistema de Iluminación
| Acción | Tecla |
|--------|-------|
| Toggle luz animada central | **ESPACIO** |
| Bajar altura luz 0 | H |
| Alejar luz 0 (eje Z) | U |
| Acercar luz 0 (eje Z) | J |

### 🐾 Animaciones de Animales

#### **Acuario (Cuadrante X, -Z)**
| Tecla | Animal | Descripción |
|-------|--------|-------------|
| **C** | 🐧 Pingüino | Aleteo  de brazos |
| **B** | 🦭 Foca | Nado  con aletas + cola  |
| **D** | 🐬 Delfín | Saltos con rotación  |
| **I** | 🦈 Tiburón | Nado lateral + saltos verticales |

#### **Selva (Cuadrante X, Z)**
| Tecla | Animal | Descripción |
|-------|--------|-------------|
| **P** | 🦫 Capibara | Camina → se detiene → naranja (rotación) |
| **N** | 🐒 Mono | 3 saltos consecutivos + caminata final |
| **O** | 🦜 Guacamaya | Vuelo horizontal → descenso  → posado en rama |

#### **Sabana (Cuadrante -X, -Z)**
| Tecla | Animal | Descripción |
|-------|--------|-------------|
| **V** | 🐘 Elefante | Camina → se acuesta de lado (rotación 90°) |
| **J** | 🦒 Jirafa | Camina  → baja cabeza para comer hojas |
| **L** | 🦓 Cebra | Recorrido cuadrado completo (4 giros de 90°) |

#### **Desierto (Cuadrante -X, Z)**
| Tecla | Animal | Descripción |
|-------|--------|-------------|
| **Q** | 🦅 Cóndor | Vuelo  con aleteo + movimiento de cabeza |
| **X** | 🐢 Tortuga | Camina hacia agua → se sumerge → emerge → gira |
| - | 🐪 Camello | **AUTOMÁTICO** (FBX): Camina hacia cactus → come (loop) |

#### **Hábitat Pandas (Cuadrante X, -Z) - Todos Automáticos**
| Animal | Descripción |
|--------|-------------|
| 🐻 Oso | FBX con animación de caminata continua |
| 🦌 Venado | Recorrido lineal ida y vuelta con giros de 180° |
| 🐼 Panda | Recorrido cuadrado completo  |

#### **Otros**
- 🦜 **Ave del Aviario:** Automática (aleteo + rotación de cabeza)
- 🧍 **Jake (NPC):** Automático en puesto de comida

### 🛠️ Sistema
| Acción | Tecla |
|--------|-------|
| Cerrar aplicación | **ESC** |

---

## 📐 Diseño Técnico

### 🎨 Sistema de Renderizado
- **Pipeline:** Forward rendering con múltiples pases
- **Shading:** Modelo de iluminación Phong con:
  - Componente ambient (luz base global)
  - Componente diffuse (reflexión difusa direccional)
  - Componente specular (brillos realistas, shininess = 32.0)
- **Texturas:** Repetición con GL_REPEAT + filtrado trilinear
- **Transparencias:** Blending con GL_SRC_ALPHA

### 🔄 Sistema de Animación
```cpp
// Ejemplo de jerarquía (Foca):
Cuerpo (padre)
├── Medio (hijo de Cuerpo, pivote en cola)
│   └── Cola (hijo de Medio, pivote en extremo)
├── Aleta Derecha (hijo de Cuerpo)
└── Aleta Izquierda (hijo de Cuerpo)

// Transformación jerárquica:
modelMedio = modelCuerpo * translate(pivot) * rotate(angle) * translate(-pivot)
```

### 🎯 Máquina de Estados (Ejemplo: Jirafa)
```cpp
if (t < 4.0f)        // FASE 1: Caminando
    └── Mover Z + animar patas
else if (t < 8.0f)   // FASE 2: Comiendo
    └── Detener patas + mover cabeza
else                 // FASE 3: Quieta
    └── Reset posiciones
```

### 🗺️ Organización Espacial
El zoológico se divide en cuadrantes cartesianos:
```
       +Z (Adelante)
        ↑
   Selva│Entrada
   (X,Z)│  (0,0)
─ ─ ─ ─ ┼ ─ ─ ─ ─ → +X (Derecha)
 Desierto│Acuario
  (-X,Z) │ (X,-Z)
        ↓
      -Z (Atrás: Pandas + Estanque)
```

---

## 🧪 Testing y Rendimiento

### ✅ Casos de Prueba Implementados
- ✔️ Alternancia fluida entre 3 modos de cámara (TAB)
- ✔️ Sincronización posición cámara/personaje en 3ra persona
- ✔️ Colisiones básicas con paredes perimetrales
- ✔️ Toggle individual de 13 animaciones sin interferencias
- ✔️ Reproducción continua de audio sin cortes
- ✔️ Transparencias (vidrio aviario + agua estanque)
- ✔️ Texturas diferenciadas por zona (8 tipos)
- ✔️ Luces puntuales con atenuación correcta

### 📖 Documentación Utilizada
- [LearnOpenGL](https://learnopengl.com/) - Tutorial base de OpenGL moderno
- [OpenGL Red Book (9th Ed.)](https://www.opengl.org/sdk/docs/books/) - Referencia oficial
- [GLFW Documentation](https://www.glfw.org/docs/latest/) - Gestión de ventanas
- [miniaudio GitHub](https://github.com/mackron/miniaudio) - Sistema de audio



## 📝 Referencias y Créditos

### 🎨 Assets Externos
- **Modelos 3D:** Creados por el equipo en Blender 3.6
- **Texturas:** Mixtas (propias + [Sketchfab.com](https://sketchfab.com))
- **Skybox:** [HDRIHaven](https://polyhaven.com/hdris) (CC0 License)
- **Audio:** Música ambiental libre de regalías de [FreeSound](https://freesound.org/)

### 🔧 Herramientas Utilizadas
- **Blender 3.6** - Modelado y rigging
- **GIMP 2.10** - Edición de texturas
- **Audacity** - Edición de audio
- **Git** - Control de versiones
- **Visual Studio 2022** - IDE principal

---

## 🔗 Enlaces Importantes

- 📂 **Repositorio:** [github.com/C4rlos316/ProyectoFinalGrafica](https://github.com/C4rlos316/ProyectoFinalGrafica)
- 📹 **Video demostración:** [Proyecto Final_ Los Bandiditos_ Computacion grafica e Interacción Humano - Computadora](https://www.youtube.com/watch?v=M_Gx74iaq7c&feature=youtu.be)
- Ejecutable ([Archivo ](https://drive.google.com/drive/folders/1Oi7PZXqJzOqoAr4KDZP-jgBhI8k4lr5m?usp=sharing)) 

---
