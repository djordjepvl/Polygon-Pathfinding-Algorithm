#include <config.h>
#include <pathfind.h>

#include <geometry.h>
#include <util.h>

static bool searching = 0;
static bool finished = 0;

int n;
std::queue<int> q;

void PathFind::bfsStart() {
    n = nodes.size();
    q = std::queue<int>();

    nodes[startTriangle].visited = 1;
    nodes[startTriangle].saturation = 0.0f;
    q.push(startTriangle);
    searching = 1;
}

bool PathFind::bfsAdvance() {
    if (finished) return 0;
    if (!searching) bfsStart();
    if (q.empty()) return 0;
    if (nodes[endTriangle].visited) {
        bfsFinish();
        return 0;
    }

    
    bool found = 0;
    int curr = q.front();
    nodes[curr].saturation = 0.35f;
    q.pop();

    for (int link : nodes[curr].links) {
        if (!nodes[link].visited) {
            nodes[link].visited = 1;
            nodes[link].saturation = 0.0f;
            q.push(link);
           nodes[link].parent = curr;
           found = 1;
        }
    }
    if (!found) PathFind::bfsAdvance();
    return 1;
}

void PathFind::bfsFinish() {
    if (!searching) return;
    int curr = endTriangle;
    while (curr != -1) {
        
        nodes[curr].partOfPath = 1;
        trianglePath.push_back(curr);
        curr = nodes[curr].parent;
    }
    std::reverse(trianglePath.begin(), trianglePath.end());
    
    for (auto& node : nodes) {
        node.visited = 0;
        node.saturation = 1.0f;
        
    }
    searching = 0;
    finished = 1;
}


void PathFind::createGraph() {
    nodes.clear();
    searching = 0;
    finished = 0;
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