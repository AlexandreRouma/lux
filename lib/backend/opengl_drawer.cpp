#include "opengl_drawer.h"
#include "flog.h"
#include "../../vendor/glm/gtc/type_ptr.hpp"

namespace lux {
    const char* VS =
        "#version 330\n"
        "uniform mat4 projMat;"
        "in vec3 pos;\n"
        "void main() {\n"
        "    gl_Position = projMat * vec4(pos, 1.0);\n"
        "}"
    ;

    const char* FS =
        "#version 330\n"
        "uniform vec4 inColor;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "    color = inColor;\n"
        "}"
    ;

    OpenGLDrawer::OpenGLDrawer(const Size& size) {
        this->size = size;
        updateProjMatrix();

        shader = std::make_shared<Shader>(VS, FS);

        // Initialize OpenGL objects
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(shader->getPosAttribute(), 3, GL_FLOAT, GL_FALSE, (3*sizeof(float)), NULL);
        glEnableVertexAttribArray(shader->getPosAttribute());

        // Save uniforms
        colorUniform = glGetUniformLocation((GLint)*shader, "inColor");
        projMatUniform = glGetUniformLocation((GLint)*shader, "projMat");
    }

    void OpenGLDrawer::setSize(const Size& size) {
        this->size = size;
        updateProjMatrix();
    }

    void OpenGLDrawer::draw(const std::shared_ptr<DrawList>& drawList, const Color& clearColor, const Point& position) {
        // Define viewport and clear
        glEnable(GL_SCISSOR_TEST); // TODO: Re-enable when actually calculated properly
        glViewport(0, 0, size.x, size.y);
        glScissor(0, 0, size.x, size.y);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Bind VAO
        glBindVertexArray(VAO);

        // Load shader
        shader->use();
        glUniformMatrix4fv(projMatUniform, 1, GL_FALSE, glm::value_ptr(projMat));

        // Draw the list
        drawFullList(drawList, position, size);
    }

    void OpenGLDrawer::drawFullList(const std::shared_ptr<DrawList>& drawList, const Point& position, const Size& parentArea) {
        // TODO: This doesn't work correctly
        auto remainingArea = parentArea - position;
        auto childArea = drawList->getDrawArea();
        auto drawArea = Size(std::min<int>(childArea.x, remainingArea.x), std::min<int>(childArea.y, remainingArea.y));
        glScissor(position.x, size.y - position.y - drawArea.y, drawArea.x, drawArea.y);
        for (const auto& step : drawList->getSteps()) {
            if (step.op == DRAW_OP_DRAW_LIST) {
                drawFullList(step.list, position + step.p, drawArea);
                continue;
            }
            drawStep(step, position);
        }
    }

    void OpenGLDrawer::drawStep(const DrawStep& step, const Point& position) {
        if (step.op == DRAW_OP_DRAW_LINE) {
            // Write positions
            auto p1 = position + step.p1;
            auto p2 = position + step.p2;
            float buf[6] = {
                p1.x, p1.y, 1.0,
                p2.x, p2.y, 1.0,
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_DYNAMIC_DRAW);

            // Write color
            glUniform4f(colorUniform, step.color.r, step.color.g, step.color.b, step.color.a);

            // Draw
            glDrawArrays(GL_LINES, 0, 2);
        }
        else if (step.op == DRAW_OP_DRAW_RECT) {
            // Write positions
            auto p1 = position + step.p1;
            auto p2 = position + step.p2;
            float buf[24] = {
                // Top
                p1.x, p1.y, 1.0,
                p2.x, p1.y, 1.0,

                // Left
                p1.x, p1.y, 1.0,
                p1.x, p2.y, 1.0,

                // Bottom
                p1.x, p2.y, 1.0,
                p2.x, p2.y, 1.0,

                // Right
                p2.x, p1.y, 1.0,
                p2.x, p2.y, 1.0
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_DYNAMIC_DRAW);

            // Write color
            glUniform4f(colorUniform, step.color.r, step.color.g, step.color.b, step.color.a);

            // Draw
            glDrawArrays(GL_LINES, 0, 8);
        }
        else if (step.op == DRAW_OP_FILL_RECT) {
            // Write positions
            auto p1 = position + step.p1;
            auto p2 = position + step.p2;
            float buf[24] = {
                // Top left
                p1.x, p1.y, 1.0,
                p2.x, p1.y, 1.0,
                p1.x, p2.y, 1.0,

                // Bottom right
                p2.x, p1.y, 1.0,
                p1.x, p2.y, 1.0,
                p2.x, p2.y, 1.0
            };
            glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_DYNAMIC_DRAW);

            // Write color
            glUniform4f(colorUniform, step.color.r, step.color.g, step.color.b, step.color.a);

            // Draw
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }
    }

    void OpenGLDrawer::updateProjMatrix() {
        projMat = glm::ortho(-0.5f, (float)(size.x - 1) + 0.5f, (float)(size.y - 1) + 0.5f, -0.5f);
    }
}
