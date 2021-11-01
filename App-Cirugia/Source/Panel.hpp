#ifndef IMGUIWIN_HPP
#define IMGUIWIN_HPP

//#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <string>
#include <fstream>

namespace Panel
{
    float speedCamera = 50.0f;

    void saveProperties();
    void renderMainWindow()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        ImGui::Begin("Cirugia Rendering");
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "[Press 'Ctrl' for wire]");
        ImGui::TextColored(ImVec4(1, 1, 0, 1), "[Press 'Q' for off camera]");
        ImGui::SliderFloat("Camera Speed", &speedCamera, 1, 300);
        ImGui::End();

    }

    void endMainWindow()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void cleanAll()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}


#endif
