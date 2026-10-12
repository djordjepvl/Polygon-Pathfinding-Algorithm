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
        if (action == GLFW_PRESS || action == GLFW_RELEASE) {
            App::input(key, action); 
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* w, int button, int action, int mods) {
    int mouseKey = -(button + 1);
    App::input(mouseKey, action);
    });
    glfwSetCursorPosCallback(window, [](GLFWwindow* w, double xpos, double ypos) {
    mouseX = xpos;
    mouseY = ypos;
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