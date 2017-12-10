#pragma once
#ifndef FBXLOADER_H
#define FBXLOADER_H
#include <fbxsdk.h>

namespace Lib
{
    class FBXLoader
    {
    public:
        typedef struct VERTEX 
        {
            float x;
            float y;
            float z;
        } Vertex;

        FBXLoader();
        ~FBXLoader();

    private:
        bool init();
        void getMesh(FbxNode *node);
        void getPosition(FbxMesh *mesh);

        int polygonCount;
        int vertexCount;
        int indexCount;
        Vertex *vertexBuffer;
        int *indexBuffer;
    };
}

#endif