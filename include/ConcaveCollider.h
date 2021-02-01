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

        unsigned long meshLen = meshes.size();

        int i = 0;
        while (i < meshLen) {
            // Load vertex and normal coordinates into the member vector
            std::vector<float> tmpVert;
            std::vector<float> tmpNorm;
            for (auto vertex : meshes[i].vertices) {
                tmpVert.push_back(vertex.Position.x);
                tmpVert.push_back(vertex.Position.y);
                tmpVert.push_back(vertex.Position.z);

                tmpNorm.push_back(vertex.Normal.x);
                tmpNorm.push_back(vertex.Normal.y);
                tmpNorm.push_back(vertex.Normal.z);
            }
            mVertices.push_back(tmpVert);
            mNormals.push_back(tmpNorm);

            // Load indices into the member vector
            std::vector<int> tmpIndi;
            for (auto index : meshes[i].indices) {
                tmpIndi.push_back(index);
            }

            mIndices.push_back(tmpIndi);

            std::cout << tmpVert[0] << std::endl;
            std:
            cout << tmpNorm[0] << std::endl;
            std::cout << tmpIndi[0] << std::endl;

            std::cout << mVertices.back()[0] << std::endl;
            std::cout << mNormals.back()[0] << std::endl;
            std::cout << mIndices.back()[0] << std::endl;

            fflush(stdout);

            // rp3d representation of vertex array for this mesh
            auto triangleArray = new rp3d::TriangleVertexArray(
                    meshes[i].vertices.size(),
                    (const void *) &mVertices.back()[0],
                    3 * sizeof(float),
                    (const void *) &mNormals.back()[0],
                    3 * sizeof(float),
                    meshes[i].numFaces,
                    (const void *) &mIndices.back()[0],
                    3 * sizeof(int),
                    rp3d::TriangleVertexArray::VertexDataType::VERTEX_FLOAT_TYPE,
                    rp3d::TriangleVertexArray::NormalDataType::NORMAL_FLOAT_TYPE,
                    rp3d::TriangleVertexArray::IndexDataType::INDEX_INTEGER_TYPE
            );

//            add this as a subpart for the triangle mesh
            triangleMesh->addSubpart(triangleArray);
            ++i;
        }
        return triangleMesh;
    }
};

#endif //PROJECT_BASE_CONCAVECOLLIDER_H
