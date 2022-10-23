#include "Shader.h"
#define _USE_MATH_DEFINES
#include <algorithm>
#include <iostream>
#include <list>
#include <cmath>
#define GLEW_STATIC 1   // This allows linking with Static Library on Windows, without DLL
#include <GL/glew.h>    // Include GLEW - OpenGL Extension Wrangler
#include <GLFW/glfw3.h> // cross-platform interface for creating a graphical context,
// initializing OpenGL and binding inputs
#include <glm/glm.hpp>  // GLM is an optimized math library with syntax to similar to OpenGL Shading Language
#include <glm/gtc/matrix_transform.hpp> // include this to create transformation matrices
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "OBJloaderV2.h"

using namespace glm;
using namespace std;


const char* Shader::getVertexShader() {
    // For now, you use a string for your shader code, in the assignment, shaders will be stored in .glsl files
    return
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;"
        "layout (location = 1) in vec3 normal;"
        "layout (location = 2) in vec2 aTexCoords;"
        ""
        "uniform mat4 worldMatrix = mat4(1.0);"// model
        "uniform mat4 viewMatrix = mat4(1.0);"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"
        ""
        ""
        "out VS_OUT{"
        "vec3 normal;"  // Normal
        "vec3 FragPos;"
        "vec2 TexCoords;"
        ""
        "} vs_out;"
        ""
        ""
        "void main()"
        "{"
        " vs_out.FragPos = vec3(worldMatrix * vec4(aPos,1.0));"
        " vec4 P= viewMatrix * worldMatrix * vec4(aPos,1.0f);"
        " vs_out.normal = transpose(inverse(mat3(worldMatrix)))*normal;"
        " vs_out.TexCoords = aTexCoords;"
        " gl_Position = projectionMatrix * P;"
        "}";

};

const char* Shader::getFragmentShader()
{
    return
        "#version 330 core\n"
        "in VS_OUT"
        "{"
        "vec3 normal;"  // Normal
        "vec3 FragPos;"
        "vec2 TexCoords;"
        "}fs_in;"
        ""
        "uniform sampler2D diffuseTexture;"
        "uniform samplerCube depthMap;"

        "uniform vec3 lightColor=vec3(0.7f,0.8f,0.7f);"
        "uniform vec3 viewPos;"
        "uniform vec3 lightPos;"
        "uniform float far_plane;"
        "uniform vec3 objectColor=vec3(0.7f,0.7f,0.7f);"// ATTRIBUTE TO CHANGE THE COLOR
        "out vec4 FragColor;"
        "vec4 color;"
        "vec3 ambient;"
        "float ShadowCalculation(vec3 fragPos)"
        "{"
        // get vector between fragment position and light position
        "vec3 fragToLight = fragPos - lightPos;"
        // use the light to fragment vector to sample from the depth map    
        "float closestDepth = texture(depthMap, fragToLight).r;"
        // it is currently in linear range between [0,1]. Re-transform back to original value
        "closestDepth *= far_plane;"
        // now get current linear depth as the length between the fragment and light position
        "float currentDepth = length(fragToLight);"
        // now test for shadows
        "float bias = 0.05;"
        "float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;"
        ""
        "return shadow;"
        "}"
        ""
        ""
        "void main()"
        "{"
        "if(objectColor == vec3(0.0f) ) {"
        "color = texture(diffuseTexture, fs_in.TexCoords);"
        "ambient = vec3(1.0f);}"
        "else {"
        "color = vec4(objectColor,1.0f); "
        "ambient = 0.8 * objectColor;}"
        "vec3 normal= normalize(fs_in.normal);"
        
        "vec3 lightDir = normalize(lightPos - fs_in.FragPos);"
        "float diff = max(dot(lightDir, normal), 0.0);"
        "vec3 diffuse = diff * lightColor;	"// specular
        "vec3 viewDir = normalize(viewPos - fs_in.FragPos);"
        "vec3 reflectDir = reflect(-lightDir, normal);"
        "float spec = 100.0;"
      
        "vec3 halfwayDir = normalize(lightDir + viewDir);"
        "spec = pow(max(dot(normal, halfwayDir), 0.0), 16.0);"
        "vec3 specular = spec * lightColor;"
        // calculate shadow
        "float shadow = ShadowCalculation(fs_in.FragPos);"
        "vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular));"
        ""
        "FragColor = vec4(lighting,1.0f) * color ;"

        ""

        ""
        "}";
}
const char* Shader::getVertexShaderShadow() {
    return
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;"
        "uniform mat4 model;"
        "void main()"
        "{"
        " "
        " gl_Position = model * vec4(aPos,1.0f);"
        "}";
}

