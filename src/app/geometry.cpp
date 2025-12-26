#include <config.h>
#include <geometry.h>

std::vector<glm::vec2> polygon;
std::vector<int> indices;
std::vector<triangleNode> nodes;
BoundingBox bBox;
glm::vec2 startPoint;
glm::vec2 endPoint;
int startTriangle;
int endTriangle;
std::vector<int> trianglePath;



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

