#include "lab_m2/lab1/lab1.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab1::Lab1()
{
}


Lab1::~Lab1()
{
}


void Lab1::Init()
{
    nrInstances = 0;
    maxInstances = 8;
    shrink = 0;
    shrink_event = false;

    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
    camera->Update();

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("bamboo");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "vegetation", "bamboo"), "bamboo.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    // Create a shader program for rendering to texture
    {
        Shader *shader = new Shader("Instances");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab1", "shaders", "GeometryShader.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Lab1::FrameStart()
{
}


void Lab1::Update(float deltaTimeSeconds)
{
    ClearScreen();

    {
        auto shader = shaders["Instances"];

        shader->Use();

        int loc_instances = shader->GetUniformLocation("instances");
        glUniform1i(loc_instances, nrInstances);

        // TODO(student): Add a shrinking parameter for scaling each
        // triangle in the geometry shader
        int loc_shrink = shader->GetUniformLocation("shrink");
        glUniform1f(loc_shrink, shrink);

        // Note that we only render a single mesh!
        RenderMesh(meshes["bamboo"], shaders["Instances"], glm::vec3(-3.3f, 0.5f, 0), glm::vec3(0.1f));
    
        if (shrink_event == false) {
            shrink -= 0.02;
        } else {
            shrink += 0.02;
        }
    }
}


void Lab1::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input with window->KeyHold()

    // TODO(student): Add events for modifying the shrinking parameter
    if (shrink >= 2) {
        shrink = 2;
    } else if (shrink <= 0) {
        shrink = 0;
    }

}


void Lab1::OnKeyPress(int key, int mods)
{
    // Add key press event

    if (key == GLFW_KEY_EQUAL)
    {
        nrInstances++;
        nrInstances %= maxInstances;
    }

    if (key == GLFW_KEY_MINUS)
    {
        nrInstances--;
        nrInstances %= maxInstances;
    }

    // Daca apas tot apas tasta P o sa faca shrink
    // iar daca tot apas tasta O o sa se expandeze

    // NU AM STIUT CE METODA VREI ASA CA LE LAS PE AMANDOUA :)

    // if (key == GLFW_KEY_P)
    // {
    //     shrink += 0.02;
    // }

    // if (key == GLFW_KEY_O) {
    //     shrink -= 0.02;
    // }

    // Daca apas tasta I face shrink automat
    if (key == GLFW_KEY_I) {
        if(shrink_event == false) {
            shrink_event = true;
        } else {
            shrink_event = false;
        }
    }

    cout << "Avem: " << nrInstances + 1 << " bambusi" << endl;
}


void Lab1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
