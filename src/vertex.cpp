#include "vertex.h"

namespace ramiel {

    Vertex_Out_PerTri Vertex_Out_PerTri::operator+(const Vertex_Out_PerTri& other) const {
        Vertex_Out_PerTri temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        return temp;
    }
    
    Vertex_Out_PerTri Vertex_Out_PerTri::operator-(const Vertex_Out_PerTri& other) const {
        Vertex_Out_PerTri temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        return temp;
    }
    
    Vertex_Out_PerTri Vertex_Out_PerTri::operator*(float n) const {
        Vertex_Out_PerTri temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        return temp;
    }
    
    Vertex_Out_PerTri Vertex_Out_PerTri::operator/(float n) const {
        n = 1.0f / n;
        Vertex_Out_PerTri temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        return temp;
    }
    
    Vertex_Out_PerTri& Vertex_Out_PerTri::operator+=(const Vertex_Out_PerTri& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        return *this;
    }
    

    Vertex_Out_PerTri_Textured Vertex_Out_PerTri_Textured::operator+(const Vertex_Out_PerTri_Textured& other) const {
        Vertex_Out_PerTri_Textured temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_Out_PerTri_Textured Vertex_Out_PerTri_Textured::operator-(const Vertex_Out_PerTri_Textured& other) const {
        Vertex_Out_PerTri_Textured temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.zinv = zinv - other.zinv;
        temp.texturePos = texturePos - other.texturePos;
        return temp;
    }
    
    Vertex_Out_PerTri_Textured Vertex_Out_PerTri_Textured::operator*(float n) const {
        Vertex_Out_PerTri_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_Out_PerTri_Textured Vertex_Out_PerTri_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_Out_PerTri_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_Out_PerTri_Textured& Vertex_Out_PerTri_Textured::operator+=(const Vertex_Out_PerTri_Textured& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        zinv += other.zinv;
        texturePos += other.texturePos;
        return *this;
    }
    
    
    Vertex_Out_PerVertex Vertex_Out_PerVertex::operator+(const Vertex_Out_PerVertex& other) const {
        Vertex_Out_PerVertex temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.color = color + other.color;
        return temp;
    }
    
    Vertex_Out_PerVertex Vertex_Out_PerVertex::operator-(const Vertex_Out_PerVertex& other) const {
        Vertex_Out_PerVertex temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.color = color - other.color;
        return temp;
    }
    
    Vertex_Out_PerVertex Vertex_Out_PerVertex::operator*(float n) const {
        Vertex_Out_PerVertex temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.color = color * n;
        return temp;
    }
    
    Vertex_Out_PerVertex Vertex_Out_PerVertex::operator/(float n) const {
        n = 1.0f / n;
        Vertex_Out_PerVertex temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.color = color * n;
        return temp;
    }
    
    Vertex_Out_PerVertex& Vertex_Out_PerVertex::operator+=(const Vertex_Out_PerVertex& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        color += other.color;
        return *this;
    }
    
    
    Vertex_Out_PerVertex_Textured Vertex_Out_PerVertex_Textured::operator+(const Vertex_Out_PerVertex_Textured& other) const {
        Vertex_Out_PerVertex_Textured temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.color = color + other.color;
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_Out_PerVertex_Textured Vertex_Out_PerVertex_Textured::operator-(const Vertex_Out_PerVertex_Textured& other) const {
        Vertex_Out_PerVertex_Textured temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.color = color - other.color;
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_Out_PerVertex_Textured Vertex_Out_PerVertex_Textured::operator*(float n) const {
        Vertex_Out_PerVertex_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.color = color * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_Out_PerVertex_Textured Vertex_Out_PerVertex_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_Out_PerVertex_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.color = color * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_Out_PerVertex_Textured& Vertex_Out_PerVertex_Textured::operator+=(const Vertex_Out_PerVertex_Textured& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        color += other.color;
        zinv += other.zinv;
        texturePos += other.texturePos;
        return *this;
    }
    
    
    Vertex_Out_PerPixel Vertex_Out_PerPixel::operator+(const Vertex_Out_PerPixel& other) const {
        Vertex_Out_PerPixel temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.worldPos = worldPos + other.worldPos;
        temp.zinv = zinv + other.zinv;
        return temp;
    }
    
    Vertex_Out_PerPixel Vertex_Out_PerPixel::operator-(const Vertex_Out_PerPixel& other) const {
        Vertex_Out_PerPixel temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.worldPos = worldPos - other.worldPos;
        temp.zinv = zinv - other.zinv;
        return temp;
    }
    
    Vertex_Out_PerPixel Vertex_Out_PerPixel::operator*(float n) const {
        Vertex_Out_PerPixel temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_Out_PerPixel Vertex_Out_PerPixel::operator/(float n) const {
        n = 1.0f / n;
        Vertex_Out_PerPixel temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_Out_PerPixel& Vertex_Out_PerPixel::operator+=(const Vertex_Out_PerPixel& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        worldPos += other.worldPos;
        zinv += other.zinv;
        return *this;
    }
    

    Vertex_Out_PerPixel_Textured Vertex_Out_PerPixel_Textured::operator+(const Vertex_Out_PerPixel_Textured& other) const {
        Vertex_Out_PerPixel_Textured temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.worldPos = worldPos + other.worldPos;
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_Out_PerPixel_Textured Vertex_Out_PerPixel_Textured::operator-(const Vertex_Out_PerPixel_Textured& other) const {
        Vertex_Out_PerPixel_Textured temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.worldPos = worldPos - other.worldPos;
        temp.zinv = zinv - other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_Out_PerPixel_Textured Vertex_Out_PerPixel_Textured::operator*(float n) const {
        Vertex_Out_PerPixel_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_Out_PerPixel_Textured Vertex_Out_PerPixel_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_Out_PerPixel_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_Out_PerPixel_Textured& Vertex_Out_PerPixel_Textured::operator+=(const Vertex_Out_PerPixel_Textured& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        worldPos += other.worldPos;
        zinv += other.zinv;
        texturePos += other.texturePos;
        return *this;
    }
    

    Vertex_Out_PerPixel_Smooth Vertex_Out_PerPixel_Smooth::operator+(const Vertex_Out_PerPixel_Smooth& other) const {
        Vertex_Out_PerPixel_Smooth temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.worldPos = worldPos + other.worldPos;
        temp.normal = normal + other.normal;
        temp.zinv = zinv + other.zinv;
        return temp;
    }
    
    Vertex_Out_PerPixel_Smooth Vertex_Out_PerPixel_Smooth::operator-(const Vertex_Out_PerPixel_Smooth& other) const {
        Vertex_Out_PerPixel_Smooth temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.worldPos = worldPos - other.worldPos;
        temp.normal = normal - other.normal;
        temp.zinv = zinv - other.zinv;
        return temp;
    }
    
    Vertex_Out_PerPixel_Smooth Vertex_Out_PerPixel_Smooth::operator*(float n) const {
        Vertex_Out_PerPixel_Smooth temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_Out_PerPixel_Smooth Vertex_Out_PerPixel_Smooth::operator/(float n) const {
        n = 1.0f / n;
        Vertex_Out_PerPixel_Smooth temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_Out_PerPixel_Smooth& Vertex_Out_PerPixel_Smooth::operator+=(const Vertex_Out_PerPixel_Smooth& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        worldPos += other.worldPos;
        normal += other.normal;
        zinv += other.zinv;
        return *this;
    }
    

    Vertex_Out_PerPixel_Smooth_Textured Vertex_Out_PerPixel_Smooth_Textured::operator+(const Vertex_Out_PerPixel_Smooth_Textured& other) const {
        Vertex_Out_PerPixel_Smooth_Textured temp;
        temp.screenPos = screenPos + other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] + other.cameraPos[Z];
        temp.worldPos = worldPos + other.worldPos;
        temp.normal = normal + other.normal;
        temp.zinv = zinv + other.zinv;
        temp.texturePos = texturePos + other.texturePos;
        return temp;
    }
    
    Vertex_Out_PerPixel_Smooth_Textured Vertex_Out_PerPixel_Smooth_Textured::operator-(const Vertex_Out_PerPixel_Smooth_Textured& other) const {
        Vertex_Out_PerPixel_Smooth_Textured temp;
        temp.screenPos = screenPos - other.screenPos;
        temp.cameraPos[Z] = cameraPos[Z] - other.cameraPos[Z];
        temp.worldPos = worldPos - other.worldPos;
        temp.normal = normal - other.normal;
        temp.zinv = zinv - other.zinv;
        temp.texturePos = texturePos - other.texturePos;
        return temp;
    }
    
    Vertex_Out_PerPixel_Smooth_Textured Vertex_Out_PerPixel_Smooth_Textured::operator*(float n) const {
        Vertex_Out_PerPixel_Smooth_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_Out_PerPixel_Smooth_Textured Vertex_Out_PerPixel_Smooth_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_Out_PerPixel_Smooth_Textured temp;
        temp.screenPos = screenPos * n;
        temp.cameraPos[Z] = cameraPos[Z] * n;
        temp.worldPos = worldPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        temp.texturePos = texturePos * n;
        return temp;
    }
    
    Vertex_Out_PerPixel_Smooth_Textured& Vertex_Out_PerPixel_Smooth_Textured::operator+=(const Vertex_Out_PerPixel_Smooth_Textured& other) {
        screenPos += other.screenPos;
        cameraPos[Z] += other.cameraPos[Z];
        worldPos += other.worldPos;
        normal += other.normal;
        zinv += other.zinv;
        texturePos += other.texturePos;
        return *this;
    }

}
