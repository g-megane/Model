#include <d3dcompiler.h>
#include "Model.h"
#include "ObjLoader.h"

namespace Lib
{
    // �R���X�g���N�^
    Model::Model()
    {
        world = Matrix::Identify;
        init();
    }

    // �f�X�g���N�^
    Model::~Model()
    {
    }

    // ���f���̕`��
    void Model::render(Color &color)
    {
        auto &directX = DirectX11::getInstance();

        ConstantBuffer cb;
        cb.world      = Matrix::transpose(world);
        cb.view       = Matrix::transpose(directX.getViewMatrix());
        cb.projection = Matrix::transpose(directX.getProjectionMatrix());
        directX.getDeviceContext()->UpdateSubresource(constantBuffer.Get(), 0, nullptr, &cb, 0, 0);

        directX.getDeviceContext()->VSSetShader(vertexShader.Get(), nullptr, 0);
        directX.getDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffer.GetAddressOf());
        directX.getDeviceContext()->PSSetShader(pixelShader.Get(), nullptr, 0);

        directX.getDeviceContext()->DrawIndexed(36, 0, 0);
    }

    // ���[���h�s���ݒ�
    void Model::setWorldMatrix(Matrix & _world)
    {
        world = _world;
    }

    // ���[���h�s����擾
    Matrix Model::getWorldMatrix() const
    {
        return world;
    }

    // ������
    HRESULT Model::init()
    {
        auto &directX = DirectX11::getInstance();
        auto hr = S_OK;

        // VertexShader�̓ǂݍ���
        auto VSBlob = shaderCompile(L"VertexShader.hlsl", "VS", "vs_4_0");
        if (VSBlob == nullptr) {
            MessageBox(nullptr, L"shaderCompile()�̎��s(VS)", L"Error", MB_OK);
            return hr;
        }

        // VertexShader�̍쐬
        hr = directX.getDevice()->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), nullptr, vertexShader.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"VS�R���p�C�����s", L"Error", MB_OK);
            return hr;
        }

        // InputLayou�̒�`
        D3D11_INPUT_ELEMENT_DESC layout[] = {
            { "POSITION", 0,    DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        };
        UINT numElements = ARRAYSIZE(layout);

        // InputLayout�̍쐬
        hr = directX.getDevice()->CreateInputLayout(layout, numElements, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), vertexLayout.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"CreateInputLayout�̎��s(VS)", L"Error", MB_OK);
            return hr;
        }

        // InputLayout���Z�b�g
        directX.getDeviceContext()->IASetInputLayout(vertexLayout.Get());

        // PixelShader�̓ǂݍ���
        auto PSBlob = shaderCompile(L"PixelShader.hlsl", "PS", "ps_4_0");
        if (PSBlob == nullptr) {
            MessageBox(nullptr, L"shaderCompile()�̎��s(VS)", L"Error", MB_OK);
            return hr;
        }

        // VertexShader�̍쐬
        hr = directX.getDevice()->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), nullptr, pixelShader.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createPixelShader()�̎��s", L"Error", MB_OK);
            return hr;
        }

        // VertexBuffer�̒�`
        //SimpleVertex vertices[] =
        //{
        //    { { -1.0f,  1.0f, -1.0f },{ 0.0f, 0.0f, 1.0f, 1.0f } },
        //    { {  1.0f,  1.0f, -1.0f },{ 0.0f, 1.0f, 0.0f, 1.0f } },
        //    { {  1.0f,  1.0f,  1.0f },{ 0.0f, 1.0f, 1.0f, 1.0f } },
        //    { { -1.0f,  1.0f,  1.0f },{ 1.0f, 0.0f, 0.0f, 1.0f } },
        //    { { -1.0f, -1.0f, -1.0f },{ 1.0f, 0.0f, 1.0f, 1.0f } },
        //    { {  1.0f, -1.0f, -1.0f },{ 1.0f, 1.0f, 0.0f, 1.0f } },
        //    { {  1.0f, -1.0f,  1.0f },{ 1.0f, 1.0f, 1.0f, 1.0f } },
        //    { { -1.0f, -1.0f,  1.0f },{ 0.0f, 0.0f, 0.0f, 1.0f } },
        //};

        ObjMesh mesh;

        ObjLoader loader;
        if (!loader.LoadMesh("Mesh/Test.obj", mesh)) {
            MessageBox(nullptr, L"LoadMesh()�̎��s", L"Error", MB_OK);
            return E_FAIL;
        }
        // vector�̗v�f�̃T�C�Y�Ɣz��̃T�C�Y�����������ׂ�
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(Vec3) * 8;
        bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bd.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData = {0};
        ZeroMemory(&initData, sizeof(initData));
        initData.pSysMem = mesh.vertexes.data();
        hr = directX.getDevice()->CreateBuffer(&bd, &initData, vertexBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()�̎��s", L"Error", MB_OK);
            return hr;
        }

        // VertexBuffer���Z�b�g
        UINT stride = sizeof(Vec3);
        UINT offset = 0;
        directX.getDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);

        // ���_�o�b�t�@�̍쐬
        //WORD indices[] =
        //{
        //    3, 1, 0,
        //    2, 1, 3,

        //    0, 5, 4,
        //    1, 5, 0,

        //    3, 4, 7,
        //    0, 4, 3,

        //    1, 6, 5,
        //    2, 6, 1,

        //    2, 7, 6,
        //    3, 7, 2,

        //    6, 4, 5,
        //    7, 4, 6,
        //};
        bd.Usage = D3D11_USAGE_DEFAULT;
        bd.ByteWidth = sizeof(WORD) * 36;//mesh.face.vertexIndex.size(); // 36���_�A12�O�p�`
        bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bd.CPUAccessFlags = 0;
        initData.pSysMem = mesh.face.vertexIndex.data();
        hr = directX.getDevice()->CreateBuffer(&bd, &initData, indexBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()�̎��s", L"Error", MB_OK);
            return hr;
        }

        // ���_�o�b�t�@���Z�b�g
        directX.getDeviceContext()->IASetIndexBuffer(indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

        // PrimitiveTopology���Z�b�g
        directX.getDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

        // ConstantBuffer�̍쐬
        bd.Usage          = D3D11_USAGE_DEFAULT;
        bd.ByteWidth      = sizeof(ConstantBuffer);
        bd.BindFlags      = D3D11_BIND_CONSTANT_BUFFER;
        bd.CPUAccessFlags = 0;
        hr = directX.getDevice()->CreateBuffer(&bd, nullptr, constantBuffer.GetAddressOf());
        if (FAILED(hr)) {
            MessageBox(nullptr, L"createBuffer()�̎��s", L"Error", MB_OK);
            return hr;
        }

        return S_OK;
    }

    // �V�F�[�_�[�̓ǂݍ���
    Microsoft::WRL::ComPtr<ID3DBlob> Model::shaderCompile(WCHAR * filename, LPCSTR entryPoint, LPCSTR shaderModel)
    {
        Microsoft::WRL::ComPtr<ID3DBlob> blobOut = nullptr;
        Microsoft::WRL::ComPtr<ID3DBlob> errorBlob = nullptr;

        DWORD shaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined(DEBUG) || defined(_DEBUG)
        shaderFlags |= D3DCOMPILE_DEBUG;
#endif

        auto hr = D3DCompileFromFile(
            filename,
            nullptr,
            nullptr,
            entryPoint,
            shaderModel,
            shaderFlags,
            0,
            blobOut.GetAddressOf(),
            errorBlob.GetAddressOf()
        );

        if (FAILED(hr)) {
            if (errorBlob != nullptr) {
                MessageBox(nullptr, static_cast<LPWSTR>(errorBlob->GetBufferPointer()), nullptr, MB_OK);
            }
            if (errorBlob) {
                errorBlob.Get()->Release();
            }
        }
        if (errorBlob) {
            errorBlob.Get()->Release();
        }

        return Microsoft::WRL::ComPtr<ID3DBlob>(blobOut);
    }
}