/*Draw call -> vertex shader -> fragment shader -> screen
Vertex shader is called once per vertex. Primary purpose of Vertex shader
is to tell OpenGL, where we want out vertex to be. We can access all our
vertices in vertex shader. Once we access this position, we can tell opengl
where we want to position this. Usage? -> say we have a camera, and we need
to change the vertex position wrt to camera i.e. transformations!
After vertex shader is called once per vertex, we move to fragment shader.
Fragment shader runs per pixel once. Primary purpose of pixel shader -> decide
which color the pixel should be. (just like a coloring book, where you have an
outline and you need to fill color).
For a triangle, vertex shader called 3 times, fragment shader can be called n
number of times. Hence we need to take care what computation we do in what shaders
to keep the number of computations less. (Think about performance!).
Lighting can be calculated per pixel. 
*/

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

    GLFWwindow* window = glfwCreateWindow(640, 480, "VertexAttrib", NULL, NULL);

    if(!window){
        //error
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    float positions[6] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.0f,  0.5f
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

    std::string vertexShader = 
        "#version 320 es\n"
        "precision mediump float;\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "gl_Position = position;\n"
        "}\n";

    std::string fragmentShader = 
        "#version 320 es\n"
        "precision mediump float;\n"
        "layout(location = 0) out vec4 color;\n"
        "void main()\n"
        "{\n"
        "color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);
    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();   

        glDrawArrays(GL_TRIANGLES, 0, 3);   

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}