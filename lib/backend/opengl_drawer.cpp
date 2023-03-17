#include "opengl_drawer.h"
#include "flog.h"
#include "../../vendor/glm/gtc/type_ptr.hpp"
#include "../lux.h"
#include <stdexcept>

namespace lux {
    const char* VS =
        "#version 330\n"
        "uniform mat4 projMat;\n"
        "uniform mat4 testMat;\n"
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

    const char* FVS =
        "#version 330\n"
        "uniform mat4 projMat;"
        "uniform mat4 testMat;\n"
        "in vec3 pos;\n"
        "in vec2 texCoord;\n"
        "out vec2 _texCoord;\n"
        "void main() {\n"
        "    gl_Position = projMat * vec4(pos, 1.0);\n"
        "    _texCoord = texCoord;\n"
        "}"
    ;

    const char* FFS =
        "#version 330\n"
        "uniform vec4 inColor;\n"
        "uniform sampler2D tex;\n"
        "in vec2 _texCoord;\n"
        "out vec4 color;\n"
        "void main() {\n"
        "    color = texture(tex, _texCoord).r * inColor;\n"
        "}"
    ;

    OpenGLDrawer::OpenGLDrawer(const Size& size) {
        this->size = size;
        updateProjMatrix();

        // Load shaders
        shader = std::make_shared<Shader>(VS, FS);
        fontShader = std::make_shared<Shader>(FVS, FFS);

        // Load font
        fontTexture = std::make_shared<Texture>(512, 512, 0, GL_RED, GL_RED, GL_UNSIGNED_BYTE, font->getBitmap());
        
        // Configure normal VAO
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glVertexAttribPointer(shader->attrib("pos"), 3, GL_FLOAT, GL_FALSE, (3*sizeof(float)), NULL);
        glEnableVertexAttribArray(shader->attrib("pos"));

        // Configure font VAO
        glGenVertexArrays(1, &fontVAO);
        glBindVertexArray(fontVAO);
        glGenBuffers(1, &fontVBO);
        glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
        glVertexAttribPointer(fontShader->attrib("pos"), 3, GL_FLOAT, GL_FALSE, (5*sizeof(float)), NULL);
        glEnableVertexAttribArray(fontShader->attrib("pos"));
        glVertexAttribPointer(fontShader->attrib("texCoord"), 2, GL_FLOAT, GL_FALSE, (5*sizeof(float)), (void*)(3*sizeof(float)));
        glEnableVertexAttribArray(fontShader->attrib("texCoord"));

        // Save normal uniforms
        colorUniform = shader->uniform("inColor");
        projMatUniform = shader->uniform("projMat");
        
        // Save font uniforms
        fontColorUniform = fontShader->uniform("inColor");
        fontProjMatUniform = fontShader->uniform("projMat");
        fontTextureUniform = fontShader->uniform("tex");
    }

    void OpenGLDrawer::setSize(const Size& size) {
        this->size = size;
        updateProjMatrix();
    }

    void OpenGLDrawer::draw(const std::shared_ptr<DrawList>& drawList, const Color& clearColor, const Point& position) {
        // Define viewport and clear
        // glEnable(GL_SCISSOR_TEST); // TODO: Re-enable when actually calculated properly
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glViewport(0, 0, size.x, size.y);
        glScissor(0, 0, size.x, size.y);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Normal mode
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
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
        else if (step.op == DRAW_OP_DRAW_TEXT) {
            auto in = font->getCharInfo(0);
            fontTexture->use(GL_TEXTURE0);

            // Switch to font VAO and shader
            glBindVertexArray(fontVAO);
            glBindBuffer(GL_ARRAY_BUFFER, fontVBO);
            fontShader->use();

            glUniformMatrix4fv(fontProjMatUniform, 1, GL_FALSE, glm::value_ptr(projMat));
            glUniform4f(fontColorUniform, step.color.r, step.color.g, step.color.b, step.color.a);
            glUniform1i(fontTextureUniform, 0);

            float x = step.p.x + position.x;
            float y = step.p.y + position.y;

            for (char c : step.text) {
                stbtt_aligned_quad q;
                stbtt_GetBakedQuad(in, 512, 512, c, &x, &y, &q, true);
                
                // Write positions
                float buf[30] = {
                    // Top left
                    q.x0, q.y0, 1.0, q.s0, q.t0,
                    q.x1, q.y0, 1.0, q.s1, q.t0,
                    q.x0, q.y1, 1.0, q.s0, q.t1,

                    // Bottom right
                    q.x1, q.y0, 1.0, q.s1, q.t0,
                    q.x0, q.y1, 1.0, q.s0, q.t1,
                    q.x1, q.y1, 1.0, q.s1, q.t1,
                };

                glBufferData(GL_ARRAY_BUFFER, sizeof(buf), buf, GL_DYNAMIC_DRAW);
                glDrawArrays(GL_TRIANGLES, 0, 6);

                // Increment
                x += in->xadvance;
            }

            // Switch back to normal VAO and shader
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            shader->use();
            glUniformMatrix4fv(projMatUniform, 1, GL_FALSE, glm::value_ptr(projMat));
        }
    }

    void OpenGLDrawer::updateProjMatrix() {
        projMat = glm::ortho(-0.5f, (float)(size.x - 1) + 0.5f, (float)(size.y - 1) + 0.5f, -0.5f);
    }
}
