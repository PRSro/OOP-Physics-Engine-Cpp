#include <SFML/Graphics.hpp>
#include <iostream>
#include "2D/src/classes.hpp"
#include "2D/include/collisions.h"
#include "3D/src/classes3D.hpp"
#include "3D/include/collisions3D.h"
#include "3D/src/shape3D.hpp"
#include "3D/src/render3D.hpp"

Shape testShape = Shapes::rectangle(100.0, 50.0);
Sprite testSprite(1.0, Materials::Wood, Position(200.0, 200.0, 0.0), testShape);
Shape groundShape = Shapes::rectangle(1010.0, 10.0);
Sprite groundSprite(1.0, Materials::Ice, Position(0.0, 590.0, 0.0), groundShape);
Shape wallShape = Shapes::rectangle(10.0, 600.0);
Sprite wallSprite(1.0, Materials::Concrete, Position(990.0, 0.0, 0.0), wallShape);

std::vector<Sprite*> obstacles = { &groundSprite, &wallSprite };

Shape testShape3D_shp = Shapes::rectangle(100.0, 50.0);
Shape3D testBox3D = Shapes3D::box(100.0, 50.0, 80.0);
Sprite3D testSprite3D(1.0, Materials::Wood, Vec3(200.0, 200.0, 150.0), testShape3D_shp, testBox3D);
Shape groundShape3D_shp = Shapes::rectangle(1010.0, 10.0);
Shape3D groundBox3D = Shapes3D::box(1010.0, 10.0, 600.0);
Sprite3D groundSprite3D(1.0, Materials::Ice, Vec3(0.0, 590.0, 0.0), groundShape3D_shp, groundBox3D);
Shape wallShape3D_shp = Shapes::rectangle(10.0, 600.0);
Shape3D wallBox3D = Shapes3D::box(10.0, 600.0, 600.0);
Sprite3D wallSprite3D(1.0, Materials::Concrete, Vec3(990.0, 0.0, 0.0), wallShape3D_shp, wallBox3D);
std::vector<Sprite3D*> obstacles3D = { &groundSprite3D, &wallSprite3D };

int maxIter=5; 
sf::ConvexShape toSFML(const Shape& shape) {
    sf::ConvexShape sfShape;
    sfShape.setPointCount(shape.vertices.size());
    int n = shape.vertices.size();
    for (int i = 0; i < n; i++)
        sfShape.setPoint(i, sf::Vector2f(shape.vertices[i].first, shape.vertices[i].second));
    sfShape.setFillColor(sf::Color::Blue);
    sfShape.setOutlineColor(sf::Color::White);
    sfShape.setOutlineThickness(1.5f);
    return sfShape;
}

