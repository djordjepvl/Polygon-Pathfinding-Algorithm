#include <config.h>
#include <generator.h>

#include <util.h>
#include <geometry.h>
#include <triangulate.h>


void Generator::generateProblem() {
    Generator::generatePolygon();
    Triangulate::triangulatePolygon();
    Generator::generateEndPoints();
    Geometry::calculateBoundingBox();
}

void Generator::generatePolygon() {
    polygon.clear();
    std::vector<glm::vec2> leftWall, rightWall;


    glm::vec2 pos(0, 0);
    float angle = 0.0f; 
    int segments = 15;

    for (int i = 0; i <= segments; ++i) {
        // Limit turn to +/- 45 degrees to prevent self-intersection
        angle += getRandFloat(-0.8f, 0.8f); 
        
        float length = getRandFloat(150.0f, 300.0f);
        float halfWidth = getRandFloat(50.0f, 100.0f); 

        pos += glm::vec2(cos(angle), sin(angle)) * length;

        // Normal vector for wall placement
        glm::vec2 normal(-sin(angle), cos(angle));
        
        leftWall.push_back(pos + normal * halfWidth);
        rightWall.push_back(pos - normal * halfWidth);
    }

    // CCW Order construction
    for (const auto& p : rightWall) polygon.push_back(p);
    std::reverse(leftWall.begin(), leftWall.end());
    for (const auto& p : leftWall) polygon.push_back(p);
}

void Generator::generateEndPoints() {
    int triCount = indices.size() / 3;
    if (triCount >= 2) {
        auto getPt = [](glm::vec2 a, glm::vec2 b, glm::vec2 c) {
            float u = getRandFloat(0, 1), v = getRandFloat(0, 1);
            if (u + v > 1) { u = 1 - u; v = 1 - v; }
            return a + u * (b - a) + v * (c - a);
        };

        // Start point in one of the first few triangles
        start = getPt(polygon[indices[0]], polygon[indices[1]], polygon[indices[2]]);
        
        // End point in one of the last few triangles
        int lastIdx = (triCount - 1) * 3;
        end = getPt(polygon[indices[lastIdx]], polygon[indices[lastIdx+1]], polygon[indices[lastIdx+2]]);
    }
}