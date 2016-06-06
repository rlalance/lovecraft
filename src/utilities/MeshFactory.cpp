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

    /*  example 5 x 3 grid
        shared corners are duplicated to hold the different UVs
        c = column
        r = Row

           c0       c1      c2      c3      c4
        0------1,2-----3,4-----5,6-----7,8------9
        |       |       |       |       |       |
    r0  |       |       |       |       |       |
        |       |       |       |       |       |
       10-----11,12---13,14---15,16---17,18----19 
        |       |       |       |       |       |
    r1  |       |       |       |       |       |
        |       |       |       |       |       |
       20-----21,22---23,24---25,26---27,28----29
        |       |       |       |       |       |
    r2  |       |       |       |       |       |
        |       |       |       |       |       |
       30-----31,32---33,34---35,36---37,38----39

    */

    YI_INT32 rIndicesCount;

    pVBOAsset = BuildGridVertices(nColumnCount, nRowCount, nTileWidth, nTileHeight);
    pIBOAsset = BuildGridFaces(nColumnCount, nRowCount, rIndicesCount);

    pNPatchMesh->AddVertexBuffer(pVBOAsset, CYIMesh::YI_BUFFER_OWNED);
    pNPatchMesh->SetIndexBuffer(pIBOAsset, CYIMesh::YI_BUFFER_OWNED);

    //Configure the mesh attribute bindings
    CYIMesh::YI_ATTRIBUTE_BINDING PositionBinding;
    CYIMesh::YI_ATTRIBUTE_BINDING UVBinding;

    PositionBinding.uAttribute = CYIMesh::YI_POSITION;
    PositionBinding.uBufferIndex = 0;//As returned by AddVertexBuffer
    PositionBinding.uByteOffset = 0;
    PositionBinding.uNumElements = 3; //3 floats per attribute
    PositionBinding.uStride = 20; //There are 20 bytes per vertex in this buffer (3 pos, 2 tex)

    UVBinding.uAttribute = CYIMesh::YI_TEXCOORD;
    UVBinding.uBufferIndex = 0;
    UVBinding.uByteOffset = 12; //3 floats deep since position comes first (XYZUV)
    UVBinding.uNumElements = 2;
    UVBinding.uStride = 20; //There are 20 bytes per vertex in this buffer (3 pos, 2 tex)

    pNPatchMesh->AddBinding(PositionBinding);
    pNPatchMesh->AddBinding(UVBinding);

    /*
    This effectively adds support for scale down and the inverse of the parent scale.
    So we store the inverse of the scale since the node will be scaling us up too.
    Earlier, we also ensured that fScaleX and fScaleY are always >= YI_EPSILON
    */
    glm::mat4 scale = glm::mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f);

    pNPatchMesh->SetTransform(scale);

    //TODO
    CYIAABB bbox;
    bbox.SetCorners(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, MESH_FACTORY_UI_COMPONENT_Z_BIAS));

    pNPatchMesh->SetAxisAlignedBoundingBox(bbox);
    pNPatchMesh->SetPrimitive(CYIMesh::YI_TRIANGLES);
    pNPatchMesh->SetIndexCount(rIndicesCount);

    return pNPatchMesh;
}

CYISharedPtr<CYIAssetBufferObjectData> MeshFactory::BuildGridVertices(YI_UINT32 nColumnCount, YI_UINT32 nRowCount, YI_UINT32 nTileWidth, YI_UINT32 nTileHeight)
{
    YI_INT32 nNumHorizontalPatches = nColumnCount;
    YI_INT32 nNumVerticalPatches = nRowCount;

    YI_INT32 nNumHorizontalVertices = nNumHorizontalPatches * 2;
    YI_INT32 nNumVerticalVertices = nNumVerticalPatches * 2;

    YI_INT32 nVertexCount = (nNumHorizontalVertices)* (nNumVerticalVertices);

    CYISharedPtr<CYIAssetBufferObjectData> pVBOAsset(CYIBufferObjectFactory::CreateVertexBufferAsset());
    CYIAssetManager *pAM = CYIFramework::GetInstance()->GetAssetManager();

    YI_INT32 nCI = 0;        // Component Index

    YI_FLOAT *pfVBO = new YI_FLOAT[nVertexCount * MESH_FACTORY_XYZUV_COMPONENT_COUNT];

    /*  example 5 x 3 grid
        shared corners are duplicated to hold the different UVs
        c = column
        r = Row

            c0      c1      c2      c3      c4
        0------1,2-----3,4-----5,6-----7,8------9
        |       |       |       |       |       |
    r0  |       |       |       |       |       |
        |       |       |       |       |       |
        10-----11,12---13,14---15,16---17,18----19
        |       |       |       |       |       |
    r1  |       |       |       |       |       |
        |       |       |       |       |       |
        20-----21,22---23,24---25,26---27,28----29
        |       |       |       |       |       |
    r2  |       |       |       |       |       |
        |       |       |       |       |       |
        30-----31,32---33,34---35,36---37,38----39
    */

    for (int row = 0; row < nNumVerticalVertices; ++row)
    {
        for (int col = 0; col < nNumHorizontalVertices; ++col)
        {
            //x
            pfVBO[nCI++] = (YI_FLOAT)(col*nTileWidth);
            //y
            pfVBO[nCI++] = (YI_FLOAT)(row*nTileHeight);
            //z
            pfVBO[nCI++] = MESH_FACTORY_UI_COMPONENT_Z_BIAS;

            // u
            pfVBO[nCI++] = 0.0f;
            // v
            pfVBO[nCI++] = 0.0f;
        }
    }

    pVBOAsset->SetData((YI_UINT8*)pfVBO, nVertexCount, MESH_FACTORY_XYZUV_COMPONENT_COUNT * sizeof(YI_FLOAT));

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
    /*
    Creation of the triangles with indices into our vertex buffer.
    */
    YI_INT32 nNumHorizontalPatches = nColumnCount;
    YI_INT32 nNumVerticalPatches = nRowCount;

    CYIAssetManager *pAM = CYIFramework::GetInstance()->GetAssetManager();
    CYISharedPtr<CYIAssetBufferObjectData> pIBOAsset(CYIBufferObjectFactory::CreateIndexBufferAsset());

    rIndicesCount = (nNumVerticalPatches)* (nNumHorizontalPatches)* MESH_FACTORY_TRIANLES_PER_QUAD * MESH_FACTORY_INDICES_PER_TRIANGLE;

    YI_UINT16 *pu16IBO = new YI_UINT16[rIndicesCount];
    YI_INT32 nI = 0; // index
    YI_INT32 stride = nNumHorizontalPatches + 1;

    for (int row = 0; row<nNumVerticalPatches; row++)
    {
        for (int col = 0; col<nNumHorizontalPatches; col++)
        {
            pu16IBO[nI++] = row        * stride + col;      // 0 + offset
            pu16IBO[nI++] = (row + 1)  * stride + col;      // 6 + offset
            pu16IBO[nI++] = row        * stride + col + 1;  // 1 + offset

            pu16IBO[nI++] = row        * stride + col + 1;  // 1 + offset
            pu16IBO[nI++] = (row + 1)  * stride + col;      // 6 + offset
            pu16IBO[nI++] = (row + 1)  * stride + col + 1;  // 7 + offset
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

