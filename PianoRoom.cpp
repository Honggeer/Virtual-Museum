#include "Room.h"
void PianoRoom::RenderScene2(GLuint vaos[], GLuint textureID[], GLuint activeVAOVertices[]) {
	glBindVertexArray(vaos[0]);
    
    float x = 1.0f;
    float z = 4.0f;
	mat4 tiles = translate(mat4(1.0f), vec3(-15.0f, 0.0f, -10.0f)) * scale(mat4(1.0f), vec3(x, 0.25f, z));
    mat4 pressedMatrix = scale(mat4(1.0f), vec3(1.0f, 0.05f, 1.0f));
	
    for(int i = 0; i < 8; ++i)
    {
        mat4 baseTileMatrix = translate(mat4(1.0f), vec3(1.0f * i, 0.0f, 1.0f)) * tiles;
        if (i % 2 == 0) {
            glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
        } else {
            glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.01f, 0.01f, 0.01f)));
        }
        
        bool tileIsSteppedOn = Room::detectCollision(cameraPosition, baseTileMatrix);
        
        if (tileIsSteppedOn) {
            // play music note
            if (i != currentTile) {
                roomSoundEngine->play2D(noteFiles[i]);
                currentTile = i;
            }
        }
        
        mat4 finalTileMatrix = tileIsSteppedOn ? baseTileMatrix * pressedMatrix : baseTileMatrix;
        
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &finalTileMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, activeVAOVertices[0]);
        
    }
    glBindVertexArray(0);
	
};

void PianoRoom::RenderRoom(GLuint textureID[]){
    Room::RenderRoom(textureID);
    glBindTexture(GL_TEXTURE_2D, textureID[6]);

    glUniform3fv(colorLocation, 1, glm::value_ptr(vec3(0.0f)));

    mat4 nearWallMatrix = translate(mat4(1.0f), vec3((width + 1) * -2.0f, 0.0f , -2.0f)) * scale(mat4(1.0f), vec3(20.0f, 10.0f, 1.0f));
    mat4 farWallMatrix = translate(mat4(1.0f), vec3((width + 1) * -2.0f, 0.0f , -4.0f * length - 2.0f)) * scale(mat4(1.0f), vec3(20.0f, 10.0f, 1.0f));
    mat4 leftWallMatrix = translate(mat4(1.0f), vec3(-4.0f * width - 2.0f, 0.0f , (length + 1) * -2.0f)) * scale(mat4(1.0f), vec3(1.0f, 10.0f, 20.0f));
    mat4 rightWallMatrix = translate(mat4(1.0f), vec3(-2.0f, 0.0f , (length + 1) * -2.0f)) * scale(mat4(1.0f), vec3(1.0f, 10.0f, 20.0f));
    
    bool hitNearWall = Room::detectCollision(cameraPosition, nearWallMatrix);
    bool hitRightWall = Room::detectCollision(cameraPosition, rightWallMatrix);
    bool hitLeftWall = Room::detectCollision(cameraPosition, leftWallMatrix);
    bool hitFarWall = Room::detectCollision(cameraPosition, farWallMatrix);
    
    if (hitLeftWall) {
        float diffValue = (0.5f * leftWallMatrix[0].x) + leftWallMatrix[3].x;
        hasCollidedWall = true;
        collidedPosition = vec3(diffValue + 0.2f, cameraPosition.y, cameraPosition.z);
    } else if (hitRightWall) {
        float diffValue = (-0.5f * rightWallMatrix[0].x) + rightWallMatrix[3].x;
        hasCollidedWall = true;
        collidedPosition = vec3(diffValue - 0.2f, cameraPosition.y, cameraPosition.z);
    } else if (hitNearWall) {
        float diffValue = (-0.5f * nearWallMatrix[2].z) + nearWallMatrix[3].z;
        hasCollidedWall = true;
        collidedPosition = vec3(cameraPosition.x, cameraPosition.y, diffValue - 0.2f);
    } else if (hitFarWall) {
        float diffValue = (0.5f * farWallMatrix[2].z) + farWallMatrix[3].z;
        hasCollidedWall = true;
        collidedPosition = vec3(cameraPosition.x, cameraPosition.y, diffValue + 0.2f);
    } else {
        hasCollidedWall = false;
    }
    
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &nearWallMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
    

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &farWallMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
    

    
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leftWallMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
    

    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &rightWallMatrix[0][0]);
    glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0

}

// store files as ISoundSource, native to irrklang
void PianoRoom::initAudioFiles() {
    noteFiles = {
        roomSoundEngine->addSoundSourceFromFile("../Source/Museum19/Assets/MusicNotes/NOTE1.wav"),
        roomSoundEngine->addSoundSourceFromFile("../Source/Museum19/Assets/MusicNotes/NOTE2.wav"),
        roomSoundEngine->addSoundSourceFromFile("../Source/Museum19/Assets/MusicNotes/NOTE3.wav"),
        roomSoundEngine->addSoundSourceFromFile("../Source/Museum19/Assets/MusicNotes/NOTE4.wav"),
        roomSoundEngine->addSoundSourceFromFile("../Source/Museum19/Assets/MusicNotes/NOTE5.wav"),
        roomSoundEngine->addSoundSourceFromFile("../Source/Museum19/Assets/MusicNotes/NOTE6.wav"),
        roomSoundEngine->addSoundSourceFromFile("../Source/Museum19/Assets/MusicNotes/NOTE7.wav"),
        roomSoundEngine->addSoundSourceFromFile("../Source/Museum19/Assets/MusicNotes/NOTE8.wav"),
    };
}
