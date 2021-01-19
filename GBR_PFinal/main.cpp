#define _USE_MATH_DEFINES
#include <cmath>
//glew include
#include <GL/glew.h>

//std includes
#include <string>
#include <iostream>
#include <array>
#include <string>
#include <sstream>
#include <iterator>
#include <fstream>

//glfw include
#include <GLFW/glfw3.h>

// program include
#include "Headers/TimeManager.h"

// Shader include
#include "Headers/Shader.h"

// Model geometric includes
#include "Headers/Sphere.h"
#include "Headers/Cylinder.h"
#include "Headers/Box.h"
#include "Headers/FirstPersonCamera.h"
#include "Headers/ThirdPersonCamera.h"

//GLM include
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Headers/Texture.h"

// Include loader Model class
#include "Headers/Model.h"

// Include Terrain
#include "Headers/Terrain.h"

#include "Headers/AnimationUtils.h"

// Include Colision headers functions
#include "Headers/Colisiones.h"

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))

int screenWidth;
int screenHeight;

GLFWwindow *window;

Shader shader;
//Shader con skybox
Shader shaderSkybox;
//Shader con multiples luces
Shader shaderMulLighting;
//Shader para el terreno
Shader shaderTerrain;

Shader shaderPrueba;

//Cámaras
std::shared_ptr<FirstPersonCamera> FPcamera(new FirstPersonCamera());
std::shared_ptr<Camera> camera(new ThirdPersonCamera());
//Para seleccionar el tipo de camara
bool cameraType = false; //false = TPCamera, true = FPCamera
bool cameraLock = false;
bool keyLock = false;
float distanceFromTarget = 5.0;
float fpcYaw, fpcPitch;

Box boxCollider;
Sphere sphereCollider(10, 10);
Sphere skyboxSphere(20, 20);

// Models complex instances
Model modelRock;
Model modelGate;
Model modelFence00, modelFence01, modelFence02, modelFence03, modelFence04;
Model modelTree00, modelTree01, modelTree02, modelPine00;
Model modelStump;
Model modelRock00, modelRock01;
Model modelLog00;
Model modelGrass;

// Model animate instance

Model wendigoAnimate;
Model heatherAnimate;
//Variable de selección de animación
int heatherAnimation = 0;

float nieblaLL, nieblaD;
float screenView = 150.0;

// Terrain model instance
Terrain terrain(-1, -1, 200, 24, "../Textures/heightmap4.png");

GLuint textureCespedID, textureWallID, textureWindowID, textureHighwayID, textureLandingPadID;
GLuint textureTerrainBackgroundID, textureTerrainRID, textureTerrainGID, textureTerrainBID, textureTerrainBlendMapID;
GLuint skyboxTextureID;

GLenum types[6] = {
GL_TEXTURE_CUBE_MAP_POSITIVE_X,
GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };

//std::string fileNames[6] = { "../Textures/mp_bloodvalley/blood-valley_ft.tga",
//		"../Textures/mp_bloodvalley/blood-valley_bk.tga",
//		"../Textures/mp_bloodvalley/blood-valley_up.tga",
//		"../Textures/mp_bloodvalley/blood-valley_dn.tga",
//		"../Textures/mp_bloodvalley/blood-valley_rt.tga",
//		"../Textures/mp_bloodvalley/blood-valley_lf.tga" };

//std::string fileNames[6] = { "../Textures/NightPath/posx.jpg",
//        "../Textures/NightPath/negx.jpg",
//        "../Textures/NightPath/posy.jpg",
//        "../Textures/NightPath/negy.jpg",
//        "../Textures/NightPath/posz.jpg",
//        "../Textures/NightPath/negz.jpg" };

std::string fileNames[6] = { "../Textures/night/grimmnightft.tga",
        "../Textures/night/grimmnightbk.tga",
        "../Textures/night/grimmnightup.tga",
        "../Textures/night/grimmnightdn.tga",
        "../Textures/night/grimmnightrt.tga",
        "../Textures/night/grimmnightlf.tga" };



bool exitApp = false;
int lastMousePosX, offsetX = 0;
int lastMousePosY, offsetY = 0;

// Model matrix definitions
glm::mat4 matrixModelRock = glm::mat4(1.0);
glm::mat4 matrixModelGate = glm::mat4(1.0);
glm::mat4 matrixModelFence00 = glm::mat4(1.0);
glm::mat4 matrixModelFence01 = glm::mat4(1.0);
glm::mat4 matrixModelFence02 = glm::mat4(1.0);
glm::mat4 matrixModelFence03 = glm::mat4(1.0);
glm::mat4 matrixModelFence04 = glm::mat4(1.0);
glm::mat4 matrixModelTree00 = glm::mat4(1.0);
glm::mat4 matrixModelStump = glm::mat4(1.0);
glm::mat4 matrixModelRock00 = glm::mat4(1.0);
glm::mat4 matrixModelRock01 = glm::mat4(1.0);
glm::mat4 matrixModelLog00 = glm::mat4(1.0);
glm::mat4 matrixModelGrass = glm::mat4(1.0f);

glm::mat4 modelMatrixWendigo = glm::mat4(1.0f);
glm::mat4 modelMatrixHeather = glm::mat4(1.0f);
glm::mat4 modelMatrixHeatherBody;
bool SaveFrame = false, aviableSave = true, modelChange = false;

int wendigoRunning = 0;
int enableChange = 0;

int modelSelected = 1;
bool enableCountSelected = true;

bool fogOn = false;

// Variables to animations keyframes
bool saveFrame = false, availableSave = true;
std::ofstream myfile;
std::string fileName = "";
bool record = false;

// Joints interpolations Dart Lego
//std::vector<std::vector<float>> keyFramesDartJoints;
//std::vector<std::vector<glm::mat4>> keyFramesDart;
//int indexFrameDartJoints = 0;
//int indexFrameDartJointsNext = 1;
//float interpolationDartJoints = 0.0;
//int maxNumPasosDartJoints = 20;
//int numPasosDartJoints = 0;
//int indexFrameDart = 0;
//int indexFrameDartNext = 1;
//float interpolationDart = 0.0;
//int maxNumPasosDart = 200;
//int numPasosDart = 0;

// Fence positions
float fenceHeight;
glm::vec3 fenceScale = glm::vec3(0.028, 0.0165, 0.04);
std::vector< glm::vec2>fence00Position = { 
    glm::vec2(1.86485, -1.0511), glm::vec2(-1.50622, 25.8365), glm::vec2(3.30794, 38.7327), glm::vec2(-2.79038, 64.2511), glm::vec2(20.128, 80.08),
    glm::vec2(32.1414, 57.7366), glm::vec2(41.3933, 18.0601), glm::vec2(39.5491, -12.9148)
    };

std::vector< glm::vec2>fence01Position = {
    glm::vec2(-0.652102, 4.05797), glm::vec2(2.71087, 29.3977), glm::vec2(-0.514344, 41.1714), glm::vec2(0.666232, 75.1669), glm::vec2(24.6944 , 76.6091),
    glm::vec2(29.5509, 52.6162), glm::vec2(27.543, 27.6974), glm::vec2(46.2644, -4.36723), glm::vec2(33.9819, -14.1432), glm::vec2(14.8981, -11.4186)
};

std::vector< glm::vec2>fence02Position = {
    glm::vec2(-0.849043, 9.68018), glm::vec2(-3.51202, 51.8872), glm::vec2(4.2834, 79.6492), glm::vec2(14.9357, 81.9379), glm::vec2(26.4688, 47.9437),
    glm::vec2(32.0711, 24.3876), glm::vec2(9.80979, -8.94707)
};

