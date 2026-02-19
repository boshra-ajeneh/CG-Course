// Project_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>
// Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
// The MAIN function, from here we start the application and run the game loop



const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n" 
"out vec3 ourColor;\n" 
"uniform float posy;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x,aPos.y+posy,aPos.z, 1.0);\n"
"   ourColor = aColor;\n" 
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n" 
"uniform float uAlpha;\n" 
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, uAlpha);\n" 
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
float posy = 0.0f;
bool flagy = false;
bool flagb = false;
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        flagy = true;
    else flagy = false;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
        flagb = true;
    else flagb = false;
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        posy = 0.2f;


    else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        posy = -0.2f;
    else posy = 0.0f;
}


int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
#endif

   
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Lab 4: Advanced Control", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window); 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }


    glEnable(GL_DEPTH_TEST); 
    glEnable(GL_BLEND);      
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
    float vertices[] = {
        
        -0.5f, -0.5f, 0.0f,  0.929f, 0.929f, 0.035f,
         0.5f, -0.5f, 0.0f,  0.929f, 0.929f, 0.035f,
         0.0f,  0.5f, 0.0f,  0.929f, 0.929f, 0.035f,

        
         -0.2f, -0.2f, 0.75f,  0.176f, 0.773f, 0.969f,
          0.8f, -0.2f, 0.75f,  0.176f, 0.773f, 0.969f,
          0.3f,  0.8f, 0.75f,  0.176f, 0.773f, 0.969f
    };


    
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO); 
    glGenBuffers(1, &VBO);

    
    glBindVertexArray(VAO);

    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0); 



    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    
    while (!glfwWindowShouldClose(window))
    {
       
        processInput(window);

        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        
        glUseProgram(shaderProgram); 
        glBindVertexArray(VAO); 
        

        int alphaLoc = glGetUniformLocation(shaderProgram, "uAlpha");
        int yloc=glGetUniformLocation(shaderProgram, "posy");

        if (flagb) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); 
        }
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform1f(yloc, posy);
        glUniform1f(alphaLoc, 1.0f);
        glDrawArrays(GL_TRIANGLES, 3, 3);



        if (flagy) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glUniform1f(yloc, posy);
        glUniform1f(alphaLoc, 0.5f);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
	return EXIT_SUCCESS;
}

    


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
