#include <config.h>
#include <funnel.h>

#include <geometry.h>
#include <util.h>


static bool finding = 0;
static bool finished = 0;

static int funnelState = -1;

glm::vec2 apex;
glm::vec2 currL;
glm::vec2 currR;

int currPrtl = 0;

void Funnel::funnelStart() {
    currPrtl = -1;
    finding = 1;
    path.clear();
    leftIndx = -1;
    leftPoint = startPoint;
    rightIndx = -1;
    rightPoint = startPoint;
    path.push_back(startPoint);
    funnelState = 0;
}
    
bool leftFunnelAdvance() {
    if (leftPoint == apex) {
        leftPoint = currL;
        leftIndx = currPrtl;
    }
    else if (crossProduct(apex, leftPoint, currL) <= 0.0f) {
        leftPoint = currL;
        leftIndx = currPrtl;
        if (crossProduct(apex, leftPoint, rightPoint) >= 0.0f) {
            path.push_back(rightPoint);
            apex = rightPoint;
            leftPoint = apex;
            rightPoint = apex;
            currPrtl = rightIndx;
            leftIndx = rightIndx;
            return 1;
        }
    }
    return 0;
}
bool rightFunnelAdvance() {
    if (rightPoint == apex) {
        rightPoint = currR;
        rightIndx = currPrtl;
    }
    else if (crossProduct(apex, rightPoint, currR) >= 0.0f) {
        rightPoint = currR;
        rightIndx = currPrtl;
        if (crossProduct(apex, rightPoint, leftPoint) <= 0.0f) {
            path.push_back(leftPoint);
            apex = leftPoint;
            rightPoint = apex;
            leftPoint = apex;
            rightIndx = leftIndx;
            currPrtl = leftIndx;
            return 1;
        }
    }
    return 0;
}
bool portalAdvance() {
    currPrtl++;
    if (currPrtl >= portals.size()) {
        Funnel::funnelEnd();
        return 0;
    }

    apex = path.back();
    currL = portals[currPrtl].l;
    currR = portals[currPrtl].r;
    return 1;
}
bool Funnel::funnelAdvance() {
    if (finished) return 0;
    if (!finding) funnelStart();
    
    if (funnelState == 0) {
        if (!portalAdvance()) return 0;
        funnelState = 1;
    }
    else if (funnelState == 1) {
        leftFunnelAdvance();
        funnelState = 2;
    }
    else if (funnelState == 2) {
        rightFunnelAdvance();
        funnelState = 0;
    }

    return 1;
}
void Funnel::funnelEnd() {
    apex = path.back();
    if (path.back() != endPoint) path.push_back(endPoint);
    finding = 0;
    finished = 1;
    funnelState = -1;
}


void Funnel::findPortals() {
    finding = 0;
    finished = 0;
    currPrtl = 0;
    funnelState = -1;
    portals.clear();
    path.clear();
    if (trianglePath.empty()) return;

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