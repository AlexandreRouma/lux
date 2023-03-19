#pragma once
#include "../vendor/glad/glad.h"
#include "../vendor/glm/glm.hpp"
#include "shader.h"
#include "../vec2.h"
#include "../color.h"
#include "../draw_list.h"
#include "texture.h"

namespace lux {
    class OpenGLDrawer {
    public:
        OpenGLDrawer(const Size& size);

        void setSize(const Size& size);

        void draw(const std::shared_ptr<DrawList>& drawList, const Color& clearColor);

    private:
        void updateProjMatrix();

        Size size;
        std::shared_ptr<Shader> shader;

        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        GLuint projMatUnif;
        int VBOCapacity = 0;
        int EBOCapacity = 0;

        glm::mat4 projMat;
    };
}
