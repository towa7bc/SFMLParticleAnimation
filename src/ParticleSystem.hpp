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
#include <random>  // for uniform_real_distribution
#include <vector>  // for vector

#include "Particle.hpp"  // for Particle
namespace sf {
class RenderTarget;
}

namespace app {

enum class Shape { CIRCLE = 0, SQUARE = 1 };
using UniRealDist = std::uniform_real_distribution<>;
using UniIntDist = std::uniform_int_distribution<>;

class ParticleSystem : public sf::Drawable {
 public:
  explicit ParticleSystem(sf::Vector2u canvasSize);
  ParticleSystem(const ParticleSystem &) = default;
  ParticleSystem(ParticleSystem &&) noexcept = default;
  ParticleSystem &operator=(const ParticleSystem &) = default;
  ParticleSystem &operator=(ParticleSystem &&) noexcept = default;
  ~ParticleSystem() override;

  void setPosition(const sf::Vector2f &position) { startPos_ = position; }
  void setShape(const Shape &shape) { shape_ = shape; }

  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;
  void fuel(int numParticles);  /*< Adds new particles */
  void update(float deltaTime); /*< Updates particles */
  [[nodiscard]] int getDissolutionRate() const { return dissolutionRate_; }
  [[nodiscard]] int getNumberOfParticles() const {
    return static_cast<int>(particles_.size());
  }
  [[nodiscard]] float getParticleSpeed() const { return particle_speed_; }
  [[nodiscard]] std::string getNumberOfParticlesString() const;

  void setCanvasSize(const sf::Vector2u &newSize) { canvasSize_ = newSize; }
  void setDissolutionRate(sf::Uint8 rate) { dissolutionRate_ = rate; }
  void setDissolve() { dissolve_ = !dissolve_; }
  void setDistribution() {
    shape_ = static_cast<Shape>((static_cast<int>(shape_) + 1) % 2);
  }
  void setGravity(float x, float y) {
    gravity_.x = x;
    gravity_.y = y;
  }
  void setGravity(const sf::Vector2f &gravity) { gravity_ = gravity; }
  void setParticleSpeed(float speed) { particle_speed_ = speed; }
  void setPosition(float x, float y) {
    startPos_.x = x;
    startPos_.y = y;
  }

 private:
  bool dissolve_;        /*< Dissolution enabled? */
  float particle_speed_; /*< Pixels per second (at most) */

  sf::Color transparent_; /*< sf::Color(0, 0, 0, 0) */

  sf::Uint8 dissolutionRate_; /*< Rate particles disolve */
  Shape shape_;               /*< Shape of distribution */

  sf::Vector2f gravity_;    /*< Influences particle velocities */
  sf::Vector2f startPos_;   /*< Particle origin */
  sf::Vector2u canvasSize_; /*< Limits of particle travel */

  std::vector<Particle> particles_;
};

}  // namespace app

#endif  // SFMLTEST_PARTICLESYSTEM_HPP
