#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include <string>
#include <fstream>
#include <algorithm>
#include <cstdint>

namespace NkMath {

    // --- Vec2d ---
    struct Vec2d {
        double x, y;
        Vec2d() : x(0), y(0) {}
        Vec2d(double x, double y) : x(x), y(y) {}
        double operator[](int i) const { return (&x)[i]; }
        double& operator[](int i) { return (&x)[i]; }
        Vec2d operator+(const Vec2d& o) const { return {x + o.x, y + o.y}; }
        Vec2d operator-(const Vec2d& o) const { return {x - o.x, y - o.y}; }
        Vec2d operator*(double s) const { return {x * s, y * s}; }
        Vec2d operator/(double s) const { return {x / s, y / s}; }
        double Norm2() const { return x * x + y * y; }
        double Norm() const { return std::sqrt(Norm2()); }
        Vec2d Normalized() const { double n = Norm(); return n > 0 ? *this / n : Vec2d(0, 0); }
    };

    inline double Dot(const Vec2d& a, const Vec2d& b) { return a.x * b.x + a.y * b.y; }
    inline double Cross2D(const Vec2d& a, const Vec2d& b) { return a.x * b.y - a.y * b.x; }

    // --- Vec3d ---
    struct Vec3d {
        double x, y, z;
        Vec3d() : x(0), y(0), z(0) {}
        Vec3d(double x, double y, double z) : x(x), y(y), z(z) {}
        double operator[](int i) const { return (&x)[i]; }
        double& operator[](int i) { return (&x)[i]; }
        Vec3d operator+(const Vec3d& o) const { return {x + o.x, y + o.y, z + o.z}; }
        Vec3d operator-(const Vec3d& o) const { return {x - o.x, y - o.y, z - o.z}; }
        Vec3d operator*(double s) const { return {x * s, y * s, z * s}; }
        Vec3d operator/(double s) const { return {x / s, y / s, z / s}; }
        double Norm2() const { return x * x + y * y + z * z; }
        double Norm() const { return std::sqrt(Norm2()); }
        Vec3d Normalized() const { double n = Norm(); return n > 0 ? *this / n : Vec3d(0, 0, 0); }
    };

    inline double Dot(const Vec3d& a, const Vec3d& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
    inline Vec3d Cross(const Vec3d& a, const Vec3d& b) {
        return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
    }

    inline Vec3d Project(const Vec3d& a, const Vec3d& b) {
        double b2 = Dot(b, b);
        return b2 > 1e-12 ? b * (Dot(a, b) / b2) : Vec3d(0, 0, 0);
    }
    inline Vec3d Reject(const Vec3d& a, const Vec3d& b) { return a - Project(a, b); }

    struct OrthoBasis { Vec3d u, v, w; };
    inline OrthoBasis GramSchmidt(Vec3d a, Vec3d b, Vec3d c) {
        Vec3d u = a.Normalized();
        Vec3d v = (b - Project(b, u)).Normalized();
        Vec3d w = (c - Project(c, u) - Project(c, v)).Normalized();
        return {u, v, w};
    }

    // --- Vec4d ---
    struct Vec4d {
        double x, y, z, w;
        Vec4d() : x(0), y(0), z(0), w(0) {}
        Vec4d(double x, double y, double z, double w) : x(x), y(y), z(z), w(w) {}
        Vec4d(const Vec3d& v, double w) : x(v.x), y(v.y), z(v.z), w(w) {}
        Vec3d ToVec3() const { return std::abs(w) > 1e-12 ? Vec3d(x / w, y / w, z / w) : Vec3d(x, y, z); }
    };

    // --- Image ---
    class NkImage {
        int w, h;
        std::vector<uint8_t> data;
    public:
        NkImage(int w, int h) : w(w), h(h), data(w * h * 3, 0) {}
        void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
            if (x >= 0 && x < w && y >= 0 && y < h) {
                int idx = (y * w + x) * 3;
                data[idx] = r; data[idx + 1] = g; data[idx + 2] = b;
            }
        }
        void SavePPM(const std::string& filename) {
            std::ofstream f(filename, std::ios::binary);
            f << "P6\n" << w << " " << h << "\n255\n";
            f.write((char*)data.data(), data.size());
        }
    };
}

void DrawLine(NkMath::NkImage& img, int x0, int y0, int x1, int y1, uint8_t r, uint8_t g, uint8_t b) {
    int dx = std::abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -std::abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (true) {
        img.SetPixel(x0, y0, r, g, b);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

int main() {
    using namespace NkMath;
    printf("=== TP2: Vecteurs & Projection ===\n\n");

    // Tests Vec2d
    Vec2d v1(1, 0), v2(0, 1);
    printf("1. Dot( (1,0), (0,1) ) = %.1f\n", Dot(v1, v2));
    printf("2. Cross2D( (1,0), (0,1) ) = %.1f\n", Cross2D(v1, v2));

    // Tests Vec3d & Gram-Schmidt
    printf("\n3. Gram-Schmidt :\n");
    OrthoBasis ob = GramSchmidt({1, 1, 0}, {1, 0, 1}, {0, 1, 1});
    printf("  u: (%.2f, %.2f, %.2f) Nom=%.2f\n", ob.u.x, ob.u.y, ob.u.z, ob.u.Norm());
    printf("  v: (%.2f, %.2f, %.2f) Norm=%.2f\n", ob.v.x, ob.v.y, ob.v.z, ob.v.Norm());
    printf("  w: (%.2f, %.2f, %.2f) Norm=%.2f\n", ob.w.x, ob.w.y, ob.w.z, ob.w.Norm());
    printf("  u.v = %.2f, u.w = %.2f, v.w = %.2f\n", Dot(ob.u, ob.v), Dot(ob.u, ob.w), Dot(ob.v, ob.w));

    // Projection Cube
    printf("\n4. Projection Cube (cube.ppm) :\n");
    NkImage img(512, 512);
    std::vector<Vec3d> cube = {
        {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, {0.5, 0.5, -0.5}, {-0.5, 0.5, -0.5},
        {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {-0.5, 0.5, 0.5}
    };
    
    auto project = [](const Vec3d& p) {
        double z_cam = 2.0;
        double fx = 500, fy = 500, cx = 256, cy = 256;
        double Z = p.z + z_cam;
        if (Z <= 0.1) Z = 0.1;
        return Vec2d(fx * (p.x / Z) + cx, fy * (p.y / Z) + cy);
    };

    std::vector<Vec2d> pts2d;
    for (auto& p : cube) pts2d.push_back(project(p));

    int edges[12][2] = {
        {0,1}, {1,2}, {2,3}, {3,0}, {4,5}, {5,6}, {6,7}, {7,4}, {0,4}, {1,5}, {2,6}, {3,7}
    };

    for (int i = 0; i < 12; i++) {
        DrawLine(img, (int)pts2d[edges[i][0]].x, (int)pts2d[edges[i][0]].y,
                      (int)pts2d[edges[i][1]].x, (int)pts2d[edges[i][1]].y, 255, 0, 0);
    }
    img.SavePPM("cube.ppm");
    printf("  Fichier cube.ppm genere.\n");

    return 0;
}
