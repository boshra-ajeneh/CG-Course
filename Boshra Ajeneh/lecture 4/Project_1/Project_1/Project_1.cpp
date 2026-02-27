#include <iostream>
#include <cmath>

#define GLEW_STATIC 
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 Pos;\n" 
"uniform vec2 move;\n"
"uniform float eyescale;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(Pos.x*eyescale+move.x, Pos.y*eyescale+move.y, Pos.z, 1.0);\n"
"}\0";



const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec3 color;\n"
"void main()\n"
"{\n"
"FragColor = vec4(color,1.0f);\n"
"}\n\0";


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}



float xx = 0.0f;
float up = 0.0f;

// Handleing input
bool leftPrev = false; bool rightPrev = false, upPrev = false;

void processInput(GLFWwindow* window)
{   
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float step = 0.03f; 
    
    int leftNow = glfwGetKey(window, GLFW_KEY_LEFT); 
    if (leftNow == GLFW_PRESS && !leftPrev) 
    
        xx -= step;
    leftPrev = (leftNow == GLFW_PRESS);


    int rightNow = glfwGetKey(window, GLFW_KEY_RIGHT); 
    if (rightNow == GLFW_PRESS && !rightPrev) 
        xx += step; 
    rightPrev = (rightNow == GLFW_PRESS);


    int upNow = glfwGetKey(window, GLFW_KEY_UP);
    if (upNow == GLFW_PRESS && !upPrev)
        up = 0.3;
    else
        up = std::max(0.0, up - 0.001);
    upPrev = (upNow == GLFW_PRESS);


}

float genpoint() {
    
    return -0.9f + (rand() / (float)RAND_MAX) * 1.8f;
}


float pointX = 0.0f;
float pointY = -0.64f;
bool isColliding() {
    float charx = 0.71f + xx;
    float chary = -0.57f + up;
    return (fabs(charx - pointX) < 0.06f 
        && fabs(chary - pointY)<0.07);
}



int main()
{
    srand(time(NULL));
    
       
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 


    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL - First Triangle", NULL, NULL);
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

    

    //  Vertex Shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // فحص الأخطاء
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //  Fragment Shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // فحص الأخطاء
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //  (Shader Program)
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // فحص أخطاء الربط
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    
    float vertices[] = {
      -1.0f, -0.65f, 0.0f,
       1.0f, -0.65f, 0.0f,
       1.0f, -1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f,
         
    };

    float chvertices[] = {
       0.75f, -0.64f, 0.0f,
       0.67f, -0.64f, 0.0f,
       0.75f, -0.50f, 0.0f,
       0.67f, -0.50f, 0.0f,

       0.69f, -0.55f, 0.0f,
       0.73f, -0.55f, 0.0f,
    
    };

    float pointvertex[]{
        0.0f, 0.0f, 0.0f
     };


     unsigned int  indices[] = {
        0, 1, 3,
        1, 2, 3,
        
      };

     unsigned int  index[] = {
      
       3, 2, 1,
       2, 1, 0
      };

    //  VBO و VAO ---
    unsigned int VBO,chVBO,pointVBO ,VAO,chVAO,pointVAO,EBO,chEBO;
    glGenVertexArrays(1, &VAO);
    glGenVertexArrays(1, &chVAO);
    glGenVertexArrays(1, &pointVAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &chVBO);
    glGenBuffers(1, &pointVBO);
    glGenBuffers(1, &EBO);
    glGenBuffers(1, &chEBO);




    // BIND
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);



    glBindVertexArray(chVAO);
    glBindBuffer(GL_ARRAY_BUFFER, chVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chvertices), chvertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
   
    glBindVertexArray(pointVAO);
    glBindBuffer(GL_ARRAY_BUFFER, pointVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(pointvertex), pointvertex, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    // فك الربط 
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    

    //loop
    while (!glfwWindowShouldClose(window))
    {
       
        processInput(window);
        
        if (isColliding()) {
            pointX = genpoint();
        }

        glClearColor(0.306f, 0.514f, 0.612f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT);

        float time = glfwGetTime();
        double blinktime = fmod(time, 2.0f);
        float eyescale; 
        if (blinktime < 0.15) {
            eyescale = 0.0f;  }
        else { eyescale = 1.0f; }

        int loc = glGetUniformLocation(shaderProgram, "color");
        int locmove = glGetUniformLocation(shaderProgram, "move");
        int loceye=glGetUniformLocation(shaderProgram, "eyescale");


        glUseProgram(shaderProgram); 
        
        
        glBindVertexArray(VAO); 
        glUniform3f(loc, 0.176f, 0.208f, 0.220f);
        glUniform2f(locmove, 0.0f,0.0f);
        glUniform1f(loceye, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        glBindVertexArray(pointVAO);
        glPointSize(30.0f);
        glUniform2f(locmove, pointX, pointY);
        glUniform1f(loceye, 1.0f);
        glUniform3f(loc, 0.910f, 0.929f, 0.510f);
        glDrawArrays(GL_POINTS, 0, 1);

        glBindVertexArray(chVAO);
        glUniform2f(locmove, xx,up);
        glUniform1f(loceye, 1.0f);
        glUniform3f(loc, 0.059f, 0.318f, 0.522f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,0);
        

        glUniform3f(loc, 0.059f, 0.318f, 0.522f);
        glUniform1f(loceye, 1.0f);
        glPointSize(35.0f);
        glDrawArrays(GL_POINTS, 0, 2);

        glUniform3f(loc, 0.80f, 0.890f, 0.910f);
        glPointSize(10.0f);
        glUniform1f(loceye, eyescale);
        if (eyescale > 0.0f)
        {
            glDrawArrays(GL_POINTS, 4, 2);
        }

        

        
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}