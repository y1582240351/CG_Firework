#include "skybox.h"
#include "stb_image.h"

float SkyBox::vertices[VERTEX_NUM] = {
        //// positions         // normals
        //-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        //-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        // 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        // 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        // 1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        //-1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 1.0f,

        //-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
        //-1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        //-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        //-1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f,
        //-1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f,
        //-1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f,

        // 1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        // 1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
        // 1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
        // 1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f,
        // 1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
        // 1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f,

        //-1.0f, -1.0f,  1.0f, 0.0f, 0.0f, -1.0f,
        //-1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,
        // 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,
        // 1.0f,  1.0f,  1.0f, 0.0f, 0.0f, -1.0f,
        // 1.0f, -1.0f,  1.0f, 0.0f, 0.0f, -1.0f,
        //-1.0f, -1.0f,  1.0f, 0.0f, 0.0f, -1.0f,

        //-1.0f,  1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        // 1.0f,  1.0f, -1.0f, 0.0f, -1.0f, 0.0f,
        // 1.0f,  1.0f,  1.0f, 0.0f, -1.0f, 0.0f,
        // 1.0f,  1.0f,  1.0f, 0.0f, -1.0f, 0.0f,
        //-1.0f,  1.0f,  1.0f, 0.0f, -1.0f, 0.0f,
        //-1.0f,  1.0f, -1.0f, 0.0f, -1.0f, 0.0f,

        //-1.0f, -1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        //-1.0f, -1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
        // 1.0f, -1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        // 1.0f, -1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
        //-1.0f, -1.0f,  1.0f, 0.0f,  1.0f, 0.0f,
        // 1.0f, -1.0f,  1.0f, 0.0f,  1.0f, 0.0f
        // positions         // normals
        -2.0f,  2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
        -2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
         2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
         2.0f, -2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
         2.0f,  2.0f, -2.0f, 0.0f, 0.0f, 1.0f,
        -2.0f,  2.0f, -2.0f, 0.0f, 0.0f, 1.0f,

        -2.0f, -2.0f,  2.0f, 1.0f, 0.0f, 0.0f,
        -2.0f, -2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
        -2.0f,  2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
        -2.0f,  2.0f, -2.0f, 1.0f, 0.0f, 0.0f,
        -2.0f,  2.0f,  2.0f, 1.0f, 0.0f, 0.0f,
        -2.0f, -2.0f,  2.0f, 1.0f, 0.0f, 0.0f,

         2.0f, -2.0f, -2.0f, -1.0f, 0.0f, 0.0f,
         2.0f, -2.0f,  2.0f, -1.0f, 0.0f, 0.0f,
         2.0f,  2.0f,  2.0f, -1.0f, 0.0f, 0.0f,
         2.0f,  2.0f,  2.0f, -1.0f, 0.0f, 0.0f,
         2.0f,  2.0f, -2.0f, -1.0f, 0.0f, 0.0f,
         2.0f, -2.0f, -2.0f, -1.0f, 0.0f, 0.0f,

        -2.0f, -2.0f,  2.0f, 0.0f, 0.0f, -1.0f,
        -2.0f,  2.0f,  2.0f, 0.0f, 0.0f, -1.0f,
         2.0f,  2.0f,  2.0f, 0.0f, 0.0f, -1.0f,
         2.0f,  2.0f,  2.0f, 0.0f, 0.0f, -1.0f,
         2.0f, -2.0f,  2.0f, 0.0f, 0.0f, -1.0f,
        -2.0f, -2.0f,  2.0f, 0.0f, 0.0f, -1.0f,

        -2.0f,  2.0f, -2.0f, 0.0f, -1.0f, 0.0f,
         2.0f,  2.0f, -2.0f, 0.0f, -1.0f, 0.0f,
         2.0f,  2.0f,  2.0f, 0.0f, -1.0f, 0.0f,
         2.0f,  2.0f,  2.0f, 0.0f, -1.0f, 0.0f,
        -2.0f,  2.0f,  2.0f, 0.0f, -1.0f, 0.0f,
        -2.0f,  2.0f, -2.0f, 0.0f, -1.0f, 0.0f,

        -2.0f, -2.0f, -2.0f, 0.0f,  1.0f, 0.0f,
        -2.0f, -2.0f,  2.0f, 0.0f,  1.0f, 0.0f,
         2.0f, -2.0f, -2.0f, 0.0f,  1.0f, 0.0f,
         2.0f, -2.0f, -2.0f, 0.0f,  1.0f, 0.0f,
        -2.0f, -2.0f,  2.0f, 0.0f,  1.0f, 0.0f,
         2.0f, -2.0f,  2.0f, 0.0f,  1.0f, 0.0f
};



SkyBox::SkyBox() : texture(-1) {
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * VERTEX_NUM, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}


SkyBox::~SkyBox() {
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    if (texture != -1) {
        glDeleteTextures(1, &texture);
    }
}


void SkyBox::loadMap(std::vector<std::string>& file_names) {
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < file_names.size(); i++)
    {
        unsigned char* data = stbi_load(file_names[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << file_names[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}


void SkyBox::draw(Shader& shader) {
    glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
    shader.use();
    // skybox cube
    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}