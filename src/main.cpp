//
// Created by Michael Wittmann on 05/06/2020.
//

#include <SFML/Graphics.hpp>
#include <sstream>

#include "ParticleSystem.hpp"

int main() {
  /* Define desired resolution and open a window */
  sf::VideoMode videoMode(1200, 1200);
  sf::RenderWindow window(sf::VideoMode(videoMode),
                          "Inside the Particle Storm");
  window.setVerticalSyncEnabled(true);

  /* Load a font and setup some texty-type stuff */
  sf::Font font;
  if (!font.loadFromFile(
          "/Users/michaelwittmann/CLionProjects/SFMLTest/fixedsys500c.ttf")) {
    return 1;
  }
  sf::Text text("", font, 12);
  text.setPosition(static_cast<float>(window.getSize().x) * 0.01F,
                   static_cast<float>(window.getSize().y) * 0.01F);

  /* Create the particle system and give it some fuel */
  app::ParticleSystem particleSystem(window.getSize());
  particleSystem.fuel(1000);

  /* Let's make a clock and junk for timing stuff! */
  sf::Clock timer;
  const sf::Uint32 UPDATE_STEP = 20;
  const sf::Uint32 MAX_UPDATE_SKIP = 5;
  sf::Uint32 nextUpdate =
      static_cast<unsigned int>(timer.getElapsedTime().asMilliseconds());

  /* For some fancy mouse stuff */
  sf::Vector2f lastMousePos(static_cast<sf::Vector2f>(window.getSize()));

  /* Main Loop */
  while (window.isOpen()) {
    /* Init a skipped frame counter */
    sf::Uint32 frameSkips = 0;

    /* Throttle handlers and update to every UPDATE_STEP */
    while (timer.getElapsedTime().asMilliseconds() >
               static_cast<sf::Uint8>(nextUpdate) &&
           frameSkips < MAX_UPDATE_SKIP) {
      /* Handle events */
      sf::Event event{};
      while (window.pollEvent(event)) {
        switch (event.type) {
          case (sf::Event::Closed): {
            window.close();
            break;
          }
          case (sf::Event::KeyPressed): {
            if (event.key.code == sf::Keyboard::Escape) {
              window.close();
            }
            if (event.key.code == sf::Keyboard::Space) {
              particleSystem.setDissolve();
            }
            if (event.key.code == sf::Keyboard::A) {
              if (particleSystem.getDissolutionRate() > 0) {
                particleSystem.setDissolutionRate(static_cast<sf::Uint8>(
                    particleSystem.getDissolutionRate() - 1));
              }
            }
            if (event.key.code == sf::Keyboard::S) {
              particleSystem.setDissolutionRate(static_cast<sf::Uint8>(
                  particleSystem.getDissolutionRate() + 1));
            }
            if (event.key.code == sf::Keyboard::W) {
              particleSystem.setParticleSpeed(
                  particleSystem.getParticleSpeed() +
                  particleSystem.getParticleSpeed() * 0.1F);
            }
            if (event.key.code == sf::Keyboard::Q &&
                particleSystem.getParticleSpeed() > 0) {
              particleSystem.setParticleSpeed(
                  particleSystem.getParticleSpeed() -
                  particleSystem.getParticleSpeed() * 0.1F);
            }
            if (event.key.code == sf::Keyboard::E) {
              particleSystem.setDistribution();
            }
            break;
          }
          default:
            break;
        }
      }

      /* Mouse Input */
      /* Set the position to match the mouse location */
      sf::Vector2f mousePos =
          static_cast<sf::Vector2f>(sf::Mouse::getPosition(window));

      /* Update Particle Emitter to Mouse Position */
      if (mousePos.x > 0 || mousePos.y > 0 ||
          mousePos.x < static_cast<float>(window.getSize().x) ||
          mousePos.y < static_cast<float>(window.getSize().y)) {
        particleSystem.setPosition(mousePos);
      }

      /* Mouse Clicks */
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        particleSystem.fuel(25);
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        sf::Vector2f newGravity = lastMousePos - mousePos;
        newGravity *= 0.75F;
        particleSystem.setGravity(newGravity);
      }
      if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
        particleSystem.setGravity(0.0F, 0.0F);
      }

      /* Update Last Mouse Position */
      lastMousePos = mousePos;

      /* Push Diag Text */
      std::ostringstream buffer;
      buffer << "Q/W to Decrease/Increase Particle Speed\n"
             << "A/S to Decrease/Increase Decay Rate\n"
             << "Right Click+Drag to Shift Gravity\n"
             << "E to Change Distribution Type\n"
             << "Middle Click clears Gravity\n"
             << "Left Click to Add\n"
             << "Particles: " << particleSystem.getNumberOfParticles();
      text.setString(buffer.str());

      /* Update particle system */
      particleSystem.update(static_cast<float>(UPDATE_STEP) / 1000);

      frameSkips++;
      nextUpdate += UPDATE_STEP;
    }
    /* Draw particle system and text */
    window.clear(sf::Color::Black);
    window.draw(text);
    window.draw(particleSystem);
    window.display();
  }
  return 0;
}
