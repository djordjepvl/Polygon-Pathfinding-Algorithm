#pragma once

#define MAX_X 2000.0f
#define MAX_Y 1000.0f
#define MIN_VERTEX_CNT 20
#define MAX_VERTEX_CNT 200

struct BoundingBox {
    float minX, maxX, minY, maxY; 
};


extern std::vector<glm::vec2> polygon;
extern std::vector<int> indices;
extern BoundingBox bBox;
extern glm::vec2 start;
extern glm::vec2 end;





namespace Geometry {
    void printPolygon();
    void printTriangles();
    void printEndPoints();
    void calculateBoundingBox();
}
