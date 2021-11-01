#include <fstream>
#include <sstream>
#include <glm\glm.hpp>
#include "ImportedModel.h"

#include <iostream>

using namespace std;
// ------------ Imported Model class
ImportedModel::ImportedModel(const char *filePath, bool withTex)
{
    // this->withTex = withTex;

    ModelImporter modelImporter = ModelImporter(withTex);
    modelImporter.parseOBJ(filePath); // uses modelImporter to get vertex information
    numVertices = modelImporter.getNumVertices();
    std::vector<float> verts = modelImporter.getVertices();
    std::vector<float> tcs;
    if (withTex) tcs = modelImporter.getTextureCoordinates();
    std::vector<float> normals = modelImporter.getNormals();
    for (int i = 0; i < numVertices; i++)
    {
        vertices.push_back(glm::vec3(verts[i * 3], verts[i * 3 + 1], verts[i * 3 + 2]));
        if (withTex) texCoords.push_back(glm::vec2(tcs[i * 2], tcs[i * 2 + 1]));
        normalVecs.push_back(glm::vec3(normals[i * 3], normals[i * 3 + 1], normals[i * 3 + 2]));
    }
}
int ImportedModel::getNumVertices() { return numVertices; } // accessors
std::vector<glm::vec3> ImportedModel::getVertices() { return vertices; }
std::vector<glm::vec2> ImportedModel::getTextureCoords() { return texCoords; }
std::vector<glm::vec3> ImportedModel::getNormals() { return normalVecs; }
// -------------- Model Importer class
// ModelImporter::ModelImporter() {}
void ModelImporter::parseOBJ(const char *filePath)
{
    float x, y, z;
    string content;
    ifstream fileStream(filePath, ios::in);
    string line = "";
    while (!fileStream.eof())
    {
        getline(fileStream, line);
        if (line.compare(0, 2, "v ") == 0)
        { // vertex position ("v" case)
            stringstream ss(line.erase(0, 1));
            ss >> x;
            ss >> y;
            ss >> z; // extract the vertex position values
            vertVals.push_back(x);
            vertVals.push_back(y);
            vertVals.push_back(z);
        }
        if (line.compare(0, 2, "vt") == 0)
        { // texture coordinates ("vt" case)
            stringstream ss(line.erase(0, 2));
            ss >> x;
            ss >> y; // extract texture coordinate values
            stVals.push_back(x);
            stVals.push_back(y);
        }
        if (line.compare(0, 2, "vn") == 0)
        { // vertex normals ("vn" case)
            stringstream ss(line.erase(0, 2));
            ss >> x;
            ss >> y;
            ss >> z; // extract the normal vector values
            normVals.push_back(x);
            normVals.push_back(y);
            normVals.push_back(z);
        }
        if (line.compare(0, 2, "f ") == 0)
        { // triangle faces ("f" case)
            string oneCorner, v, t, n;
            stringstream ss(line.erase(0, 2));
            for (int i = 0; i < 3; i++)
            {
                getline(ss, oneCorner, ' '); // extract triangle face references
                stringstream oneCornerSS(oneCorner);
                getline(oneCornerSS, v, '/');
                getline(oneCornerSS, t, '/');
                getline(oneCornerSS, n, '/');
                int vertRef;
                int tcRef;
                int normRef;
                
                // std::cout<<"v: "<<v<<std::endl;
                // std::cout<<"v:"<<v.size()<<std::endl;

                // std::cout<<"t: "<<t<<std::endl;
                // std::cout<<"t: "<<t.size()<<std::endl;

                // std::cout<<"n: "<<n<<std::endl;
                // std::cout<<"n: "<<n.size()<<std::endl;


                if(v.size() != 0) vertRef = (stoi(v) - 1) * 3; // "stoi" converts string to integer
                else vertRef = 0;
                if(t.size() != 0) tcRef = (stoi(t) - 1) * 2;
                else tcRef = 0;
                if(n.size() != 0) normRef = (stoi(n) - 1) * 3;
                else normRef = 0;

                triangleVerts.push_back(vertVals[vertRef]); // build vector of vertices
                triangleVerts.push_back(vertVals[vertRef + 1]);
                triangleVerts.push_back(vertVals[vertRef + 2]);

                if (withTex){
                    textureCoords.push_back(stVals[tcRef]); // build vector of texture coords
                    textureCoords.push_back(stVals[tcRef + 1]);
                } 
                normals.push_back(normVals[normRef]); //… and normals
                normals.push_back(normVals[normRef + 1]);
                normals.push_back(normVals[normRef + 2]);
            }
        }
    }
}
int ModelImporter::getNumVertices() { return (triangleVerts.size() / 3); } // accessors
std::vector<float> ModelImporter::getVertices() { return triangleVerts; }
std::vector<float> ModelImporter::getTextureCoordinates() { return textureCoords; }
std::vector<float> ModelImporter::getNormals() { return normals; }