std::vector< glm::vec2>fence03Position = {
    glm::vec2(-0.774681, 15.0816), glm::vec2(-2.40572, 46.4177), glm::vec2(9.33416, 81.9427), glm::vec2(28.7167, 72.4988), glm::vec2(23.8459, 42.8985),
    glm::vec2(23.1814, 37.562), glm::vec2(46.9228, 6.63597), glm::vec2(43.6721, -9.44977)
};

std::vector< glm::vec2>fence04Position = {
    glm::vec2(-1.83894, 20.2343), glm::vec2(-1.61441, 69.7806), glm::vec2(31.3878, 67.7861), glm::vec2(24.3505, 32.3623), glm::vec2(36.91, 21.5855),
    glm::vec2(47.1732, 1.17982), glm::vec2(5.43382, -5.51681)
};

float fence00Orientation[] = { -124.5, -60.4, -185.943, -78.9134, 34.7114, 114.1, 43.8209, 155.642};
float fence01Orientation[] = { -103.0, -12.3, -106.029, -56.7808, 40.0226, 119.878, 45.682, 111.669, 177.613, 200.9 };
float fence02Orientation[] = { -78.1, -90.2302, -40.3273, 5.56088, 126.234, 29.3751, -149.932};
float fence03Orientation[] = { -91.9, -105.926, -5.1798, 50.5955, 112.845, 89.5456, 86.515, 121.844 };
float fence04Orientation[] = { -107, -71.7087, 71.2682, 67.0549, 31.8903, 88.3046, -134.437 };

float treeHeight;

std::vector<glm::vec2>tree00Position = {
    glm::vec2(3.28469, 0.428148), glm::vec2(1.80947, 13.0008)
};

std::vector<glm::vec2>tree01Position = {
    glm::vec2(-0.052217, 24.374)
};

std::vector<glm::vec2>tree02Position = {
    glm::vec2(15.2902, 3.38775)
};

std::vector<glm::vec3>tree00Scale = {
    glm::vec3(0.120874), glm::vec3(0.069977)
};

std::vector<glm::vec3>tree01Scale = {
    glm::vec3(0.148011)
};

std::vector<glm::vec3>tree02Scale = {
    glm::vec3(0.146165)
};

std::vector<float>tree00Orientation = { -14.338, 48.4694 };
std::vector<float>tree01Orientation = { 80.1517};
std::vector<float>tree02Orientation = { 122.794};

float logH;
std::vector<glm::vec3>log00Position = {
    glm::vec3(32.6602, 0, 62.9065), glm::vec3(31.5247, 0, 62.9065), glm::vec3(31.4078, 1.01501, 61.8505), glm::vec3(31.4078, 1.01501, 63.1775),
};

std::vector<glm::vec3>log00Scale = {
    glm::vec3(0.022157, 0.021735, 0.016859), glm::vec3(0.022157, 0.021735, 0.016859), glm::vec3(0.016774, 0.016454, 0.012763), glm::vec3(0.016774, 0.016454, 0.012763)
};

std::vector<glm::vec3>log00Orientation = {
    glm::vec3(0.0), glm::vec3(0.0), glm::vec3(90-133.976, 88.1801, -1.75684), glm::vec3(90-133.976, 90.1801, -1.35684)
};

std::vector<glm::vec3>rock00Position = {
    glm::vec3(29.0776, 0, -11.7502), glm::vec3(25.2882, 0.567088, -10.7507), glm::vec3(20.5452, 0.567088 , -11.0072)
};

std::vector<glm::vec3>rock00Orientation = {
    glm::vec3(0, -250.313, 0), glm::vec3(-28.7718, -327.913, -23.7253 ), glm::vec3(-28.7718, -19.6365, -23.7253)
};

std::vector<glm::vec3>rock00Scale = {
    glm::vec3(2.12353), glm::vec3(2.41885), glm::vec3(2.41885)
};

double deltaTime;
double currTime, lastTime;

// Colliders
std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> > collidersOBB;
std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4> > collidersSBB;

// Se definen todos las funciones.
void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes);
void keyCallback(GLFWwindow *window, int key, int scancode, int action,
    int mode);
void mouseCallback(GLFWwindow *window, double xpos, double ypos);
void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void init(int width, int height, std::string strTitle, bool bFullScreen);
void destroy();
bool processInput(bool continueApplication = true);

