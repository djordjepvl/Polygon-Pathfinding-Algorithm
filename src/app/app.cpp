#include <config.h>
#include <app.h>

#include <geometry.h>
#include <problem.h>
#include <pathfind.h>
#include <funnel.h>

int appState = -1;
int prevState = -1;

double mouseX = 0;
double mouseY = 0;

bool doStep = 0;

bool allowCursor = false;
bool allowPolyClose = false;

float camL, camR, camB, camT;
glm::vec4 bgColor = glm::vec4(0.145f, 0.165f, 0.188f, 1.0f);
glm::vec4 polyTriangleColor = glm::vec4(0.224f, 0.251f, 0.29f, 1.0f);
glm::vec4 polyColor = glm::vec4(0.5f, 0.6f, 0.8f, 1.0f);
glm::vec4 startPointColor = glm::vec4(0.259f, 0.941f, 0.494f, 1.0f);
glm::vec4 endPointColor = glm::vec4(0.941f, 0.259f, 0.529f, 1.0f);

glm::vec4 graphColor = glm::vec4(0.941f, 0.651f, 0.259f, 1.0f);
glm::vec4 pathColor = glm::vec4(0.255f, 0.961f, 0.914f, 1.0f);
glm::vec4 trianglePathColor = glm::vec4(0.098f, 0.478f, 1.0f, 1.0f);

glm::vec4 funnelLeftColor = glm::vec4(0.988f, 0.976f, 0.208f, 1.0f);
glm::vec4 funnelRightColor = glm::vec4(0.988f, 0.208f, 0.929f, 1.0f);

glm::vec4 notAllowedColor = glm::vec4(0.690f, 0.071f, 0.184f, 1.0f);

glm::vec4 desaturateColor(glm::vec4 color, float amount) {
    glm::vec3 luminanceWeights(0.299f, 0.587f, 0.114f);
    float gray = glm::dot(glm::vec3(color), luminanceWeights);
    glm::vec3 result = glm::mix(glm::vec3(color), glm::vec3(gray), amount);
    return glm::vec4(result, color.a);
}
glm::vec4 brightenColor(glm::vec4 color, float amount) {
    glm::vec3 white(1.0f, 1.0f, 1.0f);
    glm::vec3 brightenedRGB = glm::mix(glm::vec3(color), white, amount);
    return glm::vec4(brightenedRGB, color.a);
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

glm::vec2 screenToGeometry(double screenX, double screenY) {
    // normalize screen coords to [0, 1]
    float normX = screenX / WINDOW_WIDTH;
    float normY = 1.0f - (screenY / WINDOW_HEIGHT);  // flip y because of opengl standard
    // map to geometry coordinates using camera bounds
    float worldX = camL + normX * (camR - camL);
    float worldY = camB + normY * (camT - camB);
    
    return glm::vec2(worldX, worldY);
}

void App::init() {
    appState = -1;
}

bool advanced = 0;
void advanceAppState() {
    if (advanced) return;
    prevState = appState;
    appState = std::min(appState+1, 7);
    advanced = 1;
}

int stepHold = 0;

void App::input(int key, int action) {
    if (key == -1 && action == GLFW_PRESS) {  // left click
        if (appState == 0) Problem::setStart(cursor);
        else if (appState == -5) {
            if (allowCursor) polygon.push_back(cursor);
        }
    }
    else if (key == -2 && action == GLFW_PRESS) {  // right click
        if (appState == 0) Problem::setEnd(cursor);
        else if (appState == -5) {
            if (polygon.size()>0) polygon.pop_back();
        }
    }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS) {
        advanceAppState();
        appState = -1;
    }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS) {
        advanceAppState();
        appState = -4;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS) {
        advanceAppState();
        appState = -2;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS) {
        advanceAppState();
        appState = -3;
    }
    else if (key == GLFW_KEY_ENTER && action == GLFW_PRESS) {
        if (appState == -5) {
            if (allowPolyClose) {
                Problem::create();
                App::calculateView();
                advanceAppState();
                appState = 0;
            }
        }
        else advanceAppState();
    }
    else if (key == GLFW_KEY_F) {
        if (action == GLFW_PRESS) stepHold = 1;
        else if (action == GLFW_RELEASE) stepHold = 0;
    }

}

