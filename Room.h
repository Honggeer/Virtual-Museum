#pragma once

#define _USE_MATH_DEFINES
#include <algorithm>
#include <iostream>
#include <list>
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
#include "Shader.h"
#include "CollisionState.h"

#include "irrKlang.h"
using namespace irrklang;

class Room  {
public:
    Room();
    Room(int width_, int length_);
    
    Room(int _width, int _length, int _shaderColor, int _shaderTexture, int _shaderShadow);
        
    void virtual setShaderColor(int SC);
    void virtual setShaderTexture(int ST);
    void virtual setShaderShadow(int SS);
    void virtual setWorldSkyBoxLocation(GLuint location);
    void virtual setColorLocation(GLuint location);
    void virtual setWorldMatrix(GLuint location);
    void virtual setFloorColor(vec3 color); //vec3(0.9f, 0.9f, 0.9f)
    void virtual setCameraPosition(vec3 position);
    void virtual setSoundEngine(ISoundEngine* engine);
    void virtual setFloorBounds(vector<int> bounds);
    void virtual setElevatorPosition(mat4 transform);
    
    CollisionState virtual getCollisionState();
    FloorNavigate virtual getFloorChange();
    
    void virtual RenderSkyBox(GLuint vaos[], GLuint textureID[]) {};
    void virtual RenderParticle(GLuint vaos[], GLuint textureID[], GLuint activeVAOVertices[]) {};
    void virtual RenderRoom(GLuint textureID[]); // this will be used to render the room
    void virtual RenderScene() {};
    void virtual RenderScene2(GLuint vaos[],GLuint textureID[],GLuint activeVAOVertices[]) {};
    void RenderElevator(GLuint textureID[]); // render elevator
    // this will be used to render the room
    
    
    
    void virtual DefineInputs(GLFWwindow* window) {};
    
//    void virtual rePositionPlayer(vec3 newPosition);
    void virtual closeProgram() {};
    void virtual clearFloorChange();


protected:
    int width;
    int length;
    
    mat4 elevatorPosition = scale(mat4(1.0f), vec3(1.0f, 1.5f, 0.5f)); // default elevator
    bool hasEnteredElevator = false;
    int floorToGo;
    
    bool hasCollidedWall = false;
    vec3 collidedPosition;
    
    int shaderColor;
    int shaderTexture;
    int shaderShadow;
    
    GLuint colorLocation;
    GLuint worldMatrixLocation;
    GLuint worldMatrixSkyBox;
    
    vec3 cameraPosition;
    
    ISoundEngine* roomSoundEngine;
    
    vector<int> floorBounds = { 0 };
    bool detectCollision(vec3 player, mat4 objectMatrix);
private:
    vec3 floorColor;
};

class ShapeRoom: public Room {
public:
    using Room::Room;
    void RenderRoom(GLuint textureID[]) {
        Room::RenderRoom(textureID);
        splinePoints_X = ShapeRoom::convertSplinePoints(mControlPoints_X);
        splinePoints_Y = ShapeRoom::convertSplinePoints(mControlPoints_Y);
        splinePoints_Z = ShapeRoom::convertSplinePoints(mControlPoints_Z);

    }

    void RenderScene2(GLuint vaos[],GLuint textureID[],GLuint activeVAOVertices[]);
    void DefineInputs(GLFWwindow* window);
    void RenderSkyBox(GLuint vaos[], GLuint textureID[]);
    
    vec3 GetSplinePosition(float t);
    static vec3 GetSplinePosition(float t, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& p4);
    vector<vec3> convertSplinePoints(vector<vec3> points);
private:
    GLenum enumUsed = GL_TRIANGLES;

    mat4 polygon_1;
    mat4 polygon_2;
    mat4 polygon_3;

    float rotatingSpeed = 1.0f;
    
    // spline vectors
    bool splineMode = false;
    int animationKey = 0;
    
    mat4 baseMatrixX = mat4(1.0f);
    mat4 baseMatrixY = mat4(1.0f);
    mat4 baseMatrixZ = mat4(1.0f);
    
