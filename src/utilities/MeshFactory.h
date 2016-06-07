#ifndef MESHFACTORY_H
#define MESHFACTORY_H

#include <renderer/YiMesh.h>

class MeshFactory
{
public:
    MeshFactory();
    ~MeshFactory();

    static CYISharedPtr<CYIMesh> CreateGridMesh(YI_UINT32 nColumnCount, YI_UINT32 nRowCount, YI_UINT32 nTileWidth, YI_UINT32 nTileHeight);
    static CYISharedPtr<CYIAssetBufferObjectData> BuildGridVertices(YI_UINT32 nColumnCount, YI_UINT32 nRowCount, YI_UINT32 nTileWidth, YI_UINT32 nTileHeight);
    static CYISharedPtr<CYIAssetBufferObjectData> BuildGridFaces(YI_UINT32 nColumnCount, YI_UINT32 nRowCount, YI_INT32 &rIndicesCount);
};

#endif
