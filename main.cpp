#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <cstdint>
#include "Vec.h"

namespace NkMath {
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

    Vec2d v1(1, 0), v2(0, 1);
    printf("1. Dot( (1,0), (0,1) ) = %.1f\n", Dot(v1, v2));

    printf("\n3. Gram-Schmidt :\n");
    OrthoBasis ob = GramSchmidt({1, 1, 0}, {1, 0, 1}, {0, 1, 1});
    printf("  u.v = %.2f, u.w = %.2f, v.w = %.2f\n", Dot(ob.u, ob.v), Dot(ob.u, ob.w), Dot(ob.v, ob.w));

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
