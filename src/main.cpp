//
// Created by Michael Wittmann on 05/06/2020.
//

#define GL_SILENCE_DEPRECATION

#include <SFML/Config.hpp>                 // for Uint32, Uint8
#include <SFML/Graphics/Color.hpp>         // for Color, Color::Black
#include <SFML/Graphics/Font.hpp>          // for Font
#include <SFML/Graphics/RenderWindow.hpp>  // for RenderWindow
#include <SFML/Graphics/Text.hpp>          // for Text
#include <SFML/OpenGL.hpp>
#include <SFML/System/Clock.hpp>      // for Clock
#include <SFML/System/Vector2.hpp>    // for Vector2f, Vector2u
#include <SFML/Window/Event.hpp>      // for Event, Event::(anonymous)
#include <SFML/Window/Keyboard.hpp>   // for Keyboard, Keyboard::A, Key...
#include <SFML/Window/Mouse.hpp>      // for Mouse, Mouse::Left, Mouse:...
#include <SFML/Window/VideoMode.hpp>  // for VideoMode
#include <future>                     // for async, launch, launch::async
#include <sstream>                    // for operator<<, basic_ostream

#include "ParticleSystem.hpp"  // for ParticleSystem
#include "detail/Log.hpp"

void ExecuteMainLoop(sf::RenderWindow& window, sf::Text& text,
                     app::ParticleSystem& particleSystem,
                     const sf::Clock& timer, const unsigned int UPDATE_STEP,
                     const unsigned int MAX_UPDATE_SKIP,
                     unsigned int nextUpdate, sf::Vector2f& lastMousePos) {
  /* Main Loop */
  bool running{true};
  while (running) {
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
            running = false;
            window.close();
            break;
          }
          case (sf::Event::Resized): {
            glViewport(0, 0, static_cast<GLsizei>(event.size.width),
                       static_cast<GLsizei>(event.size.height));
            break;
          }
          case (sf::Event::KeyPressed): {
            if (event.key.code == sf::Keyboard::Escape) {
              running = false;
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
          window.mapPixelToCoords(sf::Mouse::getPosition(window));

      /* Update Particle Emitter to Mouse Position */
      if (mousePos.x > 0 || mousePos.y > 0 ||
          mousePos.x < static_cast<float>(window.getSize().x) ||
          mousePos.y < static_cast<float>(window.getSize().y)) {
        particleSystem.setPosition(mousePos);
      }

      /* Mouse Clicks */
      if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        std::future<void> f1 =
            std::async(std::launch::async, [&]() { particleSystem.fuel(50); });
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
      auto f2 = std::async(std::launch::async, [&]() {
        particleSystem.update(static_cast<float>(UPDATE_STEP) / 1000);
      });
      frameSkips++;
      nextUpdate += UPDATE_STEP;
    }
    /* Draw particle system and text */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    window.clear(sf::Color::Black);
    window.draw(text);
    window.draw(particleSystem);
    window.display();
  }
}

int main() {
  /* Define desired resolution and open a window */
  if (app::Log::logger() == nullptr) {
    app::Log::init();
  }
  app::Log::logger()->trace("program started.");
  constexpr int windowWidth{1400};
  constexpr int windowHeight{1000};
  sf::RenderWindow window(sf::VideoMode{windowWidth, windowHeight},
                          "Inside the Particle Storm", sf::Style::Default,
                          sf::ContextSettings{24, 8, 4, 2, 1});
  glEnable(GL_TEXTURE_2D);
  window.setVerticalSyncEnabled(true);
  window.setActive(true);
  /* Load a font and setup some texty-type stuff */
  sf::Font font;
  if (!font.loadFromFile("../../src/detail/fixedsys500c.ttf")) {
    return 1;
  }
  sf::Text text("", font, 12);
  text.setPosition(static_cast<float>(window.getSize().x) * 0.01F,
                   static_cast<float>(window.getSize().y) * 0.01F);

  /* Create the particle system and give it some fuel */
  app::ParticleSystem particleSystem(window.getSize());
  constexpr int numParticles{1000};
  particleSystem.fuel(numParticles);

  /* Let's make a clock and junk for timing stuff! */
  sf::Clock timer;
  constexpr sf::Uint32 UPDATE_STEP = 20;
  constexpr sf::Uint32 MAX_UPDATE_SKIP = 5;
  sf::Uint32 nextUpdate =
      static_cast<unsigned int>(timer.getElapsedTime().asMilliseconds());

  /* For some fancy mouse stuff */
  sf::Vector2f lastMousePos(static_cast<sf::Vector2f>(window.getSize()));
  ExecuteMainLoop(window, text, particleSystem, timer, UPDATE_STEP,
                  MAX_UPDATE_SKIP, nextUpdate, lastMousePos);
  return 0;
}
