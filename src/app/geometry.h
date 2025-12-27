#pragma once

#define MAX_X 2000.0f
#define MAX_Y 1000.0f
#define MIN_VERTEX_CNT 20
#define MAX_VERTEX_CNT 200

struct BoundingBox {
    float minX, maxX, minY, maxY; 
};

struct triangleNode {
    int triangleIndex;
    float x, y;
    bool partOfPath;
    bool visited;
    int parent;
    float saturation;
    std::vector<int> links;
};

struct Portal {
    glm::vec2 l, r;
    Portal(glm::vec2 left, glm::vec2 right) : l(left), r(right) {}
    Portal() : l(0.0f), r(0.0f) {}
};

extern std::vector<glm::vec2> polygon;
extern std::vector<int> indices;
extern std::vector<triangleNode> nodes;
extern BoundingBox bBox;
extern glm::vec2 startPoint;
extern glm::vec2 endPoint;
extern int startTriangle;
extern int endTriangle;
extern std::vector<int> trianglePath;
extern std::vector<Portal> portals;
extern std::vector<glm::vec2> path;
extern int leftIndx;
extern glm::vec2 leftPoint;
extern int rightIndx;
extern glm::vec2 rightPoint;

namespace Geometry {
    void printPolygon();
    void printTriangles();
    void printEndPoints();
    void calculateBoundingBox();
    triangleNode makeTriangleNode();
}