    // zigzag
    vector<vec3> mControlPoints_Y = {
        vec3(-20.0f, 0.0f, 0.0f),
        vec3(-15.0f, 10.0f, 0.0f),
        vec3(-10.0f, 0.0f, 0.0f),
        vec3(-5.0f, 10.0f, 0.0f), // 4
        
        vec3(0.0f, 0.0f, 0.0f),
        vec3(5.0f, 5.0f, 0.0f),
        vec3(0.0f, 10.0f, 0.0f),
        vec3(-5.0f, 5.0f, 0.0f),
        vec3(0.0f, 0.0f, 0.0f), // 9

        vec3(5.0f, 10.0f, 0.0f),
        vec3(10.0f, 0.0f, 0.0f),
        vec3(15.0f, 10.0f, 0.0f),
        vec3(20.0f, 0.0f, 0.0f), // 13
        
        vec3(10.0f, 10.0f, 0.0f),
        vec3(0.0f, 0.0f, 0.0f),
        vec3(-10.0f, 10.0f, 0.0f), // 16
    };
    
    
    vector<vec3> mControlPoints_X = {
        vec3(0.0f, 4.0f, 4.0f),
        vec3(0.0f, 6.0f, 4.0f),
        vec3(0.0f, 8.0f, 6.0f),
        vec3(0.0f, 8.0f, 8.0f),
        vec3(0.0f, 6.0f, 10.0f),
        vec3(0.0f, 4.0f, 10.0f),
        vec3(0.0f, 2.0f, 8.0f),
        vec3(0.0f, 2.0f, 6.0f), // 8
        vec3(0.0f, 4.0f, 4.0f),
        vec3(0.0f, 6.0f, 4.0f),
        vec3(0.0f, 8.0f, 6.0f),
        vec3(0.0f, 8.0f, 8.0f),
        vec3(0.0f, 6.0f, 10.0f),
        vec3(0.0f, 4.0f, 10.0f),
        vec3(0.0f, 2.0f, 8.0f),
        vec3(0.0f, 2.0f, 6.0f), // 8
    };
    
    // random spherical 12 rows
    vector<vec3> mControlPoints_Z = {
        vec3(10.0f, 10.0f, 0.0f), // start first loop 1
        vec3(0.0f, 20.0f, 0.0f), // 2
        vec3(-10.0f, 10.0f, 0.0f), // 3
        vec3(0.0f, 0.0f, 0.0f), // 4
        
        vec3(10.0f, 10.0f, 0.0f),// start second loop 5
        vec3(0.0f, 10.0f, -10.0f),//6
        vec3(-10.0f, 10.0f, 0.0f),// 7
        vec3(0.0f, 10.0f, 10.0f),// 8
        
        vec3(10.0f, 10.0f, 0.0f),// start third loop 9
        vec3(0.0f, 10.0f, -10.0f),// 10
        vec3(0.0f, 20.0f, 0.0f),// 11
        vec3(0.0f, 10.0f, 10.0f),// 12
        
        vec3(0.0f, 0.0f, 0.0f),// smooth finish 13
        vec3(0.0f, 10.0f, -10.0f), // 14
        vec3(-10.0f, 10.0f, 0.0f),// 15
        vec3(0.0f, 10.0f, 10.0f),// 16
    };
    
    vector<vec3> splinePoints_X;
    vector<vec3> splinePoints_Y;
    vector<vec3> splinePoints_Z;

};

class PanelRoom: public Room {
public:
    using Room::Room;
    void RenderRoom(GLuint textureID[]) {};
    void RenderScene();
    void RenderSkyBox(GLuint vaos[], GLuint textureID[]);
    void RenderScene2(GLuint vaos[], GLuint textureID[], GLuint activeVAOVertices[]);
    void setLightColorLocation(GLuint LCP);
    void DefineInputs(GLFWwindow* window);
private:
    int row = 0;
    float loop = 0;
    float angle = 0;
    float rayon = 0;
    float  movement = 1.0f;
    GLuint lightColorLocation;
    vec3 lightColor= vec3(0.7f, 0.8f, 0.7f);
    float red = 0.7f;
    float green = 0.8f;
    float blue = 0.7f;
};

class SpaceRoom :public Room {
public:
    using Room::Room;
    void RenderRoom(GLuint textureID[]) {};
    void RenderScene();
    void RenderScene2(GLuint vaos[], GLuint textureID[], GLuint activeVAOVertices[]);
    void RenderSkyBox(GLuint vaos[], GLuint textureID[]);
    void RenderParticle(GLuint vaos[], GLuint textureID[],GLuint activeVAOVertices[]);
    void DefineInputs(GLFWwindow* window);
private:
    float rotation = 0.0f;
    int textureIndex = 1;

};
class PianoRoom : public Room {
public:
    using Room::Room;
    void RenderScene2(GLuint vaos[], GLuint textureID[], GLuint activeVAOVertices[]);
    void initAudioFiles();
    void RenderRoom(GLuint textureID[]);
private:
    int currentTile = -1; // start off at -1 bcuz we want a number not included in tiles indexes
    
    vector<ISoundSource*> noteFiles;
};
