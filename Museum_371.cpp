//
// COMP 371 Museum Project

//#pragma once
#include "Shader.h"
#include "Particle.h"
#include "Room.h"

#define _USE_MATH_DEFINES
#include <algorithm>
#include <iostream>
#include <list>
#include <map>
#include <vector>
#include <cmath>
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <ft2build.h>
#include FT_FREETYPE_H  
#define IRRKLANG_STATIC
#include <irrKlang.h>

using namespace irrklang;

//#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

struct Character {
    GLuint     TextureID;  // ID handle of the glyph texture
    glm::ivec2 Size;       // Size of glyph
    glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
    GLuint     Advance;    // Offset to advance to next glyph
};

GLuint VAOText, VBOText;
std::map<GLchar, Character> Characters;
//Functions defined at the end of this file
void RenderText(GLuint s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);
GLuint loadTexture(const char* filename);
unsigned int loadCubemap(vector<std::string> faces);

int main(int argc, char*argv[])
{
    //uses functions from the Shaders class
    Shader ShaderMain = Shader();
    const float WIDTH = 1024*2;
    const float LENGHT = 768*2;
    const unsigned int SHADOW_WIDTH = 1024*2;
    const unsigned int SHADOW_HEIGHT = 1024*2;
  
    if(!ShaderMain.initContext()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    ShaderMain.createDepthCubeMap(SHADOW_WIDTH, SHADOW_HEIGHT);

    
    // Black background
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    
    // Compile and link shaders here ...
    int shaderProgram = ShaderMain.compileAndLinkShaders(ShaderMain.getVertexShader(),ShaderMain.getFragmentShader());
    int shaderShadowProgram = ShaderMain.compileAndLinkShaders(ShaderMain.getVertexShaderShadow(), ShaderMain.getFragmentShaderShadow(),ShaderMain.getGeometryShaderShadow());
    int shaderParticleProgram = ShaderMain.compileAndLinkShaders(ShaderMain.getVertexShaderParticles(), ShaderMain.getFragmentShaderParticles());
    int shaderSkyBoxProgram = ShaderMain.compileAndLinkShaders(ShaderMain.getVertexShaderSky(), ShaderMain.getFragmentShaderSky());
    int shaderTextProgram = ShaderMain.compileAndLinkShaders(ShaderMain.getVertexShaderText(), ShaderMain.getFragmentShaderText());
    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(WIDTH), 0.0f, static_cast<GLfloat>(LENGHT));
    glUseProgram(shaderTextProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderTextProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    //place to load shapes...
#if defined(PLATFORM_OSX)
    string spherePath = "/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Models/sphere2.obj";
    string cubePath = "/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Models/space.obj";
    string donutPath = "/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Models/donut.obj";

#else
    string spherePath = "../Source/Museum19/Assets/Models/sphere2.obj";
    string cubePath = "../Source/Museum19/Assets/Models/space.obj";
    string donutPath = "../Source/Museum19/Assets/Models/donut.obj";
   
#endif
    int sphereVertices;
    int skyCubeVertices;
    int donutVertices;
    GLuint sphereVAO = ShaderMain.setupModelEBO(spherePath, sphereVertices);
   // GLuint donutVAO = ShaderMain.setupModelEBO(donutPath, donutVertices);
 

  

  
    //Place to load textures...
#if defined(PLATFORM_OSX)
    GLuint woodFloorID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/wood4.jpg");
    GLuint sunID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/sun.jpg");
    GLuint panelID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/panel0.jpg");
    GLuint panelRoomID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/panelRoom.png");
    GLuint planet1ID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/planet1.jpg");
    GLuint planet2ID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/planet2.jpg");
    GLuint brickID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/redcurtains.png");
    GLuint upArrowID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/up-arrow.png");
    GLuint downArrowID = loadTexture("/Users/kenny/Documents/371/Lab_Framework/Source/Museum19/Assets/Texture/down-arrow.png");

#else
   
     GLuint woodFloorID = loadTexture("../Source/Museum19/Assets/Texture/wood4.jpg");
     GLuint sunID = loadTexture("../Source/Museum19/Assets/Texture/sun.jpg");
     GLuint panelID = loadTexture("../Source/Museum19/Assets/Texture/panel0.jpg");
     GLuint panelRoomID = loadTexture("../Source/Museum19/Assets/Texture/panelRoom.png");
     GLuint planet1ID = loadTexture("../Source/Museum19/Assets/Texture/planet1.jpg");
     GLuint planet2ID = loadTexture("../Source/Museum19/Assets/Texture/planet2.jpg");
     GLuint brickID = loadTexture("../Source/Museum19/Assets/Texture/redcurtains.png");
     GLuint upArrowID = loadTexture("../Source/Museum19/Assets/Texture/up-arrow.png");
     GLuint downArrowID = loadTexture("../Source/Museum19/Assets/Texture/down-arrow.png");
    
#endif
    
    // setup sound enginer
    // start the sound engine with default parameters
    ISoundEngine* soundEngine = createIrrKlangDevice();


     vector<std::string> faces
     {
             "../Source/Museum19/Assets/Texture/spaceleft.png",
             "../Source/Museum19/Assets/Texture/spaceright.png",
             "../Source/Museum19/Assets/Texture/spacetop.png",
             "../Source/Museum19/Assets/Texture/spacebottom.png",
             "../Source/Museum19/Assets/Texture/spacefront.png",
             "../Source/Museum19/Assets/Texture/spaceback.png"
     };

     vector<std::string> faces2
     {
             "../Source/Museum19/Assets/Texture/left.jpg",
             "../Source/Museum19/Assets/Texture/right.jpg",
             "../Source/Museum19/Assets/Texture/top.jpg",
             "../Source/Museum19/Assets/Texture/bottom.jpg",
             "../Source/Museum19/Assets/Texture/back.jpg",
             "../Source/Museum19/Assets/Texture/front.jpg"
     };

     vector<std::string> faces3
     {
             "../Source/Museum19/Assets/Texture/Left_Tex.png",
             "../Source/Museum19/Assets/Texture/Right_Tex.png",
             "../Source/Museum19/Assets/Texture/Up_Tex.png",
             "../Source/Museum19/Assets/Texture/Down_Tex.png",
             "../Source/Museum19/Assets/Texture/Front_Tex.png",
             "../Source/Museum19/Assets/Texture/Back_Tex.png"
           
     };

    unsigned int cubemapTexture = loadCubemap(faces);
    unsigned int cubemapTexture2 = loadCubemap(faces2);
    unsigned int cubemapTexture3 = loadCubemap(faces3);
    GLuint textureID[] = {
        woodFloorID, // 0
        cubemapTexture, // 1
        sunID, // 2
        panelRoomID, // 3
        planet1ID, // 4
        planet2ID, // 5
        brickID, // 6
        upArrowID, // 7
        downArrowID, // 8
        cubemapTexture2,//9
        cubemapTexture3//10
    };

    bool ifSkyBox = true;
    bool ifParticule = true;
    
    glUseProgram(shaderProgram);
    //light position
    vec3  lightPosition = vec3(-12.0f, 5.0f, -15.0f);

    // Camera parameters for view transform
    vec3 cameraPosition = vec3(-12.0f, 1.0f, -4.0f);
    vec3 cameraLookAt(0.0f, 1.0f, -1.0f);
    vec3 cameraUp(0.0f, 1.0f, 0.0f);
    
    // Other camera parameters
    float cameraSpeed = 5.0f;
    float cameraFastSpeed = 2 * cameraSpeed;
    float cameraHorizontalAngle = 90.0f;
    float cameraVerticalAngle = 0.0f;
    bool  cameraFirstPerson = true; // press 1 or 2 to toggle this variable

    
    // Set projection matrix for shader, this won't change
    mat4 projectionMatrix = glm::perspective(20.0f,            // field of view in degrees
                                             WIDTH / LENGHT,  // aspect ratio
                                             0.1f, 100.0f);   // near and far (near > 0)
    
    GLuint projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]);

    // Set initial view matrix
    mat4 viewMatrix = lookAt(cameraPosition,  // eye
                             cameraPosition + cameraLookAt,  // center
                             cameraUp ); // up
    
    GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    
    
    // Define and upload geometry to the GPU here ...
    GLuint vao = ShaderMain.createVertexArrayObject();
    GLuint vaoSkyBox = ShaderMain.createVertexArraySkyBox();
    GLuint VAOs[10] = { vao, sphereVAO ,vaoSkyBox};
    GLuint activeVAOVertices[10] = { 36, (GLuint) sphereVertices};
    // For frame time
    float lastFrameTime = glfwGetTime();
    int lastMouseLeftState = GLFW_RELEASE;
    double lastMousePosX, lastMousePosY;
    glfwGetCursorPos(ShaderMain.window, &lastMousePosX, &lastMousePosY);
    
   
    // Enable Backface culling
    glEnable(GL_CULL_FACE);
    
    //Enable Depth Test
    glEnable(GL_DEPTH_TEST);
    
    //Find the location variables in shaders
    GLuint colorLocation = glGetUniformLocation(shaderProgram, "objectColor");
    GLuint worldMatrixLocation = glGetUniformLocation(shaderProgram, "worldMatrix");;
    GLuint depthMapLocation = glGetUniformLocation(shaderProgram, "depthMap");
    GLuint diffuseTextureLocation = glGetUniformLocation(shaderProgram, "diffuseTexture");
   
    //Setting Particles
    ParticleGenerator PG = ParticleGenerator(shaderParticleProgram, textureID[0], 500);

    //set depthMap
    glEnableVertexAttribArray(depthMapLocation);
    glUniform1i(depthMapLocation, 1);
    glUniform1i(diffuseTextureLocation, 0);

    // PANEL ROOM
    Room * currentRoom, baseRoom;
    int roomId = 1; // default to panel room
    mat4 panelElevatorPosition = translate(mat4(1.0f), vec3(-7.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.5f, 1.5f, 1.0f));
    mat4 pianoElevatorPosition = translate(mat4(1.0f), vec3(-12.0f, 0.0f, -20.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 0.5f));
    mat4 shapeElevatorPosition = translate(mat4(1.0f), vec3(0.0f, 0.0f, -10.0f)) * scale(mat4(1.0f), vec3(1.0f, 1.5f, 0.5f));

    // ONLY USE FOR DEBUG
    baseRoom = Room();
    baseRoom.setShaderColor(shaderProgram);
    baseRoom.setShaderShadow(shaderShadowProgram);
    baseRoom.setColorLocation(colorLocation);
    baseRoom.setWorldMatrix(worldMatrixLocation);
    baseRoom.setFloorColor(vec3(0.5f, 0.0f, 1.0f));
    baseRoom.setFloorBounds({1}); // incase user is trapped in baseRoom?
    

    PanelRoom PR = PanelRoom();
    PR.setShaderColor(shaderProgram);
    PR.setShaderShadow(shaderShadowProgram);
    PR.setColorLocation(colorLocation);
    PR.setWorldMatrix(worldMatrixLocation);
    PR.setFloorBounds({3});
    PR.setElevatorPosition(panelElevatorPosition);
    PR.setLightColorLocation( glGetUniformLocation(shaderProgram, "lightColor"));
    
    ShapeRoom SR = ShapeRoom();
    SR.setShaderColor(shaderProgram);
    SR.setShaderShadow(shaderShadowProgram);
    SR.setColorLocation(colorLocation);
    SR.setWorldMatrix(worldMatrixLocation);
    SR.setFloorColor(vec3(1.0f, 0.0f, 0.6f));
    SR.setFloorBounds({4, 3});
    SR.setElevatorPosition(shapeElevatorPosition);

    SpaceRoom SpaceR = SpaceRoom();
    SpaceR.setShaderColor(shaderProgram);
    SpaceR.setShaderShadow(shaderShadowProgram);
    SpaceR.setColorLocation(colorLocation);
    SpaceR.setWorldMatrix(worldMatrixLocation);
    SpaceR.setFloorBounds({2,1});
    SpaceR.setElevatorPosition(shapeElevatorPosition);


    PianoRoom PianoR = PianoRoom(5,5);
    PianoR.setShaderColor(shaderProgram);
    PianoR.setShaderShadow(shaderShadowProgram);
    PianoR.setColorLocation(colorLocation);
    PianoR.setWorldMatrix(worldMatrixLocation);
    PianoR.setSoundEngine(soundEngine);
    PianoR.initAudioFiles();
    PianoR.setFloorBounds({2});
    PianoR.setElevatorPosition(pianoElevatorPosition);


    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, "../Source/Museum19/Assets/Fonts/ARIALNB.ttf", 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    FT_Set_Pixel_Sizes(face, 0, 48);

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    for (GLubyte c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // Generate texture
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        // Set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Now store character for later use
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        Characters.insert(std::pair<GLchar, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    // Destroy FreeType once we're finished
    FT_Done_Face(face);
    FT_Done_FreeType(ft);


    // Configure VAO/VBO for texture quads
    glGenVertexArrays(1, &VAOText);
    glGenBuffers(1, &VBOText);
    glBindVertexArray(VAOText);
    glBindBuffer(GL_ARRAY_BUFFER, VBOText);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // default room 1
    currentRoom = &PianoR;
    glUseProgram(shaderSkyBoxProgram);
    GLuint skyBoxLocation = glGetUniformLocation(shaderSkyBoxProgram, "skybox");
    glUniform1i(skyBoxLocation, 0);
    // Entering Game Loop

    float theta = radians(cameraHorizontalAngle);

    float phi = radians(cameraVerticalAngle);

    cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
    vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

    glm::normalize(cameraSideVector);

    while(!glfwWindowShouldClose(ShaderMain.window))
    {


       

        // Frame time calculation
        float dt = glfwGetTime() - lastFrameTime;
        lastFrameTime += dt;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        PG.Update(dt,2);
//        PG.Draw();
        currentRoom->setCameraPosition(cameraPosition);
        
        ShaderMain.setShadow(shaderShadowProgram, lightPosition, SHADOW_WIDTH, SHADOW_HEIGHT);
        // Use normal shader
         // Render floor
        //render scene with shadows
        glBindVertexArray(vao);
//        glBindTexture(GL_TEXTURE_2D, woodFloorID);
        
        currentRoom->RenderRoom(textureID);
        currentRoom->RenderElevator(textureID);
        
        currentRoom->RenderScene2(VAOs, textureID, activeVAOVertices);
        // Render content in room
        glBindVertexArray(0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        //Shadows done!

        // RenderSceneWithColor
       
        if (ifSkyBox) {

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //  glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            glUseProgram(shaderSkyBoxProgram);
            GLuint projectionLocation = glGetUniformLocation(shaderSkyBoxProgram, "projection");
            GLuint viewLocation = glGetUniformLocation(shaderSkyBoxProgram, "view");

            viewMatrix = glm::mat4(glm::mat3(viewMatrix));
            glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &projectionMatrix[0][0]);
            glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &viewMatrix[0][0]);
            //GLuint worldSkyBoxLocation = glGetUniformLocation(shaderSkyBoxProgram, "wordlMatrix");


           // currentRoom->setWorldSkyBoxLocation(worldSkyBoxLocation);

            currentRoom->RenderSkyBox(VAOs, textureID);
            glDepthFunc(GL_LESS);
            // glDepthMask(GL_TRUE);

        }

        if (ifParticule) {

          //  glUseProgram(shaderParticleProgram);
           // currentRoom->RenderParticle(VAOs, textureID, activeVAOVertices);
            
            PG.Update(dt, 2, glm::vec2(dt / 2));
            PG.Draw();


        }
       

      
        // RenderScene for the texture element
        viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp);
        ShaderMain.setShaderProgram(shaderProgram, lightPosition, cameraPosition,viewMatrix);

       currentRoom->RenderScene2(VAOs,textureID,activeVAOVertices);
       glBindVertexArray(0);

        glBindVertexArray(vao);
        currentRoom->RenderRoom(textureID);
        currentRoom->RenderElevator(textureID);

        glBindTexture(GL_TEXTURE_2D, 0);
        glBindVertexArray(0);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
       

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (currentRoom == &SpaceR) {
            RenderText(shaderTextProgram, "Space Room", 10.0f, 250.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
            RenderText(shaderTextProgram, "Room showcases the use of skyboxes", 10.0f, 220.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
            RenderText(shaderTextProgram, "Press 'E','R' or 'T' to toggle between skyboxes", 10.0f, 190.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        }
        if (currentRoom == &PR) {
            RenderText(shaderTextProgram, "Panel Room", 10.0f, 250.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
            RenderText(shaderTextProgram, "Room showcases shadows, different light colors and motions", 10.0f, 220.0f, 0.5f, glm::vec3(1.0, 0.0f, 0.0f));
            RenderText(shaderTextProgram, "Press 'R','G' or 'B' to change the light color", 10.0f, 190.0f, 0.5f, glm::vec3(1.0, 0.0f, 0.0f));
        }
        if (currentRoom == &SR) {
            RenderText(shaderTextProgram, "Shape Room", 10.0f, 250.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
            RenderText(shaderTextProgram, "Room showcases shapes, splines, transformations and animation keys", 10.0f, 220.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
            RenderText(shaderTextProgram, "Press 'O' to toggle Spline animation", 10.0f, 190.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
            RenderText(shaderTextProgram, "Press 'T','L' or 'P' to toggle between the primitive shapes", 10.0f, 160.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
            RenderText(shaderTextProgram, "Press 'Q' or 'E' to increase or decrease rotation speed", 10.0f, 130.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
        }
        if (currentRoom == &PianoR) {
            RenderText(shaderTextProgram, "Piano Room", 10.0f, 250.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
            RenderText(shaderTextProgram, "Room showcases sounds and collisions", 10.0f, 220.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
          
        }
        glBlendFunc(GL_SRC_ALPHA,0);
        // Render content in room
       
        //
        //// End Frame
        //glBindVertexArray(0);


        glfwSwapBuffers(ShaderMain.window);
        glfwPollEvents();
        
        // Handle inputs
        if (glfwGetKey(ShaderMain.window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(ShaderMain.window, true);
            soundEngine->drop();
        }
       
        // This was solution for Lab02 - Moving camera exercise
        // We'll change this to be a first or third person camera
        bool fastCam = glfwGetKey(ShaderMain.window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(ShaderMain.window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;
        float currentCameraSpeed = (fastCam) ? cameraFastSpeed : cameraSpeed;
        
        double mousePosX, mousePosY;
        glfwGetCursorPos(ShaderMain.window, &mousePosX, &mousePosY);
        
        double dx = mousePosX - lastMousePosX;
        double dy = mousePosY - lastMousePosY;
        
        lastMousePosX = mousePosX;
        lastMousePosY = mousePosY;
        
        // Convert to spherical coordinates
        const float cameraAngularSpeed = 20.0f;
     
        
        // Clamp vertical angle to [-85, 85] degrees
        cameraVerticalAngle = std::max(-85.0f,std::min(85.0f, cameraVerticalAngle));
       


       
        if (cameraHorizontalAngle > 360)
        {
            cameraHorizontalAngle -= 360;
        }
        else if (cameraHorizontalAngle < -360)
        {
            cameraHorizontalAngle += 360;
        }
        
        // rotate the camera turn the view left and right
        if (glfwGetMouseButton(ShaderMain.window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
           
            cameraVerticalAngle -= dy * cameraAngularSpeed * dt;
            cameraHorizontalAngle -= dx * cameraAngularSpeed * dt;

            theta = radians(cameraHorizontalAngle);
            phi = radians(cameraVerticalAngle);

            cameraLookAt = vec3(cosf(phi) * cosf(theta), sinf(phi), -cosf(phi) * sinf(theta));
            vec3 cameraSideVector = glm::cross(cameraLookAt, vec3(0.0f, 1.0f, 0.0f));

            glm::normalize(cameraSideVector);
        }
        
        currentRoom->DefineInputs(ShaderMain.window);
            
        // detect if collided with door
        if (currentRoom->getFloorChange().hasEnteredElevator && currentRoom->getFloorChange().floor != roomId) {
            roomId = currentRoom->getFloorChange().floor;
            glUseProgram(shaderProgram);
            glUniform3fv(glGetUniformLocation(shaderProgram, "lightColor"), 1, glm::value_ptr(glm::vec3(0.7f, 0.8f, 0.7f)));
          
            switch (roomId) {
                case 1:
                    cameraPosition = vec3(0.0f,1.0f,3.0f);
                    lightPosition = vec3(0.0f, 10.0f, 0.0f);
                    currentRoom = &PR;
                    ifSkyBox = true;
                    ifParticule = false;
                    break;
                case 2:
                    cameraPosition = vec3(4.0f, 1.0f, 12.0f);
                    lightPosition = vec3(0.0f, 30.0f, 0.0f);
                    
                    currentRoom = &SR;
                    ifSkyBox = true;
                    ifParticule = false;
                    break;
                case 3:
                    cameraPosition = vec3(0.0f,1.0f,3.0f);
                    lightPosition = vec3(0.0f, 5.0f, 0.0f);

                    currentRoom = &SpaceR;
                    ifSkyBox = true;
                    ifParticule = true;
                    break;
                case 4:
                    cameraPosition = vec3(-12.0f, 1.0f, -4.0f);
                    lightPosition = vec3(-12.0f, 5.0f, -15.0f);

                    currentRoom = &PianoR;
                    ifSkyBox = false;
                    ifParticule = false;
                    break;
                default:
                    lightPosition = vec3(0.0f, 10.0f, 0.0f);
                    currentRoom = &baseRoom;
                    ifSkyBox = false;
                    ifParticule = false;
                    break;
            }
            
            // clear floor change data to avoid inf loop
            currentRoom->clearFloorChange();
        }
        
        if (glfwGetKey(ShaderMain.window, GLFW_KEY_0) == GLFW_PRESS) // swap to base room (FOR DEBUGGING ONLY)
        {
            lightPosition = vec3(0.0f, 10.0f, 0.0f);
            currentRoom = &baseRoom;
            ifSkyBox = false;
            ifParticule = false;
        }
    
        if (glfwGetKey(ShaderMain.window, GLFW_KEY_1) == GLFW_PRESS) // swap to panel room (DEFAULT)
        {
            cameraPosition = vec3(0.0f,1.0f,3.0f);
            lightPosition = vec3(0.0f, 10.0f, 0.0f);
            currentRoom = &PR;
            ifSkyBox = true;
            ifParticule = false;
        }
        
        if (glfwGetKey(ShaderMain.window, GLFW_KEY_2) == GLFW_PRESS) // swap to shape/spline room
        {
            cameraPosition = vec3(4.0f, 1.0f, 12.0f);
            lightPosition = vec3(0.0f, 30.0f, 0.0f);
            
            currentRoom = &SR;
            ifSkyBox = true;
            ifParticule = false;
        }

        if (glfwGetKey(ShaderMain.window, GLFW_KEY_3) == GLFW_PRESS) // swap to space + particles room
        {
            cameraPosition = vec3(0.0f,1.0f,3.0f);
            lightPosition = vec3(0.0f, 5.0f, 0.0f);

            currentRoom = &SpaceR;
            ifSkyBox = true;
            ifParticule = true;
        }
        if (glfwGetKey(ShaderMain.window, GLFW_KEY_4) == GLFW_PRESS) // swap to piano room
        {
            cameraPosition = vec3(-12.0f, 1.0f, -4.0f);
            lightPosition = vec3(-12.0f, 5.0f, -15.0f);

            currentRoom = &PianoR;
            ifSkyBox = false;
            ifParticule = false;
        }
        

        
        
        //check if player if room wall collision has triggered
        if (currentRoom->getCollisionState().hasWallCollision) {
            cameraPosition = currentRoom->getCollisionState().collidedPosition;
        }
       
        // visitor movement
        if (glfwGetKey(ShaderMain.window, GLFW_KEY_A) == GLFW_PRESS) // move camera to the left
        {
            cameraPosition -= glm::normalize(glm::cross(cameraLookAt, cameraUp)) * currentCameraSpeed * dt;
        }

        if (glfwGetKey(ShaderMain.window, GLFW_KEY_D) == GLFW_PRESS) // move camera to the right
        {
            cameraPosition += glm::normalize(glm::cross(cameraLookAt, cameraUp)) * currentCameraSpeed * dt;
        }

        if (glfwGetKey(ShaderMain.window, GLFW_KEY_S) == GLFW_PRESS) // move camera up
        {
            cameraPosition -= vec3(cameraLookAt.x,0.0f,cameraLookAt.z) * currentCameraSpeed * dt;
        }

        if (glfwGetKey(ShaderMain.window, GLFW_KEY_W) == GLFW_PRESS) // move camera down
        {
            cameraPosition += vec3(cameraLookAt.x, 0.0f, cameraLookAt.z) * currentCameraSpeed * dt;
        }
      
      
        mat4 viewMatrix = lookAt(cameraPosition, cameraPosition + cameraLookAt, cameraUp );

        GLuint viewMatrixLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);
        
    }

    
    // Shutdown GLFW
    glfwTerminate();
    
	return 0;
}

GLuint loadTexture(const char* filename)
{
    // Step1 Create and bind textures
    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    assert(textureId != 0);


    glBindTexture(GL_TEXTURE_2D, textureId);

    // Step2 Set filter parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Step3 Load Textures with dimension data
    int width, height, nrChannels;
    unsigned char* data = stbi_load(filename, &width, &height, &nrChannels, 0);
    if (!data)
    {
        std::cerr << "Error::Texture could not load texture file:" << filename << std::endl;
        return 0;
    }

    // Step4 Upload the texture to the PU
    GLenum format = 0;
    if (nrChannels == 1)
        format = GL_RED;
    else if (nrChannels == 3)
        format = GL_RGB;
    else if (nrChannels == 4)
        format = GL_RGBA;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height,
        0, format, GL_UNSIGNED_BYTE, data);

    // Step5 Free resources
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);
    return textureId;
}
unsigned int loadCubemap(vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 3);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
void RenderText(GLuint s, std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
    // Activate corresponding render state	
    glUseProgram(s);
    glUniform3f(glGetUniformLocation(s, "textColor"), color.x, color.y, color.z);
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(VAOText);

    // Iterate through all characters
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Character ch = Characters[*c];

        GLfloat xpos = x + ch.Bearing.x * scale;
        GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

        GLfloat w = ch.Size.x * scale;
        GLfloat h = ch.Size.y * scale;
        // Update VBO for each character
        GLfloat vertices[6][4] = {
            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos,     ypos,       0.0, 1.0 },
            { xpos + w, ypos,       1.0, 1.0 },

            { xpos,     ypos + h,   0.0, 0.0 },
            { xpos + w, ypos,       1.0, 1.0 },
            { xpos + w, ypos + h,   1.0, 0.0 }
        };
        // Render glyph texture over quad
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);
        // Update content of VBO memory
        glBindBuffer(GL_ARRAY_BUFFER, VBOText);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        // Render quad
        glDrawArrays(GL_TRIANGLES, 0, 6);
        // Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        x += (ch.Advance >> 6)* scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}