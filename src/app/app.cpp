#include <config.h>
#include <app.h>

#include <geometry.h>
#include <generator.h>

float camL, camR, camB, camT;


void App::init() {
    Generator::generateProblem();
    App::calculateView();
}

void App::input(int key) {
    if (key == GLFW_KEY_R) {
        Generator::generateProblem();
        App::calculateView();
    }
    
}

void App::update() {

}

void App::render() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(camL, camR, camB, camT, -1, 1);

    glClearColor(0.1f, 0.1f, 0.12f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    

    
    glColor4f(0.5f, 0.6f, 0.8f, 0.3f); // light blue, semi-transparent
    
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        glBegin(GL_LINE_LOOP);
        glm::vec2 a = polygon[indices[i]];
        glm::vec2 b = polygon[indices[i+1]];
        glm::vec2 c = polygon[indices[i+2]];
        glVertex2f(a.x, a.y);
        glVertex2f(b.x, b.y);
        glVertex2f(c.x, c.y);
        glEnd();
    }
    glBegin(GL_LINE_LOOP);
    for (auto& p : polygon) glVertex2f(p.x, p.y);
    glEnd();


    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glPointSize(20.0f);

    glBegin(GL_POINTS);
    glVertex2f(start.x, start.y);
    glVertex2f(end.x, end.y);
    glEnd();
}

void App::calculateView() {
    float w = (bBox.maxX - bBox.minX) * 1.2f;
    float h = (bBox.maxY - bBox.minY) * 1.2f;
    float cx = (bBox.minX + bBox.maxX) / 2.0f;
    float cy = (bBox.minY + bBox.maxY) / 2.0f;

    float screenAspect = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
    
    if (w / h > screenAspect) h = w / screenAspect;
    else w = h * screenAspect;

    camL = cx - w / 2.0f;
    camR = cx + w / 2.0f;
    camB = cy - h / 2.0f;
    camT = cy + h / 2.0f;
}