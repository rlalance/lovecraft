#include "MeshFactory.h"
#include <renderer/YiMeshFactory.h>
#include "../../../../uswish/core/dist/include/renderer/YiBufferObjectFactory.h"
#include "../../../../uswish/core/dist/include/framework/YiFramework.h"
#include <asset/YiAssetManager.h>

static const CYIString LOG_TAG("MeshFactory");

const YI_FLOAT MESH_FACTORY_UI_COMPONENT_Z_BIAS = 0.1f;
const YI_INT32 MESH_FACTORY_XYZUV_COMPONENT_COUNT = 5;
const YI_INT32 MESH_FACTORY_TRIANLES_PER_QUAD = 2;
const YI_INT32 MESH_FACTORY_INDICES_PER_TRIANGLE = 3;

MeshFactory::MeshFactory()
{
}

MeshFactory::~MeshFactory()
{
}

CYISharedPtr<CYIMesh> MeshFactory::CreateGridMesh(YI_UINT32 nColumnCount, YI_UINT32 nRowCount, YI_UINT32 nTileWidth, YI_UINT32 nTileHeight)
{
    CYISharedPtr<CYIAssetBufferObjectData> pVBOAsset;
    CYISharedPtr<CYIAssetBufferObjectData> pIBOAsset;

    CYISharedPtr<CYIMesh> pNPatchMesh(new CYIMesh());

    /*  example 2 x 2 grid
        corners are duplicated to hold the 4 different UVs required per grid cell
        c = column
        r = Row

              c0       c1   
           0------1,4-----5,
           |       |       |
       r0  |       |       |
           |       |       |
           2______3,6 _____7
           8      9,10   11,
           |       |       |
       r1  |       |       |
           |       |       |
           12-----13,14---15
    */

    YI_INT32 rIndicesCount;

    pVBOAsset = BuildGridVertices(nColumnCount, nRowCount, nTileWidth, nTileHeight);
    pIBOAsset = BuildGridFaces(nColumnCount, nRowCount, rIndicesCount);

    pNPatchMesh->AddVertexBuffer(pVBOAsset, CYIMesh::YI_BUFFER_OWNED);
    pNPatchMesh->SetIndexBuffer(pIBOAsset, CYIMesh::YI_BUFFER_OWNED);

    CYIMesh::YI_ATTRIBUTE_BINDING PositionBinding;
    CYIMesh::YI_ATTRIBUTE_BINDING UVBinding;

    PositionBinding.uAttribute      = CYIMesh::YI_POSITION;
    PositionBinding.uBufferIndex    = 0;    //As returned by AddVertexBuffer
    PositionBinding.uByteOffset     = 0;
    PositionBinding.uNumElements    = 3;    //3 floats per attribute
    PositionBinding.uStride         = 20;   //There are 20 bytes per vertex in this buffer (3 pos, 2 tex)

    UVBinding.uAttribute            = CYIMesh::YI_TEXCOORD;
    UVBinding.uBufferIndex          = 0;
    UVBinding.uByteOffset           = 12; //3 floats deep since position comes first (XYZUV)
    UVBinding.uNumElements          = 2;
    UVBinding.uStride               = 20; //There are 20 bytes per vertex in this buffer (3 pos, 2 tex)

    pNPatchMesh->AddBinding(PositionBinding);
    pNPatchMesh->AddBinding(UVBinding);

    glm::mat4 scale = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    pNPatchMesh->SetTransform(scale);

    CYIAABB bbox;
    bbox.SetCorners(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(nColumnCount * nTileWidth, nRowCount * nTileHeight, MESH_FACTORY_UI_COMPONENT_Z_BIAS));

    pNPatchMesh->SetAxisAlignedBoundingBox(bbox);
    pNPatchMesh->SetPrimitive(CYIMesh::YI_TRIANGLES);
    pNPatchMesh->SetIndexCount(rIndicesCount);

    return pNPatchMesh;
}

