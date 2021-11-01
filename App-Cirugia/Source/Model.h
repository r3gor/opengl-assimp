#ifndef MODEL_OBJ_H
#define MODEL_OBJ_H

#include <GL/glew.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "ShaderProgram.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>


class Model
{
public:
    // model data 
    std::vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    std::vector<Mesh>    meshes;
    std::string directory;
    std::string path;
    bool gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(std::string const& path, std::string const& file_path, bool gamma = false);
    void Init();
    // draws the model, and thus all its meshes
    void Draw(ShaderProgram& shader);

    /* For Gui Panel Edit*/

    void editPanel();
    void loadProp();
    void saveProp();
    glm::mat4 getModelMatrix();

    const float LIMIT_X = 100;
    const float LIMIT_Y = 100;
    const float LIMIT_Z = 100;
    const float MIN_SCALE = 0.01;
    const float MAX_SCALE = 2.0;

    float x = 0, y = 0, z = 0, scaleFactor = 1;
    float rot_x = 0.0f, rot_y=0.0f, rot_z=0.0f;
    std::string file_prop;
    bool rotateAnimation = false;

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(std::string const& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
};
#endif