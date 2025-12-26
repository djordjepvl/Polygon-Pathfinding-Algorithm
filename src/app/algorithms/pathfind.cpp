#include <config.h>
#include <pathfind.h>
#include <geometry.h>
#include <util.h>

bool searching = 0;

int n;
std::vector<int> parent;
std::vector<bool> visited;
std::queue<int> q;

void PathFind::bfsStart() {
    for (auto& node : nodes) node.partOfPath = 0;
    n = nodes.size();
    parent.assign(n, -1);
    visited.assign(n, false);
    q = std::queue<int>();

    visited[startTriangle] = 1;
    q.push(startTriangle);
    searching = 1;
}

bool PathFind::bfsAdvance() {
    if (!searching) bfsStart();
    if (q.empty()) return 0;
    int curr = q.front();
    q.pop();
    
    if (curr == endTriangle) {
        bfsFinish();
        return 0;
    }

    for (int link : nodes[curr].links) {
        if (!visited[link]) {
            visited[link] = 1;
            q.push(link);
            parent[link] = curr;
        }
    }
    return 1;
}

void PathFind::bfsFinish() {
    if (!searching) return;
    int curr = endTriangle;
    while (curr != -1) {
        nodes[curr].partOfPath = 1;
        curr = parent[curr];
    }
    searching = 0;
}


void PathFind::createGraph() {
    nodes.clear();
    searching = 0;
    int n = indices.size() / 3;
    
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        triangleNode node = Geometry::makeTriangleNode();
        
        glm::vec2 a = polygon[indices[i]];
        glm::vec2 b = polygon[indices[i+1]];
        glm::vec2 c = polygon[indices[i+2]];
        
        node.x = avgFloat(a.x, b.x, c.x);
        node.y = avgFloat(a.y, b.y, c.y);
        nodes.push_back(node);
    }
    for (size_t i = 0; i + 2 < indices.size(); i += 3) {
        for (size_t j = i+3; j + 2 < indices.size(); j += 3) {
            int cnt = 0;
            for (int k=0; k<3; k++) {
                if (indices[j+k] == indices[i] ||
                    indices[j+k] == indices[i+1] ||
                    indices[j+k] == indices[i+2]
                ) cnt++;
            }
            if (cnt==2) {
                nodes[i/3].links.push_back(j/3);
                nodes[j/3].links.push_back(i/3);
            }
        }
    }
}