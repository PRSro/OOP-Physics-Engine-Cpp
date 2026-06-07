#pragma once
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>

namespace Shapetype {
    enum Type {
        Circle,
        Rectangle,
        Triangle,
        Unknown
    };
};
struct Shape {
    std::vector<std::pair<double,double>> vertices;
    Shapetype::Type type; 
    Shape(Shapetype::Type t) : type(t) {}
    double width()  const {
        double minX=vertices[0].first,  maxX=minX;
        for (auto& v:vertices) {
            minX=std::min(minX, v.first);
            maxX=std::max(maxX, v.first);
        }
        if(type!=Shapetype::Circle){return maxX-minX;}
        else return maxX;
    }
    double height() const {
        double minY=vertices[0].second, maxY = minY;
        for (auto& v : vertices) {
            minY=std::min(minY, v.second);
            maxY=std::max(maxY, v.second);
        }
        if(type!=Shapetype::Circle){return maxY-minY;}
        else return maxY+minY;
    }
    int pointCount() const { return vertices.size(); }
};

namespace Shapes {
    inline Shape rectangle(double w, double h) {
        Shape s(Shapetype::Rectangle); 
        s.vertices={{0,0}, {w,0}, {w,h}, {0,h}};;
        return s;
    }
    inline Shape triangle(double base, double height) {
        Shape s(Shapetype::Triangle);
        s.vertices={{{0,0},{base,0},{base/2, height}}};
        return s;
    }
    inline Shape circle(double radius, int segments = 16) {
        Shape s(Shapetype::Circle);
        for (int i=0; i<segments; i++) {
            double a=2*M_PI*i/segments;
            double x=radius*std::cos(a)+radius;
            double y=radius*std::sin(a)+radius;
            s.vertices.push_back({x, y});
        }
        return s;
    }
} 
struct Material {
    double mu;
    double restitution;
    std::string type;
    std::string state;
    double max_temperature;
};

namespace Materials {
    inline Material Rubber  ={0.8, 0.7, "Elastic", "Solid", 200.0};
    inline Material Ice     ={0.005, 0.01, "Plastic", "Solid", 0.0};
    inline Material Wood    ={0.4, 0.35, "Elastic", "Solid", 150.0};
    inline Material Concrete={0.6, 0.2, "Plastic", "Solid", 300.0};
    inline Material Water   ={1.7, 1, "Viscous", "Liquid", 100.0};
    inline Material Oil     ={4.8, 5, "Viscous", "Liquid", 250.0};
}

class Forces {
public:
    double normal, friction, force_x, force_y;
    const double gravity;
    Forces(double g=980.0):gravity(g), normal(0), friction(0), force_x(0), force_y(0){}
    void calcValues(double mass, double mu) {
        normal  =mass*gravity;
        friction=mu*normal;
    }
    void decomposeForce(double applied_force, double slope) {
        force_x=applied_force*std::sin(slope);
        force_y=applied_force*std::cos(slope);
    }
};

class Position {
public:
    double x, y, slope;
    Position(double x=0, double y=0, double slope=0):x(x), y(y), slope(slope) {}
};

class Sprite {
public:
    double acceleration_x, acceleration_y, velocity_x, velocity_y, velocity_old, applied_force;
    const double mass;
    double weight;
    bool collision, fixed;
    Material material;
    Position position;
    Forces forces;
    Shape shape;
    Sprite(double m, Material mat, Position pos, Shape shp, double g = 9.8): mass(m), weight(m*g), material(mat), position(pos), forces(g), shape(shp), collision(false), fixed(false), acceleration_x(0), acceleration_y(0), velocity_x(0), velocity_y(0), applied_force(0), velocity_old(0){}
        void applyForce(double fx, double fy) {
            forces.force_x += fx;
            forces.force_y += fy;
        }
        void forceCalc() {
        forces.calcValues(mass, material.mu);
        double grav_y=mass*forces.gravity;
        double ext_fx=forces.force_x;
        double ext_fy=forces.force_y;
        forces.decomposeForce(applied_force, position.slope);
            if(!fixed){
                forces.force_x+=ext_fx;
                forces.force_y+=ext_fy+grav_y;
            }
            if (collision && !fixed) {
                double impulse=mass*velocity_y*material.restitution;
                forces.force_y-=impulse;
            }   
        }
        void update(double deltaTime, double winW, double winH) {
        if (fixed) return;
        forceCalc();
        acceleration_x=forces.force_x/mass;
        acceleration_y=forces.force_y/mass;
        velocity_x+=acceleration_x*deltaTime;
        velocity_y+=acceleration_y*deltaTime;
        position.x+=velocity_x*deltaTime;
        position.y+=velocity_y*deltaTime;
        forces.force_x=0;
        forces.force_y=0;
        if (position.y+shape.height()>=winH) {
            position.y=winH-shape.height();
            velocity_y=-velocity_y * material.restitution;
            velocity_x*=(1.0-material.mu * deltaTime);
            collision=true;
        } else { collision=false; }
        if (position.x+shape.width()>=winW) {
            position.x=winW-shape.width();
            velocity_x=-velocity_x * material.restitution;
        }
        if (position.x<=0.0) {
            position.x=0.5;
            velocity_x=-velocity_x*material.restitution;
        }
        if (position.y<=0.0) {
            position.y=0.5;
            velocity_y=-velocity_y*material.restitution;
        }
    }
};