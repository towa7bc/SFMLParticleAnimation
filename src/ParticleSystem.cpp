//
// Created by Michael Wittmann on 05/06/2020.
//

#include "ParticleSystem.hpp"

#include <SFML/Graphics/PrimitiveType.hpp>  // for Points
#include <SFML/Graphics/RenderTarget.hpp>   // for RenderTarget
#include <SFML/Graphics/Vertex.hpp>         // for Vertex
#include <SFML/System/Vector2.hpp>          // for Vector2::Vector2<T>
#include <cmath>                            // for cos, sin
#include <sstream>                          // for ostringstream, basic_ostream
#include <type_traits>                      // for move

namespace app {

ParticleSystem::ParticleSystem(sf::Vector2u canvasSize)
    : dissolve_(false),
      particle_speed_(100.0),
      transparent_(sf::Color(0, 0, 0, 0)),
      dissolutionRate_(4),
      shape_(Shape::CIRCLE),
      gravity_(sf::Vector2f(0.0, 0.0)),
      startPos_(sf::Vector2f(static_cast<float>(canvasSize.x) / 2,
                             static_cast<float>(canvasSize.y) / 2)),
      canvasSize_(canvasSize) {}

/************************************************************/
ParticleSystem::~ParticleSystem() {
  /* Clear vector */
  particles_.clear();
}

/************************************************************/
void ParticleSystem::draw(sf::RenderTarget& target,
                          sf::RenderStates states) const {
  for (const auto& item : particles_) {
    target.draw(&item.getDrawVertex(), 1, sf::Points, states);
  }
}

/************************************************************/
void ParticleSystem::fuel(int numParticles) {
  for (int i = 0; i < numParticles; ++i) {
    /* Generate a new particle and put it at the generation point */
    Particle particle;
    sf::Vector2f pos{startPos_.x, startPos_.y};
    particle.setDrawVertexPosition(pos);

    /* Randomizer initialization */
    std::random_device rd;
    std::mt19937 gen(rd());

    switch (shape_) {
      case Shape::CIRCLE: {
        /* Generate a random angle */
        UniRealDist randomAngle(0.0, (2.0 * 3.14159265));
        auto angle = randomAngle(gen);

        /* Use the random angle as a thrust vector for the particle */
        UniRealDist randomAngleCos(0.0, cos(angle));
        UniRealDist randomAngleSin(0.0, sin(angle));
        sf::Vector2f vel{static_cast<float>(randomAngleCos(gen)),
                         static_cast<float>(randomAngleSin(gen))};
        particle.setVelocity(vel);
        break;
      }
      case Shape::SQUARE: {
        /* Square generation */
        UniRealDist randomSide(-1.0, 1.0);
        sf::Vector2f vel{static_cast<float>(randomSide(gen)),
                         static_cast<float>(randomSide(gen))};
        particle.setVelocity(vel);

        break;
      }
    }

    /* Randomly change the colors of the particles */
    UniIntDist randomColor(0, 255);
    sf::Color color{static_cast<sf::Uint8>(randomColor(gen)),
                    static_cast<sf::Uint8>(randomColor(gen)),
                    static_cast<sf::Uint8>(randomColor(gen)), 255};
    particle.setDrawVertexColor(color);

    particles_.push_back(std::move(particle));
  }
}

/************************************************************/
std::string ParticleSystem::getNumberOfParticlesString() const {
  std::ostringstream oss;
  oss << particles_.size();
  return oss.str();
}

/************************************************************/
void ParticleSystem::update(float deltaTime) {
  /* Run through each particle and apply our system to it */
  for (auto it = particles_.begin(); it != particles_.end(); ++it) {
    /* Apply Gravity */
    sf::Vector2f vel{gravity_.x * deltaTime, gravity_.y * deltaTime};
    (*it).updateVelocity(vel);

    /* Apply thrust */
    sf::Vector2f vertexPosition{(*it).getDrawVertex().position.x,
                                (*it).getDrawVertex().position.y};
    vertexPosition.x += (*it).getVelocity().x * deltaTime * particle_speed_;
    vertexPosition.y += (*it).getVelocity().y * deltaTime * particle_speed_;
    (*it).setDrawVertexPosition(vertexPosition);

    /* If they are set to disolve, disolve */
    if (dissolve_) {
      (*it).updateDrawVertexColorAlpha(dissolutionRate_);
    }

    if ((*it).getDrawVertex().position.x > static_cast<float>(canvasSize_.x) ||
        (*it).getDrawVertex().position.x < 0 ||
        (*it).getDrawVertex().position.y > static_cast<float>(canvasSize_.y) ||
        (*it).getDrawVertex().position.y < 0 ||
        (*it).getDrawVertex().color.a < 10) {
      it = particles_.erase(it);
      if (it == particles_.end()) {
        return;
      }
    }
  }
}

}  // namespace app