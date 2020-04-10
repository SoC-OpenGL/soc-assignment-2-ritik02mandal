#include "GL_Framework/gl_framework.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "Texture/stb_image.h"
#include "camera.h"
#include "Shader/shader.h"



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

Vertex vertices[] =
{
  //Position                    //Color                   //Texcoords
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 0.0f),
    glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 1.0f),
    glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 1.0f),
    glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 0.0f),
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 0.0f),
    glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 1.0f),
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 1.0f),
    glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 0.0f),
    glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 1.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 0.0f),
    glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 1.0f),
    glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 0.0f),
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 1.0f),
    glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 0.0f),
    glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f),
    glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 1.0f),
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(1.0f, 0.0f),
    glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 0.0f),
    glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(1.f,0.f,0.f),  glm::vec2(0.0f, 1.0f)
};

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

unsigned int indices[36];

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);
unsigned nrOfIndices = sizeof(indices) / sizeof(unsigned int);

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

  window = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT , "Texture Ring", NULL, NULL);
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

  std::cout<<"Vendor: "<<glGetString (GL_VENDOR)<<std::endl;
  std::cout<<"Renderer: "<<glGetString (GL_RENDERER)<<std::endl;
  std::cout<<"Version: "<<glGetString (GL_VERSION)<<std::endl;
  std::cout<<"GLSL Version: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<std::endl;

  opengl::initGL();

  for(int i=0;i<36;i++)
  {
  indices[i]=i;
  }

  Shader ourShader("Shader/simple_vs.glsl","Shader/simple_fs.glsl");
  Shader skyShader("Shader/sky_vs.glsl","Shader/sky_fs.glsl");

  unsigned int vbo, vao, ebo; 

  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);
  
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

  glGenBuffers(1, &ebo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray (0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
  glEnableVertexAttribArray(2);

  //GLCall(glBindVertexArray(0));

  unsigned int skyVAO, skyVBO;
  glGenVertexArrays(1, &skyVAO);
  glGenBuffers(1, &skyVBO);
  glBindVertexArray(skyVAO);
  glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), &skyVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

  unsigned int texture= loadTexture("Texture/smile.jpg");

  std::vector<std::string> faces=
  {
    "skybox/right.jpg",
    "skybox/left.jpg",
    "skybox/top.jpg",
    "skybox/bottom.jpg",
    "skybox/front.jpg",
    "skybox/back.jpg"
  };
  unsigned int cubemapTexture = loadCubemap(faces);  

  skyShader.use();
  skyShader.setInt("skybox", 0);

  ourShader.use();
  ourShader.setInt("texture", 0);

  while (glfwWindowShouldClose(window) == 0)
    {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;  

      processInput(window);
      glfwSetScrollCallback(window, scroll_callback); 
      glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
  	  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


      GLCall(ourShader.use());

      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, texture);
    
      glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
      GLCall(ourShader.setMat4("projection", projection));    
      glm::mat4 view;
      view = camera.GetViewMatrix();
      GLCall(ourShader.setMat4("view", view));

      GLCall(glBindVertexArray (vao));

      for(unsigned int i = 0; i < 10; i++)
      {
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, cubePositions[i]);
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
        
        ourShader.setMat4("transform", transform);
        
        glDrawArrays(GL_TRIANGLES, 0, 36);
      }

      glBindVertexArray(0);

      glDepthFunc(GL_LEQUAL);

      skyShader.use();

      view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
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