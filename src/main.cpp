#include <config.h>
#include <app.h>

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Funnel Pathfinding", NULL, NULL);
    
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    App::init();

    glfwSetKeyCallback(window, [](GLFWwindow* w, int key, int scancode, int action, int mods) {
        if (action == GLFW_PRESS) {
            App::input(key); 
        }
    });

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        
        App::update();
        App::render();

        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}