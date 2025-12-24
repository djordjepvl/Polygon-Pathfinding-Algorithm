#include <config.h>
#include <generator.h>

#include <util.h>
#include <geometry.h>
#include <triangulate.h>


void Generator::generateProblem() {
    Generator::generatePolygon();
    Triangulate::triangulatePolygon();
    Generator::generateEndPoints();
    Geometry::printPolygon();
    Geometry::printTriangles();
    Geometry::printEndPoints();
}

void Generator::generatePolygon() {
    polygon.clear();
    int vertexCnt = getRandInt(MIN_VERTEX_CNT, MAX_VERTEX_CNT);

    std::vector<float> angles;
    for (int i = 0; i < vertexCnt; ++i) 
        angles.push_back(getRandFloat(0.0f, 2.0f * 3.14159f));
    
    std::sort(angles.begin(), angles.end());

    for (float a : angles) {
        float r = getRandFloat(200.0f, MAX_Y); 
        polygon.push_back({ cosf(a) * r, sinf(a) * r });
    }
}

void Generator::generateEndPoints() {
    int triCount = indices.size() / 3;
    if (triCount > 0) {
        int startTriIdx = getRandInt(0, triCount - 1) * 3;
        int endTriIdx = getRandInt(0, triCount - 1) * 3;
        
        glm::vec2 a1 = polygon[indices[startTriIdx]], b1 = polygon[indices[startTriIdx+1]], c1 = polygon[indices[startTriIdx+2]];
        glm::vec2 a2 = polygon[indices[endTriIdx]],   b2 = polygon[indices[endTriIdx+1]],   c2 = polygon[indices[endTriIdx+2]];
        
        auto getPt = [](glm::vec2 a, glm::vec2 b, glm::vec2 c) {
            float u = getRandFloat(0, 1), v = getRandFloat(0, 1);
            if (u + v > 1) { u = 1 - u; v = 1 - v; }
            return a + u * (b - a) + v * (c - a);
        };
        
        start = getPt(a1, b1, c1);
        end = getPt(a2, b2, c2);
    }
}