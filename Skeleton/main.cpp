// Include standard headers
#include <iostream>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include common libraries
#include <common/geometry.hpp>
#include <common/model.hpp>


using namespace std;
using namespace glm;

// Global variable
GLFWwindow *g_window;
float g_window_width = 1024.f;
float g_window_height = 768.f;
int g_framebuffer_width = 1024;
int g_framebuffer_height = 768;
float g_fov = 45.f; // 45-degree of fov
float g_fovy = g_fov; // y-angle of fov

std::vector<Model> g_models;
std::vector<mat4> g_model_rbts;

// Transformations
mat4 g_projection;
// Camera model property
mat4 g_eye_rbt;
// World model property
mat4 g_world_rbt = mat4(1.0f);

// Event functions
static void WindowSizeCallback(GLFWwindow *, int, int);

static void MouseButtonCallback(GLFWwindow *, int, int, int);

static void CursorPosCallback(GLFWwindow *, double, double);

static void KeyboardCallback(GLFWwindow *, int, int, int, int);

void UpdateFovy(void);

void CleanUp(void);

// Helper function: Update the vertical field-of-view(float fovy in global)
void UpdateFovy() {
    if (g_framebuffer_width >= g_framebuffer_height) {
        g_fovy = g_fov;
    } else {
        const float RAD_PER_DEG = 0.5f * glm::pi<float>() / 180.0f;
        g_fovy = (float) atan2(sin(g_fov * RAD_PER_DEG) * ((float) g_framebuffer_height / (float) g_framebuffer_width),
                               cos(g_fov * RAD_PER_DEG)) / RAD_PER_DEG;
    }
}

void CleanUp() {
    for (int i = 0; i < g_models.size(); i++) {
        g_models[i].CleanUp();
    }
    g_models.clear();
    g_models.shrink_to_fit();

    g_model_rbts.clear();
    g_model_rbts.shrink_to_fit();
}

// TODO: Modify GLFW window resized callback function
static void WindowSizeCallback(GLFWwindow *a_window, int a_width, int a_height) {
    // Get resized size and set to current window
    g_window_width = (float) a_width;
    g_window_height = (float) a_height;

    // glViewport accept pixel size, please use glfwGetFramebufferSize rather than window size.
    // window size != framebuffer size
    glfwGetFramebufferSize(a_window, &g_framebuffer_width, &g_framebuffer_height);
    glViewport(0, 0, (GLsizei) g_framebuffer_width, (GLsizei) g_framebuffer_height);

    // Update projection matrix
    g_projection = perspective(g_fov, ((float) g_framebuffer_width / (float) g_framebuffer_height), 0.1f, 100.0f);
}

// TODO: Fill up GLFW mouse button callback function
static void MouseButtonCallback(GLFWwindow *a_window, int a_button, int a_action, int a_mods) {
    //example code for get x position and y position of mouse click
    if (a_button == GLFW_MOUSE_BUTTON_LEFT && a_action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(a_window, &xpos, &ypos);
        xpos = xpos / ((double) g_window_width) * ((double) g_framebuffer_width);
        ypos = ypos / ((double) g_window_height) * ((double) g_framebuffer_height);
    }
}

// TODO: Fill up GLFW cursor position callback function
static void CursorPosCallback(GLFWwindow *a_window, double a_xpos, double a_ypos) {

}

static void KeyboardCallback(GLFWwindow *a_window, int a_key, int a_scancode, int a_action, int a_mods) {
    if (a_action == GLFW_PRESS) {
        switch (a_key) {
            case GLFW_KEY_H:
                cout << "CS580 Homework Assignment 1" << endl;
                cout << "keymaps:" << endl;
                cout << "h\t\t Help command" << endl;
                cout << "p\t\t Enable/Disable picking" << endl;
                break;
            default:
                break;
        }
    }
}