// Implementacion de todas las funciones.
void init(int width, int height, std::string strTitle, bool bFullScreen) {

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        exit(-1);
    }

    screenWidth = width;
    screenHeight = height;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (bFullScreen)
        window = glfwCreateWindow(width, height, strTitle.c_str(),
            glfwGetPrimaryMonitor(), nullptr);
    else
        window = glfwCreateWindow(width, height, strTitle.c_str(), nullptr,
            nullptr);

    if (window == nullptr) {
        std::cerr
            << "Error to create GLFW window, you can try download the last version of your video card that support OpenGL 3.3+"
            << std::endl;
        destroy();
        exit(-1);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    glfwSetWindowSizeCallback(window, reshapeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Init glew
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Failed to initialize glew" << std::endl;
        exit(-1);
    }

    glViewport(0, 0, screenWidth, screenHeight);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // Inicialización de los shaders
   /* shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
    shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox.fs");
    shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation.vs", "../Shaders/multipleLights.fs");
    shaderTerrain.initialize("../Shaders/terrain.vs", "../Shaders/terrain.fs");*/

    // Inicialización de los shaders
    shader.initialize("../Shaders/colorShader.vs", "../Shaders/colorShader.fs");
    shaderSkybox.initialize("../Shaders/skyBox.vs", "../Shaders/skyBox_fog.fs");
    shaderMulLighting.initialize("../Shaders/iluminacion_textura_animation_fog.vs", "../Shaders/multipleLights_fog.fs");
    shaderTerrain.initialize("../Shaders/terrain_fog.vs", "../Shaders/terrain_fog.fs");
    shaderPrueba.initialize("../Shaders/prueba.vs", "../Shaders/multipleLights_fog.fs");
    // Inicializacion de los objetos.
    skyboxSphere.init();
    skyboxSphere.setShader(&shaderSkybox);
    skyboxSphere.setScale(glm::vec3(20.0f, 20.0f, 20.0f));

    boxCollider.init();
    boxCollider.setShader(&shader);
    boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

    sphereCollider.init();
    sphereCollider.setShader(&shader);
    sphereCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));

    modelRock.loadModel("../models/rock/rock.obj");
    modelRock.setShader(&shaderMulLighting);
    
    modelGate.loadModel("../models/Gate/g5.fbx");
    modelGate.setShader(&shaderMulLighting);

    modelFence00.loadModel("../models/fence/fence00.obj");
    modelFence00.setShader(&shaderMulLighting);

    modelFence01.loadModel("../models/fence/fence01.obj");
    modelFence01.setShader(&shaderMulLighting);

    modelFence02.loadModel("../models/fence/fence02.obj");
    modelFence02.setShader(&shaderMulLighting);

    modelFence03.loadModel("../models/fence/fence03.obj");
    modelFence03.setShader(&shaderMulLighting);

    modelFence04.loadModel("../models/fence/fence04.obj");
    modelFence04.setShader(&shaderMulLighting);

    //modelTree00.loadModel("../models/tree2/tree_mango_var01.obj");
    modelTree00.loadModel("../models/tree2/prueba.obj");
    modelTree00.setShader(&shaderMulLighting);

    modelTree01.loadModel("../models/tree2/tree_mango_var02.obj");
    modelTree01.setShader(&shaderMulLighting);

    modelTree02.loadModel("../models/tree2/prueba2.obj");
    modelTree02.setShader(&shaderMulLighting);

    modelPine00.loadModel("../models/pine/pine_tree_model2.obj");
    modelPine00.setShader(&shaderMulLighting);

    modelStump.loadModel("../models/stump/stump.obj");
    modelStump.setShader(&shaderMulLighting);

    modelLog00.loadModel("../models/logs/log.obj");
    modelLog00.setShader(&shaderMulLighting);

    modelRock00.loadModel("../models/rocks/rock000.obj");
    modelRock00.setShader(&shaderMulLighting);

    modelRock01.loadModel("../models/rocks/rock001.obj");
    modelRock01.setShader(&shaderMulLighting);

    modelGrass.loadModel("../models/grass/Grass_fbx/grass06.obj");
    modelGrass.setShader(&shaderPrueba);

    terrain.init();
    terrain.setShader(&shaderTerrain);
    terrain.setPosition(glm::vec3(100, 0, 100));

    //Wendigo
    wendigoAnimate.loadModel("../models/Wendigo/wendigoAnimado.fbx");
    wendigoAnimate.setShader(&shaderMulLighting);

    heatherAnimate.loadModel("../models/heather/heatherAnimate.fbx");
    heatherAnimate.setShader(&shaderMulLighting);

    camera->setPosition(glm::vec3(0.0, 0.2, 20.0));
    camera->setDistanceFromTarget(distanceFromTarget);
    camera->setSensitivity(0.25f);

    FPcamera->setPosition(glm::vec3(0.0, 1.0, 0.0));
    FPcamera->setDistanceFromTarget(distanceFromTarget);
    FPcamera->setSensitivity(5.0f);
    
    // Definimos el tamanio de la imagen
    int imageWidth, imageHeight;
    FIBITMAP *bitmap;
    unsigned char *data;

    // Carga de texturas para el skybox
    Texture skyboxTexture = Texture("");
    glGenTextures(1, &skyboxTextureID);
    // Tipo de textura CUBE MAP
    glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTextureID);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    for (int i = 0; i < ARRAY_SIZE_IN_ELEMENTS(types); i++) {
        skyboxTexture = Texture(fileNames[i]);
        FIBITMAP *bitmap = skyboxTexture.loadImage(true);
        unsigned char *data = skyboxTexture.convertToData(bitmap, imageWidth,
            imageHeight);
        if (data) {
            glTexImage2D(types[i], 0, GL_RGBA, imageWidth, imageHeight, 0,
                GL_BGRA, GL_UNSIGNED_BYTE, data);
        }
        else
            std::cout << "Failed to load texture" << std::endl;
        skyboxTexture.freeImage(bitmap);
    }

    // Definiendo la textura a utilizar
    Texture textureCesped("../Textures/cesped.jpg");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureCesped.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureCesped.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureCespedID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureCespedID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureCesped.freeImage(bitmap);

    
    // Definiendo la textura a utilizar
    Texture textureTerrainBackground("../Textures/forest2.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainBackground.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainBackground.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainBackgroundID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainBackground.freeImage(bitmap);

    // Definiendo la textura a utilizar
    Texture textureTerrainR("../Textures/grass3.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainR.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainR.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainRID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainR.freeImage(bitmap);

    // Definiendo la textura a utilizar
    Texture textureTerrainG("../Textures/moss.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainG.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainG.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainGID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainG.freeImage(bitmap);

    // Definiendo la textura a utilizar
    Texture textureTerrainB("../Textures/rockb.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainB.loadImage();
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainB.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainBID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainB.freeImage(bitmap);

    // Definiendo la textura a utilizar
    Texture textureTerrainBlendMap("../Textures/blendMap2.png");
    // Carga el mapa de bits (FIBITMAP es el tipo de dato de la libreria)
    bitmap = textureTerrainBlendMap.loadImage(true);
    // Convertimos el mapa de bits en un arreglo unidimensional de tipo unsigned char
    data = textureTerrainBlendMap.convertToData(bitmap, imageWidth,
        imageHeight);
    // Creando la textura con id 1
    glGenTextures(1, &textureTerrainBlendMapID);
    // Enlazar esa textura a una tipo de textura de 2D.
    glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Verifica si se pudo abrir la textura
    if (data) {
        // Transferis los datos de la imagen a memoria
        // Tipo de textura, Mipmaps, Formato interno de openGL, ancho, alto, Mipmaps,
        // Formato interno de la libreria de la imagen, el tipo de dato y al apuntador
        // a los datos
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageWidth, imageHeight, 0,
            GL_BGRA, GL_UNSIGNED_BYTE, data);
        // Generan los niveles del mipmap (OpenGL es el ecargado de realizarlos)
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
        std::cout << "Failed to load texture" << std::endl;
    // Libera la memoria de la textura
    textureTerrainBlendMap.freeImage(bitmap);
}

void destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
    // --------- IMPORTANTE ----------
    // Eliminar los shader y buffers creados.

    // Shaders Delete
    shader.destroy();
    shaderMulLighting.destroy();
    shaderSkybox.destroy();
    shaderTerrain.destroy();
    shaderPrueba.destroy();

    // Basic objects Delete
    skyboxSphere.destroy();

    boxCollider.destroy();
    sphereCollider.destroy();

    // Terrains objects Delete
    terrain.destroy();
    modelRock.destroy();
    modelGate.destroy();
    modelFence00.destroy();
    modelFence01.destroy();
    modelFence02.destroy();
    modelFence03.destroy();
    modelFence04.destroy();
    modelTree00.destroy();
    modelTree01.destroy();
    modelTree02.destroy();
    modelPine00.destroy();
    modelLog00.destroy();
    modelStump.destroy();
    modelRock00.destroy();
    modelRock01.destroy();
    modelGrass.destroy();

    // Custom objects animate
    wendigoAnimate.destroy();
    heatherAnimate.destroy();

    // Textures Delete
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &textureCespedID);
    glDeleteTextures(1, &textureTerrainBackgroundID);
    glDeleteTextures(1, &textureTerrainRID);
    glDeleteTextures(1, &textureTerrainGID);
    glDeleteTextures(1, &textureTerrainBID);
    glDeleteTextures(1, &textureTerrainBlendMapID);

    // Cube Maps Delete
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
    glDeleteTextures(1, &skyboxTextureID);
}

void reshapeCallback(GLFWwindow *Window, int widthRes, int heightRes) {
    screenWidth = widthRes;
    screenHeight = heightRes;
    glViewport(0, 0, widthRes, heightRes);
}

void keyCallback(GLFWwindow *window, int key, int scancode, int action,
    int mode) {
    if (action == GLFW_PRESS) {
        switch (key) {
        case GLFW_KEY_ESCAPE:
            exitApp = true;
            break;
        }
    }
}

void mouseCallback(GLFWwindow *window, double xpos, double ypos) {
    offsetX = xpos - lastMousePosX;
    offsetY = ypos - lastMousePosY;
    lastMousePosX = xpos;
    lastMousePosY = ypos;
}

void scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
    distanceFromTarget -= yoffset;
    if (distanceFromTarget > 10)
        distanceFromTarget = 10;
    if (distanceFromTarget < 1.5)
        distanceFromTarget = 1.5;
    camera->setDistanceFromTarget(distanceFromTarget);
};

void mouseButtonCallback(GLFWwindow *window, int button, int state, int mod) {
    if (state == GLFW_PRESS) {
        switch (button) {
        case GLFW_MOUSE_BUTTON_RIGHT:
            std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
            break;
        case GLFW_MOUSE_BUTTON_LEFT:
            std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            std::cout << "lastMousePos.x:" << lastMousePosX << std::endl;
            std::cout << "lastMousePos.y:" << lastMousePosY << std::endl;
            break;
        }
    }
}