const char* Shader:: getFragmentShaderShadow() {
    return
        "#version 330 core\n"
        "in vec4 FragPos;"
        "uniform vec3 lightPos;"
        "uniform float far_plane;"
        "void main()"
        "{"
        "float lightDistance = length(FragPos.xyz- lightPos);"
        "lightDistance = lightDistance/ far_plane;"
        "gl_FragDepth = lightDistance;"
        "}";
}
const char* Shader::getGeometryShaderShadow() {
    return
        "#version 330 core\n"
        "layout(triangles) in;"
        "layout(triangle_strip, max_vertices = 18) out;"
        "uniform mat4 shadowMatrices[6];"
        "out vec4 FragPos; "// FragPos from GS (output per emitvertex)
        "void main()"
        "{"
        "for (int face = 0; face < 6; ++face)"
        "{"
        "gl_Layer = face;" // built-in variable that specifies to which face we render.
        "for (int i = 0; i < 3; ++i) "// for each triangle's vertices
        "{"
        "FragPos = gl_in[i].gl_Position;"
        "gl_Position = shadowMatrices[face] * FragPos;"
        "EmitVertex();"
        "}"
        "EndPrimitive();"
        "}"
        "}";
}

const char* Shader::getVertexShaderSky()
{
    return
        "#version 330 core\n"
        "layout(location = 0) in vec3 aPos;"
       
        "out vec3 TexCoords;"
       // "uniform mat4 worldMatrix;"
        "uniform mat4 projection;"
        "uniform mat4 view;"
       // "uniform mat4 scale=mat4(0.0001f);"

        "void main()"
        "{"
        "TexCoords = aPos;"
        "vec4 pos = projection * view * vec4(aPos, 1.0);"
        "gl_Position = pos.xyww;"
        "}";
}
const char* Shader::getFragmentShaderSky()
{
    return 
        "#version 330 core\n"
        "out vec4 FragColor;"

        "in vec3 TexCoords;"

        "uniform samplerCube skybox;"

        "void main()"
        "{"
        "FragColor = texture(skybox, TexCoords);"
        "}";
}
const char* Shader::getVertexShaderParticles() {
    return
        "#version 330 core\n"
        "layout (location = 0) in vec4 vertex;"
      
        "uniform vec2 offset;"
        "uniform vec4 color;"  // default value for view matrix (identity)
        "uniform mat4 projectionMatrix = mat4(1.0);"

        "out vec2 TexCoords;"
        "out vec4 ParticleColor;"

        "void main()"
        "{"
        "float scale =1.0f;"
        "TexCoords = vertex.zw;"
        "ParticleColor = color;"
        

        " gl_Position = projectionMatrix * vec4((vertex.xy * scale) + offset, 0.0f,1.0f);"
        "}";

};

const char* Shader::getFragmentShaderParticles() {
    return
        "#version 330 core\n"
        "in vec2 TexCoords;"
        "in vec4 ParticleColor;"
        "out vec4 color;"

        "uniform sampler2D sprite;"// ATTRIBUTE TO CHANGE THE COLOR

        "void main()"
        "{"
        
        "color = (texture( sprite, TexCoords) * ParticleColor);"

        "}";


};

const char* Shader::getVertexShaderText()
{
    return
   " #version 330 core\n"
   " layout(location = 0) in vec4 vertex; "// <vec2 pos, vec2 tex>
    "out vec2 TexCoords;"

   " uniform mat4 projection;"

   " void main()"
   " {"
       " gl_Position = projection * vec4(vertex.xy, 0.0, 1.0);"
        "TexCoords = vertex.zw;"
    "}";
}

