#include <config.h>
#include <pathfind.h>
#include <geometry.h>
#include <util.h>

void PathFind::bfs() {
    int n = nodes.size();
    std::vector<int> parent(n, -1);
    std::vector<bool> visited(n, 0);
    
    std::queue<int> q;

    visited[startTriangle] = 1;
    q.push(startTriangle);

    while(!q.empty()) {
        int curr = q.front();
        q.pop();
        
        if (curr == endTriangle) break;

        for (int link : nodes[curr].links) {
            if (!visited[link]) {
                visited[link] = 1;
                q.push(link);
                parent[link] = curr;
            }
        }
    }

    int curr = endTriangle;
    while (curr != -1) {
        nodes[curr].partOfPath = 1;
        curr = parent[curr];
    }
}

void PathFind::createGraph() {
    nodes.clear();
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