bool processInput(bool continueApplication) {
    if (exitApp || glfwWindowShouldClose(window) != 0) {
        return false;
    }

    if (cameraType) {
        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            //FPcamera->mouseMoveCamera(offsetX, 0.0, deltaTime);
        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            //FPcamera->mouseMoveCamera(0.0, offsetY, deltaTime);
    }
    else {
        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
            camera->mouseMoveCamera(offsetX, 0.0, deltaTime);
        //if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
            camera->mouseMoveCamera(0.0, offsetY, deltaTime);
    }

    offsetX = 0;
    offsetY = 0;

    // Seleccionar modelo
    if (enableCountSelected && glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS) {
        enableCountSelected = false;
        modelSelected++;
        if (modelSelected > 3)
            modelSelected = 1;
    }
    else if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE)
        enableCountSelected = true;

    if (glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS && cameraLock == false) {
        cameraLock = true;
        cameraType = !cameraType;
        if (cameraType) {
            keyLock = true;
        }
        else {
            keyLock = false;
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE &&
        glfwGetKey(window, GLFW_KEY_K) == GLFW_RELEASE && cameraLock == true) {
        cameraLock = false;
    }

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS) {
        camera->resetCamera();
    }
        
    //Heather
    if (modelSelected == 1 && cameraType) {
        FPcamera->mouseMoveCamera(offsetX, 0.0, deltaTime);
        FPcamera->mouseMoveCamera(0.0, offsetY, deltaTime);
        offsetX = 0.0;
        offsetY = 0.0;
        glm::vec3 frente = FPcamera->mouseMoveCamera2(offsetX, 0.0, deltaTime);
        float aver = modelMatrixHeather[2][3];
        modelMatrixHeather[2] = glm::vec4(frente, aver);
    }
    else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        modelMatrixHeather = glm::rotate(modelMatrixHeather, 0.045f, glm::vec3(0, 1, 0));
        
    }
    else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        modelMatrixHeather = glm::rotate(modelMatrixHeather, -0.045f, glm::vec3(0, 1, 0));
        //FPcamera->setFront(modelMatrixHeather[2]);
    }
    if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE) {
        heatherAnimation = 1;
        modelMatrixHeather = glm::translate(modelMatrixHeather, glm::vec3(0.0, 0.0, 0.08));
    }
    else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        heatherAnimation = 2;
        modelMatrixHeather = glm::translate(modelMatrixHeather, glm::vec3(0.0, 0.0, -0.06));
    }
    else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS &&
        glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        heatherAnimation = 3;
        modelMatrixHeather = glm::translate(modelMatrixHeather, glm::vec3(0.0, 0.0, 0.165));
    }
    //if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS && enableChange == 1) {
    //    enableChange = 0;
    //    heatherAnimation = 4;
    //    TimeManager::Instance().resetStartTime();
    //}
    //else if (modelSelected == 1 && glfwGetKey(window, GLFW_KEY_L) == GLFW_RELEASE)
    //    enableChange = 1;

    else if (modelSelected == 1)
        heatherAnimation = 0;


    //Para rotar todas las animaciones que hay del model de Heather
    //if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS && enableChange == 1) {
    //    enableChange = 0;
    //    heatherAnimation++;
    //    TimeManager::Instance().resetStartTime();
    //    if (heatherAnimation > 7)
    //        heatherAnimation = 0;
    //}
    //else if (modelSelected == 4 && glfwGetKey(window, GLFW_KEY_C) == GLFW_RELEASE)
    //    enableChange = 1;

    if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        modelMatrixWendigo = glm::rotate(modelMatrixWendigo, 0.02f, glm::vec3(0, 1, 0));
    else if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        modelMatrixWendigo = glm::rotate(modelMatrixWendigo, -0.02f, glm::vec3(0, 1, 0));
    if (modelSelected == 2 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        //modelMatrixWendigo = glm::translate(modelMatrixWendigo, glm::vec3(0.0, 0.0, 0.06));
        wendigoRunning = 1;
    else
        wendigoRunning = 0;

    if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        matrixModelGate = glm::translate(matrixModelGate,  glm::vec3(0, 0, -0.01));
    if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        matrixModelGate = glm::translate(matrixModelGate, glm::vec3(0, 0, 0.01));
    if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        matrixModelGate = glm::translate(matrixModelGate, glm::vec3(0.01, 0, 0));
    if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        matrixModelGate = glm::translate(matrixModelGate, glm::vec3(-0.01, 0, 0));
    if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        matrixModelGate = glm::rotate(matrixModelGate, 0.02f, glm::vec3(0, 1, 0));
    if (modelSelected == 3 && glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        matrixModelGate = glm::rotate(matrixModelGate, -0.02f, glm::vec3(0, 1, 0));

    if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS) {
        fogOn = true;
    }

    if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS) {
        fogOn = false;
    }
    
    
    //std::cout << matrixModelGate[3][0] << matrixModelGate[3][1] << matrixModelGate[3][2] << std::endl;

    glfwPollEvents();
    return continueApplication;
}

