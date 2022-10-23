#include "Room.h"
#include "Particle.h"

void SpaceRoom::RenderScene() {
   


    mat4 panelMatrix = mat4(1.0f);

  

    for (int i = 0; i < 6; i++) {
       
        glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0f)));
        panelMatrix =translate(mat4(1.0f), vec3(0.0f, -10.0f, 0.0f))* scale(mat4(1.0f), vec3(20.0f, 20.0f, 20.0f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
        glDrawArrays(GL_TRIANGLES, i * 6, 6);
       
    } // 36 vertices, starting at index 0
   
  
   
}
void SpaceRoom::RenderParticle(GLuint vaos[], GLuint textureID[], GLuint activeVAOVertices[]){
   
    



   



}
void SpaceRoom::DefineInputs(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) // points
    {
        textureIndex = 1;

    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // points
    {
      
        textureIndex = 10;
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // points
    {

        textureIndex = 9;
    }
}
void SpaceRoom::RenderSkyBox(GLuint vaos[], GLuint textureID[]) {
    
    glBindVertexArray(vaos[2]);
    
   

    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID[textureIndex]);

   
   

    glDrawArrays(GL_TRIANGLES, 0, 36);
   
   
  
    glBindVertexArray(0);

};
void SpaceRoom::RenderScene2(GLuint vaos[],GLuint textureID[], GLuint activeVAOVertices[])
{
   
    
   mat4 panelMatrix=mat4(1.0f);
   
   
    glBindVertexArray(vaos[1]);
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0f)));
    glBindTexture(GL_TEXTURE_2D,textureID[2]);// The sun
    panelMatrix = mat4(1.0f)*rotate(mat4(1.0f), radians(rotation*15), vec3(0.0f, 1.0f, 1.0f)) * scale(mat4(1.0f),vec3(0.5f,0.5f,0.5f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, activeVAOVertices[1], GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, textureID[4]);// The sun
    panelMatrix =rotate(mat4(1.0f), radians(rotation), vec3(0.0f, 1.0f, 0.0f))* translate(mat4(1.0f),vec3(1.0f,0.0f,0.0f)) *rotate(mat4(1.0f), radians(rotation), vec3(0.0f, 1.0f, 1.0f)) * scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, activeVAOVertices[1], GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);// The sun

    glBindTexture(GL_TEXTURE_2D, textureID[5]);// The sun
    panelMatrix = rotate(mat4(1.0f), radians(rotation*10), vec3(0.0f, 1.0f, 0.0f)) * translate(mat4(1.0f), vec3(0.0f, 0.0f, 2.0f)) * rotate(mat4(1.0f), radians(rotation * 10), vec3(0.0f, 1.0f, 0.0f))*scale(mat4(1.0f), vec3(0.2f, 0.2f, 0.2f));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, activeVAOVertices[1], GL_UNSIGNED_INT, 0);
    glBindTexture(GL_TEXTURE_2D, 0);// The sun

    panelMatrix = rotate(mat4(1.0f), radians(rotation * 10), vec3(0.0f, 1.0f, 1.0f)) * translate(mat4(1.0f), vec3(0.0f, 0.0f, 4.0f)) * rotate(mat4(1.0f), radians(rotation * 10), vec3(0.0f, 1.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.02f, 0.02f, 0.02f));
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, activeVAOVertices[1], GL_UNSIGNED_INT, 0);

    rotation += 0.05f;
    for (int i = 0; i < 1; i++) {
    
    
        panelMatrix = scale(mat4(1.0f), vec3(20.0f, 10.0f, 20.0f));
        glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.93f, 0.709f, 0.10f)));
       // glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.23f, 0.109f, 0.7f)));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
        glDrawElements(GL_LINES, activeVAOVertices[1], GL_UNSIGNED_INT, 0);
    
    
    
    }

    /*glBindVertexArray(vaos[2]);
    panelMatrix =  scale(mat4(1.0f), vec3(20.0f, 1.0f, 20.0f));
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
    glDrawElements(GL_TRIANGLES, activeVAOVertices[2], GL_UNSIGNED_INT, 0);*/

    //glBindVertexArray(0);
    //glCullFace(GL_BACK);
    //glFrontFace(GL_CW);
    //glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0f)));
    //glBindTexture(GL_TEXTURE_2D, textureID[1]);
    //panelMatrix = scale(mat4(1.0f), vec3(50.0f, 50.0f, 50.0f));
    //glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
    //glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
    //glBindTexture(GL_TEXTURE_2D, 0);
    //glBindVertexArray(0);
    //glCullFace(GL_CULL_FACE);
    //glFrontFace(GL_CCW);
}
;

