#include <config.h>

#include <triangulate.h>
#include <geometry.h>

bool isConvex(glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    float cross = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    return cross > 1e-8f;
}

bool isPointInTri(glm::vec2 p, glm::vec2 a, glm::vec2 b, glm::vec2 c) {
    auto sign = [](glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
        return (p1.x - p3.x) * (p2.y - p3.y) - (p2.x - p3.x) * (p1.y - p3.y);
    };
    
    float d1 = sign(p, a, b);
    float d2 = sign(p, b, c);
    float d3 = sign(p, c, a);
    
    bool hasNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
    bool hasPos = (d1 > 1e-8f) || (d2 > 1e-8f) || (d3 > 1e-8f);
    
    return !(hasNeg && hasPos);
}

void Triangulate::triangulatePolygon() {
    indices.clear();
    if (polygon.size() < 3) return;

    std::vector<int> idx(polygon.size());
    for (int i = 0; i < polygon.size(); i++) idx[i] = i;

    while (idx.size() > 2) {
        bool earFound = false;
        
        for (int i = 0; i < idx.size(); i++) {
            int prev = idx[(i - 1 + idx.size()) % idx.size()];
            int curr = idx[i];
            int next = idx[(i + 1) % idx.size()];
            
            glm::vec2 a = polygon[prev];
            glm::vec2 b = polygon[curr];
            glm::vec2 c = polygon[next];
            
            // check if ear is convex
            if (!isConvex(a, b, c)) continue;
            
            // check if any other point is inside this triangle
            bool hasInside = false;
            for (int j = 0; j < idx.size(); j++) {
                if (j == (i - 1 + idx.size()) % idx.size() || j == i || j == (i + 1) % idx.size()) continue;
                if (isPointInTri(polygon[idx[j]], a, b, c)) {
                    hasInside = true;
                    break;
                }
            }
            
            if (!hasInside) {
                indices.push_back(prev);
                indices.push_back(curr);
                indices.push_back(next);
                idx.erase(idx.begin() + i);
                earFound = true;
                break;
            }
        }
        
        if (!earFound) break;
    }
}