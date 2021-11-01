#pragma once

#include "ImportedModel.h"
#include <GL/glew.h>
#include <string>

class ModelOld{
public:
    ModelOld() {}
    ModelOld(std::string file_obj, std::string file_prop, bool withTex) { 
        objModel = new ImportedModel(file_obj.c_str(), withTex);
        this->file_prop = file_prop;
    }

    void init();
    void draw(GLuint shader);
    void editPanel();
    void loadProp();
    void saveProp();
    glm::mat4 getModelMatrix();

private:
    
    const float LIMIT_X = 100; 
    const float LIMIT_Y = 100; 
    const float LIMIT_Z = 100;
    const float MIN_SCALE = 0.01;
    const float MAX_SCALE = 2.0;

    float x = 0, y = 0, z = 0, scaleFactor = 1;
    std::string file_prop;
    bool rotateAnimation = false;

    ImportedModel *objModel;
    GLuint vao;
    GLuint vbo_vert;
    GLuint vbo_text;
    GLuint vbo_norm;
};