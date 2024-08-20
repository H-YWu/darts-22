/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2022 by Wojciech Jarosz
*/

#include <darts/factory.h>
#include <darts/material.h>
#include <darts/scene.h>

/// A smooth dielectric surface that reflects and refracts light according to the specified index of refraction #ior.
/// \ingroup Materials
class Dielectric : public Material
{
public:
    Dielectric(const json &j = json::object());

    bool scatter(const Ray3f &ray, const HitInfo &hit, Color3f &attenuation, Ray3f &scattered) const override;


    float ior; ///< The (relative) index of refraction of the material
};

Dielectric::Dielectric(const json &j) : Material(j)
{
    ior = j.value("ior", ior);
}

bool Dielectric::scatter(const Ray3f &ray, const HitInfo &hit, Color3f &attenuation, Ray3f &scattered) const
{
    // TODO: Implement dielectric scattering

    attenuation = Color3f(1.f);

    Vec3f unit_direction = normalize(ray.d);
    float ri, cos_theta;
    Vec3f n;
    if (dot(unit_direction, hit.sn) < 0) {
        ri = 1.f/ior;
        cos_theta = std::fmin(dot(-unit_direction, hit.sn), 1.0f);
        n = hit.sn;
    } else {
        ri = ior;
        cos_theta = std::fmin(dot(unit_direction, hit.sn), 1.0f);
        n = -hit.sn;
    }
    Vec3f direction;

    if (!refract(unit_direction, n, ri, direction) ||
        fresnel_dielectric(cos_theta, 1.f, ior) > randf())
        direction = reflect(unit_direction, n);

    scattered = Ray3f(hit.p, direction);
    return true;
}


DARTS_REGISTER_CLASS_IN_FACTORY(Material, Dielectric, "dielectric")

/**
    \file
    \brief Dielectric Material
*/
