#include "vertex.h"

namespace ramiel {

    Vertex_PerTri Vertex_PerTri::operator+(const Vertex_PerTri& other) const {
        Vertex_PerTri temp;
        temp.scrPos = scrPos + other.scrPos;
        temp.z = z + other.z;
        return temp;
    }
    
    Vertex_PerTri Vertex_PerTri::operator-(const Vertex_PerTri& other) const {
        Vertex_PerTri temp;
        temp.scrPos = scrPos - other.scrPos;
        temp.z = z - other.z;
        return temp;
    }
    
    Vertex_PerTri Vertex_PerTri::operator*(float n) const {
        Vertex_PerTri temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        return temp;
    }
    
    Vertex_PerTri Vertex_PerTri::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerTri temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        return temp;
    }
    
    Vertex_PerTri& Vertex_PerTri::operator+=(const Vertex_PerTri& other) {
        scrPos += other.scrPos;
        z += other.z;
        return *this;
    }
    

    Vertex_PerTri_Textured Vertex_PerTri_Textured::operator+(const Vertex_PerTri_Textured& other) const {
        Vertex_PerTri_Textured temp;
        temp.scrPos = scrPos + other.scrPos;
        temp.z = z + other.z;
        temp.zinv = zinv + other.zinv;
        temp.uv = uv + other.uv;
        return temp;
    }
    
    Vertex_PerTri_Textured Vertex_PerTri_Textured::operator-(const Vertex_PerTri_Textured& other) const {
        Vertex_PerTri_Textured temp;
        temp.scrPos = scrPos - other.scrPos;
        temp.z = z - other.z;
        temp.zinv = zinv - other.zinv;
        temp.uv = uv - other.uv;
        return temp;
    }
    
    Vertex_PerTri_Textured Vertex_PerTri_Textured::operator*(float n) const {
        Vertex_PerTri_Textured temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.zinv = zinv * n;
        temp.uv = uv * n;
        return temp;
    }
    
    Vertex_PerTri_Textured Vertex_PerTri_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerTri_Textured temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.zinv = zinv * n;
        temp.uv = uv * n;
        return temp;
    }
    
    Vertex_PerTri_Textured& Vertex_PerTri_Textured::operator+=(const Vertex_PerTri_Textured& other) {
        scrPos += other.scrPos;
        z += other.z;
        zinv += other.zinv;
        uv += other.uv;
        return *this;
    }
    
    
    Vertex_PerVertex Vertex_PerVertex::operator+(const Vertex_PerVertex& other) const {
        Vertex_PerVertex temp;
        temp.scrPos = scrPos + other.scrPos;
        temp.z = z + other.z;
        temp.color = color + other.color;
        return temp;
    }
    
    Vertex_PerVertex Vertex_PerVertex::operator-(const Vertex_PerVertex& other) const {
        Vertex_PerVertex temp;
        temp.scrPos = scrPos - other.scrPos;
        temp.z = z - other.z;
        temp.color = color - other.color;
        return temp;
    }
    
    Vertex_PerVertex Vertex_PerVertex::operator*(float n) const {
        Vertex_PerVertex temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.color = color * n;
        return temp;
    }
    
    Vertex_PerVertex Vertex_PerVertex::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerVertex temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.color = color * n;
        return temp;
    }
    
    Vertex_PerVertex& Vertex_PerVertex::operator+=(const Vertex_PerVertex& other) {
        scrPos += other.scrPos;
        z += other.z;
        color += other.color;
        return *this;
    }
    
    
    Vertex_PerVertex_Textured Vertex_PerVertex_Textured::operator+(const Vertex_PerVertex_Textured& other) const {
        Vertex_PerVertex_Textured temp;
        temp.scrPos = scrPos + other.scrPos;
        temp.z = z + other.z;
        temp.color = color + other.color;
        temp.zinv = zinv + other.zinv;
        temp.uv = uv + other.uv;
        return temp;
    }
    
    Vertex_PerVertex_Textured Vertex_PerVertex_Textured::operator-(const Vertex_PerVertex_Textured& other) const {
        Vertex_PerVertex_Textured temp;
        temp.scrPos = scrPos - other.scrPos;
        temp.z = z - other.z;
        temp.color = color - other.color;
        temp.zinv = zinv + other.zinv;
        temp.uv = uv + other.uv;
        return temp;
    }
    
    Vertex_PerVertex_Textured Vertex_PerVertex_Textured::operator*(float n) const {
        Vertex_PerVertex_Textured temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.color = color * n;
        temp.zinv = zinv * n;
        temp.uv = uv * n;
        return temp;
    }
    
    Vertex_PerVertex_Textured Vertex_PerVertex_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerVertex_Textured temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.color = color * n;
        temp.zinv = zinv * n;
        temp.uv = uv * n;
        return temp;
    }
    
    Vertex_PerVertex_Textured& Vertex_PerVertex_Textured::operator+=(const Vertex_PerVertex_Textured& other) {
        scrPos += other.scrPos;
        z += other.z;
        color += other.color;
        zinv += other.zinv;
        uv += other.uv;
        return *this;
    }
    
    
    Vertex_PerPixel Vertex_PerPixel::operator+(const Vertex_PerPixel& other) const {
        Vertex_PerPixel temp;
        temp.scrPos = scrPos + other.scrPos;
        temp.z = z + other.z;
        temp.wrlPos = wrlPos + other.wrlPos;
        temp.zinv = zinv + other.zinv;
        return temp;
    }
    
    Vertex_PerPixel Vertex_PerPixel::operator-(const Vertex_PerPixel& other) const {
        Vertex_PerPixel temp;
        temp.scrPos = scrPos - other.scrPos;
        temp.z = z - other.z;
        temp.wrlPos = wrlPos - other.wrlPos;
        temp.zinv = zinv - other.zinv;
        return temp;
    }
    
    Vertex_PerPixel Vertex_PerPixel::operator*(float n) const {
        Vertex_PerPixel temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.wrlPos = wrlPos * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_PerPixel Vertex_PerPixel::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerPixel temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.wrlPos = wrlPos * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_PerPixel& Vertex_PerPixel::operator+=(const Vertex_PerPixel& other) {
        scrPos += other.scrPos;
        z += other.z;
        wrlPos += other.wrlPos;
        zinv += other.zinv;
        return *this;
    }
    

    Vertex_PerPixel_Textured Vertex_PerPixel_Textured::operator+(const Vertex_PerPixel_Textured& other) const {
        Vertex_PerPixel_Textured temp;
        temp.scrPos = scrPos + other.scrPos;
        temp.z = z + other.z;
        temp.wrlPos = wrlPos + other.wrlPos;
        temp.zinv = zinv + other.zinv;
        temp.uv = uv + other.uv;
        return temp;
    }
    
    Vertex_PerPixel_Textured Vertex_PerPixel_Textured::operator-(const Vertex_PerPixel_Textured& other) const {
        Vertex_PerPixel_Textured temp;
        temp.scrPos = scrPos - other.scrPos;
        temp.z = z - other.z;
        temp.wrlPos = wrlPos - other.wrlPos;
        temp.zinv = zinv - other.zinv;
        temp.uv = uv + other.uv;
        return temp;
    }
    
    Vertex_PerPixel_Textured Vertex_PerPixel_Textured::operator*(float n) const {
        Vertex_PerPixel_Textured temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.wrlPos = wrlPos * n;
        temp.zinv = zinv * n;
        temp.uv = uv * n;
        return temp;
    }
    
    Vertex_PerPixel_Textured Vertex_PerPixel_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerPixel_Textured temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.wrlPos = wrlPos * n;
        temp.zinv = zinv * n;
        temp.uv = uv * n;
        return temp;
    }
    
    Vertex_PerPixel_Textured& Vertex_PerPixel_Textured::operator+=(const Vertex_PerPixel_Textured& other) {
        scrPos += other.scrPos;
        z += other.z;
        wrlPos += other.wrlPos;
        zinv += other.zinv;
        uv += other.uv;
        return *this;
    }
    

    Vertex_PerPixel_Smooth Vertex_PerPixel_Smooth::operator+(const Vertex_PerPixel_Smooth& other) const {
        Vertex_PerPixel_Smooth temp;
        temp.scrPos = scrPos + other.scrPos;
        temp.z = z + other.z;
        temp.wrlPos = wrlPos + other.wrlPos;
        temp.normal = normal + other.normal;
        temp.zinv = zinv + other.zinv;
        return temp;
    }
    
    Vertex_PerPixel_Smooth Vertex_PerPixel_Smooth::operator-(const Vertex_PerPixel_Smooth& other) const {
        Vertex_PerPixel_Smooth temp;
        temp.scrPos = scrPos - other.scrPos;
        temp.z = z - other.z;
        temp.wrlPos = wrlPos - other.wrlPos;
        temp.normal = normal - other.normal;
        temp.zinv = zinv - other.zinv;
        return temp;
    }
    
    Vertex_PerPixel_Smooth Vertex_PerPixel_Smooth::operator*(float n) const {
        Vertex_PerPixel_Smooth temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.wrlPos = wrlPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_PerPixel_Smooth Vertex_PerPixel_Smooth::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerPixel_Smooth temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.wrlPos = wrlPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        return temp;
    }
    
    Vertex_PerPixel_Smooth& Vertex_PerPixel_Smooth::operator+=(const Vertex_PerPixel_Smooth& other) {
        scrPos += other.scrPos;
        z += other.z;
        wrlPos += other.wrlPos;
        normal += other.normal;
        zinv += other.zinv;
        return *this;
    }
    

    Vertex_PerPixel_Smooth_Textured Vertex_PerPixel_Smooth_Textured::operator+(const Vertex_PerPixel_Smooth_Textured& other) const {
        Vertex_PerPixel_Smooth_Textured temp;
        temp.scrPos = scrPos + other.scrPos;
        temp.z = z + other.z;
        temp.wrlPos = wrlPos + other.wrlPos;
        temp.normal = normal + other.normal;
        temp.zinv = zinv + other.zinv;
        temp.uv = uv + other.uv;
        return temp;
    }
    
    Vertex_PerPixel_Smooth_Textured Vertex_PerPixel_Smooth_Textured::operator-(const Vertex_PerPixel_Smooth_Textured& other) const {
        Vertex_PerPixel_Smooth_Textured temp;
        temp.scrPos = scrPos - other.scrPos;
        temp.z = z - other.z;
        temp.wrlPos = wrlPos - other.wrlPos;
        temp.normal = normal - other.normal;
        temp.zinv = zinv - other.zinv;
        temp.uv = uv - other.uv;
        return temp;
    }
    
    Vertex_PerPixel_Smooth_Textured Vertex_PerPixel_Smooth_Textured::operator*(float n) const {
        Vertex_PerPixel_Smooth_Textured temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.wrlPos = wrlPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        temp.uv = uv * n;
        return temp;
    }
    
    Vertex_PerPixel_Smooth_Textured Vertex_PerPixel_Smooth_Textured::operator/(float n) const {
        n = 1.0f / n;
        Vertex_PerPixel_Smooth_Textured temp;
        temp.scrPos = scrPos * n;
        temp.z = z * n;
        temp.wrlPos = wrlPos * n;
        temp.normal = normal * n;
        temp.zinv = zinv * n;
        temp.uv = uv * n;
        return temp;
    }
    
    Vertex_PerPixel_Smooth_Textured& Vertex_PerPixel_Smooth_Textured::operator+=(const Vertex_PerPixel_Smooth_Textured& other) {
        scrPos += other.scrPos;
        z += other.z;
        wrlPos += other.wrlPos;
        normal += other.normal;
        zinv += other.zinv;
        uv += other.uv;
        return *this;
    }

}
