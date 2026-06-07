#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>
#include <algorithm>
#include "shape3D.hpp"

inline sf::Vector2f project(double x, double y, double z,
                            double fov, double cx, double cy) {
    double camZ = z + fov;
    if (camZ <= 0) camZ = 0.001;
    double p = fov / camZ;
    return {(float)(cx + (x - cx) * p), (float)(cy + (y - cy) * p)};
}

inline void drawShape3D(const Shape3D& shape, const Vec3& worldPos,
                        sf::RenderWindow& window,
                        double fov, double cx, double cy) {
    struct FaceSort {
        const Face* face;
        double avgZ;
    };
    std::vector<FaceSort> sorted;
    for (const auto& f : shape.faces) {
        double sumZ = 0;
        for (const auto& v : f.vertices) {
            double vz = std::get<2>(v) + worldPos.z;
            sumZ += vz;
        }
        sorted.push_back({&f, sumZ / f.vertices.size()});
    }
    std::sort(sorted.begin(), sorted.end(),
              [](auto& a, auto& b) { return a.avgZ > b.avgZ; });

    for (auto& fs : sorted) {
        const Face& face = *fs.face;
        if (face.vertices.empty()) continue;
        sf::ConvexShape poly;
        poly.setPointCount(face.vertices.size());
        for (size_t i = 0; i < face.vertices.size(); i++) {
            double vx = std::get<0>(face.vertices[i]) + worldPos.x;
            double vy = std::get<1>(face.vertices[i]) + worldPos.y;
            double vz = std::get<2>(face.vertices[i]) + worldPos.z;
            sf::Vector2f proj = project(vx, vy, vz, fov, cx, cy);
            poly.setPoint(i, proj);
        }
        poly.setFillColor(face.color);
        poly.setOutlineColor(sf::Color::White);
        poly.setOutlineThickness(1.0f);
        window.draw(poly);
    }
}
