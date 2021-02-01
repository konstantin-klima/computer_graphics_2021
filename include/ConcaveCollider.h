//
// Created by rentorious on 01/02/2021.
//

#ifndef PROJECT_BASE_CONCAVECOLLIDER_H
#define PROJECT_BASE_CONCAVECOLLIDER_H

#include <vector>
#include <iostream>


#include "learnopengl//mesh.h"
#include "reactphysics3d/reactphysics3d.h"

class ConcaveCollider {
private:
    std::vector<std::vector<float>> mVertices;
    std::vector<std::vector<float>> mNormals;
    std::vector<std::vector<int>> mIndices;

    rp3d::TriangleMesh *triangleMesh;
public:
    rp3d::ConcaveMeshShape *collider;

    ConcaveCollider(const std::vector<Mesh> meshes, rp3d::PhysicsCommon &physicsCommon) {


        triangleMesh = createTriangleMesh(meshes, physicsCommon);
        collider = physicsCommon.createConcaveMeshShape(triangleMesh);
    }

private:
    rp3d::TriangleMesh *createTriangleMesh(std::vector<Mesh> meshes, rp3d::PhysicsCommon &physicsCommon) {
        auto triangleMesh = physicsCommon.createTriangleMesh();

        for (const Mesh &mesh : meshes) {
            mVertices.emplace_back();
            mNormals.emplace_back();
            mIndices.emplace_back();

            // Load vertex and normal coordinates into the member vector
            for (auto vertex : mesh.vertices) {
                mVertices.back().push_back(vertex.Position.x);
                mVertices.back().push_back(vertex.Position.y);
                mVertices.back().push_back(vertex.Position.z);

                mNormals.back().push_back(vertex.Normal.x);
                mNormals.back().push_back(vertex.Normal.y);
                mNormals.back().push_back(vertex.Normal.z);
            }

            // Load indices into the member vector
            for (auto index : mesh.indices) {
                mIndices.back().push_back(index);
            }

            // rp3d representation of vertex array for this mesh
            auto triangleArray = new rp3d::TriangleVertexArray(
                    mesh.vertices.size(),
                    (const void *) &mVertices.back()[0],
                    3 * sizeof(float),
                    (const void *) &mNormals.back()[0],
                    3 * sizeof(float),
                    mesh.numFaces,
                    (const void *) &mIndices.back()[0],
                    3 * sizeof(int),
                    rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
                    rp3d::TriangleVertexArray::NormalDataType::NORMAL_FLOAT_TYPE,
                    rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE
            );

//            add this as a subpart for the triangle mesh
            triangleMesh->addSubpart(triangleArray);
        }
        return triangleMesh;
    }
};

#endif //PROJECT_BASE_CONCAVECOLLIDER_H
