// Copyright Oprea Olivia 341C5 2021

#include "lab_m2/Tema1/Tema1.h"

#include <vector>
#include <iostream>

#include "stb/stb_image.h"

using namespace std;
using namespace m2;

Tema1::Tema1()
{
    on_greyscale = 0;
    contor = -1;
    rand_albastru = false;
    rand_rosu = true;
}

Tema1::~Tema1()
{
}

void Tema1::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(4.5, 5, 1), glm::quat(glm::vec3(-50 * TO_RADIANS, 0, 0)));
    camera->Update();

    std::string texturePath = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "sky");
    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders");

    // shader piesa 1
    {
        Shader *shader = new Shader("Piesa1");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "Piesa1.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "Piesa1.GS.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "Piesa1.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // shader piesa 2
    {
        Shader *shader = new Shader("Piesa2");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "Piesa2.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "Piesa2.GS.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "Piesa2.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        // Parameters related to surface generation
        no_of_generated_points = 10;            // number of points on a Bezier curve
        no_of_instances = 40;                    // number of instances (number of curves that contain the surface)
        max_rotate = glm::radians(360.0f);      // for the rotation surface, it's the angle between the first and the last curve
        
        // Define control points
        control_p0 = glm::vec3(-1.88255, -2.5,  1.0);
        control_p1 = glm::vec3(1.84731,  1.5,  1.0);
        control_p2 = glm::vec3(-4.16978,  3.0,  1.0);
        control_p3 = glm::vec3(-2.92301,  5.5,  1.0);

        control_p4 = glm::vec3(-3, -2.5,  1.0);
        control_p5 = glm::vec3(1,  1.5,  1.0);
        control_p6 = glm::vec3(-1,  3.0,  1.0);
        control_p7 = glm::vec3(-2,  5.5,  1.0);

        control_p8 = glm::vec3(3.88255, -2.5,  1.0);
        control_p9 = glm::vec3(-2.84731,  1.5,  1.0);
        control_p10 = glm::vec3(-9.16978,  3.0,  1.0);
        control_p11 = glm::vec3(1.92301,  5.5,  1.0);

        control_p01 = glm::vec3(-1.88255, -2.5,  1.0);
        control_p110 = glm::vec3(1.84731,  1.5,  1.0);
        control_p21 = glm::vec3(-4.16978,  3.0,  1.0);
        control_p31 = glm::vec3(-2.92301,  5.5,  1.0);

        control_p41 = glm::vec3(-1, -2.5,  1.0);
        control_p51 = glm::vec3(9,  1.5,  1.0);
        control_p61 = glm::vec3(-7,  3.0,  1.0);
        control_p71 = glm::vec3(-4,  5.5,  1.0);

        control_p81 = glm::vec3(1.88255, -2.5,  1.0);
        control_p91 = glm::vec3(-2.84731,  1.5,  1.0);
        control_p101 = glm::vec3(-2.16978,  3.0,  1.0);
        control_p111 = glm::vec3(1.92301,  5.5,  1.0);

        // Create a bogus mesh with 2 points (a line)
        {
            vector<VertexFormat> vertices
            {
                VertexFormat(control_p0, glm::vec3(0, 1, 1)),
                VertexFormat(control_p3, glm::vec3(0, 1, 0))
            };

            vector<unsigned int> indices =
            {
                0, 1
            };

            meshes["surface"] = new Mesh("generated initial surface points");
            meshes["surface"]->InitFromData(vertices, indices);
            meshes["surface"]->SetDrawMode(GL_LINES);
        }
    }

    // shader patratica neagra
    {
        Shader *shader = new Shader("PatraticaNeagra");
        shader->AddShader(PATH_JOIN(shaderPath, "PatraticaNeagra.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "PatraticaNeagra.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // shader patratica alba
    {
        Shader *shader = new Shader("PatraticaAlba");
        shader->AddShader(PATH_JOIN(shaderPath, "PatraticaAlba.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "PatraticaAlba.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // patratica tabla
    {
        Mesh* mesh = new Mesh("patratica");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // shader background
    {
        Shader *shader = new Shader("Background");
        shader->AddShader(PATH_JOIN(shaderPath, "Normal.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "Normal.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // cube
    {
        Mesh* mesh = new Mesh("cube");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    // cubemap files
    cubeMapTextureID = UploadCubeMapTexture(
        PATH_JOIN(texturePath, "posx.png"),
        PATH_JOIN(texturePath, "posy.png"),
        PATH_JOIN(texturePath, "posz.png"),
        PATH_JOIN(texturePath, "negx.png"),
        PATH_JOIN(texturePath, "negy.png"),
        PATH_JOIN(texturePath, "negz.png"));

    // mapare unde sunt piese
    pozitionarePiese.push_back(-1);
    for (int i = 1; i <= 3; i++) {
        int cont = 1;
        if (i % 2 == 0) {
            cont = 2;
        }
        for (int j = cont; j <= 8; j += 2) {
            pozitionarePiese.push_back(100.0f + 10 * i + j);
        }
    }

    for (int i = 8; i >= 6; i--) {
        int cont = 1;
        if (i % 2 == 0) {
            cont = 2;
        }
        for (int j = cont; j <= 8; j += 2) {
            pozitionarePiese.push_back(100.0f + 10 * i + j);
        }
    }

    // for (int i = 1; i <= 24; i++) {
    //     cout << "Piesa " << i << " este la pozitia " << pozitionarePiese[i] << endl;
    // }

    // pentru a tine minte pozitiile pieselor
    pozitie_piesa1_i.push_back(-1);
    pozitie_piesa1_j.push_back(-1);
    for (int i = 1; i <= 3; i++) {
        int cont = 1;
        if (i % 2 == 0) {
            cont = 2;
        }
        for (int j = cont; j <= 8; j += 2) {
            pozitie_piesa1_i.push_back(-i);
            pozitie_piesa1_j.push_back(j);
        }
    }

    for (int i = 8; i >= 6; i--) {
        int cont = 1;
        if (i % 2 == 0) {
            cont = 2;
        }
        for (int j = cont; j <= 8; j += 2) {
            pozitie_piesa1_i.push_back(-i);
            pozitie_piesa1_j.push_back(j);
        }
    }
}

void Tema1::FrameStart()
{
} 

void Tema1::Update(float deltaTimeSeconds)
{
    ClearScreen();

    auto camera = GetSceneCamera();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // background
    {
        Shader *shader = shaders["Background"];
        shader->Use();

        glm::mat4 modelMatrix = glm::scale(glm::mat4(1), glm::vec3(30));

        glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
        glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
        int loc_texture = shader->GetUniformLocation("texture_cubemap");
        glUniform1i(loc_texture, 0);

        meshes["cube"]->Render();
    }

    // make the board
    MakeBoard(8, 8);

    // piesa 1
    {
        float id = 0.0f;
        for (int i = 1; i <= 3; i++) {
            int cont = 1;
            if (i % 2 == 0) {
                cont = 2;
            }
            for (int j = cont; j <= 8; j += 2) {
                id += 10;

                Shader *shader = shaders["Piesa1"];
                shader->Use();

                glm::mat4 modelMatrix(1);
                modelMatrix = glm::translate(modelMatrix,  glm::vec3(pozitie_piesa1_j[id / 10], 0.77, pozitie_piesa1_i[id / 10]));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1));

                glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);

                glUniform3f(glGetUniformLocation(shader->program, "control_p4"), control_p4.x, control_p4.y, control_p4.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p5"), control_p5.x, control_p5.y, control_p5.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p6"), control_p6.x, control_p6.y, control_p6.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p7"), control_p7.x, control_p7.y, control_p7.z);

                glUniform3f(glGetUniformLocation(shader->program, "control_p8"), control_p8.x, control_p8.y, control_p8.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p9"), control_p9.x, control_p9.y, control_p9.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p10"), control_p10.x, control_p10.y, control_p10.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p11"), control_p11.x, control_p11.y, control_p11.z);

                glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), no_of_instances);
                glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), no_of_generated_points);
                glUniform1f(glGetUniformLocation(shader->program, "max_rotate"), max_rotate);

                glUniform1i(glGetUniformLocation(shader->program, "on_greyscale"), on_greyscale);
                glUniform1f(shader->GetUniformLocation("id"), id);
                glUniform1f(glGetUniformLocation(shader->program, "piesa_apasata"), piesa_apasata);

                auto cameraPosition = camera->m_transform->GetWorldPosition();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
                int loc_texture = shader->GetUniformLocation("texture_cubemap");
                glUniform1i(loc_texture, 0);

                int loc_camera = shader->GetUniformLocation("camera_position");
                glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);

                Mesh* mesh = meshes["surface"];
                RenderMeshInstanced(mesh, shader, modelMatrix, no_of_instances);
            }
        }
    }

    // piesa 2
    {
        float id = 120.0f;
        for (int i = 8; i >= 6; i--) {
            int cont = 1;
            if (i % 2 == 0) {
                cont = 2;
            }
            for (int j = cont; j <= 8; j += 2) {
                id += 10;

                Shader *shader = shaders["Piesa2"];
                shader->Use();

                glm::mat4 modelMatrix(1);
                modelMatrix = glm::translate(modelMatrix,  glm::vec3(pozitie_piesa1_j[id / 10], 0.77, pozitie_piesa1_i[id / 10]));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1));

                glUniform3f(glGetUniformLocation(shader->program, "control_p01"), control_p01.x, control_p01.y, control_p01.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p110"), control_p110.x, control_p110.y, control_p110.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p21"), control_p21.x, control_p21.y, control_p21.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p31"), control_p31.x, control_p31.y, control_p31.z);

                glUniform3f(glGetUniformLocation(shader->program, "control_p41"), control_p41.x, control_p41.y, control_p41.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p51"), control_p51.x, control_p51.y, control_p51.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p61"), control_p61.x, control_p61.y, control_p61.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p71"), control_p71.x, control_p71.y, control_p71.z);

                glUniform3f(glGetUniformLocation(shader->program, "control_p81"), control_p81.x, control_p81.y, control_p81.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p91"), control_p91.x, control_p91.y, control_p91.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p101"), control_p101.x, control_p101.y, control_p101.z);
                glUniform3f(glGetUniformLocation(shader->program, "control_p111"), control_p111.x, control_p111.y, control_p111.z);

                glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), no_of_instances);
                glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), no_of_generated_points);
                glUniform1f(glGetUniformLocation(shader->program, "max_rotate"), max_rotate);

                glUniform1i(glGetUniformLocation(shader->program, "on_greyscale"), on_greyscale);
                glUniform1f(shader->GetUniformLocation("id"), id);
                glUniform1f(glGetUniformLocation(shader->program, "piesa_apasata"), piesa_apasata);

                auto cameraPosition = camera->m_transform->GetWorldPosition();

                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
                int loc_texture = shader->GetUniformLocation("texture_cubemap");
                glUniform1i(loc_texture, 0);

                int loc_camera = shader->GetUniformLocation("camera_position");
                glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);

                Mesh* mesh = meshes["surface"];
                RenderMeshInstanced(mesh, shader, modelMatrix, no_of_instances);
            }
        }
    }
}

