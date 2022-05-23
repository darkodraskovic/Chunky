#include "model.h"

Mesh MeshAlloc(int vertexCount, int triangleCount) {
  Mesh mesh = {0};

  mesh.vertexCount = vertexCount;
  mesh.vertices = (float *)MemAlloc(vertexCount * 3 * sizeof(float));
  mesh.texcoords = (float *)MemAlloc(vertexCount * 2 * sizeof(float));
  mesh.normals = (float *)MemAlloc(vertexCount * 3 * sizeof(float));

  if (triangleCount > 0) {
    mesh.triangleCount = triangleCount;
    mesh.indices = (unsigned short *)MemAlloc(triangleCount * 3 * sizeof(unsigned short));
  }

  return mesh;
}
