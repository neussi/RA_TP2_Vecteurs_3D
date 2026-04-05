#pragma once
#include <cmath>
#include <vector>
#include <iostream>

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
        Vec2d Normalized() const { double n = Norm(); return n > 1e-12 ? *this / n : Vec2d(0, 0); }
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
        Vec3d Normalized() const { double n = Norm(); return n > 1e-12 ? *this / n : Vec3d(0, 0, 0); }
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

}
