#include "textures.h"

uchar_t* loadImage(const char* filename, int &width, int &height) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) return nullptr;

    BMPHeader header;
    BMPInfoHeader infoHeader;

    if (!file.read(reinterpret_cast<char*>(&header), sizeof(header))) return nullptr;
    if (!file.read(reinterpret_cast<char*>(&infoHeader), sizeof(infoHeader))) return nullptr;
    
    if (header.fileType != 0x4D42) {
        file.close();
        return nullptr;
    }

    width = infoHeader.width;
    height = infoHeader.height;
    int channels = infoHeader.bitCount / 8;

    int row_padded = (width * channels + 3) & (~3);
    uchar_t* data = new uchar_t[row_padded * height];
    uchar_t* result = new uchar_t[width * height * channels];

    file.seekg(header.offsetData, std::ios::beg);
    file.read(reinterpret_cast<char*>(data), row_padded * height);
    file.close();

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int srcIndex = (y * row_padded) + (x * channels);
            int dstIndex = ((height - 1 - y) * width + x) * channels;
            result[dstIndex] = data[srcIndex + 2];
            result[dstIndex + 1] = data[srcIndex + 1];
            result[dstIndex + 2] = data[srcIndex];
            if (channels == 4) {
                result[dstIndex + 3] = data[srcIndex + 3];
            }
        }
    }

    delete[] data;
    return result;
}

GLuint loadTexture(const char* filename) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    int width, height;
    uchar_t* data = loadImage(filename, width, height);

    if (data) {
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        delete[] data;
    }

    return textureID;
}

void drawGrass() {
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, gtextureID);

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1000.0f, -0.6f, -1000.0f);
    glTexCoord2f(100.0f, 0.0f); glVertex3f(1000.0f, -0.6f, -1000.0f);
    glTexCoord2f(100.0f, 100.0f); glVertex3f(1000.0f, -0.6f, 1000.0f);
    glTexCoord2f(0.0f, 100.0f); glVertex3f(-1000.0f, -0.6f, 1000.0f);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
