#include "ObjLoader.h"

namespace Lib
{
    ObjLoader::ObjLoader()
    {
    }

    ObjLoader::~ObjLoader()
    {
    }

    // objメッシュの読み込み
    bool ObjLoader::LoadMesh(const char *filePath, ObjMesh &mesh)
    {
        std::ifstream ifs(filePath);

        if (ifs.fail()) {
            std::cout << "failed" << std::endl;
            return false;
        }

        std::string buf;
        std::string::size_type comment_start = 0;
        while (!ifs.eof()) {
            std::getline(ifs, buf);

            // '#'はコメントとして無視
            if ((comment_start = buf.find('#')) != std::string::size_type(-1)) {
                buf = buf.substr(0, comment_start);
            }

            // 行頭のスペース、タブを削除(基本的には無いはず)
            ignoreSpace(buf);

            // 空行を無視
            if (buf.empty()) {
                continue;
            }

            if (buf[0] == 'v') {
                auto tmp = split(buf, ' ');
                // 先頭の文字列が'v'
                if (tmp[0] == "v") {
                    Vec3 vec;
                    vec.x = std::stof(tmp[1]);
                    vec.y = std::stof(tmp[2]);
                    vec.z = std::stof(tmp[3]);
                    mesh.vertexes.push_back(vec);
                }
                // 先頭の文字列が'vt'
                else if (tmp[0] == "vt") {
                    Vec2 vec;
                    vec.x = std::stof(tmp[1]);
                    vec.y = std::stof(tmp[2]);
                    mesh.vtexcoords.push_back(vec);
                }
                // 先頭の文字列が'vn'
                else if (tmp[0] == "vn") {
                    Vec3 vec;
                    vec.x = std::stof(tmp[1]);
                    vec.y = std::stof(tmp[2]);
                    vec.z = std::stof(tmp[3]);
                    mesh.vnormals.push_back(vec);
                }
            }
            else if (buf[0] == 'f') {
                auto tmp = split(buf, ' ');
                //TODO: 1行に3ブロックじゃないと動かない
                auto indexes1 = split(tmp[1], '/');
                mesh.face.vertexIndex.push_back(std::stoi(indexes1[0]));
                mesh.face.uvIndex.push_back(std::stoi(indexes1[1]));
                mesh.face.normalIndex.push_back(std::stoi(indexes1[2]));
                auto indexes2 = split(tmp[2], '/');
                mesh.face.vertexIndex.push_back(std::stoi(indexes2[0]));
                mesh.face.uvIndex.push_back(std::stoi(indexes2[1]));
                mesh.face.normalIndex.push_back(std::stoi(indexes2[2]));
                auto indexes3 = split(tmp[3], '/');
                mesh.face.vertexIndex.push_back(std::stoi(indexes3[0]));
                mesh.face.uvIndex.push_back(std::stoi(indexes3[1]));
                mesh.face.normalIndex.push_back(std::stoi(indexes3[2]));
            }
        }

        return true;
    }

    void ObjLoader::ignoreSpace(std::string & buf)
    {
        size_t pos;
        while ((pos = buf.find_first_of("  \t")) == 0) {
            buf.erase(buf.begin());
            if (buf.empty()) {
                break;
            }
        }
    }

    std::vector<std::string> ObjLoader::split(const std::string & input, char delimiter)
    {
        std::stringstream stream(input);

        std::string field;
        std::vector<std::string> result;
        while (std::getline(stream, field, delimiter)) {
            result.push_back(field);
        }

        return result;
    }
}