/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2022 by Wojciech Jarosz
*/

#include <darts/sphere.h>
#include <darts/stats.h>

Sphere::Sphere(float radius, shared_ptr<const Material> material, const Transform &xform) :
    XformedSurfaceWithMaterial(material, xform), m_radius(radius)
{
}

Sphere::Sphere(const json &j) : XformedSurfaceWithMaterial(j)
{
    m_radius = j.value("radius", m_radius);
}

STAT_RATIO("Intersections/Sphere intersection tests per hit", num_sphere_tests, num_sphere_hits);

bool Sphere::intersect(const Ray3f &ray, HitInfo &hit) const
{
    ++g_num_total_intersection_tests;
    ++num_sphere_tests;
    // TODO: Assignment 1: Implement ray-sphere intersection

    Ray3f tray = m_xform.inverse().ray(ray);
    // Center of sphere?
    Vec3f center(0.f);
    Vec3f oc = center - tray.o;
    float a = length2(tray.d);
    float h = dot(tray.d, oc);
    float c = length2(oc) - m_radius * m_radius;
    float discriminant = h*h - a*c;
    if (discriminant < 0)
        return false;
    float sqrtd = std::sqrt(discriminant);
    // Find the nearest root that lies in the acceptable range.
    float root = (h - sqrtd) / a;
    if (root < tray.mint || tray.maxt < root) {
        root = (h + sqrtd) / a;
        if (root < tray.mint || tray.maxt < root)
            return false;
    }

    // TODO: If the ray misses the sphere, you should return false
    // TODO: If you successfully hit something, you should compute the hit point (p),
    //       hit distance (t), and normal (n) and fill in these values
    float t = root;
    Vec3f p = tray(t);
    Vec3f n = m_xform.normal((p - center) / m_radius);
    p = m_xform.point(p);

    // For this assignment you can leave these two values as is
    Vec3f shading_normal = n;
    Vec2f uv             = Vec2f(0.0f, 0.0f);

    // You should only assign hit and return true if you successfully hit something
    hit.t   = t;
    hit.p   = p;
    hit.gn  = n;
    hit.sn  = shading_normal;
    hit.uv  = uv;
    hit.mat = m_material.get();

    ++num_sphere_hits;
    return true;
}

Box3f Sphere::local_bounds() const
{
    return Box3f{Vec3f{-m_radius}, Vec3f{m_radius}};
}


DARTS_REGISTER_CLASS_IN_FACTORY(Surface, Sphere, "sphere")

/**
    \file
    \brief Sphere Surface
*/