void Tema1::FrameEnd()
{
}

void Tema1::RenderMeshInstanced(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int instances, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, instances);
}

// function from the lab to load the texture of the cubemap
unsigned int Tema1::UploadCubeMapTexture(const std::string &pos_x, const std::string &pos_y, const std::string &pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z)
{
    int width, height, chn;

    unsigned char* data_pos_x = stbi_load(pos_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_y = stbi_load(pos_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_z = stbi_load(pos_z.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_x = stbi_load(neg_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_y = stbi_load(neg_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_z = stbi_load(neg_z.c_str(), &width, &height, &chn, 0);

    unsigned int textureID = 0;

    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (GLEW_EXT_texture_filter_anisotropic) {
        float maxAnisotropy;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_x);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_y);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_z);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_x);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_y);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_z);


    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    SAFE_FREE(data_pos_x);
    SAFE_FREE(data_pos_y);
    SAFE_FREE(data_pos_z);
    SAFE_FREE(data_neg_x);
    SAFE_FREE(data_neg_y);
    SAFE_FREE(data_neg_z);

    return textureID;
}

void Tema1::OnInputUpdate(float deltaTime, int mods)
{
}


void Tema1::OnKeyPress(int key, int mods)
{
    if (key == GLFW_KEY_Z) {
        // schimba in greyscale
        on_greyscale = 1;
    }

    if (key == GLFW_KEY_X) {
        // schimba inapoi din greyscale
        on_greyscale = 0;
    }
}