void App::update() {
    cursor = screenToGeometry(mouseX,mouseY);
    allowCursor = Geometry::allowNewPoint(cursor, 0.005f);
    allowPolyClose = polygon.size()>2 && Geometry::allowNewPoint(polygon[0], 0.0f);

    if (stepHold) {
        doStep = (stepHold-1)%20==0;
        stepHold++;
    }

    if (doStep) {
        switch (appState) {
            case 0:
                advanceAppState();
                stepHold = 0;
                break;
            case 1:
                if (!PathFind::bfsAdvance()) {
                    advanceAppState();
                    stepHold = 0;
                }
                break;
            case 2:
                advanceAppState();
                stepHold = 0;
                break;
            case 3:
                advanceAppState();
                stepHold = 0;
                break;
            case 4:
                advanceAppState();
                stepHold = 0;
                break;
            case 5:
                if (!Funnel::funnelAdvance()) {
                    advanceAppState();
                    stepHold = 0;
                }
                break;
            case 6:
                advanceAppState();
                stepHold = 0;
                break;
        }
    }
    doStep = 0;

    if (appState != -1 && appState == prevState) return;
    switch (appState) {
        case -1:
            nodes.clear();
            trianglePath.clear();
            portals.clear();
            path.clear();
            Problem::load();
            App::calculateView();
            appState = 0;
            break;
        case -2:
            nodes.clear();
            trianglePath.clear();
            portals.clear();
            path.clear();
            Problem::loadPrev();
            App::calculateView();
            appState = 0;
            break;
        case -3:
            nodes.clear();
            trianglePath.clear();
            portals.clear();
            path.clear();
            Problem::loadNext();
            App::calculateView();
            appState = 0;
            break;
        case -4:
            nodes.clear();
            trianglePath.clear();
            portals.clear();
            path.clear();
            indices.clear();
            polygon.clear();
            appState = -5;
            break;
        case 1:
            PathFind::createGraph();
            break;
        case 2:
            while (PathFind::bfsAdvance());
            break;
        case 5:
            Funnel::findPortals();
            break;
        case 6:
            while (Funnel::funnelAdvance());
            break;
    }
    prevState = appState;
    advanced = 0;
}


void setColor(glm::vec4 color, float opacityOverride = -1) {
    if (opacityOverride >= 0.0f && opacityOverride <= 1.0f)
        glColor4f(color.r, color.g, color.b, opacityOverride);
    else
        glColor4f(color.r, color.g, color.b, color.a);
}

