#pragma once
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

namespace Lib
{
    struct Vec3
    {
        float x;
        float y;
        float z;
    };

    struct Vec2
    {
        float x;
        float y;
    };

    struct Face
    {
        std::vector<WORD> vertexIndex;
        std::vector<WORD> uvIndex;
        std::vector<WORD> normalIndex;
    };

    struct ObjMesh
    {
        std::vector<Vec3> vertexes;
        std::vector<Vec2> vtexcoords;
        std::vector<Vec3> vnormals;
        Face face;
    };


    class ObjLoader
    {
    public:
        ObjLoader();
        ~ObjLoader();

        bool LoadMesh(const char *filePath, ObjMesh &mesh);

    private:
        void ignoreSpace(std::string &buf);
        std::vector<std::string> split(const std::string& input, char delimiter);
        void combertPoligon3(ObjMesh &_mesh);
    };
}

#endif