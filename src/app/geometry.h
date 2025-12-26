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

extern std::vector<glm::vec2> polygon;
extern std::vector<int> indices;
extern std::vector<triangleNode> nodes;
extern BoundingBox bBox;
extern glm::vec2 startPoint;
extern glm::vec2 endPoint;
extern int startTriangle;
extern int endTriangle;
extern std::vector<int> trianglePath;





namespace Geometry {
    void printPolygon();
    void printTriangles();
    void printEndPoints();
    void calculateBoundingBox();
    triangleNode makeTriangleNode();
}