const char* Shader::getFragmentShaderText()
{
    return 
    "#version 330 core\n"
    "in vec2 TexCoords;"
    "out vec4 color;"

   " uniform sampler2D text;"
    "uniform vec3 textColor;"

    "void main()"
   " {"
       " vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"
       " color = vec4(textColor, 1.0) * sampled;"
   " }";
}

int Shader:: compileAndLinkShaders(const char* vertex, const char* fragment, const char* geometry)
{
    // compile and link shader program
    // return shader program id
    // ------------------------------------

    // vertex shader
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderSource = vertex;
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderSource = fragment;
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //geometry Shader
    unsigned int geometryShader;
    if (geometry != nullptr)
    {

        geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
        const char* geometryShaderSource = geometry;
        glShaderSource(geometryShader, 1, &geometryShaderSource, NULL);
        glCompileShader(geometryShader);

        //check for shader compile errors
        glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(geometryShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::GEOMETRY::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
    // shader Program
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    if (geometry != nullptr)
        glAttachShader(shaderProgram, geometryShader);


    // link shaders
    glLinkProgram(shaderProgram);

    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // delete the shaders as they're linked into our program now and no longer necessery
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    if (geometry != nullptr)
        glDeleteShader(geometryShader);

    return shaderProgram;
}

void Shader::setShaderProgram(int shaderTextureProgram, vec3 lightPosition, vec3 cameraPosition,mat4 viewMatrix)
{
    glUseProgram(shaderTextureProgram);
    GLuint colorLocation = glGetUniformLocation(shaderTextureProgram, "objectColor");
    //GLuint shadowLocation = glGetUniformLocation(shaderTextureProgram, "shadows");
    GLuint far_planeLocation = glGetUniformLocation(shaderTextureProgram, "far_plane");
    GLuint lightPosLocation = glGetUniformLocation(shaderTextureProgram, "lightPos");
    GLuint viewMatrixLocation = glGetUniformLocation(shaderTextureProgram, "viewMatrix");
    GLuint  viewPosLocation = glGetUniformLocation(shaderTextureProgram, "viewPos");
    GLuint  projectionMatrixLocation = glGetUniformLocation(shaderTextureProgram, "projectionMatrix");
    GLuint depthMapLocation = glGetUniformLocation(shaderTextureProgram, "depthMap");
    glUniform1i(depthMapLocation, 1);
    // Set Light Pos
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]);

    glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPosition));
    //Set far_plane
    glUniform1f(far_planeLocation, far_plane);
    glUniform3fv(viewPosLocation, 1, glm::value_ptr(cameraPosition));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);

    glActiveTexture(GL_TEXTURE0);
    GLuint textureLocation = glGetUniformLocation(shaderTextureProgram, "diffuseTexture");
    glBindTexture(GL_TEXTURE, 0);
    glUniform3fv(colorLocation, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
   
 
}
;

