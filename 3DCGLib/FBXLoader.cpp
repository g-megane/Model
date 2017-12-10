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
        FbxManager *manager = nullptr; // SDK�S�̂��Ǘ�����N���X
        FbxScene   *scene   = nullptr; // ���̃N���X��ʂ��Ē��_���ȂǂɃA�N�Z�X���Ă���

        // SDK�S�̂��Ǘ�����N���X�𐶐�
        manager = FbxManager::Create();

        if (manager != nullptr) {
            // FbxImporter�𐶐�
            FbxImporter *importer = FbxImporter::Create(manager, "");

            if (importer != nullptr) {
                // FbxScene�̍쐬
                scene = FbxScene::Create(manager, "");

                if (scene != nullptr) {
                    // FBX�t�@�C�����J��
                    bool result = importer->Initialize("TestModel.fbx");

                    // ��������
                    if (result) {
                        // FBX�t�@�C����scene�ɓǂݍ���
                        result = importer->Import(scene);

                        // ���[�g�m�[�h���擾
                        FbxNode *rootNode = scene->GetRootNode();

                        if (rootNode != nullptr) {
                            // ���[�g�m�[�h�̎q�m�[�h�����擾
                            int childCount = rootNode->GetChildCount();

                            // �q�m�[�h�̐������T��
                            for (int i = 0; i < childCount; ++i) {
                                getMesh(rootNode->GetChild(i));
                            }
                        }
                    }
                }
            }
            // �K�v�������Ȃ���FbxImporter�̔j��
            importer->Destroy();


            return false;
        }

        // FbxManager�̔j��(�֘A�t����ꂽ�N���X���S�Ĕj�������)
        manager->Destroy();


        delete[] vertexBuffer;
        delete[] indexBuffer;

        return true;
    }

    void FBXLoader::getMesh(FbxNode * node)
    {
        // �m�[�h�̑������擾
        FbxNodeAttribute *attr = node->GetNodeAttribute();

        if (attr != nullptr) {
            // �����𔻕�
            switch (attr->GetAttributeType()) {
                // ���b�V���m�[�h�̔���
                case FbxNodeAttribute::eMesh:
                {
                    FbxMesh *mesh = node->GetMesh();
                    getPosition(mesh);
                }
                break;
            }
        }

        // �q�m�[�h�̍ċA�T��
        int childCount = node->GetChildCount();
        for (int i = 0; i < childCount; ++i) {
            getMesh(node->GetChild(i));
        }
    }

    void FBXLoader::getPosition(FbxMesh * mesh)
    {
        // �|���S�������擾
        polygonCount = mesh->GetPolygonCount();
        // �����_�����擾
        vertexCount = mesh->GetControlPointsCount();
        // ���_�o�b�t�@�̎擾
        FbxVector4 *vertex = mesh->GetControlPoints();
        // �C���f�b�N�X�o�b�t�@�̎擾
        int *index = mesh->GetPolygonVertices();
        // ���_���W�̃L���X�g�ƃn�[�h�R�s�[
        vertexBuffer = new Vertex[vertexCount];
         
        for (int i = 0; i < vertexCount; ++i) {
            // i�Ԗڂ̒��_�̍��WX���擾
            vertexBuffer[i].x = static_cast<float>(vertex[i][0]);
            // i�Ԗڂ̒��_�̍��WX���擾
            vertexBuffer[i].y = static_cast<float>(vertex[i][1]);
            // i�Ԗڂ̒��_�̍��WX���擾
            vertexBuffer[i].z = static_cast<float>(vertex[i][2]);
        }

        // �C���f�b�N�X�o�b�t�@�̃n�[�h�R�s�[
        indexBuffer = new int[indexCount];

        for (int i = 0; i < indexCount; ++i) {
            indexBuffer[i] = index[i];
        }
    }
}