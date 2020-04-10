#include "include/gl_framework.hpp"
#include "include/shader_util.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include "include/camera.h"

//void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

unsigned int shaderProgram;
unsigned int vbo, vao, EBO;	

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

unsigned nrOfVertices = sizeof(vertices) / sizeof(Vertex);

void texture()
{

 unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;

    unsigned char *data = stbi_load("Texture/smile.jpg", &width, &height, &nrChannels, 0);
   
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
}

unsigned int initShadersGL(void)
{
  std::string vertex_shader_file("Shader/simple_vs.glsl");
  std::string fragment_shader_file("Shader/simple_fs.glsl");

  std::vector<unsigned int> shaderList;
  shaderList.push_back(opengl::LoadShaderGL(GL_VERTEX_SHADER, vertex_shader_file));
  shaderList.push_back(opengl::LoadShaderGL(GL_FRAGMENT_SHADER, fragment_shader_file));

  return opengl::CreateProgramGL(shaderList);
}

void initVertexBufferGL(void)
{

  glGenVertexArrays (1, &vao);
  glBindVertexArray (vao);
  
  glGenBuffers (1, &vbo);
  glBindBuffer (GL_ARRAY_BUFFER, vbo);
  glBufferData (GL_ARRAY_BUFFER, sizeof (vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray (0);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, color));
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
  glEnableVertexAttribArray(2);

  glBindVertexArray(0);
}

void renderGL(void)
{
  glClearColor( 0.1f, 0.1f, 0.1f, 1.0f );
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  const float radius = 20.0f;
  float camX = sin(glfwGetTime()) * radius;
  float camZ = cos(glfwGetTime()) * radius;

  glm::mat4 view;
  view = camera.GetViewMatrix();
  glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

  glUseProgram(shaderProgram);
  
  unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
  unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");

  glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

  glBindVertexArray (vao);

  for(unsigned int i = 0; i < 10; i++)
  {
  glm::mat4 transform = glm::mat4(1.0f);
  transform = glm::translate(transform, cubePositions[i]);
  transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 1.0f, 1.0f));
  
  glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
  glDrawArrays(GL_TRIANGLES, 0, 36);
  }

}


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

  window = glfwCreateWindow(SCR_WIDTH , SCR_HEIGHT , "Cubes!", NULL, NULL);
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

  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (GLEW_OK != err)
    {
      std::cerr<<"GLEW Init Failed : %s"<<std::endl;
    }
  glfwSetKeyCallback(window, opengl::key_callback);
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  opengl::initGL();
  shaderProgram = initShadersGL();
  initVertexBufferGL();
  texture();

  while (glfwWindowShouldClose(window) == 0)
    {
      float currentFrame = glfwGetTime();
	  deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;  

      processInput(window);
      glfwSetScrollCallback(window, scroll_callback); 
      
      renderGL();

      glfwSwapBuffers(window);
      
      glfwPollEvents();
    }

  glDeleteVertexArrays(1, &vbo);
  glDeleteBuffers(1, &vao);
  
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

// glfw: whenever the mouse moves, this callback is called
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}