CYISharedPtr<CYIAssetBufferObjectData> MeshFactory::BuildGridVertices(YI_UINT32 nColumnCount, YI_UINT32 nRowCount, YI_UINT32 nTileWidth, YI_UINT32 nTileHeight)
{
    YI_INT32 nVertexCount = (nColumnCount) * (nRowCount) * 4;

    CYISharedPtr<CYIAssetBufferObjectData> pVBOAsset(CYIBufferObjectFactory::CreateVertexBufferAsset());

    CYIAssetManager *pAM = CYIFramework::GetInstance()->GetAssetManager();

    YI_INT32 nCI = 0;        // Component Index

    YI_FLOAT *pfVBO = new YI_FLOAT[nVertexCount * MESH_FACTORY_XYZUV_COMPONENT_COUNT];

    for (YI_UINT32 row = 0; row < nRowCount; ++row)
    {
        for (YI_UINT32 col = 0; col < nColumnCount; ++col)
        {
            pfVBO[nCI++] = static_cast<YI_FLOAT>(col * nTileWidth);         //x
            pfVBO[nCI++] = static_cast<YI_FLOAT>(row * nTileHeight);        //y
            pfVBO[nCI++] = MESH_FACTORY_UI_COMPONENT_Z_BIAS;                //z
            pfVBO[nCI++] = 0.0f;                                            //u
            pfVBO[nCI++] = 0.0f;                                            //v

            pfVBO[nCI++] = static_cast<YI_FLOAT>(col * nTileWidth);         //x
            pfVBO[nCI++] = static_cast<YI_FLOAT>((row + 1) * nTileHeight);  //y
            pfVBO[nCI++] = MESH_FACTORY_UI_COMPONENT_Z_BIAS;                //z
            pfVBO[nCI++] = 0.0f;                                            //u
            pfVBO[nCI++] = 1.0f;                                            //v

            pfVBO[nCI++] = static_cast<YI_FLOAT>((col + 1) * nTileWidth);   //x
            pfVBO[nCI++] = static_cast<YI_FLOAT>(row * nTileHeight);        //y
            pfVBO[nCI++] = MESH_FACTORY_UI_COMPONENT_Z_BIAS;                //z
            pfVBO[nCI++] = 1.0f;                                            //u
            pfVBO[nCI++] = 0.0f;                                            //v

            pfVBO[nCI++] = static_cast<YI_FLOAT>((col + 1) * nTileWidth);   //x
            pfVBO[nCI++] = static_cast<YI_FLOAT>((row + 1) * nTileHeight);  //y
            pfVBO[nCI++] = MESH_FACTORY_UI_COMPONENT_Z_BIAS;                //z
            pfVBO[nCI++] = 1.0f;                                            //u
            pfVBO[nCI++] = 1.0f;                                            //v
        }
    }

    pVBOAsset->SetData((YI_UINT8*)pfVBO, nVertexCount * MESH_FACTORY_XYZUV_COMPONENT_COUNT, sizeof(YI_FLOAT));

    delete[] pfVBO;
    pfVBO = YI_NULL;

    if (!pAM->AddAsset(pVBOAsset))
    {
        YI_LOGE(LOG_TAG, "Could not add vertex buffer object asset");
        return CYISharedPtr<CYIAssetBufferObjectData>(YI_NULL);
    }

    return pVBOAsset;
}

CYISharedPtr<CYIAssetBufferObjectData> MeshFactory::BuildGridFaces(YI_UINT32 nColumnCount, YI_UINT32 nRowCount, YI_INT32 &rIndicesCount)
{
    CYIAssetManager *pAM = CYIFramework::GetInstance()->GetAssetManager();
    CYISharedPtr<CYIAssetBufferObjectData> pIBOAsset(CYIBufferObjectFactory::CreateIndexBufferAsset());

    rIndicesCount = (nRowCount) * (nColumnCount) * MESH_FACTORY_TRIANLES_PER_QUAD * MESH_FACTORY_INDICES_PER_TRIANGLE;

    YI_UINT16 *pu16IBO = new YI_UINT16[rIndicesCount];
    YI_INT32 nI = 0; // index
    YI_UINT32 si = 0;

    for (YI_UINT32 row = 0; row < nRowCount; row++)
    {
        for (YI_UINT32 col = 0; col < nColumnCount; col++)
        {
            pu16IBO[nI++] = si;    
            pu16IBO[nI++] = si + 1;
            pu16IBO[nI++] = si + 2;

            pu16IBO[nI++] = si + 2;
            pu16IBO[nI++] = si + 1;
            pu16IBO[nI++] = si + 3;
            si += 4;
        }
    }

    pIBOAsset->SetData((YI_UINT8*)pu16IBO, rIndicesCount, sizeof(YI_INT16));

    delete[] pu16IBO;
    pu16IBO = YI_NULL;

    if (!pAM->AddAsset(pIBOAsset))
    {
        YI_ASSERT(false, LOG_TAG, "Could not add index buffer object asset");
        return CYISharedPtr<CYIAssetBufferObjectData>(YI_NULL);
    }

    return pIBOAsset;
}

