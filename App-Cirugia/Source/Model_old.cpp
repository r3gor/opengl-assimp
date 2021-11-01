#include "Model_old.h"
#include "imgui.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <fstream>
#include <iostream>
#include <GLFW/glfw3.h>

void ModelOld::init(){

    std::vector<glm::vec3> vert = objModel->getVertices();
    std::vector<glm::vec3> norm = objModel->getNormals();
    int numObjVertices = objModel->getNumVertices();

    std::vector<float> pvalues;
    std::vector<float> tvalues;
    std::vector<float> nvalues;

    for (int i=0; i<numObjVertices; i++){
        pvalues.push_back(vert[i].x);
        pvalues.push_back(vert[i].y);
        pvalues.push_back(vert[i].z);

        nvalues.push_back(norm[i].x);
        nvalues.push_back(norm[i].y);
        nvalues.push_back(norm[i].z);
    }

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo_vert);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glBufferData(GL_ARRAY_BUFFER, pvalues.size() * 4, &pvalues[0], GL_STATIC_DRAW);

    glGenBuffers(1, &vbo_norm);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_norm);
    glBufferData(GL_ARRAY_BUFFER, nvalues.size() * 4, &nvalues[0], GL_STATIC_DRAW);

    loadProp();
}

void ModelOld::draw(GLuint shader){

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo_vert);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glDrawArrays(GL_TRIANGLES, 0, objModel->getNumVertices());
}

void ModelOld::loadProp(){
    std::ifstream inFile(file_prop.c_str());
    if (!inFile.is_open()) return;

    std::string line;
    while(getline(inFile, line)){
        if (line.compare(0, 4, "#prop")){
            inFile >> x >> y >> z;
            inFile >> scaleFactor;
        }
    }
}

void ModelOld::saveProp(){
    std::ofstream outFile(file_prop.c_str());
    outFile << "#prop" << "\n";
    outFile << x << " " << y << " " << z << "\n";
    outFile << scaleFactor;
}

glm::mat4 ModelOld::getModelMatrix(){
    glm::mat4 t = glm::mat4(1.0);
    t = glm::translate(t, glm::vec3(x, y, z));
    t = glm::scale(t, glm::vec3(scaleFactor, scaleFactor, scaleFactor));
    if (!rotateAnimation) return t;
    float time = glfwGetTime();
    return glm::rotate(t, time*0.5f, glm::vec3(0,1,0));
}

void ModelOld::editPanel(){
    std::string title = "Edit Panel: " + file_prop;

    ImGui::Begin(title.c_str());
    ImGui::SliderFloat("pos X", &x, -100, 100);
    ImGui::SliderFloat("pos Y", &y, -100, 100);
    ImGui::SliderFloat("pos Z", &z, -100, 100);
    ImGui::SliderFloat("Scale factor", &scaleFactor, 0.1, 2);
    ImGui::Checkbox("Animar (rotacion)", &rotateAnimation);
    if (ImGui::Button("Save properties")) saveProp();
    ImGui::End();
}
