#include "vertex.h"

namespace ramiel {

    Vertex_PerTri Vertex_PerTri::operator+(const Vertex_PerTri& other) const {
        Vertex_PerTri temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        return temp;
    }
    
    Vertex_PerTri Vertex_PerTri::operator-(const Vertex_PerTri& other) const {
        Vertex_PerTri temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        return temp;
    }
    
    Vertex_PerTri Vertex_PerTri::operator*(float n) const {
        Vertex_PerTri temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        return temp;
    }
    
    Vertex_PerTri Vertex_PerTri::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerTri temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        return temp;
    }
    
    Vertex_PerTri& Vertex_PerTri::operator+=(const Vertex_PerTri& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        return *this;
    }
    

    Vertex_PerTri_Textured Vertex_PerTri_Textured::operator+(const Vertex_PerTri_Textured& other) const {
        Vertex_PerTri_Textured temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_PerTri_Textured Vertex_PerTri_Textured::operator-(const Vertex_PerTri_Textured& other) const {
        Vertex_PerTri_Textured temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.zinv = zinv - other.zinv;
        temp.texturePos = texturePos - other.texturePos;
        return temp;
    }
    
    Vertex_PerTri_Textured Vertex_PerTri_Textured::operator*(float n) const {
        Vertex_PerTri_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_PerTri_Textured Vertex_PerTri_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerTri_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_PerTri_Textured& Vertex_PerTri_Textured::operator+=(const Vertex_PerTri_Textured& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        zinv += other.zinv;
        texturePos += other.texturePos;
        return *this;
    }
    
    
    Vertex_PerVertex Vertex_PerVertex::operator+(const Vertex_PerVertex& other) const {
        Vertex_PerVertex temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.color = color + other.color;
        return temp;
    }
    
    Vertex_PerVertex Vertex_PerVertex::operator-(const Vertex_PerVertex& other) const {
        Vertex_PerVertex temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.color = color - other.color;
        return temp;
    }
    
    Vertex_PerVertex Vertex_PerVertex::operator*(float n) const {
        Vertex_PerVertex temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.color = color * n;
        return temp;
    }
    
    Vertex_PerVertex Vertex_PerVertex::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerVertex temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.color = color * n;
        return temp;
    }
    
    Vertex_PerVertex& Vertex_PerVertex::operator+=(const Vertex_PerVertex& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        color += other.color;
        return *this;
    }
    
    
    Vertex_PerVertex_Textured Vertex_PerVertex_Textured::operator+(const Vertex_PerVertex_Textured& other) const {
        Vertex_PerVertex_Textured temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.light = light + other.light;
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_PerVertex_Textured Vertex_PerVertex_Textured::operator-(const Vertex_PerVertex_Textured& other) const {
        Vertex_PerVertex_Textured temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.light = light - other.light;
        temp.zinv = zinv - other.zinv;
        temp.texturePos = texturePos - other.texturePos;
        return temp;
    }
    
    Vertex_PerVertex_Textured Vertex_PerVertex_Textured::operator*(float n) const {
        Vertex_PerVertex_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.light = light * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_PerVertex_Textured Vertex_PerVertex_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerVertex_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.light = light * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_PerVertex_Textured& Vertex_PerVertex_Textured::operator+=(const Vertex_PerVertex_Textured& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        light += other.light;
        zinv += other.zinv;
        texturePos += other.texturePos;
        return *this;
    }
    
    
    Vertex_PerPixel Vertex_PerPixel::operator+(const Vertex_PerPixel& other) const {
        Vertex_PerPixel temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.worldPos = worldPos + other.worldPos;
        temp.zinv = zinv + other.zinv;
        return temp;
    }
    
    Vertex_PerPixel Vertex_PerPixel::operator-(const Vertex_PerPixel& other) const {
        Vertex_PerPixel temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.worldPos = worldPos - other.worldPos;
        temp.zinv = zinv - other.zinv;
        return temp;
    }
    
    Vertex_PerPixel Vertex_PerPixel::operator*(float n) const {
        Vertex_PerPixel temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_PerPixel Vertex_PerPixel::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerPixel temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_PerPixel& Vertex_PerPixel::operator+=(const Vertex_PerPixel& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        worldPos += other.worldPos;
        zinv += other.zinv;
        return *this;
    }
    

    Vertex_PerPixel_Textured Vertex_PerPixel_Textured::operator+(const Vertex_PerPixel_Textured& other) const {
        Vertex_PerPixel_Textured temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.worldPos = worldPos + other.worldPos;
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_PerPixel_Textured Vertex_PerPixel_Textured::operator-(const Vertex_PerPixel_Textured& other) const {
        Vertex_PerPixel_Textured temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.worldPos = worldPos - other.worldPos;
        temp.zinv = zinv - other.zinv;
        temp.texturePos = texturePos - other.texturePos;
        return temp;
    }
    
    Vertex_PerPixel_Textured Vertex_PerPixel_Textured::operator*(float n) const {
        Vertex_PerPixel_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_PerPixel_Textured Vertex_PerPixel_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerPixel_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_PerPixel_Textured& Vertex_PerPixel_Textured::operator+=(const Vertex_PerPixel_Textured& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        worldPos += other.worldPos;
        zinv += other.zinv;
        texturePos += other.texturePos;
        return *this;
    }
    

    Vertex_PerPixel_Smooth Vertex_PerPixel_Smooth::operator+(const Vertex_PerPixel_Smooth& other) const {
        Vertex_PerPixel_Smooth temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.worldPos = worldPos + other.worldPos;
        temp.normal = normal + other.normal;
        temp.zinv = zinv + other.zinv;
        return temp;
    }
    
    Vertex_PerPixel_Smooth Vertex_PerPixel_Smooth::operator-(const Vertex_PerPixel_Smooth& other) const {
        Vertex_PerPixel_Smooth temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.worldPos = worldPos - other.worldPos;
        temp.normal = normal - other.normal;
        temp.zinv = zinv - other.zinv;
        return temp;
    }
    
    Vertex_PerPixel_Smooth Vertex_PerPixel_Smooth::operator*(float n) const {
        Vertex_PerPixel_Smooth temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_PerPixel_Smooth Vertex_PerPixel_Smooth::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerPixel_Smooth temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_PerPixel_Smooth& Vertex_PerPixel_Smooth::operator+=(const Vertex_PerPixel_Smooth& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        worldPos += other.worldPos;
        normal += other.normal;
        zinv += other.zinv;
        return *this;
    }
    

    Vertex_PerPixel_Smooth_Textured Vertex_PerPixel_Smooth_Textured::operator+(const Vertex_PerPixel_Smooth_Textured& other) const {
        Vertex_PerPixel_Smooth_Textured temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.worldPos = worldPos + other.worldPos;
        temp.normal = normal + other.normal;
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_PerPixel_Smooth_Textured Vertex_PerPixel_Smooth_Textured::operator-(const Vertex_PerPixel_Smooth_Textured& other) const {
        Vertex_PerPixel_Smooth_Textured temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.worldPos = worldPos - other.worldPos;
        temp.normal = normal - other.normal;
        temp.zinv = zinv - other.zinv;
        temp.texturePos = texturePos - other.texturePos;
        return temp;
    }
    
    Vertex_PerPixel_Smooth_Textured Vertex_PerPixel_Smooth_Textured::operator*(float n) const {
        Vertex_PerPixel_Smooth_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_PerPixel_Smooth_Textured Vertex_PerPixel_Smooth_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerPixel_Smooth_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_PerPixel_Smooth_Textured& Vertex_PerPixel_Smooth_Textured::operator+=(const Vertex_PerPixel_Smooth_Textured& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        worldPos += other.worldPos;
        normal += other.normal;
        zinv += other.zinv;
        texturePos += other.texturePos;
        return *this;
    }

}
