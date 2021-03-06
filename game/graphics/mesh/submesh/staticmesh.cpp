#include "staticmesh.h"

StaticMesh::StaticMesh(const ZenLoad::PackedMesh &mesh) {
  static_assert(sizeof(Vertex)==sizeof(ZenLoad::WorldVertex),"invalid landscape vertex format");
  const Vertex* vert=reinterpret_cast<const Vertex*>(mesh.vertices.data());
  vbo = Resources::vbo<Vertex>  (vert,mesh.vertices.size());
  ibo = Resources::ibo<uint32_t>(mesh.indices.data(),mesh.indices.size());

  sub.resize(mesh.subMeshes.size());
  for(size_t i=0;i<mesh.subMeshes.size();++i){
    sub[i].texName   = mesh.subMeshes[i].material.texture;
    sub[i].material  = Resources::loadMaterial(mesh.subMeshes[i].material,mesh.isUsingAlphaTest);
    sub[i].iboOffset = mesh.subMeshes[i].indexOffset;
    sub[i].iboSize   = mesh.subMeshes[i].indexSize;
    }
  bbox.assign(mesh.bbox);
  }

StaticMesh::StaticMesh(const ZenLoad::PackedSkeletalMesh &mesh) {
  std::vector<Vertex> cvbo(mesh.vertices.size());
  for(size_t i=0;i<cvbo.size();++i){
    cvbo[i].color   = mesh.vertices[i].Color;

    cvbo[i].norm[0] = mesh.vertices[i].Normal.x;
    cvbo[i].norm[1] = mesh.vertices[i].Normal.y;
    cvbo[i].norm[2] = mesh.vertices[i].Normal.z;

    cvbo[i].uv[0]   = mesh.vertices[i].TexCoord.x;
    cvbo[i].uv[1]   = mesh.vertices[i].TexCoord.y;

    cvbo[i].pos[0]  = mesh.vertices[i].LocalPositions[0].x;
    cvbo[i].pos[1]  = mesh.vertices[i].LocalPositions[0].y;
    cvbo[i].pos[2]  = mesh.vertices[i].LocalPositions[0].z;
    }
  vbo = Resources::vbo<Vertex>(cvbo.data(),cvbo.size());

  sub.resize(mesh.subMeshes.size());
  for(size_t i=0;i<mesh.subMeshes.size();++i){
    sub[i].texName   = mesh.subMeshes[i].material.texture;
    sub[i].material  = Resources::loadMaterial(mesh.subMeshes[i].material,true);
    sub[i].iboOffset = mesh.subMeshes[i].indexOffset;
    sub[i].iboSize   = mesh.subMeshes[i].indexSize;
    //sub[i].ibo      = Resources::ibo(mesh.subMeshes[i].indices.data(),mesh.subMeshes[i].indices.size());
    }
  bbox.assign(mesh.bbox);
  }

StaticMesh::StaticMesh(const Material& mat, std::vector<Resources::Vertex> cvbo, std::vector<uint32_t> cibo) {
  vbo = Resources::vbo<Vertex>(cvbo.data(),cvbo.size());
  ibo = Resources::ibo(cibo.data(),cibo.size());
  sub.resize(1);
  for(size_t i=0;i<1;++i) {
    sub[i].texName        = "";
    sub[i].material       = mat;
    sub[i].iboSize        = ibo.size();
    }
  bbox.assign(cvbo);
  }
