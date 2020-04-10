#include "include/gl_framework.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/camera.h"
#include "include/shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

unsigned int loadTexture(const char *path);
unsigned int loadCubemap(std::vector<std::string> faces);

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

Vertex vertices[] =
{
  //Position                    //Color                   //Texcoords
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f),                                   
    glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f),                                    
    glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f, -1.0f),                                    
    glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f, -1.0f),                                    
    glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  0.0f, -1.0f),                                    
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f),                                    
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f),                                   
    glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f),                                   
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f,  1.0f),                                   
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  0.0f,  1.0f),                                   
    glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  0.0f,  1.0f),                                   
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f),                                   
    glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),                                   
    glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f),                                   
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f),                                   
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3(-1.0f,  0.0f,  0.0f),                                   
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),                                   
    glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f),                                   
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f),                                   
    glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 1.0f,  0.0f,  0.0f),                                   
    glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 1.0f,  0.0f,  0.0f),                                   
    glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 1.0f,  0.0f,  0.0f),                                   
    glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f),                                   
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f),                                   
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, -1.0f,  0.0f),                                   
    glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f, -1.0f,  0.0f),                                   
    glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f),                                   
    glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f),                                   
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f),                                   
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f, -1.0f,  0.0f),                                   
    glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  1.0f,  0.0f),                                   
    glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec2(1.0f, 1.0f), glm::vec3( 0.0f,  1.0f,  0.0f),                                   
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f),                                   
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec2(1.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f),                                   
    glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f),                                   
    glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, 1.0f), glm::vec3( 0.0f,  1.0f,  0.0f)                                   
};

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
};

glm::vec3 pointLightPositions[] = {
    glm::vec3( 0.7f,  0.2f,  2.0f),
    glm::vec3( 2.3f, -3.3f, -4.0f),
    glm::vec3(-4.0f,  2.0f, -12.0f),
    glm::vec3( 0.0f,  0.0f, -3.0f)
};