void applicationLoop() {
    bool psi = true;

    matrixModelRock = glm::translate(matrixModelRock, glm::vec3(-3.0, 0.0, 2.0));

    modelMatrixWendigo = glm::translate(modelMatrixWendigo, glm::vec3(0.0, 0.0, -15.0));
    
    //std::vector<float> grassOr;
    //for (int j = 250; j < 280; j++) {
    //    for (int k = 250; k < 280; k++) {
    //        grassOr.push_back(rand() % 360);
    //    }
    //}

    //modelMatrixHeather = glm::rotate(modelMatrixHeather, glm::radians(90.0f),  glm::vec3(0, 1, 0));

    // Variables to interpolation key frames
    //fileName = "../animaciones/animation_dart_joints.txt";
    //keyFramesDartJoints = getKeyRotFrames(fileName);
    //keyFramesDart = getKeyFrames("../animaciones/animation_dart.txt");

    lastTime = TimeManager::Instance().GetTime();

    std::vector<glm::vec2> coordenadas;
    std::ifstream infile("../models/grass/coords3.txt");
    float coordX, coordZ;
    while (infile >> coordX >> coordZ) {
        coordenadas.push_back(glm::vec2(coordX, coordZ));
//        std::cout << "prueba" << coordX << std::endl;
    }

    std::cout << "size " << coordenadas.size() << std::endl;

    unsigned int amount = coordenadas.size();
    glm::mat4* modelMatrices;
    modelMatrices = new glm::mat4[amount];
    srand(glfwGetTime());
    
    int i = 0;
    for (std::vector<glm::vec2>::iterator it = coordenadas.begin(); it != coordenadas.end(); it++) {
        glm::mat4 model = glm::mat4(1.0);
        //float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float xCoord = it->x;
        if (xCoord < -100) xCoord = -99.9;
        if (xCoord > 100) xCoord = 99.9;
        //r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        float zCoord = it->y;
        if (zCoord < -100) zCoord = -99.9;
        if (zCoord > 100) zCoord = 99.9;
        float yCoord = terrain.getHeightTerrain(xCoord, zCoord);
        glm::vec3 grassNormal = terrain.getNormalTerrain(xCoord, zCoord);
        glm::vec3 xaxis = glm::vec3(model[0]);
        glm::vec3 zaxis = glm::normalize(glm::cross(xaxis, grassNormal));
        model = glm::translate(model, glm::vec3(xCoord, yCoord-0.1, zCoord));
        //float r3 = 0.0 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (HI - LO)));
        model[1] = glm::vec4(grassNormal, 0.0);
        model[2] = glm::vec4(zaxis, 0.0);
        float orientation = rand() % 360;
        model = glm::scale(model, glm::vec3(0.7, 3.2, 0.7));
        //model = glm::scale(model, glm::vec3(0.015, 0.019, 0.015));
        model = glm::rotate(model, glm::radians(orientation), glm::vec3(0, 1, 0));
        modelMatrices[i] = model;
        i++;
    }


    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STATIC_DRAW);

    for (unsigned int i = 0; i < modelGrass.getMeshes().size(); i++)
    {
        unsigned int VAO = modelGrass.getMeshes()[i]->getVAO();
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(7);
        glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(8);
        glVertexAttribPointer(8, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(9);
        glVertexAttribPointer(9, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(6, 1);
        glVertexAttribDivisor(7, 1);
        glVertexAttribDivisor(8, 1);
        glVertexAttribDivisor(9, 1);

        glBindVertexArray(0);
    }


    while (psi) {
        currTime = TimeManager::Instance().GetTime();
        if (currTime - lastTime < 0.016666667) {
            glfwPollEvents();
            continue;
        }
        lastTime = currTime;
        TimeManager::Instance().CalculateFrameRate(true);
        deltaTime = TimeManager::Instance().DeltaTime;
        psi = processInput(true);

        std::map<std::string, bool> collisionDetection;

        // Variables donde se guardan las matrices de cada articulacion por 1 frame
        std::vector<float> matrixDartJoints;
        std::vector<glm::mat4> matrixDart;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*screenView -= 0.1;
        if (screenView < 50.0) screenView = 50.0;*/
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)screenWidth / (float)screenHeight, 0.01f, 150.0f);

        glm::vec3 axis;
        glm::vec3 target;
        float angleTarget;

        if (modelSelected == 1) {
            axis = glm::axis(glm::quat_cast(modelMatrixHeather));
            angleTarget = glm::angle(glm::quat_cast(modelMatrixHeather));
            target = modelMatrixHeather[3];
            target.x += 0.8;
            target.y += 4;
        }

        //std::cout << axis.x << "   " << axis.y << "    " << axis.z << std::endl;
        /*glm::vec3 frente = FPcamera->getFront();
        std::cout << frente.x << "  " << frente.y << "  " << frente.z << std::endl;*/
        glm::mat4 view;
        if (cameraType) {
            FPcamera->setPosition(glm::vec3(modelMatrixHeather * glm::vec4(0, 3.75, 0.75, 1)));
            FPcamera->setFront(modelMatrixHeather[2]);
            /*glm::vec3 frente = FPcamera->getFront();
            std::cout << frente.x << "  " << frente.y << "  " << frente.z << std::endl;*/
            view = FPcamera->getViewMatrix();
            
        }
        else {
            if (std::isnan(angleTarget))
                angleTarget = 0.0;
            if (axis.y < 0)
                angleTarget = -angleTarget;
            camera->setCameraTarget(target);
            camera->setAngleTarget(angleTarget);
            camera->updateCamera();
            view = camera->getViewMatrix();
        }

        // Settea la matriz de vista y projection al shader con solo color
        shader.setMatrix4("projection", 1, false, glm::value_ptr(projection));
        shader.setMatrix4("view", 1, false, glm::value_ptr(view));

        // Settea la matriz de vista y projection al shader con skybox
        shaderSkybox.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderSkybox.setMatrix4("view", 1, false,
            glm::value_ptr(glm::mat4(glm::mat3(view))));
        // Settea la matriz de vista y projection al shader con multiples luces
        shaderMulLighting.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderMulLighting.setMatrix4("view", 1, false,
            glm::value_ptr(view));
        // Settea la matriz de vista y projection al shader con multiples luces
        shaderTerrain.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderTerrain.setMatrix4("view", 1, false,
            glm::value_ptr(view));

        shaderPrueba.setMatrix4("projection", 1, false,
            glm::value_ptr(projection));
        shaderPrueba.setMatrix4("view", 1, false,
            glm::value_ptr(view));

        /*******************************************
        * Propiedades de la neblina
        *******************************************/
        shaderMulLighting.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.4)));
        shaderTerrain.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.4)));
        //shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
        shaderSkybox.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.4)));

        shaderMulLighting.setFloat("density", nieblaD);
        shaderTerrain.setFloat("density", nieblaD);
        shaderSkybox.setFloat("lowerLimit", 0.0);
        shaderSkybox.setFloat("upperLimit", nieblaLL);

        shaderPrueba.setVectorFloat3("fogColor", glm::value_ptr(glm::vec3(0.4)));
        shaderPrueba.setFloat("density", nieblaD);

        if (fogOn) {
            nieblaLL += 5.5 / 1800;
            nieblaD += 0.08 / 1800;
            if (nieblaLL > 5.5) nieblaLL = 5.5;
            if (nieblaD > 0.08) nieblaD = 0.08;
        }
        
        if (!fogOn) {
            nieblaLL = 0.0;
            nieblaD = 0.0001;
        }

        /*******************************************
         * Propiedades Luz direccional
         *******************************************/
        shaderMulLighting.setVectorFloat3("viewPos", glm::value_ptr(FPcamera->getPosition()));
        shaderMulLighting.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.11)));
        shaderMulLighting.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.21)));
        shaderMulLighting.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.00001, 0.00001, 0.00001)));
        shaderMulLighting.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-0.5, 0.0, -1.0)));

        /*******************************************
         * Propiedades Luz direccional Terrain
         *******************************************/
        shaderTerrain.setVectorFloat3("viewPos", glm::value_ptr(FPcamera->getPosition()));
        shaderTerrain.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.11)));
        shaderTerrain.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.21)));
        shaderTerrain.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.00001, 0.00001, 0.00001)));
        shaderTerrain.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

        shaderPrueba.setVectorFloat3("viewPos", glm::value_ptr(FPcamera->getPosition()));
        shaderPrueba.setVectorFloat3("directionalLight.light.ambient", glm::value_ptr(glm::vec3(0.1, 0.1, 0.11)));
        shaderPrueba.setVectorFloat3("directionalLight.light.diffuse", glm::value_ptr(glm::vec3(0.2, 0.2, 0.21)));
        shaderPrueba.setVectorFloat3("directionalLight.light.specular", glm::value_ptr(glm::vec3(0.00001, 0.00001, 0.00001)));
        shaderPrueba.setVectorFloat3("directionalLight.direction", glm::value_ptr(glm::vec3(-1.0, 0.0, 0.0)));

        /*******************************************
         * Propiedades SpotLights
         *******************************************/
        shaderMulLighting.setInt("spotLightCount", 1);
        shaderTerrain.setInt("spotLightCount", 1);

        shaderPrueba.setInt("spotLightCount", 1);

        /*******************************************
         * Propiedades PointLights
         *******************************************/
        shaderMulLighting.setInt("pointLightCount", 0);
        shaderTerrain.setInt("pointLightCount", 0);

        shaderPrueba.setInt("pointLightCount", 0);

        /*******************************************
         * Terrain Cesped
         *******************************************/
        // Se activa la textura del background
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureTerrainBackgroundID);
        shaderTerrain.setInt("backgroundTexture", 0);


        //Se activa la textura de la tierra
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureTerrainRID);
        shaderTerrain.setInt("rTexture", 1);
        //Se activa la textura de flores
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, textureTerrainGID);
        shaderTerrain.setInt("gTexture", 2);
        //Se activa la textura del camino
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, textureTerrainBID);
        shaderTerrain.setInt("bTexture", 4);
        //Se activa la textura del mapa de mezcla
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, textureTerrainBlendMapID);
        shaderTerrain.setInt("blendMapTexture", 5);

        shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(30, 30)));
        terrain.render();
        shaderTerrain.setVectorFloat2("scaleUV", glm::value_ptr(glm::vec2(0, 0)));
        glBindTexture(GL_TEXTURE_2D, 0);

        /*******************************************
         * Custom objects obj
         *******************************************/
         //Rock render
        matrixModelRock[3][1] = terrain.getHeightTerrain(matrixModelRock[3][0], matrixModelRock[3][2]);
        modelRock.render(matrixModelRock);

        for (int i = 0; i < fence00Position.size(); i++) {
            fenceHeight = terrain.getHeightTerrain(fence00Position[i].x, fence00Position[i].y);
            modelFence00.setPosition(glm::vec3(fence00Position[i].x, fenceHeight, fence00Position[i].y));
            modelFence00.setScale(fenceScale);
            modelFence00.setOrientation(glm::vec3(0, fence00Orientation[i], 0));
            modelFence00.render(matrixModelFence00);
        }

        for (int i = 0; i < fence01Position.size(); i++) {
            fenceHeight = terrain.getHeightTerrain(fence01Position[i].x, fence01Position[i].y);
            modelFence01.setPosition(glm::vec3(fence01Position[i].x, fenceHeight, fence01Position[i].y));
            modelFence01.setScale(fenceScale);
            modelFence01.setOrientation(glm::vec3(0, fence01Orientation[i], 0));
            modelFence01.render(matrixModelFence01);
        }

        for (int i = 0; i < fence02Position.size(); i++) {
            fenceHeight = terrain.getHeightTerrain(fence02Position[i].x, fence02Position[i].y);
            modelFence02.setPosition(glm::vec3(fence02Position[i].x, fenceHeight, fence02Position[i].y));
            modelFence02.setScale(fenceScale);
            modelFence02.setOrientation(glm::vec3(0, fence02Orientation[i], 0));
            modelFence02.render(matrixModelFence02);
        }

        for (int i = 0; i < fence03Position.size(); i++) {
            fenceHeight = terrain.getHeightTerrain(fence03Position[i].x, fence03Position[i].y);
            modelFence03.setPosition(glm::vec3(fence03Position[i].x, fenceHeight, fence03Position[i].y));
            modelFence03.setScale(fenceScale);
            modelFence03.setOrientation(glm::vec3(0, fence03Orientation[i], 0));
            modelFence03.render(matrixModelFence03);
        }

        for (int i = 0; i < fence04Position.size(); i++) {
            fenceHeight = terrain.getHeightTerrain(fence04Position[i].x, fence04Position[i].y);
            modelFence04.setPosition(glm::vec3(fence04Position[i].x, fenceHeight, fence04Position[i].y));
            modelFence04.setScale(fenceScale);
            modelFence04.setOrientation(glm::vec3(0, fence04Orientation[i], 0));
            modelFence04.render(matrixModelFence04);
        }

        for (int i = 0; i < tree00Position.size(); i++) {
            glDisable(GL_CULL_FACE);
            treeHeight = terrain.getHeightTerrain(tree00Position[i].x, tree00Position[i].y);
            modelTree00.setPosition(glm::vec3(tree00Position[i].x, 0, tree00Position[i].y));
            modelTree00.setScale(tree00Scale[i]);
            modelTree00.setOrientation(glm::vec3(0, tree00Orientation[i], 0));
            modelTree00.render(matrixModelTree00);
            glEnable(GL_CULL_FACE);
        }

        for (int i = 0; i < tree01Position.size(); i++) {
            glDisable(GL_CULL_FACE);
            treeHeight = terrain.getHeightTerrain(tree01Position[i].x, tree01Position[i].y);
            modelTree01.setPosition(glm::vec3(tree01Position[i].x, 0, tree01Position[i].y));
            modelTree01.setScale(tree01Scale[i]);
            modelTree01.setOrientation(glm::vec3(0, tree01Orientation[i], 0));
            modelTree01.render(matrixModelTree00);
            glEnable(GL_CULL_FACE);
        }

        for (int i = 0; i < tree02Position.size(); i++) {
            glDisable(GL_CULL_FACE);
            treeHeight = terrain.getHeightTerrain(tree02Position[i].x, tree02Position[i].y);
            modelTree02.setPosition(glm::vec3(tree02Position[i].x, treeHeight+0.5, tree02Position[i].y));
            modelTree02.setScale(tree02Scale[i]);
            modelTree02.setOrientation(glm::vec3(0, tree02Orientation[i], 0));
            modelTree02.render(matrixModelTree00);
            glEnable(GL_CULL_FACE);
        }

        for (int i = 0; i < log00Position.size(); i++) {
            logH = terrain.getHeightTerrain(log00Position[i].x, log00Position[i].z);
            modelLog00.setPosition(glm::vec3(log00Position[i].x, logH+log00Position[i].y, log00Position[i].z));
            modelLog00.setScale(log00Scale[i]);
            modelLog00.setOrientation(log00Orientation[i]);
            modelLog00.render(matrixModelLog00);
        }

        /*modelPine00.setPosition(glm::vec3(1.70629, 0, 14.4389));
        modelPine00.setScale(glm::vec3(2.41813));
        modelPine00.setOrientation(glm::vec3(0));
        modelPine00.render(matrixModelTree00);  */
      
         float stumpH = terrain.getHeightTerrain(13.2114, 71.083);
        modelStump.setPosition(glm::vec3(13.2114, stumpH-0.05, 71.083));
        modelStump.setScale(glm::vec3(32.0));
        modelStump.setOrientation(glm::vec3(0, -24.3, 0));
        modelStump.render(matrixModelStump);

        float rockH = terrain.getHeightTerrain(-2.35956, 55.6855);
        modelRock00.setPosition(glm::vec3(-2.35956, rockH-0.5, 55.6855));
        modelRock00.setScale(glm::vec3(1));
        modelRock00.setOrientation(glm::vec3(0, 0, 0));
        modelRock00.render(matrixModelRock00);

        rockH = terrain.getHeightTerrain(-2.33666, 58.1415);
        modelRock00.setPosition(glm::vec3(-2.33666, rockH - 0.5, 58.1415));
        modelRock00.setScale(glm::vec3(1));
        modelRock00.setOrientation(glm::vec3(90-49.4727, 0, 0));
        modelRock00.render(matrixModelRock00);

        rockH = terrain.getHeightTerrain(-2.02599, 60.2344);
        modelRock00.setPosition(glm::vec3(-2.02599, rockH - 0.5, 60.2344));
        modelRock00.setScale(glm::vec3(1));
        modelRock00.setOrientation(glm::vec3(90 - 49.4727, -95.596, 0));
        modelRock00.render(matrixModelRock00);

        for (int i = 0; i < rock00Position.size(); i++) {
            rockH = terrain.getHeightTerrain(rock00Position[i].x, rock00Position[i].z);
            modelRock00.setPosition(glm::vec3(rock00Position[i].x, rock00Position[i].y, rock00Position[i].z));
            modelRock00.setScale(rock00Scale[i]);
            modelRock00.setOrientation(rock00Orientation[i]);
            modelRock00.render(matrixModelRock00);
        }

        rockH = terrain.getHeightTerrain(42.8546, 11.7888);
        modelRock01.setPosition(glm::vec3(42.8546, rockH-0.5, 11.7888));
        modelRock01.setScale(glm::vec3(1.18227));
        modelRock01.setOrientation(glm::vec3(0, 173.309, 0));
        modelRock01.render(matrixModelRock01);

        glDisable(GL_CULL_FACE);
        modelGrass.renderInstance(matrixModelGrass, amount);
        glEnable(GL_CULL_FACE);
        //std::vector<glm::vec2> grassPos = { glm::vec2(246, 228), glm::vec2(247, 228), glm::vec2(246, 229), glm::vec2(247, 229) };
        //int q = 0;
        //for (int j = 250; j < 280; j++) {
        //    for (int k = 250; k < 280; k++) {
        //        glm::vec2 grassPos = glm::vec2(j, k);
        //        
        //        float xPos = grassPos.x * 200 / 512 - 100;
        //        float zPos = grassPos.y * 200 / 512 - 100;
        //        float grassH = terrain.getHeightTerrain(xPos, zPos);
        //        modelGrass.setPosition(glm::vec3(xPos, grassH, zPos));
        //        modelGrass.setScale(glm::vec3(1.5));
        //        modelGrass.setOrientation(glm::vec3(0, grassOr[q], 0));
        //        modelGrass.render(matrixModelGrass);
        //        q++;
        //    }
        //}
        /*std::vector<float> grassOr = { 15.0, 90.0, 63.0, 90.0};
        for (int i = 0; i < grassPos.size(); i++) {
            float xPos = grassPos[i].x * 200 / 512 - 100;
            float zPos = grassPos[i].y * 200 / 512 - 100;
            float grassH = terrain.getHeightTerrain(xPos, zPos);
            modelGrass.setPosition(glm::vec3(xPos, grassH, zPos));
            modelGrass.setScale(glm::vec3(1.5));
            modelGrass.setOrientation(glm::vec3(0, grassOr[i],0));
            modelGrass.render(matrixModelGrass);
        }*/
        
        
        // Forze to enable the unit texture to 0 always ----------------- IMPORTANT
        glActiveTexture(GL_TEXTURE0);

        /*******************************************
         * Custom Anim objects obj
         *******************************************/

        float gateH = terrain.getHeightTerrain(matrixModelGate[3][0], matrixModelGate[3][2]);
        //glm::mat4 matrixModelGate2 = glm::scale(matrixModelGate, glm::vec3(0.0001, 0.0001, 0.0001));
        //modelGate.setPosition(glm::vec3(-35.15625, 0, -50.78125));
        modelGate.setPosition(glm::vec3(-40.18627, gateH, -53.11125));
        //modelGate.setScale(glm::vec3(4.4, 4.5, 4.5));
        modelGate.setOrientation(glm::vec3(0, -85, 0));
        modelGate.setAnimationIndex(0);
        modelGate.render(matrixModelGate);

        //Heather
        modelMatrixHeather[3][1] = terrain.getHeightTerrain(modelMatrixHeather[3][0], modelMatrixHeather[3][2]);
        modelMatrixHeatherBody = glm::mat4(modelMatrixHeather);
        modelMatrixHeatherBody = glm::scale(modelMatrixHeatherBody, glm::vec3(0.025, 0.025, 0.025));
        heatherAnimate.setAnimationIndex(heatherAnimation);
        heatherAnimate.render(modelMatrixHeatherBody);

        glm::vec3 spotLightPosition = glm::vec3(modelMatrixHeather * glm::vec4(0.15, 3.22043, 0.75, 1));
        glm::vec3 spotLightOrientation = glm::normalize(glm::vec3(modelMatrixHeather[2]));
        spotLightOrientation.y -= 0.25;

        //Para el shader de modelos
        shaderMulLighting.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.55, 0.55, 0.5)));
        shaderMulLighting.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.45, 0.45, 0.4)));
        shaderMulLighting.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
        shaderMulLighting.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotLightPosition));
        shaderMulLighting.setVectorFloat3("spotLights[0].direction", glm::value_ptr(spotLightOrientation));
        //Vectores de atenuacion
        shaderMulLighting.setFloat("spotLights[0].constant", 0.25);
        shaderMulLighting.setFloat("spotLights[0].linear", 0.015);
        shaderMulLighting.setFloat("spotLights[0].quadratic", 0.0008);
        //Angulo
        shaderMulLighting.setFloat("spotLights[0].cutOff", cos(glm::radians(15.0f)));
        shaderMulLighting.setFloat("spotLights[0].outerCutOff", cos(glm::radians(23.0f)));
        //Para el shader del terrenos
        shaderTerrain.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.55, 0.55, 0.5)));
        shaderTerrain.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.45, 0.45, 0.4)));
        shaderTerrain.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
        shaderTerrain.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotLightPosition));
        //shaderTerrain.setVectorFloat3("spotLights[1].direction", glm::value_ptr(glm::vec3(0.0, -0.4, 1.0)));
        shaderTerrain.setVectorFloat3("spotLights[0].direction", glm::value_ptr(spotLightOrientation));
        //Vectores de atenuacion
        shaderTerrain.setFloat("spotLights[0].constant", 0.25);
        shaderTerrain.setFloat("spotLights[0].linear", 0.015);
        shaderTerrain.setFloat("spotLights[0].quadratic", 0.0008);
        //Angulo
        shaderTerrain.setFloat("spotLights[0].cutOff", cos(glm::radians(15.0f)));
        shaderTerrain.setFloat("spotLights[0].outerCutOff", cos(glm::radians(23.0f)));

        shaderPrueba.setVectorFloat3("spotLights[0].light.ambient", glm::value_ptr(glm::vec3(0.55, 0.55, 0.5)));
        shaderPrueba.setVectorFloat3("spotLights[0].light.diffuse", glm::value_ptr(glm::vec3(0.45, 0.45, 0.4)));
        shaderPrueba.setVectorFloat3("spotLights[0].light.specular", glm::value_ptr(glm::vec3(0.001, 0.001, 0.001)));
        shaderPrueba.setVectorFloat3("spotLights[0].position", glm::value_ptr(spotLightPosition));
        shaderPrueba.setVectorFloat3("spotLights[0].direction", glm::value_ptr(spotLightOrientation));
        //Vectores de atenuacion
        shaderPrueba.setFloat("spotLights[0].constant", 0.25);
        shaderPrueba.setFloat("spotLights[0].linear", 0.015);
        shaderPrueba.setFloat("spotLights[0].quadratic", 0.0008);
        //Angulo
        shaderPrueba.setFloat("spotLights[0].cutOff", cos(glm::radians(15.0f)));
        shaderPrueba.setFloat("spotLights[0].outerCutOff", cos(glm::radians(23.0f)));


        modelMatrixWendigo[3][1] = terrain.getHeightTerrain(modelMatrixWendigo[3][0], modelMatrixWendigo[3][2]);
        glm::vec3 wendigoNormal = terrain.getNormalTerrain(modelMatrixWendigo[3][0], modelMatrixWendigo[3][2]);

        glm::vec3 xaxis = glm::vec3(modelMatrixWendigo[0]);
        glm::vec3 zaxis = glm::normalize(glm::cross(xaxis, wendigoNormal));
        modelMatrixWendigo[1] = glm::vec4(wendigoNormal, 0.0);
        modelMatrixWendigo[2] = glm::vec4(zaxis, 0.0);

        glm::mat4 modelMatrixWendigoBody = glm::mat4(modelMatrixWendigo);
        modelMatrixWendigoBody = glm::scale(modelMatrixWendigoBody, glm::vec3(0.035, 0.035, 0.035));
        if (wendigoRunning) {
            modelMatrixWendigo = glm::translate(modelMatrixWendigo, glm::vec3(0.0, 0.0, 0.20));
            wendigoAnimate.setAnimationIndex(0);
        }
        else
            wendigoAnimate.setAnimationIndex(1);
        wendigoAnimate.render(modelMatrixWendigoBody);


        /*******************************************
         * Skybox
         *******************************************/
        GLint oldCullFaceMode;
        GLint oldDepthFuncMode;
        // deshabilita el modo del recorte de caras ocultas para ver las esfera desde adentro
        glGetIntegerv(GL_CULL_FACE_MODE, &oldCullFaceMode);
        glGetIntegerv(GL_DEPTH_FUNC, &oldDepthFuncMode);
        shaderSkybox.setFloat("skybox", 0);
        glCullFace(GL_FRONT);
        glDepthFunc(GL_LEQUAL);
        glActiveTexture(GL_TEXTURE0);
        skyboxSphere.render();
        glCullFace(oldCullFaceMode);
        glDepthFunc(oldDepthFuncMode);

        /**********************
        Colisiones
        **********************/

        //Heater
        AbstractModel::OBB heatherCollider;
        glm::mat4 modelMatrixColliderHeather = glm::mat4(modelMatrixHeather);
        //Antes de escalar la matriz se debe de obtener la orientación
        heatherCollider.u = glm::quat_cast(modelMatrixColliderHeather);
        modelMatrixColliderHeather = glm::scale(modelMatrixColliderHeather, glm::vec3(0.025, 0.025, 0.025));
        modelMatrixColliderHeather = glm::translate(modelMatrixColliderHeather, heatherAnimate.getObb().c);
        heatherCollider.c = modelMatrixColliderHeather[3];
        //heatherCollider.e = heatherAnimate.getObb().e * glm::vec3(1.6, 4.9, 4.2);
        heatherCollider.e = heatherAnimate.getObb().e * glm::vec3(0.5, 4.9, 0.5);
        addOrUpdateColliders(collidersOBB, "heather", heatherCollider, modelMatrixHeather);

        //Cercas
        for (int i = 0; i < fence00Position.size(); i++) {
            AbstractModel::OBB fenceCollider00;
            glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence00Position[i].x, 0, fence00Position[i].y));
            modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence00Orientation[i]), glm::vec3(0, 1, 0));
            fenceCollider00.u = glm::quat_cast(modelMatrixColliderFence);
            modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence00.getObb().c);
            fenceCollider00.c = glm::vec3(modelMatrixColliderFence[3]);
            fenceCollider00.e = modelFence00.getObb().e * fenceScale;
            addOrUpdateColliders(collidersOBB, "fence00-" + std::to_string(i), fenceCollider00, modelMatrixColliderFence);
        }

        for (int i = 0; i < fence01Position.size(); i++) {
            AbstractModel::OBB fenceCollider01;
            glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence01Position[i].x, 0, fence01Position[i].y));
            modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence01Orientation[i]), glm::vec3(0, 1, 0));
            fenceCollider01.u = glm::quat_cast(modelMatrixColliderFence);
            modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence01.getObb().c);
            fenceCollider01.c = glm::vec3(modelMatrixColliderFence[3]);
            fenceCollider01.e = modelFence00.getObb().e * fenceScale;
            addOrUpdateColliders(collidersOBB, "fence01-" + std::to_string(i), fenceCollider01, modelMatrixColliderFence);
        }

        for (int i = 0; i < fence02Position.size(); i++) {
            AbstractModel::OBB fenceCollider02;
            glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence02Position[i].x, 0, fence02Position[i].y));
            modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence02Orientation[i]), glm::vec3(0, 1, 0));
            fenceCollider02.u = glm::quat_cast(modelMatrixColliderFence);
            modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence02.getObb().c);
            fenceCollider02.c = glm::vec3(modelMatrixColliderFence[3]);
            fenceCollider02.e = modelFence00.getObb().e * fenceScale;
            addOrUpdateColliders(collidersOBB, "fence02-" + std::to_string(i), fenceCollider02, modelMatrixColliderFence);
        }

        for (int i = 0; i < fence03Position.size(); i++) {
            AbstractModel::OBB fenceCollider03;
            glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence03Position[i].x, 0, fence03Position[i].y));
            modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence03Orientation[i]), glm::vec3(0, 1, 0));
            fenceCollider03.u = glm::quat_cast(modelMatrixColliderFence);
            modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence03.getObb().c);
            fenceCollider03.c = glm::vec3(modelMatrixColliderFence[3]);
            fenceCollider03.e = modelFence00.getObb().e * fenceScale;
            addOrUpdateColliders(collidersOBB, "fence03-" + std::to_string(i), fenceCollider03, modelMatrixColliderFence);
        }
        
        for (int i = 0; i < fence04Position.size(); i++) {
            AbstractModel::OBB fenceCollider04;
            glm::mat4 modelMatrixColliderFence = glm::mat4(1.0f);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, glm::vec3(fence04Position[i].x, 0, fence04Position[i].y));
            modelMatrixColliderFence = glm::rotate(modelMatrixColliderFence, glm::radians(fence04Orientation[i]), glm::vec3(0, 1, 0));
            fenceCollider04.u = glm::quat_cast(modelMatrixColliderFence);
            modelMatrixColliderFence = glm::scale(modelMatrixColliderFence, fenceScale);
            modelMatrixColliderFence = glm::translate(modelMatrixColliderFence, modelFence04.getObb().c);
            fenceCollider04.c = glm::vec3(modelMatrixColliderFence[3]);
            fenceCollider04.e = modelFence00.getObb().e * fenceScale;
            addOrUpdateColliders(collidersOBB, "fence04-" + std::to_string(i), fenceCollider04, modelMatrixColliderFence);
        }

        /*******************************************
         * Render de colliders
         *******************************************/
        for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4> >::iterator it =
            collidersOBB.begin(); it != collidersOBB.end(); it++) {
            glm::mat4 matrixCollider = glm::mat4(1.0);
            matrixCollider = glm::translate(matrixCollider, std::get<0>(it->second).c);
            matrixCollider = matrixCollider * glm::mat4(std::get<0>(it->second).u);
            matrixCollider = glm::scale(matrixCollider, std::get<0>(it->second).e * 2.0f);
            boxCollider.setColor(glm::vec4(1.0, 1.0, 1.0, 1.0));
            boxCollider.enableWireMode();
            boxCollider.render(matrixCollider);
        }

        /********************************************
        Para realizar las pruebas de colisión
        ********************************************/

        for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it = collidersOBB.begin();
            it != collidersOBB.end(); it++) {
            bool isCollision = false;
            for (std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator jt = collidersOBB.begin();
                jt != collidersOBB.end(); jt++) {
                if (it != jt && testOBBOBB(std::get<0>(it->second), std::get<0>(jt->second))) {
                    //std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
                    isCollision = true;
                }
            }
            for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt = collidersSBB.begin();
                jt != collidersSBB.end(); jt++) {
                if (testSphereOBox(std::get<0>(jt->second), std::get<0>(it->second))) {
                    //std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
                    isCollision = true;
                    addOrUpdateCollisionDetection(collisionDetection, jt->first, isCollision);
                }
            }
            addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
        }

        for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator it = collidersSBB.begin();
            it != collidersSBB.end(); it++) {
            bool isCollision = false;
            for (std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt = collidersSBB.begin();
                jt != collidersSBB.end(); jt++) {
                if (it != jt && testSphereSphereIntersection(std::get<0>(it->second), std::get<0>(jt->second))) {
                    std::cout << "Colision " << it->first << " with " << jt->first << std::endl;
                    isCollision = true;
                }
            }
            addOrUpdateCollisionDetection(collisionDetection, it->first, isCollision);
        }

        std::map<std::string, bool>::iterator colIt;
        for (colIt = collisionDetection.begin(); colIt != collisionDetection.end(); colIt++) {
            std::map<std::string, std::tuple<AbstractModel::OBB, glm::mat4, glm::mat4>>::iterator it = collidersOBB.find(colIt->first);
            std::map<std::string, std::tuple<AbstractModel::SBB, glm::mat4, glm::mat4>>::iterator jt = collidersSBB.find(colIt->first);
            if (jt != collidersSBB.end()) {
                if (!colIt->second)
                    addOrUpdateColliders(collidersSBB, jt->first);
            }
            if (it != collidersOBB.end())
                if (!colIt->second)
                    addOrUpdateColliders(collidersOBB, it->first);
                else {
                   if (it->first.compare("heather") == 0)
                        modelMatrixHeather = std::get<1>(it->second);
                }
        }


        /*******************************************
         * State machines
         *******************************************/

       
        glfwSwapBuffers(window);
    }
}

int main(int argc, char **argv) {
    init(1920, 1080, "Window GLFW", false);
    applicationLoop();
    destroy();
    return 1;
}
