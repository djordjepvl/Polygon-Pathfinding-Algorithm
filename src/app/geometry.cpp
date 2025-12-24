#include <config.h>
#include <geometry.h>

std::vector<glm::vec2> polygon;
std::vector<int> indices;
glm::vec2 start;
glm::vec2 end;

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
    std::cout << "Start Point: " << start.x << ", " << start.y << std::endl;
    std::cout << "End Point: " << end.x << ", " << end.y << std::endl;
}