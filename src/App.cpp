//
// Created by Michael Wittmann on 06/06/2020.
//

#define GL_SILENCE_DEPRECATION

#include "App.hpp"

#include <cmath>
#include <future>
#include <memory>
#include <sstream>  // for operator<<, basic_ostream

#include "detail/Core.hpp"  // for create_ref
#include "detail/Log.hpp"

namespace app {

int App::Run() {
  sf::Clock timer;
  sf::Uint32 nextUpdate =
      static_cast<unsigned int>(timer.getElapsedTime().asMilliseconds());
  while (running_) {
    UpdateFPS();
    sf::Uint32 frameSkips = 0;
    while (timer.getElapsedTime().asMilliseconds() >
               static_cast<sf::Uint8>(nextUpdate) &&
           frameSkips < MAX_UPDATE_SKIP) {
      UpdateSFMLEvents();
      Update();
      frameSkips++;
      nextUpdate += UPDATE_STEP;
    }
    Draw();
  }
  return 0;
}

void App::UpdateSFMLEvents() {
  sf::Event event{};
  while (window_->pollEvent(event)) {
    switch (event.type) {
      case (sf::Event::Closed): {
        running_ = false;
        window_->close();
        break;
      }
      case (sf::Event::Resized): {
        glViewport(0, 0, static_cast<GLsizei>(event.size.width),
                   static_cast<GLsizei>(event.size.height));
        glGetError();
        break;
      }
      case (sf::Event::KeyPressed): {
        if (event.key.code == sf::Keyboard::Escape) {
          running_ = false;
          window_->close();
        }
        if (event.key.code == sf::Keyboard::F) {
          static bool fullscreen{true};
          std::vector<sf::VideoMode> modes =
              sf::VideoMode::getFullscreenModes();
          window_->create(
              sf::VideoMode{modes[0].width, modes[0].height,
                            modes[0].bitsPerPixel},
              "Inside the Particle Storm",
              fullscreen ? sf::Style::Fullscreen : sf::Style::Default,
              sf::ContextSettings{24, 8, 4, 2, 1});
          fullscreen = !fullscreen;
          window_->setActive(true);
          glEnable(GL_TEXTURE_2D);
          window_->setVerticalSyncEnabled(true);
        }
        if (event.key.code == sf::Keyboard::Space) {
          particleSystem_->setDissolve();
        }
        if (event.key.code == sf::Keyboard::A) {
          if (particleSystem_->getDissolutionRate() > 0) {
            particleSystem_->setDissolutionRate(static_cast<sf::Uint8>(
                particleSystem_->getDissolutionRate() - 1));
          }
        }
        if (event.key.code == sf::Keyboard::S) {
          particleSystem_->setDissolutionRate(static_cast<sf::Uint8>(
              particleSystem_->getDissolutionRate() + 1));
        }
        if (event.key.code == sf::Keyboard::W) {
          particleSystem_->setParticleSpeed(
              particleSystem_->getParticleSpeed() +
              particleSystem_->getParticleSpeed() * 0.1F);
        }
        if (event.key.code == sf::Keyboard::Q &&
            particleSystem_->getParticleSpeed() > 0) {
          particleSystem_->setParticleSpeed(
              particleSystem_->getParticleSpeed() -
              particleSystem_->getParticleSpeed() * 0.1F);
        }
        if (event.key.code == sf::Keyboard::E) {
          particleSystem_->setDistribution();
        }
        break;
      }
      default:
        break;
    }
  }

  auto f2 = std::async(std::launch::async, [&]() {
    /* Mouse Input */
    /* Set the position to match the mouse location */
    sf::Vector2f mousePos =
        window_->mapPixelToCoords(sf::Mouse::getPosition(*window_));

    /* Update Particle Emitter to Mouse Position */
    if (mousePos.x > 0 || mousePos.y > 0 ||
        mousePos.x < static_cast<float>(window_->getSize().x) ||
        mousePos.y < static_cast<float>(window_->getSize().y)) {
      particleSystem_->setPosition(mousePos);
    }
    /* Mouse Clicks */
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      particleSystem_->fuel(50);
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
      sf::Vector2f newGravity = lastMousePos_ - mousePos;
      newGravity *= 0.75F;
      particleSystem_->setGravity(newGravity);
    }
    if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
      particleSystem_->setGravity(0.0F, 0.0F);
    }

    /* Update Last Mouse Position */
    lastMousePos_ = mousePos;

    /* Push Diag Text */
    std::ostringstream buffer;
    buffer << "Q/W to Decrease/Increase Particle Speed\n"
           << "A/S to Decrease/Increase Decay Rate\n"
           << "F to Toggle Fullscreen\n"
           << "Right Click+Drag to Shift Gravity\n"
           << "E to Change Distribution Type\n"
           << "Middle Click clears Gravity\n"
           << "Left Click to Add\n"
           << "Frames per Second (FPS): " << fps_ << "\n"
           << "Particles: " << particleSystem_->getNumberOfParticles();
    text_->setString(buffer.str());
  });
}

void App::Update() {
  /* Update particle system */
  auto f2 = std::async(std::launch::async, [&]() {
    particleSystem_->update(static_cast<float>(UPDATE_STEP) / 1000);
  });
}

void App::Draw() {
  /* Draw particle system and text */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glGetError();
  window_->setActive(false);
  auto f2 = std::async(std::launch::async, [&]() {
    window_->setActive(true);
    window_->clear(sf::Color::Black);
    window_->resetGLStates();
    window_->draw(*text_);
    window_->draw(*particleSystem_);
    window_->display();
  });
}

void App::Setup() {
  if (Log::logger() == nullptr) {
    Log::Initialize();
  }
  Log::logger()->trace("program started.");
  constexpr int windowWidth{1400};
  constexpr int windowHeight{1000};
  window_ = create_scope<sf::RenderWindow>(
      sf::VideoMode{windowWidth, windowHeight}, "Inside the Particle Storm",
      sf::Style::Default, sf::ContextSettings{24, 8, 4, 2, 1});
  window_->setActive(true);
  glEnable(GL_TEXTURE_2D);
  window_->setVerticalSyncEnabled(true);
  particleSystem_ = create_scope<ParticleSystem>(window_->getSize());
  particleSystem_->fuel(1000);
  if (!font_.loadFromFile("../../src/detail/fixedsys500c.ttf")) {
    return;
  }
  text_ = create_scope<sf::Text>("", font_, 12);
  text_->setPosition(static_cast<float>(window_->getSize().x) * 0.01F,
                     static_cast<float>(window_->getSize().y) * 0.01F);
  lastMousePos_ = static_cast<sf::Vector2f>(window_->getSize());
}

App::App() { Setup(); }

void App::UpdateFPS() {
  auto f2 = std::async(std::launch::async, [&]() {
    return std::round(1.f / fpsClock_.restart().asSeconds());
  });
  fps_ = f2.get();
}

}  // namespace app