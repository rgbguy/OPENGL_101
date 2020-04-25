//run using g++ main.cpp -lglut -lGL -lglfw3 -lpthread -ldl -o outfilename
//if we want to run functionality after OpenGL 1.1, we can use glfw
#include <GL/glew.h> //glew needs to be before glfw
#include <GLFW/glfw3.h>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /*vertex positions*/
    float positions[] = {
        -0.5,-0.5,
         0.5,-0.5,
         0.5, 0.5
    };

    /*Define a buffer, give it an ID, bind the buffer, Tell what data it has*/
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);
    
    

    /*Enable vertex attrip array, define vertex attributes - strides, type etc..*/
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        //used when we do not have an index buffer -> why?
        glDrawArrays(GL_TRIANGLES, 0, 3);
        //glDrawElements -> used with index buffer

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