void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    id_casuta_next_move = -1;
    unsigned char data[4];
    mouseY = window->props.resolution.y - mouseY;
    glReadPixels(mouseX, mouseY, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);
    int id = data [3];

    cout << id << endl;

    if (id != 0) {
        piesa_apasata = id;
    }

    if (piesa_apasata != 0 && (piesa_apasata == 10 || piesa_apasata == 20 ||
    piesa_apasata == 30 || piesa_apasata == 40 || piesa_apasata == 50 || piesa_apasata == 60 ||
    piesa_apasata == 70 || piesa_apasata == 80 || piesa_apasata == 90 || piesa_apasata == 100 ||
    piesa_apasata == 110 || piesa_apasata == 120) && rand_rosu == true) {
        id_piesa_capturata = -1;

        id_piesa_apasata = piesa_apasata / 10;
        int id_casuta_piesa_apasata = pozitionarePiese[id_piesa_apasata];
        int coordonata_i_casuta = id_casuta_piesa_apasata / 10 % 10;
        int coordonata_j_casuta = id_casuta_piesa_apasata % 10;
        nextMoveStanga = 100.0f + ((coordonata_i_casuta + 1) * 10) + (coordonata_j_casuta - 1);
        nextMoveDreapta = 100.0f + ((coordonata_i_casuta + 1) * 10) + (coordonata_j_casuta + 1);

        for (int i = 1; i <= 12; i++) {
            if (pozitionarePiese[i] == nextMoveStanga) {
                nextMoveStanga = -1;
            }
            if (pozitionarePiese[i] == nextMoveDreapta) {
                nextMoveDreapta = -1;
            }
        }

        for (int i = 13; i <= 24; i++) {
            if (pozitionarePiese[i] == nextMoveStanga) {
                id_piesa_capturata = i;
                nextMoveStanga = 100.0f + ((coordonata_i_casuta + 2) * 10) + (coordonata_j_casuta - 2);
                for (int j = 1; j <= 24; j++) {
                    if(pozitionarePiese[j] == nextMoveStanga) {
                        nextMoveStanga = -1;
                    }
                }
                ataca_stanga = true;
            }
            if (pozitionarePiese[i] == nextMoveDreapta) {
                id_piesa_capturata = i;
                nextMoveDreapta = 100.0f + ((coordonata_i_casuta + 2) * 10) + (coordonata_j_casuta + 2);
                for (int j = 1; j <= 24; j++) {
                    if(pozitionarePiese[j] == nextMoveDreapta) {
                        nextMoveDreapta = -1;
                    }
                }
                ataca_dreapta = true;
            }
        }
    }

    if (piesa_apasata != 0 && (piesa_apasata == 130 || piesa_apasata == 140 ||
    piesa_apasata == 150 || piesa_apasata == 160 || piesa_apasata == 170 || piesa_apasata == 180 ||
    piesa_apasata == 190 || piesa_apasata == 200 || piesa_apasata == 210 || piesa_apasata == 220 ||
    piesa_apasata == 230 || piesa_apasata == 240) && rand_albastru == true) {
        id_piesa_capturata = -1;

        id_piesa_apasata = piesa_apasata / 10;
        int id_casuta_piesa_apasata = pozitionarePiese[id_piesa_apasata];
        int coordonata_i_casuta = id_casuta_piesa_apasata / 10 % 10;
        int coordonata_j_casuta = id_casuta_piesa_apasata % 10;
        nextMoveStanga = 100.0f + ((coordonata_i_casuta - 1) * 10) + (coordonata_j_casuta - 1);
        nextMoveDreapta = 100.0f + ((coordonata_i_casuta - 1) * 10) + (coordonata_j_casuta + 1);

        for (int i = 13; i <= 24; i++) {
            if (pozitionarePiese[i] == nextMoveStanga) {
                nextMoveStanga = -1;
            }
            if (pozitionarePiese[i] == nextMoveDreapta) {
                nextMoveDreapta = -1;
            }
        }

        for (int i = 1; i <= 12; i++) {
            if (pozitionarePiese[i] == nextMoveStanga) {
                id_piesa_capturata = i;
                nextMoveStanga = 100.0f + ((coordonata_i_casuta - 2) * 10) + (coordonata_j_casuta - 2);
                for (int j = 1; j <= 24; j++) {
                    if(pozitionarePiese[j] == nextMoveStanga) {
                        nextMoveStanga = -1;
                    }
                }
                ataca_stanga = true;
            }
            if (pozitionarePiese[i] == nextMoveDreapta) {
                id_piesa_capturata = i;
                nextMoveDreapta = 100.0f + ((coordonata_i_casuta - 2) * 10) + (coordonata_j_casuta + 2);
                for (int j = 1; j <= 24; j++) {
                    if(pozitionarePiese[j] == nextMoveDreapta) {
                        nextMoveDreapta = -1;
                    }
                }
                ataca_dreapta = true;
            }
        }
    }

    if (piesa_apasata != 0 && piesa_apasata == nextMoveStanga) {
        id_casuta_next_move = piesa_apasata;
        int new_i = id_casuta_next_move / 10 % 10;
        int new_j = id_casuta_next_move % 10;
        pozitie_piesa1_i[id_piesa_apasata] = -new_i;
        pozitie_piesa1_j[id_piesa_apasata] = new_j;
        nextMoveDreapta = -1;
        nextMoveStanga = -1;

        pozitionarePiese[id_piesa_apasata] = id_casuta_next_move;

        if (id_piesa_capturata != -1 && ataca_stanga == true) {
            ataca_stanga = false;
            pozitionarePiese[id_piesa_capturata] = -1;
            pozitie_piesa1_i[id_piesa_capturata] = contor;
            pozitie_piesa1_j[id_piesa_capturata] = 0;
            contor--;
        }

        if (rand_rosu == true) {
            rand_rosu = false;
            rand_albastru = true;
        } else {
            rand_rosu = true;
            rand_albastru = false;
        }
    }
    if (piesa_apasata != 0 && piesa_apasata == nextMoveDreapta) {
        id_casuta_next_move = piesa_apasata;
        int new_i = id_casuta_next_move / 10 % 10;
        int new_j = id_casuta_next_move % 10;
        pozitie_piesa1_i[id_piesa_apasata] = -new_i;
        pozitie_piesa1_j[id_piesa_apasata] = new_j;
        nextMoveStanga = -1;
        nextMoveDreapta = -1;

        pozitionarePiese[id_piesa_apasata] = id_casuta_next_move;

        if (id_piesa_capturata != -1 && ataca_dreapta == true) {
            ataca_dreapta = false;
            pozitionarePiese[id_piesa_capturata] = -1;
            pozitie_piesa1_i[id_piesa_capturata] = contor;
            pozitie_piesa1_j[id_piesa_capturata] = 0;
            contor--;
        }

        if (rand_rosu == true) {
            rand_rosu = false;
            rand_albastru = true;
        } else {
            rand_rosu = true;
            rand_albastru = false;
        }
    }

}