float skyVertices[] = {
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

int main(int argc, char** argv)
{
  GLFWwindow* window;

  glfwSetErrorCallback(opengl::error_callback);

  if (!glfwInit())
    return -1;

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); 
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

  window = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT , "Lights", NULL, NULL);
  if (!window)
    {
      glfwTerminate();
      return -1;
    }
  
  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, opengl::framebuffer_size_callback);
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwSetKeyCallback(window, opengl::key_callback);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }

  opengl::initGL();

  Shader ourShader("Shader/simple_vs.glsl","Shader/simple_fs.glsl");
  Shader skyShader("Shader/sky_vs.glsl","Shader/sky_fs.glsl");
  Shader lampShader("Shader/light_vs.glsl","Shader/light_fs.glsl");

  unsigned int vbo, vao; 

  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);
  
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
  glEnableVertexAttribArray(2);
  
  unsigned int skyVAO, skyVBO;
  glGenVertexArrays(1, &skyVAO);
  glGenBuffers(1, &skyVBO);
  glBindVertexArray(skyVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), &skyVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  unsigned int lightVAO;
  glGenVertexArrays(1, &lightVAO);
  glBindVertexArray(lightVAO);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

  unsigned int diffuseMap= loadTexture("Texture/container2.jpg");
  unsigned int specularMap = loadTexture("Texture/container2_specular.jpg");

  std::vector<std::string> faces=
  {
    "Texture/skybox/right.jpg",
    "Texture/skybox/left.jpg",
    "Texture/skybox/top.jpg",
    "Texture/skybox/bottom.jpg",
    "Texture/skybox/front.jpg",
    "Texture/skybox/back.jpg"
  };
  unsigned int cubemapTexture = loadCubemap(faces);  

  skyShader.use();
  skyShader.setInt("skybox", 0);

  ourShader.use();
  ourShader.setInt("material.diffuse", 0);
  ourShader.setInt("material.specular", 1);
  
  while (glfwWindowShouldClose(window) == 0)
    {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;  

      processInput(window);
      glfwSetScrollCallback(window, scroll_callback); 
      glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
  	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      ourShader.use();
      ourShader.setVec3("viewPos", camera.Position);
      ourShader.setFloat("material.shininess", 32.0f);
      // directional light
      ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
      ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
      ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
      ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
      // point light 1
      ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);
      ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
      ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
      ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
      ourShader.setFloat("pointLights[0].constant", 1.0f);
      ourShader.setFloat("pointLights[0].linear", 0.09);
      ourShader.setFloat("pointLights[0].quadratic", 0.032);
      // point light 2
      ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
      ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
      ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
      ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
      ourShader.setFloat("pointLights[1].constant", 1.0f);
      ourShader.setFloat("pointLights[1].linear", 0.09);
      ourShader.setFloat("pointLights[1].quadratic", 0.032);
      // point light 3
      ourShader.setVec3("pointLights[2].position", pointLightPositions[2]);
      ourShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
      ourShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
      ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
      ourShader.setFloat("pointLights[2].constant", 1.0f);
      ourShader.setFloat("pointLights[2].linear", 0.09);
      ourShader.setFloat("pointLights[2].quadratic", 0.032);
      // point light 4
      ourShader.setVec3("pointLights[3].position", pointLightPositions[3]);
      ourShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
      ourShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
      ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
      ourShader.setFloat("pointLights[3].constant", 1.0f);
      ourShader.setFloat("pointLights[3].linear", 0.09);
      ourShader.setFloat("pointLights[3].quadratic", 0.032);
      // spotLight
      ourShader.setVec3("spotLight.position", camera.Position);
      ourShader.setVec3("spotLight.direction", camera.Front);
      ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
      ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
      ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
      ourShader.setFloat("spotLight.constant", 1.0f);
      ourShader.setFloat("spotLight.linear", 0.09);
      ourShader.setFloat("spotLight.quadratic", 0.032);
      ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
      ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));     
      // view/projection transformations
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      glm::mat4 view = camera.GetViewMatrix();
      ourShader.setMat4("projection", projection);
      ourShader.setMat4("view", view);
      // world transformation
      glm::mat4 model = glm::mat4(1.0f);
      ourShader.setMat4("model", model);
      // bind diffuse map
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, diffuseMap);
      // bind specular map
      glActiveTexture(GL_TEXTURE1);
      glBindTexture(GL_TEXTURE_2D, specularMap);
      // render containers
      glBindVertexArray(vao);

      for (unsigned int i = 0; i < 10; i++)
      {
          glm::mat4 model = glm::mat4(1.0f);
          model = glm::translate(model, cubePositions[i]);
          float angle = 20.0f * i;
          model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
          ourShader.setMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);
      }
    
      lampShader.use();
      lampShader.setMat4("projection", projection);
      lampShader.setMat4("view", view);
 
      glBindVertexArray(lightVAO);
      for (unsigned int i = 0; i < 4; i++)
      {
          model = glm::mat4(1.0f);
          model = glm::translate(model, pointLightPositions[i]);
          model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
          lampShader.setMat4("model", model);
          glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glDepthFunc(GL_LEQUAL);
      skyShader.use();
      view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
      skyShader.setMat4("view", view);
      skyShader.setMat4("projection", projection);
      glBindVertexArray(skyVAO);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
      glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      glDepthFunc(GL_LESS); // set depth function back to default
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glDeleteVertexArrays(1, &vbo);
  glDeleteBuffers(1, &vao);
  glDeleteVertexArrays(1, &skyVAO);
  glDeleteBuffers(1, &skyVAO);

  glfwTerminate();
  return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}

unsigned int loadTexture(char const * path)
{
    
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_2D, textureID);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  int width, height, nrChannels;

  unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
   
  if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
  else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
  stbi_image_free(data); 

    return textureID;
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
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