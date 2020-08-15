//NOTES: Index buffer is initialised like a normal vertex buffer EXCEPT....
//GL_ELEMENT_ARRAY_BUFFER is used. and glDrawElements used instead of glDrawArray
//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); 
//6 is the number of indices
//last param is nullptr as we have already bound the index buffer
//otherwise we would have to give index buffer pointer
//IMPORTANT: Index buffers need to be unsigned int!!! Always!

#define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
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

    GLFWwindow* window = glfwCreateWindow(640, 480, "Index Buffer", NULL, NULL);

    if(!window){
        //error
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    // The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("download.jpeg", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    //stbi_image_free(data);

    std::string vertexShader = 
        "#version 320 es\n"
        "precision mediump float;\n"
        "layout(location = 0) in vec4 position;\n"
        "layout(location = 1) in vec4 in_color;\n"
	"layout(location = 2) in vec2 aTexCoord;\n"
        "out vec4 out_color;\n" //sending color to next shader
	"out vec2 TexCoord;\n" //sending texture coordinate to next shader
        "void main()\n"
        "{\n"
        "gl_Position = position;\n"
	"TexCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
        "out_color = in_color;\n"
        "}\n";

    std::string fragmentShader = 
        "#version 320 es\n"
        "precision mediump float;\n"
        "layout(location = 0) out vec4 color;\n"
        "in vec4 out_color;\n"
	"in vec2 TexCoord;\n"
	"uniform sampler2D texture1;\n"
        "void main()\n"
        "{\n"
        //"color = out_color;\n"
	"color = texture(texture1, TexCoord);\n"
        "}\n";

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);
    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();   

        //glDrawArrays(GL_TRIANGLES, 0, 4);   
        //glDrawElements when we give index buffer. 6 is the number of indices
        //last param is nullptr as we have already bound the index buffer
        //otherwise we would have to give index buffer pointer
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); 
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}
