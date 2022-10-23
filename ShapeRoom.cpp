//
//  ShapeRoom.cpp
//  COMP371_Labs
//
//  Created by Kenny on 2020-04-05.
//  Copyright © 2020 Concordia. All rights reserved.
//
#include "Room.h"

vec3 ShapeRoom::GetSplinePosition(float t, const vec3& p1, const vec3& p2, const vec3& p3, const vec3& p4)
{
    // Returns the position of the object on the spline
    // based on parameter t and the four control points p1, p2, p3, p4
    vec4 params(t*t*t, t*t, t, 1);
    mat4 coefficients(vec4(-1, 3, -3, 1), vec4(3, -6, 0, 4), vec4(-3, 3, 3, 1), vec4(1, 0, 0, 0));
    vec4 product = (1.0f / 6.0f) * params * coefficients;
    
    return vec3(vec4(product.x * p1 + product.y * p2 + product.z * p3 + product.w * p4, 1.0f) );
}

vector<vec3> ShapeRoom::convertSplinePoints(vector<vec3> points)
{
    vector<vec3> splinePoints;

    const int numPointsPerSegment = 60;
    float increment = 1.0f / numPointsPerSegment;

    for (int i=0; i < points.size(); ++i)
    {
        float t = 0.0f;
        
        // Set current control points
        vec3 p1 = points[i];
        vec3 p2 = points[(i + 1) % points.size()];
        vec3 p3 = points[(i + 2) % points.size()];
        vec3 p4 = points[(i + 3) % points.size()];
        
        for (int j=0; j < numPointsPerSegment; ++j)
        {
            splinePoints.push_back(GetSplinePosition(t, p1, p2, p3, p4));
            t += increment;
        }
    }
    
    return splinePoints;
}

void ShapeRoom::RenderScene2(GLuint vaos[],GLuint texture[], GLuint activeVAOVertices[])
{
    // rotate models on spline
    
    if (animationKey >= splinePoints_X.size()-1)
    {
        animationKey = 0;
    }
    else {
        animationKey += 1;
    }
   
    vec3 xSpline = splinePoints_X[animationKey];
    vec3 ySpline = splinePoints_Y[animationKey];
    vec3 zSpline = splinePoints_Z[animationKey];
    
    mat4 moveAboutX = translate(mat4(1.0f), xSpline);
    mat4 moveAboutY = translate(mat4(1.0f), ySpline);
    mat4 moveAboutZ = translate(mat4(1.0f), zSpline);
    
    baseMatrixY = baseMatrixY * rotate(mat4(1.0f), radians(rotatingSpeed), vec3(0.0f, 1.0f, 0.0f));
    baseMatrixX = baseMatrixX * rotate(mat4(1.0f), radians(rotatingSpeed), vec3(0.0f, 1.0f, 0.0f));
    baseMatrixZ = baseMatrixZ * rotate(mat4(1.0f), radians(rotatingSpeed), vec3(0.0f, 1.0f, 0.0f));
    
    mat4 polygon1 = splineMode ? moveAboutY : translate(mat4(1.0f), vec3(4.0f, 2.0f, 0.0f)) * baseMatrixY;
    mat4 polygon2 = splineMode ? moveAboutX : translate(mat4(1.0f), vec3(0.0f, 2.0f, 0.0f)) * baseMatrixX;
    mat4 polygon3 = splineMode ? moveAboutZ : translate(mat4(1.0f), vec3(8.0f, 2.0f, 0.0f)) * baseMatrixZ;

    glBindVertexArray(vaos[0]);

    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.5f, 0.5f, 1.0f)));
    polygon_1 = polygon1;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &polygon_1[0][0]);
    glDrawArrays(enumUsed, 0, activeVAOVertices[0]);
    glBindVertexArray(0);
    
    glBindVertexArray(vaos[1]);

    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.5f, 0.2f, 1.0f)));

    polygon_2 = polygon2;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &polygon_2[0][0]);
    glDrawElements(enumUsed, activeVAOVertices[1], GL_UNSIGNED_INT, 0);

    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(0.0f, 1.0f, 0.0f)));

    polygon_3 = polygon3;// * moveAboutZ;
    glUniformMatrix4fv(worldMatrixLocation, 1, GL_FALSE, &polygon_3[0][0]);
    glDrawElements(enumUsed, activeVAOVertices[1], GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

}

void ShapeRoom::RenderSkyBox(GLuint vaos[], GLuint textureID[]){
    glBindVertexArray(vaos[2]);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID[2]);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
}

void ShapeRoom::DefineInputs(GLFWwindow* window) {
    
    /**
        Toggle polygon used
     */
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) // points
    {
        enumUsed = GL_POINTS;
    }
    
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) // lines
    {
        enumUsed = GL_LINES;
    }

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) // triangles
    {
        enumUsed = GL_TRIANGLES;
    }
    
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) // triangles
    {
        splineMode = !splineMode;
    }
    /**
        Toggle rotation speed
        Q: faster
        E: slower
     */
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        if (rotatingSpeed > 1.0f) {
            rotatingSpeed = rotatingSpeed - 2.0f;
        }
    }
    
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        if (rotatingSpeed < 100.0f) {
            rotatingSpeed = rotatingSpeed + 2.0f;
        }
    }
    
}
