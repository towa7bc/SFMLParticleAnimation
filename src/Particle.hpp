//
// Created by Michael Wittmann on 05/06/2020.
//

#ifndef SFMLTEST_PARTICLE_HPP
#define SFMLTEST_PARTICLE_HPP

#include <SFML/Graphics.hpp>

namespace app {

class Particle : public sf::Drawable {
 public:
  void draw(sf::RenderTarget &target, sf::RenderStates states) const override;

  [[nodiscard]] const sf::Vertex &getDrawVertex() const { return draw_vertex_; }
  [[nodiscard]] const sf::Vector2f &getVelocity() const { return velocity_; }
  void setVelocity(const sf::Vector2f &vel) { velocity_ = vel; }
  void setDrawVertexColor(const sf::Color &color) {
    draw_vertex_.color = color;
  }
  void setDrawVertexPosition(const sf::Vector2f &pos) {
    draw_vertex_.position = pos;
  }

  void updateDrawVertexColorAlpha(const sf::Uint8 &alpha);
  void updateVelocity(const sf::Vector2f &vel);

 private:
  sf::Vertex draw_vertex_;
  sf::Vector2f velocity_;
};

}  // namespace app

#endif  // SFMLTEST_PARTICLE_HPP
