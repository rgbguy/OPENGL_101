#include <math.h>       /* cos */

#define PI 3.14159265
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str(); //can be written as &source[0]
    glShaderSource(id, 1, &src, nullptr);//checkout docs.gl to know all params
    glCompileShader(id);

    //Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result); // i = integer, v = vector
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char)); //alloca is a function
        //it lets you allocate at stack dynamically. cpp doesnt allow dynamic arrays. hence we use this.
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile shader! "
                  <<  (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
                  << " shader" << std::endl;
        std::cout << message << std:: endl;
        glDeleteShader(id);
        return 0;
    }
    
    return id;
    
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int Program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    //next steps, link both shaders into 1. Think it just like cpp linking
    glAttachShader(Program, vs);
    glAttachShader(Program, fs);
    glLinkProgram(Program);
    glValidateProgram(Program);

    //now we can delete shaders, as they are already linked to program. We have the program
    //so go ahead and delete the intermediate
    glDeleteShader(vs);
    glDeleteShader(fs);

    return Program;
}

int main(){

    if(!glfwInit()){
        //error
    }

    glewExperimental = GL_TRUE;

    glewInit();

    
    GLFWwindow* window = glfwCreateWindow(640, 480, "Circle", NULL, NULL);

    if(!window){
        //error
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();
    glClearColor(0.2f, 0.3f, 1.0f, 1.0f);

    float positionsCircle[360];
    float r = 0.25;
    for (int i = 0; i < 360; i=i+2)
    {
        positionsCircle[i] = r*cos ( i * PI / 180.0 );
        positionsCircle[i+1] = r*sin ( i * PI / 180.0 );
        std::cout << positionsCircle[i] << " " << positionsCircle[i+1] << std::endl;
    }
    
    unsigned int bufferCircle;
    glGenBuffers(1, &bufferCircle);
    glBindBuffer(GL_ARRAY_BUFFER, bufferCircle);
    glBufferData(GL_ARRAY_BUFFER, 360 * sizeof(float), positionsCircle, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);


    std::string vertexShader = 
        "#version 320 es\n"
        "precision mediump float;\n"
        "layout(location = 0) in vec4 position;\n"
        "out vec4 out_color;\n" //sending color to next shader
        "vec4 lightDir = vec4(1.0, 1.0, 0.0,1.0);\n"
        "void main()\n"
        "{\n"
        "gl_Position = position;\n"
        "float calculated = pow(dot(lightDir, position),2.0);\n"
        "out_color = normalize(vec4(calculated,calculated,calculated,1.0f)) ;\n"
        "}\n";

    std::string fragmentShader = 
        "#version 320 es\n"
        "precision mediump float;\n"
        "layout(location = 0) out vec4 color;\n"
        "in vec4 out_color;\n"
        "void main()\n"
        "{\n"
        "color = out_color;\n"
        "}\n";
        
    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();   

        glClear(GL_COLOR_BUFFER_BIT); //clears the screen every frame
        glDrawArrays(GL_TRIANGLE_FAN, 0, 180);   

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}