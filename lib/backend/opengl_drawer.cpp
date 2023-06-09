#include "opengl_drawer.h"
#include "flog.h"
#include "../../vendor/glm/gtc/type_ptr.hpp"
#include "../lux.h"
#include <stdexcept>
#include <algorithm>

namespace lux {
    const char* VS =
        "#version 120\n"
        "uniform mat4 projMatUnif;\n"
        "uniform vec2 posUnif;\n"
        "attribute vec2 posAttr;\n"
        "attribute vec4 colorAttr;\n"
        "attribute vec2 texCoordAttr;\n"
        "varying vec4 color;\n"
        "varying vec2 texCoord;\n"
        "void main() {\n"
        "    gl_Position = projMatUnif * vec4((posAttr + posUnif), 1.0, 1.0);\n"
        "    color = colorAttr;\n"
        "    texCoord = texCoordAttr;\n"
        "}"
    ;

    const char* FS =
        "#version 120\n"
        "uniform sampler2D sampler;\n"
        "varying vec4 color;\n"
        "varying vec2 texCoord;\n"
        "void main() {\n"
        "    gl_FragColor = texture2D(sampler, texCoord) * color;\n"
        "}"
    ;

    OpenGLDrawer::OpenGLDrawer(const Size& size) {
        this->size = size;
        updateProjMatrix();

        // Load shader and get variable positions
        shader = std::make_shared<Shader>(VS, FS);
        projMatUnif = shader->getUniform("projMatUnif");
        posUnif = shader->getUniform("posUnif");
        samplerUnif = shader->getUniform("sampler");
        GLuint posAttr = shader->getAttribute("posAttr");
        GLuint colorAttr = shader->getAttribute("colorAttr");
        GLuint texCoordAttr = shader->getAttribute("texCoordAttr");
        
        // Allocate VAO, VBO and EBO
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // Define vertex attributes
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glVertexAttribPointer(posAttr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(0 * sizeof(float)));
        glVertexAttribPointer(colorAttr, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
        glVertexAttribPointer(texCoordAttr, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(posAttr);
        glEnableVertexAttribArray(colorAttr);
        glEnableVertexAttribArray(texCoordAttr);

        // Load null texutre
        uint32_t white = 0xFFFFFFFF;
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &white);

        // Load font texture
        glActiveTexture(GL_TEXTURE0);
        glGenTextures(1, &fontTexId);
        glBindTexture(GL_TEXTURE_2D, fontTexId);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 512, 512, 0, GL_RGBA, GL_UNSIGNED_BYTE, font->getBitmap());
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    void OpenGLDrawer::setSize(const Size& size) {
        this->size = size;
        updateProjMatrix();
    }

    void OpenGLDrawer::draw(const std::shared_ptr<DrawList>& drawList, const Color& clearColor) {
        // Define vuewport and clear buffers
        glViewport(0, 0, size.x, size.y);
        glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        //glEnable(GL_SCISSOR_TEST);
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Load arrays and buffers
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // Enable shader and load uniforms
        shader->use();
        glUniformMatrix4fv(projMatUnif, 1, GL_FALSE, glm::value_ptr(projMat));
        glUniform1i(samplerUnif, 0);

        // Force texutre rebind (TODO: might not be required)
        currentTexId = -1;

        // Draw recursively
        drawListRecurse(drawList, Point(0, 0), size);
    }

    void OpenGLDrawer::drawListRecurse(const std::shared_ptr<DrawList>& drawList, const Point& position, const Size& viewArea) {
        // Apply scissor and update position
        glScissor(position.x, position.y, viewArea.x, viewArea.y);
        glUniform2f(posUnif, position.x, position.y);

        // Draw each element
        const auto& elemList = drawList->getElements();
        for (const auto& elem : elemList) {
            // Bind texture if needed
            if (currentTexId != elem.texId) {
                currentTexId = elem.texId;
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, (GLuint)elem.texId);
            }

            // If recursive draw
            if (elem.drawList) {
                // Calculate arguments
                auto newPos = position + elem.drawListPos;
                auto newViewArea = viewArea - elem.drawListPos;
                auto recurseViewArea = elem.drawList->getDrawArea();
                newViewArea.x = std::min<int>(newViewArea.x, recurseViewArea.x);
                newViewArea.y = std::min<int>(newViewArea.y, recurseViewArea.y);
                if (newViewArea.x <= 0 || newViewArea.y <= 0) { continue; }

                // Draw
                drawListRecurse(elem.drawList, newPos, newViewArea);

                // Reset scissor and uniform then continue
                glScissor(position.x, position.y, viewArea.x, viewArea.y);
                glUniform2f(posUnif, position.x, position.y);
                continue;
            }

            // Don't render if empty
            if (elem.indices.empty()) { continue; }

            // Load vertices and indices. Only reallocate if more space is required
            int vertCount = elem.vertices.size();
            int indCount = elem.indices.size();
            if (vertCount > VBOCapacity) {
                glBufferData(GL_ARRAY_BUFFER, vertCount * sizeof(VertexAttrib), elem.vertices.data(), GL_DYNAMIC_DRAW);
                VBOCapacity = vertCount;
                flog::debug("Reallocating vertex buffer to {}", vertCount);
            }
            else {
                glBufferSubData(GL_ARRAY_BUFFER, 0, vertCount * sizeof(VertexAttrib), elem.vertices.data());
            }
            if (indCount > EBOCapacity) {
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, indCount * sizeof(int), elem.indices.data(), GL_DYNAMIC_DRAW);
                EBOCapacity = indCount;
                flog::debug("Reallocating index buffer to {}", indCount);
            }
            else {
                glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indCount * sizeof(int), elem.indices.data());
            }

            // Draw indices
            glDrawElements(GL_TRIANGLES, elem.indices.size(), GL_UNSIGNED_INT, NULL);
        }
    }

    void OpenGLDrawer::updateProjMatrix() {
        projMat = glm::ortho(-0.5f, (float)(size.x - 1) + 0.5f, (float)(size.y - 1) + 0.5f, -0.5f);
    }
}
