//
// Created by Michael Wittmann on 05/06/2020.
//

#include "Particle.hpp"

namespace app {

void Particle::draw(sf::RenderTarget &target, sf::RenderStates states) const {
  target.draw(&draw_vertex_, 1, sf::Points, states);
}

void Particle::updateDrawVertexColorAlpha(const sf::Uint8 &alpha) {
  draw_vertex_.color.a -= alpha;
}

void Particle::updateVelocity(const sf::Vector2f &vel) { velocity_ += vel; }

}  // namespace app