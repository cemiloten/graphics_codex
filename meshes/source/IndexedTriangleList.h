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
        TextOutput to(filename);
        to.writeSymbols("OFF");
        to.writeNewlines(2);
        //to.writeSymbols(
        //    format(
        //        "%d %d %d",
        //        vertexArray.size(),
        //        indexArray.size() / 3,
        //        vertexArray.size() + indexArray.size() / 3));

        to.writeSymbols(
            format(
                "%d %d 0",
                vertexArray.size(),
                indexArray.size() / 3));
        to.writeNewline();

        for (int i = 0; i < vertexArray.size(); ++i) {
            to.writeSymbols(
                format(
                    "%f %f %f",
                    vertexArray[i].x,
                    vertexArray[i].y,
                    vertexArray[i].z));
            to.writeNewline();
        }

        for (int i = 0; i < indexArray.size();  i += 3) {
            to.writeSymbols(
                format(
                    "3  %d %d %d",
                    indexArray[i],
                    indexArray[i + 1],
                    indexArray[i + 2]));
            to.writeNewline();
        }

        to.commit();
        return;
    }
};
