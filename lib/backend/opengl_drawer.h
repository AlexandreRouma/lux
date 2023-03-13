#pragma once
#include "../vendor/glad/glad.h"
#include "../vendor/glm/glm.hpp"
#include "shader.h"
#include "../point.h"
#include "../color.h"
#include "../draw_list.h"

namespace lux {
    class OpenGLDrawer {
    public:
        OpenGLDrawer(const Size& size);

        void setSize(const Size& size);

        void clear(const Color& clearColor = Color(0.1, 0.1, 0.1));
        void draw(const std::shared_ptr<DrawList>& drawList, const Point& position = Point(0, 0));

    private:
        void drawStep(const DrawStep& step, const Point& position);
        void updateProjMatrix();

        Size size;
        std::shared_ptr<Shader> shader;

        GLuint VAO;
        GLuint VBO;
        GLuint colorUniform;
        GLuint projMatUniform;
        glm::mat4 projMat;
    };
}
