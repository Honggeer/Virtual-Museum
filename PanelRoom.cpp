#include "Room.h"
void PanelRoom::RenderSkyBox(GLuint vaos[], GLuint textureID[]){
    glBindVertexArray(vaos[2]);




    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID[1]);




    glDrawArrays(GL_TRIANGLES, 0, 36);



    glBindVertexArray(0);
}




void PanelRoom::RenderScene() {


    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
    mat4 panelMatrix;
    float dimension = 0.25f;
    loop += 0.01f;
    if (loop >= 180.0f) {
        loop = 0;

    }
    if (row == 9)
        row = 0;
    else
        row++;
    float verticalAngle = 0;
    float degree = loop;
    for (int k = 0; k < 2; k++) {
        for (int i = 0; i < 50; i++) {
            degree += 0.5;
            float sinus = sin(degree);
            for (int j = 0; j < 10; j++) {

                angle = 0;
                if (sinus >= 0) {
                    if (j <= (sinus + 1) * 5 && j > 4) {

                        angle = -4 * j;
                    }
                }


                else
                    if (j >= (sinus + 1) * 5 && j < 5)
                    {


                        angle = -12 * j - 12;
                    }



                panelMatrix = translate(mat4(1.0f), vec3((0.03f + dimension) * i - 7.5f, (0.03f + dimension) * j, -7.5f + 15.0f * k)) * translate(mat4(1.0f), vec3(0.0f, dimension / 2, 0.0f)) * rotate(mat4(1.0f), radians(angle), vec3(1.0f, 0.0f, 0.0f)) * translate(mat4(1.0f), vec3(0.0f, -dimension / 2, 0.0f)) * scale(mat4(1.0f), vec3(dimension, dimension, 0.025f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
            }
            //glDrawElements(GL_TRIANGLES, activeVAOVertices[1], GL_UNSIGNED_INT, 0);
        }
    }


    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
    float dx;
    float dz;
    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            dx = 1.03f * i + -7.5f;
            dz = 1.03f * j - 7.5f;


            float distance = pow((dx * dx + dz * dz), 0.5f);
            if (distance < rayon) {


                glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
                panelMatrix = translate(mat4(1.0f), vec3(dx, 0.0f + 3.0f, dz)) * scale(mat4(1.0f), vec3(1.0f, 0.02f, 1.0f));




            }

            else {

                if (movement > -100) {
                    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
                    panelMatrix = translate(mat4(1.0f), vec3(dx, 3.0f, dz)) * rotate(mat4(1.0f), radians(90.0f), vec3(1.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 0.02f, 1.0f));
                }
                /* else {
                     glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
                     panelMatrix = translate(mat4(1.0f), vec3(dx, 0.0f, dz)) * scale(mat4(1.0f), vec3(1.0f, 0.02f, 1.0f));
                 }*/
            }

            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


        }
        rayon += 0.003f * movement;
        if (rayon >= 10.925)
            movement *= -1.0f;
        else
            if (rayon < 0)
                movement *= -1.0f;
    }

    for (int i = 0; i < 15; i++) {
        for (int j = 0; j < 15; j++) {
            dx = 1.03f * i + -7.5f;
            dz = 1.03f * j - 7.5f;

            glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
            panelMatrix = translate(mat4(1.0f), vec3(dx, 0.0f, dz)) * scale(mat4(1.0f), vec3(1.0f, 0.02f, 1.0f));
            glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
            glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


        }

    }
    /* if (rayon > 7.25f)
         rayon = 0;*/






    glBindVertexArray(0);

}
void PanelRoom::RenderScene2(GLuint vaos[],GLuint textureID[], GLuint activeVAOVertices[])
{
    glUniform3fv(lightColorLocation, 1, glm::value_ptr(glm::vec3(lightColor)));
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
   // glBindTexture(GL_TEXTURE_2D, textureID[3]);
    glBindVertexArray(vaos[0]);
   
    mat4 leftWallMatrix = translate(mat4(1.0f), vec3(4.3f *  2.0f, 0.0f, 0)) * scale(mat4(1.0f), vec3(1.0f, 3.0f, 15.0f));
    mat4 rightWallMatrix = translate(mat4(1.0f), vec3(4.8f*-2.0f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(1.0f, 3.0f, 15.0f));
   



    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &leftWallMatrix[0][0]);
   glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &rightWallMatrix[0][0]);
   glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
    

    mat4 panelMatrix;
    float dimension = 0.25f;
        loop += 0.01f;
        if (loop >= 180.0f) {
            loop = 0;

        }
        if (row == 9)
            row = 0;
        else
            row++;
        float verticalAngle = 0;
        float degree = loop;
        for (int k = 0; k < 2; k++) {
        for (int i = 0; i < 50; i++) {
            degree += 0.5;
            float sinus = sin(degree);
            for (int j = 0; j < 10; j++) {
               
                angle = 0;
                if (sinus >= 0) {
                    if (j <= (sinus + 1) * 5 && j > 4) {

                        angle = -4 * j;
                    }
                }


                else
                    if (j >= (sinus + 1) * 5 && j < 5)
                    {


                        angle = -12 * j-12;
                    }

                
               
                panelMatrix = translate(mat4(1.0f), vec3((0.03f+dimension) * i-7.5f, (0.03f + dimension) * j, -7.5f+15.0f*k)) * translate(mat4(1.0f), vec3(0.0f, dimension/2,0.0f))*  rotate(mat4(1.0f), radians(angle), vec3(1.0f, 0.0f, 0.0f)) * translate(mat4(1.0f), vec3(0.0f, -dimension/2, 0.0f)) * scale(mat4(1.0f), vec3(dimension, dimension, 0.025f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
                }
                //glDrawElements(GL_TRIANGLES, activeVAOVertices[1], GL_UNSIGNED_INT, 0);
            }
        }
      //  glBindTexture(GL_TEXTURE_2D, 0);
       
        glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
        float dx;
        float dz;
        for (int i = 0; i <  50; i++) {
            for (int j = 0; j < 50; j++) {
                dx = 0.203f * i -5.075f;
                dz = 0.203f * j - 5.075f;


                float distance = pow((dx * dx + dz * dz-rayon*rayon), 0.5f);
                if (distance < rayon) {

                   
                        glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(i/15.0f+0.01f,rayon,j/15.0f)));
                        panelMatrix = translate(mat4(1.0f), vec3(dx, 3.0f-distance/2.0f, dz)) * scale(mat4(1.0f), vec3(0.2f, 0.02f, 0.2f));
                    

                   

                }
                
            /*   else {
                   
                    if (movement>0) {
                        glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
                        panelMatrix = translate(mat4(1.0f), vec3(dx, 0.0f, dz)) * scale(mat4(1.0f), vec3(1.0f, 0.02f, 1.0f));
                    }
                    else {
                        glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
                        panelMatrix = translate(mat4(1.0f), vec3(dx, 0.0f, dz)) * scale(mat4(1.0f), vec3(1.0f, 0.02f, 1.0f));
                    }
                }*/

                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


            }
            rayon += 0.0003f * movement;
            if (rayon >= 10.925)
                movement *= -1.0f;
            else
                if (rayon < 0)
                    movement *= -1.0f;
        }
       
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                dx = 1.03f * i + -7.5f;
                dz = 1.03f * j - 7.5f;

                glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f)));
                panelMatrix = translate(mat4(1.0f), vec3(dx, 0.0f, dz)) * scale(mat4(1.0f), vec3(1.0f, 0.02f, 1.0f));
                glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
                glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0


            }

        }
                /* if (rayon > 7.25f)
                     rayon = 0;*/
       // glClear(GL_COLOR_BUFFER_BIT);
        glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0f)));
        glBindTexture(GL_TEXTURE_2D, textureID[2]);

        panelMatrix = translate(mat4(1.0f), vec3(-7.5f, 0.0f, 0.0f)) * scale(mat4(1.0f), vec3(0.1f, 10.0f, 7.5f));
        glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &panelMatrix[0][0]);
       // glDrawArrays(GL_TRIANGLES, 0, 36); // 36 vertices, starting at index 0
        glBindTexture(GL_TEXTURE_2D, 0);

            glBindVertexArray(0);
           
        
        
}
void PanelRoom::setLightColorLocation(GLuint LCP)
{
    lightColorLocation = LCP;
}

void PanelRoom::DefineInputs(GLFWwindow* window) {

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // points
    {
        red += 0.02;
        if (red > 1.0f)
            red = 0.0f;
        lightColor = vec3(red, green, blue);
       
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // points
    {
        green += 0.02;
        if (green > 1.0f)
            green= 0.0f;
        lightColor = vec3(red, green, blue);

    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // points
    {
        blue += 0.02;
        if (blue > 1.0f)
            blue = 0.0f;
        lightColor = vec3(red, green, blue);

    }
   

}
;
