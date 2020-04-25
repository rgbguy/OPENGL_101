#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    while(!glfwWindowShouldClose(window)){

        glfwPollEvents();   

        glDrawArrays(GL_TRIANGLES, 0, 3);   

        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);

    glfwTerminate();

    return 0;
}