// functie care imi creaza plansa de joc
void Tema1::MakeBoard(int size_x, int size_y) {
    auto camera = GetSceneCamera();
    float id;

    for (int i = 1; i <= size_y; i++) {
        for (int j = 1; j <= size_x ; j++)
        {
            id = 100.0f + i * 10 + j;

            string shader_name = "PatraticaNeagra";

            if ((i + j) % 2 == 1) {
                shader_name = "PatraticaAlba";
            }

            Shader *shader = shaders[shader_name];
            shader->Use();

            glm::mat4 modelMatrix = glm::translate(glm::mat4(1),  glm::vec3(j, 0, -i));
            modelMatrix *= glm::scale(glm::mat4(1), glm::vec3(1));

            glUniform1i(glGetUniformLocation(shader->program, "on_greyscale"), on_greyscale);
            glUniform1f(shader->GetUniformLocation("id"), id);
            glUniform1f(glGetUniformLocation(shader->program, "nextMoveDreapta"), nextMoveDreapta);
            glUniform1f(glGetUniformLocation(shader->program, "nextMoveStanga"), nextMoveStanga);

            glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
            glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
            glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetProjectionMatrix()));

            auto cameraPosition = camera->m_transform->GetWorldPosition();

            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTextureID);
            int loc_texture = shader->GetUniformLocation("texture_cubemap");
            glUniform1i(loc_texture, 0);

            int loc_camera = shader->GetUniformLocation("camera_position");
            glUniform3f(loc_camera, cameraPosition.x, cameraPosition.y, cameraPosition.z);

            RenderMesh(meshes["patratica"], shaders[shader_name], modelMatrix);

            mapShader[(i * 8) + j] = shader_name;
        }
    }
}