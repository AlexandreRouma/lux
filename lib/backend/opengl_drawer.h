#pragma once
#include "../vendor/glad/glad.h"
#include "../vendor/glm/glm.hpp"
#include "shader.h"
#include "../point.h"
#include "../color.h"
#include "../draw_list.h"
#include "texture.h"

namespace lux {
    class OpenGLDrawer {
    public:
        OpenGLDrawer(const Size& size);

        void setSize(const Size& size);

        void draw(const std::shared_ptr<DrawList>& drawList, const Color& clearColor, const Point& position = Point(0, 0));

    private:
        void drawFullList(const std::shared_ptr<DrawList>& drawList, const Point& position, const Size& parentArea);
        void drawStep(const DrawStep& step, const Point& position);
        void updateProjMatrix();

        Size size;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Shader> fontShader;
        std::shared_ptr<Texture> fontTexture;

        GLuint VAO;
        GLuint VBO;
        GLuint colorUniform;
        GLuint projMatUniform;

        GLuint fontVAO;
        GLuint fontVBO;
        GLuint fontColorUniform;
        GLuint fontProjMatUniform;
        GLuint fontTextureUniform;

        glm::mat4 projMat;
    };
}
