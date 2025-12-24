#include "config.h"
#include "app.h"

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // Lock that aspect ratio!
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Funnel Pathfinding", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    App app;
    app.init();

    // Set up input callbacks
    glfwSetWindowUserPointer(window, &app);
    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        App* appPtr = (App*)glfwGetWindowUserPointer(w);
        if (action == GLFW_PRESS) appPtr->onKeyPress(key);
    });

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        app.update();
        app.render();

        glfwSwapBuffers(window);

        
    }

    glfwTerminate();
    return 0;
}