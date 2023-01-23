#include "draw/gui.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// imgui library
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "draw/definitions.hpp"
#include "draw/variables.hpp"

void setUpImgui(GLFWwindow* window, const char* glsl_version) {
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void) io;
  // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsClassic();

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

void drawImgui() {
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  if (drawing::SHOW_SETTINGS_WINDOW) {
    ImGui::Begin("Settings", &drawing::SHOW_SETTINGS_WINDOW);
    ImGui::Text("mouse x = %f", input::mouse::x);  // DEBUG
    ImGui::Text("mouse y = %f", input::mouse::y);  // DEBUG
    ImGui::Text(
        "Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    ImGui::Spacing();
    ImGui::Checkbox("BTSP exact", &drawing::ACTIVE[(unsigned int) ProblemType::BTSP_exact]);
    ImGui::SliderFloat(
        "thickness##BTSP exact", &drawing::THICKNESS[(unsigned int) ProblemType::BTSP_exact], 0.0f, 20.0f, "%.1f");
    ImGui::ColorEdit3("##BTSP exact", (float*) &drawing::COLOUR[(unsigned int) ProblemType::BTSP_exact]);

    ImGui::Checkbox("TSP  exact", &drawing::ACTIVE[(unsigned int) ProblemType::TSP_exact]);
    ImGui::ColorEdit3("##TSP exact", (float*) &drawing::COLOUR[(unsigned int) ProblemType::TSP_exact]);
    ImGui::SliderFloat("thickness##TSP exact", &drawing::THICKNESS[(unsigned int) ProblemType::TSP_exact], 0.0f, 30.0f);

    ImGui::ColorEdit3("vertex colour", (float*) &drawing::VERTEX_COLOUR);
    ImGui::End();
  }

  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void cleanUpImgui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
}
