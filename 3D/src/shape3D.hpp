#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <tuple>

struct Face {
    std::vector<std::tuple<double,double,double>> vertices;
    sf::Color color;
};

struct Shape3D {
    std::vector<Face> faces;
    double width, height, depth;
};

namespace Shapes3D {
    inline Shape3D box(double w, double h, double d) {
        Shape3D s;
        s.width = w; s.height = h; s.depth = d;
        Face front, back, left, right, top, bottom;

        front.vertices  = {{0,0,d}, {w,0,d}, {w,h,d}, {0,h,d}};
        front.color = sf::Color(200, 200, 200);

        back.vertices   = {{w,0,0}, {0,0,0}, {0,h,0}, {w,h,0}};
        back.color = sf::Color(80, 80, 80);

        left.vertices   = {{0,0,0}, {0,0,d}, {0,h,d}, {0,h,0}};
        left.color = sf::Color(140, 140, 140);

        right.vertices  = {{w,0,d}, {w,0,0}, {w,h,0}, {w,h,d}};
        right.color = sf::Color(160, 160, 160);

        top.vertices    = {{0,0,0}, {w,0,0}, {w,0,d}, {0,0,d}};
        top.color = sf::Color(220, 220, 220);

        bottom.vertices = {{0,h,d}, {w,h,d}, {w,h,0}, {0,h,0}};
        bottom.color = sf::Color(60, 60, 60);

        s.faces = {front, back, left, right, top, bottom};
        return s;
    }

    inline Shape3D parallelepiped(double w, double h, double d,
                                   double shearX, double shearZ) {
        Shape3D s;
        s.width=w; s.height=h; s.depth=d;
        double tx=shearX, tz=shearZ;
        Face front; front.color=sf::Color(200,200,200);
        front.vertices={{tx,0,d+tz},{tx+w,0,d+tz},{w,h,d},{0,h,d}};
        Face back; back.color=sf::Color(80,80,80);
        back.vertices={{w,h,0},{0,h,0},{tx,0,tz},{tx+w,0,tz}};
        Face left; left.color=sf::Color(140,140,140);
        left.vertices={{0,h,0},{0,h,d},{tx,0,d+tz},{tx,0,tz}};
        Face right; right.color=sf::Color(160,160,160);
        right.vertices={{tx+w,0,tz},{tx+w,0,d+tz},{w,h,d},{w,h,0}};
        Face top; top.color=sf::Color(220,220,220);
        top.vertices={{tx,0,tz},{tx+w,0,tz},{tx+w,0,d+tz},{tx,0,d+tz}};
        Face bottom; bottom.color=sf::Color(60,60,60);
        bottom.vertices={{0,h,d},{w,h,d},{w,h,0},{0,h,0}};
        s.faces={front,back,left,right,top,bottom};
        return s;
    }

    inline Shape3D prism(double w, double h, double d) {
        Shape3D s;
        s.width=w; s.height=h; s.depth=d;
        Face front; front.color=sf::Color(200,200,200);
        front.vertices={{0,h,d},{w/2,0,d},{w,h,d}};
        Face back; back.color=sf::Color(80,80,80);
        back.vertices={{w,h,0},{w/2,0,0},{0,h,0}};
        Face sideL; sideL.color=sf::Color(140,140,140);
        sideL.vertices={{0,h,0},{0,h,d},{w/2,0,d},{w/2,0,0}};
        Face sideR; sideR.color=sf::Color(160,160,160);
        sideR.vertices={{w/2,0,0},{w/2,0,d},{w,h,d},{w,h,0}};
        Face bottom; bottom.color=sf::Color(60,60,60);
        bottom.vertices={{0,h,d},{w,h,d},{w,h,0},{0,h,0}};
        s.faces={front,back,sideL,sideR,bottom};
        return s;
    }

    inline Shape3D pyramid(double w, double h, double d) {
        Shape3D s;
        s.width=w; s.height=h; s.depth=d;
        double ax=w/2, ay=0, az=d/2;
        Face base; base.color=sf::Color(60,60,60);
        base.vertices={{0,h,0},{w,h,0},{w,h,d},{0,h,d}};
        Face front; front.color=sf::Color(200,200,200);
        front.vertices={{0,h,d},{w,h,d},{ax,ay,az}};
        Face back; back.color=sf::Color(80,80,80);
        back.vertices={{w,h,0},{0,h,0},{ax,ay,az}};
        Face left; left.color=sf::Color(140,140,140);
        left.vertices={{0,h,0},{0,h,d},{ax,ay,az}};
        Face right; right.color=sf::Color(160,160,160);
        right.vertices={{w,h,d},{w,h,0},{ax,ay,az}};
        s.faces={base,front,back,left,right};
        return s;
    }

    inline Shape3D sphere(double radius, int latSegs=8, int lonSegs=12) {
        Shape3D s;
        s.width=radius*2; s.height=radius*2; s.depth=radius*2;
        auto sph = [&](double lat, double lon) -> std::tuple<double,double,double> {
            return {
                radius + radius*std::sin(lat)*std::cos(lon),
                radius - radius*std::cos(lat),
                radius + radius*std::sin(lat)*std::sin(lon)
            };
        };
        for (int i=0; i<latSegs; i++) {
            double lat0 = M_PI * i     / latSegs;
            double lat1 = M_PI * (i+1) / latSegs;
            for (int j=0; j<lonSegs; j++) {
                double lon0 = 2*M_PI * j     / lonSegs;
                double lon1 = 2*M_PI * (j+1) / lonSegs;
                Face f;
                uint8_t shade = (uint8_t)(80 + 140.0*i/latSegs);
                f.color = sf::Color(shade, shade, shade);
                f.vertices = {sph(lat0,lon0), sph(lat0,lon1),
                              sph(lat1,lon1), sph(lat1,lon0)};
                s.faces.push_back(f);
            }
        }
        return s;
    }
}
