#include <Unitest/Unitest.h>
#include <Unitest/TestMacro.h>
#include "../Vec.h"

using namespace NkMath;

TEST_CASE(TP2, VectorArithmetic) {
    Vec3d a(1, 2, 3), b(4, 5, 6);
    Vec3d c = a + b;
    ASSERT_EQUAL(c.x, 5.0);
    ASSERT_EQUAL(c.y, 7.0);
    ASSERT_EQUAL(c.z, 9.0);
}

TEST_CASE(TP2, DotCross) {
    Vec3d a(1, 0, 0), b(0, 1, 0);
    ASSERT_EQUAL(Dot(a, b), 0.0);
    Vec3d c = Cross(a, b);
    ASSERT_EQUAL(c.z, 1.0);
    ASSERT_EQUAL(c.x, 0.0);
    ASSERT_EQUAL(c.y, 0.0);
}

TEST_CASE(TP2, GramSchmidt) {
    OrthoBasis ob = GramSchmidt({1, 1, 0}, {1, 0, 1}, {0, 1, 1});
    ASSERT_NEAR(Dot(ob.u, ob.v), 0.0, 1e-9);
    ASSERT_NEAR(Dot(ob.u, ob.w), 0.0, 1e-9);
    ASSERT_NEAR(Dot(ob.v, ob.w), 0.0, 1e-9);
    ASSERT_NEAR(ob.u.Norm(), 1.0, 1e-9);
}

TEST_CASE(TP2, Dehomogenization) {
    Vec4d v4(2, 4, 6, 2);
    Vec3d v3 = v4.ToVec3();
    ASSERT_EQUAL(v3.x, 1.0);
    ASSERT_EQUAL(v3.y, 2.0);
    ASSERT_EQUAL(v3.z, 3.0);
}
