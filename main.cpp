#include <SFML/Graphics.hpp>
#include <iostream>
#include "2D/src/classes.hpp"
#include "2D/src/collisions.hpp"

Shape testShape = Shapes::rectangle(50.0, 50.0);
Sprite testSprite(1.0, Materials::Wood, Position(200.0, 200.0, 0.0), testShape);

sf::ConvexShape toSFML(const Shape& shape) {
	sf::ConvexShape sfShape;
	sfShape.setPointCount(shape.vertices.size());
	for (int i=0; i<(int)shape.vertices.size(); i++)
		sfShape.setPoint(i, sf::Vector2f(
		                     shape.vertices[i].first,
		                     shape.vertices[i].second
		                 ));
	sfShape.setFillColor(sf::Color::Green);
	sfShape.setOutlineColor(sf::Color::White);
	sfShape.setOutlineThickness(1.f);
	return sfShape;
}

int main() {
	sf::RenderWindow window(sf::VideoMode({800, 600}), "Physics Simulation");
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
                std::cout << "key pressed: " << (int)key << "\n"; // add this
				if (key == sf::Keyboard::Key::Num2) current = Screen::SIM_2D;
				else if (key == sf::Keyboard::Key::Num3) current = Screen::SIM_3D;
			}
			if (event->is<sf::Event::Resized>()) {
				auto& resized = event->getIf<sf::Event::Resized>()->size;
				window.setView(sf::View(sf::FloatRect(
				{0.f, 0.f},
				{(float)resized.x, (float)resized.y}
				                        )));
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
			testSprite.update(deltaTime, (double)size.x, (double)size.y);
             std::cout << "pos: " << testSprite.position.x 
              << " " << testSprite.position.y
              << " vy: " << testSprite.velocity_y
              << " fy: " << testSprite.forces.force_y
              << "\n";
			sf::ConvexShape sfShape = toSFML(testShape);
			sfShape.setPosition({
				(float)testSprite.position.x,
				(float)testSprite.position.y
			});
			window.draw(sfShape);
		}
		else if (current == Screen::SIM_3D) {

		}

		window.display();
	}
	window.display();
}