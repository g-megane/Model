#include "FBXLoader.h"

namespace Lib
{
    FBXLoader::FBXLoader()
    {
        polygonCount = 0;
        vertexCount  = 0;
        indexCount   = 0;
        vertexBuffer = nullptr;
        indexBuffer  = nullptr;
        init();
    }

    FBXLoader::~FBXLoader()
    {
    }

    bool FBXLoader::init()
    {
        FbxManager *manager = nullptr; // SDK全体を管理するクラス
        FbxScene   *scene   = nullptr; // このクラスを通じて頂点情報などにアクセスしていく

        // SDK全体を管理するクラスを生成
        manager = FbxManager::Create();

        if (manager != nullptr) {
            // FbxImporterを生成
            FbxImporter *importer = FbxImporter::Create(manager, "");

            if (importer != nullptr) {
                // FbxSceneの作成
                scene = FbxScene::Create(manager, "");

                if (scene != nullptr) {
                    // FBXファイルを開く
                    bool result = importer->Initialize("TestModel.fbx");

                    // 準備完了
                    if (result) {
                        // FBXファイルをsceneに読み込む
                        result = importer->Import(scene);

                        // ルートノードを取得
                        FbxNode *rootNode = scene->GetRootNode();

                        if (rootNode != nullptr) {
                            // ルートノードの子ノード数を取得
                            int childCount = rootNode->GetChildCount();

                            // 子ノードの数だけ探索
                            for (int i = 0; i < childCount; ++i) {
                                getMesh(rootNode->GetChild(i));
                            }
                        }
                    }
                }
            }
            // 必要が無くなったFbxImporterの破棄
            importer->Destroy();


            return false;
        }

        // FbxManagerの破棄(関連付けられたクラスも全て破棄される)
        manager->Destroy();


        delete[] vertexBuffer;
        delete[] indexBuffer;

        return true;
    }

    void FBXLoader::getMesh(FbxNode * node)
    {
        // ノードの属性を取得
        FbxNodeAttribute *attr = node->GetNodeAttribute();

        if (attr != nullptr) {
            // 属性を判別
            switch (attr->GetAttributeType()) {
                // メッシュノードの発見
                case FbxNodeAttribute::eMesh:
                {
                    FbxMesh *mesh = node->GetMesh();
                    getPosition(mesh);
                }
                break;
            }
        }

        // 子ノードの再帰探索
        int childCount = node->GetChildCount();
        for (int i = 0; i < childCount; ++i) {
            getMesh(node->GetChild(i));
        }
    }

    void FBXLoader::getPosition(FbxMesh * mesh)
    {
        // ポリゴン数を取得
        polygonCount = mesh->GetPolygonCount();
        // 総長点数を取得
        vertexCount = mesh->GetControlPointsCount();
        // 頂点バッファの取得
        FbxVector4 *vertex = mesh->GetControlPoints();
        // インデックスバッファの取得
        int *index = mesh->GetPolygonVertices();
        // 頂点座標のキャストとハードコピー
        vertexBuffer = new Vertex[vertexCount];
         
        for (int i = 0; i < vertexCount; ++i) {
            // i番目の頂点の座標Xを取得
            vertexBuffer[i].x = static_cast<float>(vertex[i][0]);
            // i番目の頂点の座標Xを取得
            vertexBuffer[i].y = static_cast<float>(vertex[i][1]);
            // i番目の頂点の座標Xを取得
            vertexBuffer[i].z = static_cast<float>(vertex[i][2]);
        }

        // インデックスバッファのハードコピー
        indexBuffer = new int[indexCount];

        for (int i = 0; i < indexCount; ++i) {
            indexBuffer[i] = index[i];
        }
    }
}