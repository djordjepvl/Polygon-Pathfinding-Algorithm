#include <config.h>
#include <app.h>

#include <geometry.h>
#include <generator.h>
#include <pathfind.h>

float camL, camR, camB, camT;
glm::vec4 bgColor = glm::vec4(0.145f, 0.165f, 0.188f, 1.0f);
glm::vec4 polyTriangleColor = glm::vec4(0.224f, 0.251f, 0.29f, 1.0f);
glm::vec4 polyEdgeColor = glm::vec4(0.5f, 0.6f, 0.8f, 1.0f);
glm::vec4 startPointColor = glm::vec4(0.259f, 0.941f, 0.494f, 1.0f);
glm::vec4 endPointColor = glm::vec4(0.941f, 0.259f, 0.529f, 1.0f);

glm::vec4 graphNodeColor = glm::vec4(0.941f, 0.651f, 0.259f, 1.0f);
glm::vec4 graphEdgeColor = glm::vec4(0.929f, 0.745f, 0.376f, 1.0f);



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

void App::init() {
    Generator::generateProblem();
    App::calculateView();
}

void App::input(int key) {
    if (key == GLFW_KEY_R) {
        Generator::generateProblem();
        App::calculateView();
    }
    if (key == GLFW_KEY_ENTER) PathFind::createGraph();
    
}

void App::update() {
    
}


void setColor(glm::vec4 color) {
    glColor4f(color.r, color.g, color.b, color.a);
}

void drawPolyTriangles() {
    setColor(polyTriangleColor);
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
}

void drawPolyEdges() {
    setColor(polyEdgeColor);
    glBegin(GL_LINE_LOOP);
    for (auto& p : polygon) glVertex2f(p.x, p.y);
    glEnd();
}

void drawEndTriangles() {
    if (startTriangle == endTriangle) glLineWidth(5.0f);
    setColor(endPointColor);
    glm::vec2 a = polygon[indices[endTriangle*3]];
    glm::vec2 b = polygon[indices[endTriangle*3+1]];
    glm::vec2 c = polygon[indices[endTriangle*3+2]];
    glBegin(GL_LINE_LOOP);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glVertex2f(c.x, c.y);
    glEnd();
    if (startTriangle == endTriangle) glLineWidth(2.0f);
    setColor(startPointColor);
    a = polygon[indices[startTriangle*3]];
    b = polygon[indices[startTriangle*3+1]];
    c = polygon[indices[startTriangle*3+2]];
    glBegin(GL_LINE_LOOP);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glVertex2f(c.x, c.y);
    glEnd();
}

void drawEndPoints() {
    glBegin(GL_POINTS);
    setColor(startPointColor);
    glVertex2f(startPoint.x, startPoint.y);
    setColor(endPointColor);
    glVertex2f(endPoint.x, endPoint.y);
    glEnd();
}

void drawGraphEdges() {
    setColor(graphNodeColor);
    glBegin(GL_LINES);
    for (auto& node : nodes) {
        for (auto& link : node.links) {
            if (link==-1) continue;
            glVertex2f(node.x, node.y);
            glVertex2f(nodes[link].x, nodes[link].y);
        }
    }
    glEnd();
}
void drawGraphNodes() {
    setColor(graphNodeColor);
    glBegin(GL_POINTS);
    for (auto& node : nodes) glVertex2f(node.x, node.y);
    glEnd();
}

void App::render() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(camL, camR, camB, camT, -1, 1);
    glClearColor(bgColor.r, bgColor.g, bgColor.b, bgColor.a);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glPointSize(10.0f);
    glLineWidth(2.0f);
    
    drawPolyTriangles();
    drawPolyEdges();
    
    drawGraphEdges();
    drawGraphNodes();

    drawEndTriangles();
    drawEndPoints();
}
