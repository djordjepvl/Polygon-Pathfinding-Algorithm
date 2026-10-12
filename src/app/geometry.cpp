#include <config.h>
#include <geometry.h>
#include <util.h>

std::vector<glm::vec2> polygon;
std::vector<int> indices;
std::vector<triangleNode> nodes;
BoundingBox bBox;
glm::vec2 startPoint;
glm::vec2 endPoint;
int startTriangle;
int endTriangle;
std::vector<int> trianglePath;
std::vector<Portal> portals;
std::vector<glm::vec2> path;
int leftIndx;
glm::vec2 leftPoint;
glm::vec2 leftTarget;
int rightIndx;
glm::vec2 rightPoint;
glm::vec2 rightTarget;

glm::vec2 cursor;


void Geometry::printPolygon() {
    for (int i = 0; i < polygon.size(); i++) {
        std::cout << "P[" << i << "]: " << polygon[i].x << ", " << polygon[i].y << std::endl;
    }
}

void Geometry::printTriangles() {
    std::cout << "--- Triangles (" << indices.size() / 3 << ") ---" << std::endl;
    for (size_t i = 0; i < indices.size(); i += 3) {
        std::cout << "Tri " << i / 3 << ": "
                  << "Indices(" << indices[i] << ", " << indices[i+1] << ", " << indices[i+2] << ") "
                  << "Coords([" << polygon[indices[i]].x << "," << polygon[indices[i]].y << "], "
                  << "[" << polygon[indices[i+1]].x << "," << polygon[indices[i+1]].y << "], "
                  << "[" << polygon[indices[i+2]].x << "," << polygon[indices[i+2]].y << "])" 
                  << std::endl;
    }
}

void Geometry::printEndPoints() {
    std::cout << "Start Point: " << startPoint.x << ", " << startPoint.y << std::endl;
    std::cout << "End Point: " << endPoint.x << ", " << endPoint.y << std::endl;
}


void Geometry::calculateBoundingBox() {
    bBox = { 1e9, -1e9, 1e9, -1e9 };
    for (auto& p : polygon) {
        if (p.x < bBox.minX) bBox.minX = p.x;
        if (p.x > bBox.maxX) bBox.maxX = p.x;
        if (p.y < bBox.minY) bBox.minY = p.y;
        if (p.y > bBox.maxY) bBox.maxY = p.y;
    }
}

triangleNode Geometry::makeTriangleNode() {
    triangleNode newNode;
    newNode.triangleIndex = -1;
    newNode.x = 0;
    newNode.y = 0;
    newNode.partOfPath = 0;
    newNode.visited = 0;
    newNode.saturation = 1.0f;
    newNode.parent = -1;
    return newNode;
}

bool Geometry::pointInPolygon(glm::vec2 point) {
    int n = polygon.size();
    int count = 0;
    
    for (int i = 0; i < n; i++) {
        glm::vec2 a = polygon[i];
        glm::vec2 b = polygon[(i + 1) % n];
        
        if ((a.y <= point.y && point.y < b.y) || (b.y <= point.y && point.y < a.y)) {
            float xIntersect = a.x + (point.y - a.y) * (b.x - a.x) / (b.y - a.y);
            if (point.x < xIntersect) {
                count++;
            }
        }
    }
    
    return count % 2 == 1;
}

bool Geometry::segmentsIntersect(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3, glm::vec2 p4) {
    float cp1 = crossProduct(p1, p2, p3);
    float cp2 = crossProduct(p1, p2, p4);
    
    float cp3 = crossProduct(p3, p4, p1);
    float cp4 = crossProduct(p3, p4, p2);

    if (((cp1 > 0 && cp2 < 0) || (cp1 < 0 && cp2 > 0)) &&
        ((cp3 > 0 && cp4 < 0) || (cp3 < 0 && cp4 > 0))) {
        return true;
    }

    return false;
}

float distPointToSegment(glm::vec2 p, glm::vec2 a, glm::vec2 b) {
    glm::vec2 ab = b - a;
    glm::vec2 ap = p - a;

    float l2 = ab.x * ab.x + ab.y * ab.y; 
    if (l2 == 0.0f) return glm::distance(p, a);

    float t = std::max(0.0f, std::min(1.0f, glm::dot(ap, ab) / l2));
    glm::vec2 projection = a + t * ab;
    
    return glm::distance(p, projection);
}

bool Geometry::allowNewPoint(glm::vec2 potentialPoint, const float epsilon) {

    for (const auto& p : polygon) {
        if (glm::distance(potentialPoint, p) < epsilon) return false;
    }

    if (polygon.size() < 1) return true;

    for (size_t i = 0; i + 1 < polygon.size(); ++i) {
        glm::vec2 a = polygon[i];
        glm::vec2 b = polygon[i+1];
        
        float d = distPointToSegment(potentialPoint, a, b);
        if (d < epsilon) return false;
    }

    if (polygon.size() < 2) return true;

    glm::vec2 newStart = polygon.back();
    glm::vec2 newEnd = potentialPoint;
    for (size_t i = 0; i + 1 < polygon.size() - 1; ++i) {
        if (segmentsIntersect(newStart, newEnd, polygon[i], polygon[i+1])) {
            return false;
        }
    }

    return true;
}

bool Geometry::polyIsCCW() {
    float area = 0.0f;
    int n = polygon.size();
    
    for (int i = 0; i < n; i++) {
        glm::vec2 p1 = polygon[i];
        glm::vec2 p2 = polygon[(i + 1) % n];
        
        area += (p2.x - p1.x) * (p2.y + p1.y);
    }

    return area < 0;
}