void drawFillLayer(float opacity) {
    setColor(bgColor, opacity);
    glBegin(GL_QUADS);
        glVertex2f(camL, camB);
        glVertex2f(camR, camB);
        glVertex2f(camR, camT);
        glVertex2f(camL, camT);
    glEnd();
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

void drawPoly() {
    setColor(polyColor);

    if (appState == -5 && polygon.size()>0) {
        glBegin(GL_LINE_STRIP);
        for (auto& p : polygon) glVertex2f(p.x, p.y);
        glEnd();
        glBegin(GL_POINTS);
        glVertex2f(polygon[0].x, polygon[0].y);
        glEnd();
    }
    else {
        glBegin(GL_LINE_LOOP);
        for (auto& p : polygon) glVertex2f(p.x, p.y);
        glEnd();
    }
}

void drawEndTriangles() {
    if (startTriangle == endTriangle) glLineWidth(5.0f);
    setColor(endPointColor);
    glm::vec2 a = polygon[indices[endTriangle*3]];
    glm::vec2 b = polygon[indices[endTriangle*3+1]];
    glm::vec2 c = polygon[indices[endTriangle*3+2]];
    
    
    glm::vec4 fillCol = endPointColor;
    fillCol.a = 0.15f; 
    setColor(fillCol);
    
    glBegin(GL_TRIANGLES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glVertex2f(c.x, c.y);
    glEnd();

    setColor(endPointColor);
    
    glBegin(GL_LINE_LOOP);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glVertex2f(c.x, c.y);
    glEnd();

    if (startTriangle == endTriangle) glLineWidth(2.0f);

    a = polygon[indices[startTriangle*3]];
    b = polygon[indices[startTriangle*3+1]];
    c = polygon[indices[startTriangle*3+2]];
    
    fillCol = startPointColor;
    fillCol.a = 0.15f; 
    setColor(fillCol);
    
    glBegin(GL_TRIANGLES);
    glVertex2f(a.x, a.y);
    glVertex2f(b.x, b.y);
    glVertex2f(c.x, c.y);
    glEnd();

    setColor(startPointColor);

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
    glBegin(GL_LINES);
    for (auto& node : nodes) {
        for (auto& link : node.links) {
            if (link==-1) continue;
            if (node.partOfPath && nodes[link].partOfPath) setColor(pathColor);
            else if (node.visited && nodes[link].visited) setColor(brightenColor(graphColor, std::max(1.0f-node.saturation, 1.0f-nodes[link].saturation)));
            else setColor(graphColor);
            glVertex2f(node.x, node.y);
            glVertex2f(nodes[link].x, nodes[link].y);
        }
    }
    glEnd();
}
void drawGraphNodes() {
    glBegin(GL_POINTS);
    for (auto& node : nodes) {
        if (node.partOfPath) setColor(pathColor); 
        else if (node.visited) setColor(brightenColor(graphColor, 1.0f-node.saturation));
        else setColor(graphColor);
        glVertex2f(node.x, node.y);
    }
    glEnd();
}

void drawTrianglePath() {
    glm::vec4 fillCol = trianglePathColor;
    fillCol.a = 0.15f; 
    setColor(fillCol);
    
    glBegin(GL_TRIANGLES);
    for (auto& i : trianglePath) {
        if (i == startTriangle || i == endTriangle) continue;
        glVertex2f(polygon[indices[i*3]].x,   polygon[indices[i*3]].y);
        glVertex2f(polygon[indices[i*3+1]].x, polygon[indices[i*3+1]].y);
        glVertex2f(polygon[indices[i*3+2]].x, polygon[indices[i*3+2]].y);
    }
    glEnd();

    setColor(trianglePathColor);
    for (auto& i : trianglePath) {
        if (i == startTriangle || i == endTriangle) continue;
        glBegin(GL_LINE_LOOP);
        glVertex2f(polygon[indices[i*3]].x , polygon[indices[i*3]].y);
        glVertex2f(polygon[indices[i*3+1]].x , polygon[indices[i*3+1]].y);
        glVertex2f(polygon[indices[i*3+2]].x , polygon[indices[i*3+2]].y);
        glEnd();
    }
}

void drawPortals() {
    setColor(trianglePathColor);
    int n = portals.size();
    for (int i=0; i<n; i++) {
        if (currPrtl == i) {
            setColor(brightenColor(trianglePathColor, 0.9f));
            if (portals[i].l == portals[i].r) {
                glBegin(GL_POINTS);
                glVertex2f(portals[i].l.x, portals[i].l.y);
                glEnd();
            }
            else {
                glBegin(GL_LINES);
                glVertex2f(portals[i].l.x, portals[i].l.y);
                glVertex2f(portals[i].r.x, portals[i].r.y);
                glEnd();
            }
            continue;
        }
        setColor(trianglePathColor);
        glBegin(GL_LINES);
        glVertex2f(portals[i].l.x, portals[i].l.y);
        glVertex2f(portals[i].r.x, portals[i].r.y);
        glEnd();
    }
}

void drawFunnelLines() {
    if (path.empty()) return;
    
    setColor(funnelLeftColor);
    glBegin(GL_LINES);
    glVertex2f(path.back().x, path.back().y);
    glVertex2f(leftPoint.x, leftPoint.y);
    glEnd();
    setColor(funnelRightColor);
    glBegin(GL_LINES);
    glVertex2f(path.back().x, path.back().y);
    glVertex2f(rightPoint.x, rightPoint.y);
    glEnd();
    
    setColor(funnelLeftColor, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(path.back().x, path.back().y);
    glVertex2f(leftTarget.x, leftTarget.y);
    glEnd();
    setColor(funnelRightColor, 0.5f);
    glBegin(GL_LINES);
    glVertex2f(path.back().x, path.back().y);
    glVertex2f(rightTarget.x, rightTarget.y);
    glEnd();
}

void drawPath() {
    if (path.size() < 2) return;
    setColor(pathColor);
    glBegin(GL_LINE_STRIP);
    for (auto& point : path) {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void drawCursor() {
    if (allowCursor) setColor(polyColor, 0.6f);
    else setColor(notAllowedColor, 0.6f);
    if (polygon.size()>0) {
        glBegin(GL_LINE_STRIP);
        glVertex2f(polygon.back().x, polygon.back().y);
        glVertex2f(cursor.x,cursor.y);
        glEnd();
    }
    glBegin(GL_POINTS);
    glVertex2f(cursor.x, cursor.y);
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

    if (appState == -5) {
        drawPoly();
        drawCursor();
        if (polygon.size()>2) {
            if (allowPolyClose) setColor(polyColor, 0.4f);
            else setColor(notAllowedColor, 0.4f);
            glBegin(GL_LINE_STRIP);
            glVertex2f(polygon.back().x, polygon.back().y);
            glVertex2f(polygon[0].x, polygon[0].y);
            glEnd();
        }
        return;
    }
    
    drawPolyTriangles();
    drawPoly();
    
    if (appState>=3 && appState<7) {
        drawFillLayer(0.65f);
        drawTrianglePath();
    }
    
    if (appState>=1 && appState<4) {
        drawGraphEdges();
        drawGraphNodes();
    }
    
    if (appState >= 3 && appState < 7) drawEndTriangles();
    
    
    if (appState>=5 && appState<7) drawFillLayer(0.65f);
    drawEndPoints();
    if (appState>=5 && appState<7) {
        drawPortals();
        if (appState<6) drawFunnelLines();
    }

    if (appState>=5) drawPath();

}