int main() {
    sf::RenderWindow window(sf::VideoMode({1000, 600}), "Physics Simulation");
    auto initSize = window.getSize();
    groundShape = Shapes::rectangle(initSize.x, 10.0);
    groundSprite.shape = groundShape;
    groundSprite.position.x = 0.0;
    groundSprite.position.y = initSize.y - 10.0;
    wallShape = Shapes::rectangle(10.0, initSize.y);
    wallSprite.shape = wallShape;
    wallSprite.position.x = initSize.x - 10.0;
    wallSprite.position.y = 0.0;
    groundSprite.fixed = true;
    wallSprite.fixed = true;
    sf::Clock clock;
    sf::Font font;
    if (!font.openFromFile("arial.ttf")) {
        std::cout << "Font not found\n";
        return 1;
    }
    sf::Text title(font, "Choose dimension: press 2 for 2D, 3 for 3D", 30);
    sf::Text opt2D(font, "2D", 50);
    sf::Text opt3D(font, "3D", 50);
    title.setPosition({100.f, 100.f});
    opt2D.setPosition({300.f, 250.f});
    opt3D.setPosition({450.f, 250.f});
    enum class Screen { MENU, SIM_2D, SIM_3D };
    Screen current = Screen::MENU;
    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
            if (event->is<sf::Event::KeyPressed>()) {
                auto& key = event->getIf<sf::Event::KeyPressed>()->code;
                std::cout << "key pressed: " << (int)key << "\n";
                if (key == sf::Keyboard::Key::Num2) current = Screen::SIM_2D;
                else if (key == sf::Keyboard::Key::Num3) current = Screen::SIM_3D;
                else if (key == sf::Keyboard::Key::F) testSprite.fixed = !testSprite.fixed;
            }
            if (event->is<sf::Event::Resized>()) {
                auto& resized = event->getIf<sf::Event::Resized>()->size;
                window.setView(sf::View(sf::FloatRect({0.f, 0.f},
                    {(float)resized.x, (float)resized.y})));
                groundShape = Shapes::rectangle(resized.x, 10.0);
                groundSprite.shape = groundShape;
                groundSprite.position.x = 0.0;
                groundSprite.position.y = resized.y - 10.0;
                wallShape = Shapes::rectangle(10.0, resized.y);
                wallSprite.shape = wallShape;
                wallSprite.position.x = resized.x - 10.0;
                wallSprite.position.y = 0.0;
            }
        }
        window.clear();
        if (current == Screen::MENU) {
            window.draw(title);
            window.draw(opt2D);
            window.draw(opt3D);
        }
        else if (current == Screen::SIM_2D) {
            float deltaTime = clock.restart().asSeconds();
            auto size = window.getSize();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                testSprite.applyForce(-500.0, 0.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                testSprite.applyForce(500.0, 0.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                testSprite.applyForce(0.0, -500.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                testSprite.applyForce(0.0, 500.0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
                    testShape = Shapes::triangle(100.0, 50.0);
                    testSprite.shape = testShape;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) {
                    testShape = Shapes::circle(50.0, 50.0);
                    testSprite.shape = testShape;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    testShape = Shapes::rectangle(100.0, 50.0);
                    testSprite.shape = testShape;
                }
            }
            resolveAllCollisions(testSprite, obstacles, maxIter);
            testSprite.update(deltaTime, (double)size.x, (double)size.y);
            sf::ConvexShape sfShape = toSFML(testShape);
            sfShape.setPosition({(float)testSprite.position.x,(float)testSprite.position.y});
            window.draw(sfShape);
            for (auto* obs : obstacles) {
                sf::ConvexShape sfObs = toSFML(obs->shape);
                sfObs.setPosition({(float)obs->position.x,
                                   (float)obs->position.y});
                sfObs.setFillColor(sf::Color::Red);
                window.draw(sfObs);
            }
        }
        else if (current == Screen::SIM_3D) {
            float deltaTime = clock.restart().asSeconds();
            auto size = window.getSize();
            double depth = 800.0;

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))
                testSprite3D.applyForce(-500.0, 0.0, 0.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))
                testSprite3D.applyForce(500.0, 0.0, 0.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
                testSprite3D.applyForce(0.0, -500.0, 0.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
                testSprite3D.applyForce(0.0, 500.0, 0.0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q))
                testSprite3D.applyForce(0.0, 0.0, -500.0);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E))
                testSprite3D.applyForce(0.0, 0.0, 500.0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) {
                    testShape3D_shp = Shapes::triangle(100.0, 50.0);
                    testSprite3D.shape = testShape3D_shp;
                    testBox3D = Shapes3D::prism(100.0, 50.0, 80.0);
                    testSprite3D.shape3D = testBox3D;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) {
                    testBox3D = Shapes3D::pyramid(100.0, 80.0, 100.0);
                    testSprite3D.shape3D = testBox3D;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
                    testBox3D = Shapes3D::sphere(50.0);
                    testSprite3D.shape3D = testBox3D;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) {
                    testShape3D_shp = Shapes::rectangle(100.0, 50.0);
                    testSprite3D.shape = testShape3D_shp;
                    testBox3D = Shapes3D::box(100.0, 50.0, 80.0);
                    testSprite3D.shape3D = testBox3D;
                }
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
                    testBox3D = Shapes3D::parallelepiped(100.0, 50.0, 80.0, 20.0, 10.0);
                    testSprite3D.shape3D = testBox3D;
                }
            }

            groundSprite3D.shape = Shapes::rectangle(size.x, 10.0);
            groundSprite3D.position.x = 0.0;
            groundSprite3D.position.y = size.y - 10.0;
            groundSprite3D.position.z = 300.0;
            wallSprite3D.shape = Shapes::rectangle(10.0, size.y);
            wallSprite3D.position.x = size.x - 10.0;
            wallSprite3D.position.y = 0.0;
            wallSprite3D.position.z = 300.0;
            groundSprite3D.shape3D = Shapes3D::box(size.x, 10.0, 600.0);
            groundBox3D = groundSprite3D.shape3D;
            wallSprite3D.shape3D = Shapes3D::box(10.0, size.y, 600.0);
            wallBox3D = wallSprite3D.shape3D;
            groundSprite3D.fixed = true;
            wallSprite3D.fixed = true;

            resolveAllCollisions3D(testSprite3D, obstacles3D, maxIter);
            testSprite3D.update(deltaTime, (double)size.x, (double)size.y, depth);

            double fov = 500.0;
            double cx = size.x / 2.0;
            double cy = size.y / 2.0;

            struct DrawEntry { double z; Shape3D* shape; Vec3* pos; };
            DrawEntry entries[] = {
                {groundSprite3D.position.z, &groundBox3D, &groundSprite3D.position},
                {wallSprite3D.position.z,   &wallBox3D,   &wallSprite3D.position},
                {testSprite3D.position.z,   &testSprite3D.shape3D, &testSprite3D.position}
            };
            std::sort(std::begin(entries), std::end(entries),
                [](auto& a, auto& b) { return a.z > b.z; });
            for (auto& e : entries)
                drawShape3D(*e.shape, *e.pos, window, fov, cx, cy);

            sf::Text zText(font, "Z: " + std::to_string((int)testSprite3D.position.z) + "  Q/E:Z  A+T=prism A+P=pyramid A+S=sphere A+R=box A+L=paral", 18);
            zText.setPosition({10.f, 10.f});
            zText.setFillColor(sf::Color::White);
            window.draw(zText);
        }
        window.display();
    }
    window.display();
}