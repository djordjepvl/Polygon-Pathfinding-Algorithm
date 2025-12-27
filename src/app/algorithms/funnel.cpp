#include <config.h>
#include <funnel.h>

#include <geometry.h>
#include <util.h>


void Funnel::findPortals() {
    portals.clear();
    if (trianglePath.empty()) return;

    portals.push_back({startPoint, startPoint});

    for (int i = 0; i < trianglePath.size() - 1; i++) {
        int curr = trianglePath[i];
        int next = trianglePath[i+1];

        int sharedPts[2];
        int found = 0;
        
        for (int j=0; j<3; j++) {
            int currVert = indices[curr*3+j];
            for (int k=0; k<3; k++) {
                if (currVert == indices[next*3+k]) {
                    sharedPts[found++] = currVert;
                    if (found==2) break;
                }
            }
            if (found==2) break;
        }
        if (found < 2) {
            throw std::runtime_error("Funnel Error: Triangles in path are not adjacent (shared edge not found).");
        }

        int a = indices[curr * 3 + 0];
        int b = indices[curr * 3 + 1];
        int c = indices[curr * 3 + 2];
        int v1 = sharedPts[0], v2 = sharedPts[1];

        Portal newPortal;
        if ((v1 == a && v2 == b) || (v1 == b && v2 == c) || (v1 == c && v2 == a)) {
            newPortal.l = polygon[v2];
            newPortal.r = polygon[v1];
        } else {
            newPortal.l = polygon[v1];
            newPortal.r = polygon[v2];
        }
        
        portals.push_back(newPortal);
    }

    portals.push_back({endPoint, endPoint});
}