int main(void) {
    // Initialise GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Open a window and create its OpenGL context
    g_window = glfwCreateWindow(1024, 768, "Homework 1: Your Student ID - Your Name", NULL, NULL);

    if (g_window == NULL) {
        fprintf(stderr, "Failed to open GLFW window.");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(g_window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(g_window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetWindowSizeCallback(g_window, WindowSizeCallback);
    glfwSetMouseButtonCallback(g_window, MouseButtonCallback);
    glfwSetCursorPosCallback(g_window, CursorPosCallback);
    glfwSetKeyCallback(g_window, KeyboardCallback);

    // Clear with black backround
    glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);
    // Enable culling
    glEnable(GL_CULL_FACE);
    // Backface culling
    glCullFace(GL_BACK);

    // Setting viewport
    // glViewport accept pixel size, please use glfwGetFramebufferSize rather than window size.
    // window size != framebuffer size
    glfwGetFramebufferSize(g_window, &g_framebuffer_width, &g_framebuffer_height);
    glViewport(0, 0, (GLsizei) g_framebuffer_width, (GLsizei) g_framebuffer_height);

    g_projection = perspective(g_fov, ((float) g_framebuffer_width / (float) g_framebuffer_height), 0.1f, 100.0f);

    g_eye_rbt = translate(mat4(1.0f), vec3(0.0, 5.0f, 10.0));

    // initialize models
    g_models = std::vector<Model>();
    g_model_rbts = std::vector<mat4>();

    // initialize directional light
    vec3 dir_light = vec3(0.0f, -1.0f, 1.0f);

    Model cube = Model();
    InitDataCube(cube, vec3(0.0f, 1.0f, 0.0f));
    cube.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
    cube.SetProjection(&g_projection);
    cube.SetEyeRbt(&g_eye_rbt);

    mat4 cube_rbt = translate(g_world_rbt, vec3(-1.5f, 0.0f, 0.0f));
    g_model_rbts.push_back(cube_rbt);
    cube.SetModelRbt(&cube_rbt);
    cube.SetDirectionalLight(dir_light);
    cube.SetModelId(0);
    g_models.push_back(cube);

    Model another_cube = Model();
    InitDataCube(another_cube, vec3(1.0f, 0.0f, 0.0f));
    another_cube.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
    another_cube.SetProjection(&g_projection);
    another_cube.SetEyeRbt(&g_eye_rbt);

    mat4 another_cube_rbt = translate(g_world_rbt, vec3(1.5f, 0.0f, 0.0f));
    g_model_rbts.push_back(another_cube_rbt);
    another_cube.SetDirectionalLight(dir_light);
    another_cube.SetModelId(1);
    g_models.push_back(another_cube);

    Model cube3 = Model();
    InitDataCube(cube3, vec3(1.0f, 0.0f, 1.0f));
    cube3.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");
    cube3.SetProjection(&g_projection);
    cube3.SetEyeRbt(&g_eye_rbt);

    mat4 cube3_rbt = translate(g_world_rbt, vec3(2.0f, 0.0f, 0.0f));
    g_model_rbts.push_back(cube3_rbt);
    cube3.SetDirectionalLight(dir_light);
    cube3.SetModelId(1);
    g_models.push_back(cube3);

    Model column = Model();
    InitDataTriangle(column, vec3(1.0,1.0,0));
    column.InitializeGLSL(DRAW_TYPE::ARRAY, "VertexShader.glsl", "FragmentShader.glsl");

    column.SetProjection(&g_projection);
    column.SetEyeRbt(&g_eye_rbt);

    mat4 column_rbt = translate(g_world_rbt, vec3(2.0f, 5.0f, 0.0f));
    column.SetModelRbt(&column_rbt);
    column.SetDirectionalLight(dir_light);
    column.SetModelId(3);


    Sphere sphere = Sphere();
    mat4 sphere_rbt = translate(g_world_rbt, vec3(-1.5f, 0.0f, 0.0f));
    sphere.SetModelId(0);
    sphere.init(0);
    sphere.SetProjection(&g_projection);
    sphere.SetEyeRbt(&g_eye_rbt);
    sphere.SetModelRbt(&sphere_rbt);
    sphere.SetDirectionalLight(dir_light);
    std::shared_ptr<Sphere> root;
    std::shared_ptr<Sphere> child1;


    Sphere sphere2 = Sphere();
    mat4 sphere_rbt_2 = translate(g_world_rbt, vec3(0.0f, 0.0f, 1.0f));
    sphere2.SetModelId(1);
    sphere2.init(0);

    sphere2.SetProjection(&g_projection);
    sphere2.SetEyeRbt(&g_eye_rbt);
    sphere2.SetModelRbt(&sphere_rbt_2);
    sphere2.SetDirectionalLight(dir_light);

    do {
        // Clear the screen
        // Fill the background
        glClearColor((GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f, (GLclampf) 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        another_cube.Draw();
        column.Draw();
        sphere.draw();

        cube.SetParentModelMatrix(another_cube.GetModelMatrix());
        cube.Draw();
        sphere2.SetParentModelMatrix(cube.GetModelMatrix());
        sphere2.draw();


//        cube3.Draw();


        glfwSwapBuffers(g_window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(g_window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(g_window) == 0);

    // Cleanup all resources
    CleanUp();
    sphere.cleanup();
    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}
