//
// Created by Michael Wittmann on 05/06/2020.
//

#ifndef SFMLTEST_PARTICLESYSTEM_HPP
#define SFMLTEST_PARTICLESYSTEM_HPP

#include <SFML/Config.hpp>                 // for Uint8
#include <SFML/Graphics/Color.hpp>         // for Color
#include <SFML/Graphics/Drawable.hpp>      // for Drawable
#include <SFML/Graphics/RenderStates.hpp>  // for RenderStates
#include <SFML/System/Vector2.hpp>         // for Vector2f, Vector2u
#include <algorithm>                       // for uniform_int_distribution
#include <iosfwd>                          // for string
#include <memory>
#include <random>
#include <random>  // for uniform_real_distribution
#include <vector>
#include <vector>  // for vector

#include "Particle.hpp"  // for Particle
namespace sf {
class RenderTarget;
}

namespace Shape {
enum { CIRCLE, SQUARE };
}
namespace app {

using UniRealDist = std::uniform_real_distribution<>;
using UniIntDist = std::uniform_int_distribution<>;

class ParticleSystem : public sf::Drawable {
 public:
  /* Constructors/Destructor */

  explicit ParticleSystem(sf::Vector2u canvasSize);
  ParticleSystem(const ParticleSystem &) = default;
  ParticleSystem(ParticleSystem &&) noexcept = default;
  ParticleSystem &operator=(const ParticleSystem &) = default;
  ParticleSystem &operator=(ParticleSystem &&) noexcept = default;
  ~ParticleSystem() override;

  /* Getters and Setters */

  [[nodiscard]] int getDissolutionRate() const { return dissolutionRate_; }
  [[nodiscard]] int getNumberOfParticles() const {
    return static_cast<int>(particles_.size());
  }
  [[nodiscard]] float getParticleSpeed() const { return particle_speed_; }
  [[nodiscard]] std::string getNumberOfParticlesString() const;

  void setCanvasSize(sf::Vector2u newSize) { canvasSize_ = newSize; }
  void setDissolutionRate(sf::Uint8 rate) { dissolutionRate_ = rate; }
  void setDissolve() { dissolve_ = !dissolve_; }
  void setDistribution() { shape_ = (shape_ + 1) % 2; }
  void setGravity(float x, float y) {
    gravity_.x = x;
    gravity_.y = y;
  }
  void setGravity(sf::Vector2f gravity) { gravity_ = gravity; }
  void setParticleSpeed(float speed) { particle_speed_ = speed; }
  void setPosition(float x, float y) {
    startPos_.x = x;
    startPos_.y = y;
  }
  void setPosition(sf::Vector2f position) { startPos_ = position; }
  void setShape(sf::Uint8 shape) { shape_ = shape; }

  /* Member Functions */

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  void fuel(int numParticles);  /*< Adds new particles */
  void update(float deltaTime); /*< Updates particles */

 private:
  /* Data Members */

  bool dissolve_;        /*< Dissolution enabled? */
  float particle_speed_; /*< Pixels per second (at most) */

  sf::Color transparent_; /*< sf::Color(0, 0, 0, 0) */

  sf::Uint8 dissolutionRate_; /*< Rate particles disolve */
  sf::Uint8 shape_;           /*< Shape of distribution */

  sf::Vector2f gravity_;    /*< Influences particle velocities */
  sf::Vector2f startPos_;   /*< Particle origin */
  sf::Vector2u canvasSize_; /*< Limits of particle travel */

  /* Container */
  std::vector<Particle> particles_;
};

}  // namespace app

#endif  // SFMLTEST_PARTICLESYSTEM_HPP
