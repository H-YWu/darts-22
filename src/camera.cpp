/*
    This file is part of darts – the Dartmouth Academic Ray Tracing Skeleton.

    Copyright (c) 2017-2022 by Wojciech Jarosz
*/
#include <darts/camera.h>
#include <darts/stats.h>

STAT_COUNTER("Integrator/Camera rays traced", num_camera_rays);


Camera::Camera(const json &j)
{
    if (j.count("transform")) {
        Mat44f m;
        json transj = j.value<json>("transform", {{"from", {5.0f, 15.0f, -25.0f}}, {"to", {0.0f, 0.0f, 0.0f}}, {"up", {0.0f, 1.0f, 0.0f}}});
        from_json(transj, m);
        m_xform = Transform(m);
    }
    m_resolution      = j.value("resolution", m_resolution);
    m_focal_distance  = j.value("fdist", m_focal_distance);
    m_aperture_radius = j.value("aperture", m_aperture_radius);

    // TODO: Assignment 1: read the vertical field-of-view from j ("vfov"),
    // and compute the width and height of the image plane. Remember that
    // the "vfov" parameter is specified in degrees, but C++ math functions
    // expect it in radians. You can use deg2rad() from common.h to convert
    // from one to the other
    float vfov = j.value<float>("vfov", 90.f);
    float theta = Spherical::deg2rad(vfov);
    float h = std::tan(theta/2);
    float viewport_height = 2 * h * m_focal_distance;
    float viewport_width = viewport_height * (static_cast<float>(m_resolution.x)/static_cast<float>(m_resolution.y));
    m_size = Vec2f(viewport_width, viewport_height);
}

Ray3f Camera::generate_ray(const Vec2f &pixel) const
{
    ++num_camera_rays;
    // TODO: Assignment 1: Implement camera ray generation
    Vec3f ray_direction(
        m_size.x * ( (pixel.x)/static_cast<float>(m_resolution.x) - 0.5 ),
        m_size.y * ( 0.5 - (pixel.y)/static_cast<float>(m_resolution.y) ),
        -m_focal_distance
    );
    return m_xform.ray(Ray3f(Vec3f(0.f), ray_direction));
}