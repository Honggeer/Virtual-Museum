//
//  room.cpp
//  COMP371_Labs
//
//  Created by Kenny on 2020-04-04.
//  Copyright © 2020 Concordia. All rights reserved.
//

#include <stdio.h>
#include "Room.h"

Room::Room(): width(10), length(10), floorColor(vec3(0.9f, 0.9f, 0.9f)) {}

Room::Room(int _width, int _length) {
    width = _width;
    length = _length;
}

Room::Room(int _width, int _length, int _shaderColor, int _shaderTexture = 0, int _shaderShadow = 0):
    width(_width),
    length(_length),
    shaderColor(_shaderColor),
	shaderTexture(_shaderTexture),
	shaderShadow(_shaderShadow)
{};

void Room::setShaderColor(int SC) {
    shaderShadow = SC;
};

void Room::setShaderTexture(int ST) {
    shaderTexture = ST;
}

void Room::setShaderShadow(int SS) {
    shaderShadow = SS;
}

void Room::setWorldSkyBoxLocation(GLuint location)
{
    worldMatrixSkyBox = location;
}

void Room::setColorLocation(GLuint location) {
    colorLocation = location;
}

void Room::setWorldMatrix(GLuint world) {
    worldMatrixLocation = world;
}


void Room::setFloorColor(vec3 color) {
    floorColor = color;
}

void Room::setCameraPosition(vec3 position) {
    cameraPosition = position;
};

void Room::setSoundEngine(ISoundEngine* engine){
     roomSoundEngine = engine;
};

void Room::setFloorBounds(vector<int> bounds){
    floorBounds = bounds;
}

void Room::setElevatorPosition(mat4 transform) {
    elevatorPosition = transform;
}

CollisionState Room::getCollisionState() {
    CollisionState cs;
    cs.hasWallCollision = hasCollidedWall;
    cs.collidedPosition = collidedPosition;
    
    return cs;
}

FloorNavigate Room::getFloorChange() {
    FloorNavigate fn;
    fn.hasEnteredElevator = hasEnteredElevator;
    fn.floor = floorToGo;
    return fn;
}

void Room::clearFloorChange() {
    hasEnteredElevator = false;
    floorToGo = 0;
}

void Room::RenderRoom(GLuint textureID[]) {
    glBindTexture(GL_TEXTURE_2D, textureID[0]);

    glUniform3fv(colorLocation, 1, glm::value_ptr(vec3(0.0f)));
    // Draw ground
    mat4 groundWorldMatrix;
    
    for (int i = 0; i < width; i++) {//floor
        for (int j = 0; j < length; j++) {
            groundWorldMatrix = translate(mat4(1.0f), vec3(-20.0f+4.00f * i, 0.0f , -20.0f+ 4.0f*j)) *  scale(mat4(1.0f), vec3(4.0f, 0.01f, 4.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &groundWorldMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
        }
    }
}

void Room::RenderElevator(GLuint textureID[]) {
    glUniform3fv(colorLocation, 1, glm::value_ptr(vec3(0.0f)));
    
    mat4 doorMatrix1 = elevatorPosition;
    mat4 doorMatrix2 = doorMatrix1 * translate(mat4(1.0f), vec3(-2.0f, 0.0f , 0.0f));
    glBindTexture(GL_TEXTURE_2D, 0);
    
    if (floorBounds.size() > 1) {
        glBindTexture(GL_TEXTURE_2D, textureID[7]); // up
    } else if(floorBounds[0] == (int) 2){
        glBindTexture(GL_TEXTURE_2D, textureID[8]);
    } else {
        glBindTexture(GL_TEXTURE_2D, textureID[7]);
    }
    
    
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &doorMatrix1[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
    
    glBindTexture(GL_TEXTURE_2D, 0);

    if (detectCollision(cameraPosition, doorMatrix1)) {
        hasEnteredElevator = true;
        floorToGo = floorBounds[0];
    }

    if (floorBounds.size() > 1) {
        glBindTexture(GL_TEXTURE_2D, textureID[8]);
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &doorMatrix2[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
        
        if(detectCollision(cameraPosition, doorMatrix2)) {
            hasEnteredElevator = true;
            floorToGo = floorBounds[1];
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    
}


bool Room::detectCollision(vec3 player, mat4 objectMatrix) {
    // these values are hardcopied from the vbo defined in the shader class.
    // we keep y-value as 0.0f because we only care if player has stepped on object
    vec3 objectOriginTopLeft = vec3(-0.5f, 0.0f, 0.5f);
    vec3 objectOriginBottomRight = vec3(0.5f, 0.0f, -0.5f);

    // extracting translate and scale value from matrix.
    float translateX = objectMatrix[3].x;
    float translateZ = objectMatrix[3].z;
    float scaleX = objectMatrix[0].x;
    float scaleZ = objectMatrix[2].z;
    
    float xMin = (objectOriginTopLeft.x * scaleX) + translateX;
    float xMax = (objectOriginBottomRight.x * scaleX) + translateX;
    float zMin = (objectOriginBottomRight.z * scaleZ) +  translateZ;
    float zMax = (objectOriginTopLeft.z * scaleZ) + translateZ;
    
    // x-axis collision
    bool xCollision = player.x >= xMin && player.x <= xMax;
    // z-axis collision
    bool zCollision = player.z >= zMin && player.z <= zMax;
    
    return xCollision && zCollision;
}
