#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"

#include <string>

namespace m2
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
        
        unsigned int UploadCubeMapTexture(const std::string &pos_x, const std::string &pos_y, const std::string &pos_z, const std::string &neg_x, const std::string &neg_y, const std::string &neg_z);
        void RenderMeshInstanced(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int instances, const glm::vec3 &color = glm::vec3(1));


        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;

         // functiile mele
        void MakeBoard(int size_x, int size_z);
         // tine shaserele patratelor din tabla de joc
        std::unordered_map<int, std::string> mapShader;

        // Info about the generated surfaces
        glm::vec3 control_p0, control_p1, control_p2, control_p3;
        glm::vec3 control_p4, control_p5, control_p6, control_p7;
        glm::vec3 control_p8, control_p9, control_p10, control_p11;

        glm::vec3 control_p01, control_p110, control_p21, control_p31;
        glm::vec3 control_p41, control_p51, control_p61, control_p71;
        glm::vec3 control_p81, control_p91, control_p101, control_p111;

        unsigned int no_of_generated_points, no_of_instances;
        float max_translate, max_rotate;

     private:
        int cubeMapTextureID;
        int on_greyscale;
        std::vector<float> pozitionarePiese;
        std::vector<int> pozitie_piesa1_i;
        std::vector<int> pozitie_piesa1_j;
        float piesa_apasata;
        float nextMoveStanga;
        float nextMoveDreapta;
        int id_piesa_apasata;
        int id_casuta_next_move;
        float id_piesa_capturata;
        int contor;
        bool ataca_stanga;
        bool ataca_dreapta;
        bool rand_rosu;
        bool rand_albastru;
    };
} // namespace m2