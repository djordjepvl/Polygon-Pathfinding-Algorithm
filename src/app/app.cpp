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
        Geometry::printPolygon();
        Geometry::printTriangles();
        Geometry::printEndPoints();
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

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glPointSize(20.0f);
    glBegin(GL_POINTS);
        glColor3f(1.0f, 1.0f, 0.0f);
        glVertex2f(bBox.minX, bBox.minY);
        glVertex2f(bBox.maxX, bBox.minY);
        glVertex2f(bBox.maxX, bBox.maxY);
        glVertex2f(bBox.minX, bBox.maxY);
    glEnd();

    glLineWidth(2.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(bBox.minX, bBox.minY);
        glVertex2f(bBox.maxX, bBox.minY);
        glVertex2f(bBox.maxX, bBox.maxY);
        glVertex2f(bBox.minX, bBox.maxY);
    glEnd();

    glBegin(GL_LINE_LOOP);
    for (auto& p : polygon) glVertex2f(p.x, p.y);
    
    glEnd();
}

void deleteThisLaterLol() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, 1600, 0, 900, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // 2. Prepare the Canvas
    glClearColor(0.1f, 0.1f, 0.12f, 1.0f); // Dark blue-gray
    glClear(GL_COLOR_BUFFER_BIT);

    // 3. Test: Thick Magenta Line (A "Portal")
    glLineWidth(5.0f);
    glColor3f(1.0f, 0.0f, 1.0f); 
    glBegin(GL_LINES);
        glVertex2f(100, 100); 
        glVertex2f(1500, 800);
    glEnd();

    // 4. Test: Thin White Line (A "Triangle Edge")
    glLineWidth(1.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
        glVertex2f(400, 200);
        glVertex2f(1200, 200);
        glVertex2f(800, 700);
    glEnd();

    // 5. Test: Rounded Dots (The "Start/End Points")
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

    glPointSize(20.0f);
    glBegin(GL_POINTS);
        glColor3f(0.0f, 1.0f, 0.0f); // Green Point
        glVertex2f(400, 200);
        
        glColor3f(1.0f, 0.0f, 0.0f); // Red Point
        glVertex2f(1200, 200);
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