void Shader::createDepthCubeMap(int SHADOW_WIDTH, int SHADOW_HEIGHT)
{
   // unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    //create depth cubemap texture
    
    glGenTextures(1, &depthCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubeMap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubeMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

void Shader::setShadow(int shaderShadowProgram,vec3 lightPos, int SHADOW_WIDTH,int SHADOW_HEIGHT)
{
    float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
    float near_plane = 1.0f;
    far_plane = 50.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near_plane, far_plane);

    std::vector<glm::mat4> shadowTransforms;
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
    shadowTransforms.push_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));


    // 1. first render to depth map
    //--------------------------------
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    glUseProgram(shaderShadowProgram);


    // We can set the shader once, since we have only one


    GLuint far_planeLocation = glGetUniformLocation(shaderShadowProgram, "far_plane");
    GLuint lightPosLocation = glGetUniformLocation(shaderShadowProgram, "lightPos");
    GLuint shadowMatricesLocation0 = glGetUniformLocation(shaderShadowProgram, "shadowMatrices[0]");
    GLuint shadowMatricesLocation1 = glGetUniformLocation(shaderShadowProgram, "shadowMatrices[1]");
    GLuint shadowMatricesLocation2 = glGetUniformLocation(shaderShadowProgram, "shadowMatrices[2]");
    GLuint shadowMatricesLocation3 = glGetUniformLocation(shaderShadowProgram, "shadowMatrices[3]");
    GLuint shadowMatricesLocation4 = glGetUniformLocation(shaderShadowProgram, "shadowMatrices[4]");
    GLuint shadowMatricesLocation5 = glGetUniformLocation(shaderShadowProgram, "shadowMatrices[5]");
    glEnableVertexAttribArray(far_planeLocation);
    glEnableVertexAttribArray(lightPosLocation);
    glEnableVertexAttribArray(shadowMatricesLocation0);
    glEnableVertexAttribArray(shadowMatricesLocation1);
    glEnableVertexAttribArray(shadowMatricesLocation2);
    glEnableVertexAttribArray(shadowMatricesLocation3);
    glEnableVertexAttribArray(shadowMatricesLocation4);
    glEnableVertexAttribArray(shadowMatricesLocation5);


    glUniformMatrix4fv(shadowMatricesLocation0, 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
    glUniformMatrix4fv(shadowMatricesLocation1, 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
    glUniformMatrix4fv(shadowMatricesLocation2, 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
    glUniformMatrix4fv(shadowMatricesLocation3, 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
    glUniformMatrix4fv(shadowMatricesLocation4, 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
    glUniformMatrix4fv(shadowMatricesLocation5, 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
    glUniform1f(far_planeLocation, far_plane);
    glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPos));


}

int Shader::createVertexArraySkyBox()
{

    float skyboxVertices[] = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

 
    // skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    return skyboxVAO;
}

int Shader::createVertexArrayObject()
{
    // Cube model
    float vertexArray[] = {  // position,                            color
        -0.5f, 0.0f,-0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,//left - normal
        -0.5f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, 1.0f, 0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,

        -0.5f, 0.0f,-0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 0.0f,
        -0.5f, 1.0f, 0.5f, -1.0f, 0.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, 1.0f,-0.5f, -1.0f, 0.0f, 0.0f,  0.0f, 1.0f,

         0.5f, 1.0f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f,// far - blue
        -0.5f, 0.0f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,
        -0.5f, 1.0f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f, 1.0f,

         0.5f, 1.0f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f, 1.0f,
         0.5f, 0.0f,-0.5f,  0.0f, 0.0f,-1.0f,  0.0f, 0.0f,
        -0.5f, 0.0f,-0.5f,  0.0f, 0.0f,-1.0f,  1.0f, 0.0f,

         0.5f, 0.0f, 0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 1.0f,// bottom - turquoise
        -0.5f, 0.0f,-0.5f,  0.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         0.5f, 0.0f,-0.5f,  0.0f, -1.0f, 0.0f,  1.0f, 0.0f,

         0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 1.0f,
        -0.5f, 0.0f,-0.5f, 0.0f, -1.0f, 0.0f,  0.0f, 0.0f,

        -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,// near - green
        -0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

         0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f, 1.0f, 0.5f, 0.0f, 0.0f, 1.0f,   0.0f, 1.0f,
         0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f,

         0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f,   0.0f, 1.0f,// right - purple
         0.5f, 0.0f,-0.5f, 1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.5f, 1.0f,-0.5f, 1.0f, 0.0f, 0.0f,   1.0f, 1.0f,

         0.5f, 0.0f,-0.5f, 1.0f, 0.0f, 0.0f,   1.0f, 0.0f,
         0.5f, 1.0f, 0.5f, 1.0f, 0.0f, 0.0f,   0.0f, 1.0f,
         0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,   0.0f, 0.0f,

         0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,// top - yellow
         0.5f, 1.0f,-0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,
        -0.5f, 1.0f,-0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,

         0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 1.0f,
        -0.5f, 1.0f,-0.5f, 0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
        -0.5f, 1.0f, 0.5f, 0.0f, 1.0f, 0.0f,  1.0f, 0.0f,


        -50.0f, 0.0f,-50.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, //Grid
        -50.0f, 0.0f, 50.0f, 1.0f, 1.0f, 0.0f, 1.0f,1.0f,

        0.0f, 5.02f,0.0f, 0.0f, 1.0f, 0.0f, 1.0f,1.0f,// Y Axis in green
        0.0f, 0.02f, 0.0f, 0.0f, 1.0f, 0.0f,1.0f,1.0f,

        5.0f, 0.2f,0.0f, 0.0f, 0.0f, 1.0f, 1.0f,1.0f,// x Axis in Blue
        0.0f, 0.2f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f,1.0f,

        0.0f, 0.2f,-5.0f, 0.0f, 0.0f, 1.0f,1.0f,1.0f, // z Axis in Red
        0.0f, 0.15f, 0.0f, 0.0f, 0.0f, 1.0f,1.0f,1.0f,


    };

    // Create a vertex array
    GLuint vertexArrayObject;
    glGenVertexArrays(1, &vertexArrayObject);
    glBindVertexArray(vertexArrayObject);



    // Upload Vertex Buffer to the GPU, keep a reference to it (vertexBufferObject)
    GLuint vertexBufferObject;
    glGenBuffers(1, &vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glVertexAttribPointer(0,                   // attribute 0 matches aPos in Vertex Shader
        3,                   // size
        GL_FLOAT,            // type
        GL_FALSE,            // normalized?
        8 * sizeof(float), // stride - each vertex contain 2 vec3 (position, color)
        (void*)0             // array buffer offset
    );
    glEnableVertexAttribArray(0);


    glVertexAttribPointer(1,                            // attribute 1 matches aColor in Vertex Shader
        3,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(3 * sizeof(float))     // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2,                            // attribute 1 matches aColor in Vertex Shader
        2,
        GL_FLOAT,
        GL_FALSE,
        8 * sizeof(float),
        (void*)(6 * sizeof(float))     // color is offseted a vec3 (comes after position)
    );
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
    glBindVertexArray(0);

    return vertexArrayObject;
};

GLuint Shader::setupModelEBO(string path, int& vertexCount)
{
    vector<int> vertexIndices; //The contiguous sets of three indices of vertices, normals and UVs, used to make a triangle
    vector<glm::vec3> vertices;
    vector<glm::vec3> normals;
    vector<glm::vec2> UVs;

    //read the vertices from the cube.obj file
    //We won't be needing the normals or UVs for this program
    loadOBJ2(path.c_str(), vertexIndices, vertices, normals, UVs);

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO); //Becomes active VAO
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).

    //Vertex VBO setup
    GLuint vertices_VBO;
    glGenBuffers(1, &vertices_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //Normals VBO setup
    GLuint normals_VBO;
    glGenBuffers(1, &normals_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, normals_VBO);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(1);

    //UVs VBO setup
    GLuint uvs_VBO;
    glGenBuffers(1, &uvs_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, uvs_VBO);
    glBufferData(GL_ARRAY_BUFFER, UVs.size() * sizeof(glm::vec2), &UVs.front(), GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(2);

    //EBO setup
    GLuint EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexIndices.size() * sizeof(int), &vertexIndices.front(), GL_STATIC_DRAW);

    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
    vertexCount = vertexIndices.size();
    return VAO;
};

bool Shader::initContext() {     // Initialize GLFW and OpenGL version
    glfwInit();

#if defined(PLATFORM_OSX)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    // On windows, we set OpenGL version to 2.1, to support more hardware
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
#endif

    // Create Window and rendering context using GLFW, resolution is 800x600
    window = glfwCreateWindow(2048, 768*2, "Comp371 - Lab 03", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window);

    // @TODO 3 - Disable mouse cursor
    // ...

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to create GLEW" << std::endl;
        glfwTerminate();
        return false;
    }
    return true;
}
unsigned int Shader::getDepthCubeMap()
{
    return depthCubeMap;
}
float Shader::getFar_plane()
{
    return far_plane;
}
;


