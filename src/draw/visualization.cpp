#include "draw/visualization.hpp"

#include <cstdio>
#include <iostream>
#include <stdexcept>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "draw/buffers.hpp"
#include "draw/definitions.hpp"
#include "draw/draw.hpp"
#include "draw/events.hpp"
#include "draw/gui.hpp"
#include "draw/shader.hpp"
#include "draw/variables.hpp"

#include "solve/exactsolver.hpp"

// error callback function which prints glfw errors in case they arise
static void glfw_error_callback(int error, const char* description) {
  fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void initDrawingVariables() {
  drawing::SHOW_DEBUG_WINDOW                = drawing::INITIAL_SHOW_DEBUG_WINDOW;
  drawing::SHOW_SETTINGS_WINDOW             = drawing::INITIAL_SHOW_SETTINGS_WINDOW;
  drawing::ACTIVE                           = drawing::INITIAL_ACTIVENESS;
  drawing::COLOUR                           = drawing::INITIAL_COLOUR;
  drawing::BTSP_DRAW_BICONNECTED_GRAPH      = drawing::INITIAL_BTSP_DRAW_BICONNECTED_GRAPH;
  drawing::BTSP_DRAW_OPEN_EAR_DECOMPOSITION = drawing::INITIAL_BTSP_DRAW_OPEN_EAR_DECOMPOSITION;
  drawing::BTSP_DRAW_HAMILTON_CYCLE         = drawing::INITIAL_BTSP_DRAW_HAMILTON_CYCLE;
  drawing::BTSPP_DRAW_BICONNECTED_GRAPH     = drawing::INITIAL_BTSPP_DRAW_BICONNECTED_GRAPH;
  drawing::BTSPP_DRAW_HAMILTON_PATH         = drawing::INITIAL_BTSPP_DRAW_HAMILTON_PATH;
  drawing::INITIALIZED                      = drawing::INITIAL_ACTIVENESS;
  drawing::THICKNESS                        = drawing::INITIAL_THICKNESS;
  drawing::CLEAR_COLOUR                     = drawing::INITIAL_CLEAR_COLOUR;
  drawing::VERTEX_COLOUR                    = drawing::INITIAL_VERTEX_COLOUR;
}

static void initInputVariables() {
  input::mouse::NODE_IN_MOTION = input::mouse::INITIAL_NODE_IN_MOTION;
}

static const Buffers& setUpBufferMemory(const graph::Euclidean& euclidean) {
  drawing::EUCLIDEAN = euclidean;
  drawing::updatePointsfFromEuclidean();  // convert to 32 bit floats because opengl isn't capable to deal with 64 bit

  const VertexBuffer& coordinates     = *new VertexBuffer(drawing::POINTS_F, 2);  // components per vertex
  const ShaderBuffer& tourCoordinates = *new ShaderBuffer(drawing::POINTS_F);     // copy vertex coords to shader buffer
  const ShaderBuffer& tour = *new ShaderBuffer(std::vector<unsigned int>(euclidean.numberOfNodes() + 3));  // just allocate memory

  return *new Buffers{coordinates, tour, tourCoordinates};
}

static const VertexArray& bindBufferMemory(const Buffers& buffers, const ShaderProgramCollection& programs) {
  const VertexArray& vao = *new VertexArray;
  vao.bind();
  vao.mapBufferToAttribute(buffers.coordinates, programs.drawCircles.id(), "vertexPosition");
  vao.enable(programs.drawCircles.id(), "vertexPosition");
  vao.bindBufferBase(buffers.tourCoordinates, 0);
  vao.bindBufferBase(buffers.tour, 1);
  return vao;
}

void visualize(const graph::Euclidean& euclidean) {
  // set error colback function
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    throw std::runtime_error("[GLFW] Failed to initialize glfw!");
  }

  // create window in specified size
  GLFWwindow* window = glfwCreateWindow(mainwindow::INITIAL_WIDTH, mainwindow::INITIAL_HEIGHT, mainwindow::NAME, nullptr, nullptr);
  if (window == nullptr) {
    throw std::runtime_error("[GLFW] Failed to create window!");
  }
  glfwMakeContextCurrent(window);

  // initialize glew
  glewExperimental = GL_TRUE;
  if (glewInit()) {
    throw std::runtime_error(" [GLEW] Failed to initialize glew!");
  }

  const char* glsl_version = imguiVersionHints();

  const ShaderCollection collection;
  const ShaderProgram drawCircles      = collection.linkCircleDrawProgram();
  const ShaderProgram drawPathSegments = collection.linkPathSegementDrawProgram();
  const ShaderProgram drawLineProgram  = collection.linkLineDrawProgram();
  const ShaderProgramCollection programs(drawCircles, drawPathSegments, drawLineProgram);

  const Buffers& buffers = setUpBufferMemory(euclidean);
  const VertexArray& vao = bindBufferMemory(buffers, programs);

  // enable vsync
  glfwSwapInterval(1);

  // set callbacks for keyboard and mouse, must be called before Imgui
  glfwSetKeyCallback(window, keyCallback);
  glfwSetMouseButtonCallback(window, mouseButtonCallback);

  // setup Dear ImGui
  setUpImgui(window, glsl_version);

  // set initial state of variables for drawing and input
  initDrawingVariables();
  initInputVariables();

  // main loop
  while (!glfwWindowShouldClose(window)) {
    // runs only through the loop if something changed
    glfwPollEvents();

    // handle Events triggert by user input, like keyboard etc.
    handleEvents(window, buffers);

    // draw the content
    draw(window, programs, buffers);

    // draw the gui
    drawImgui();

    // swap the drawings to the displayed frame
    glfwSwapBuffers(window);
  }

  // clean up memory
  delete &vao;
  delete &buffers;

  // clean up Dear ImGui
  cleanUpImgui();

  // clean up glfw window
  glfwDestroyWindow(window);
  glfwTerminate();
}