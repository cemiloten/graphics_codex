#pragma once
#include <G3D/G3D.h>

class IndexedTriangleList {
public:
    Array<Point3> vertexArray;
    Array<int> indexArray;
    
    Triangle triangle(int t) const {
        return Triangle (
            vertexArray[indexArray[t * 3]],
            vertexArray[indexArray[t * 3 + 1]],
            vertexArray[indexArray[t * 3 + 2]]);
    }
    
    int size() const {
        return indexArray.size() / 3;
    }
    
    void saveAsOff(const String filename) {
        return;
    }
};
