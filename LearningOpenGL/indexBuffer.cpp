//NOTES: Index buffer is initialised like a normal vertex buffer EXCEPT....
//GL_ELEMENT_ARRAY_BUFFER is used. and glDrawElements used instead of glDrawArray
//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr); 
//6 is the number of indices
//last param is nullptr as we have already bound the index buffer
//otherwise we would have to give index buffer pointer
//IMPORTANT: Index buffers need to be unsigned int!!! Always!

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    float positions[8] = {
        0.0f, 0.0f,  // lower-left corner  
        1.0f, 0.0f,  // lower-right corner
        0.5f, 1.0f   // top-center corner
    };

    unsigned int index[6] = {
        0, 1, 2,
    };

    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

    unsigned int indexbuffer;
    glGenBuffers(1, &indexbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), index, GL_STATIC_DRAW);

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