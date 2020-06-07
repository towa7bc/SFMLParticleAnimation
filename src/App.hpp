//
// Created by Michael Wittmann on 06/06/2020.
//

#ifndef SFMLTEST_APP_HPP
#define SFMLTEST_APP_HPP

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
#include <memory>

#include "ParticleSystem.hpp"  // for ParticleSystem
#include "detail/Core.hpp"     // for create_ref

namespace app {

class App {
 public:
  App();
  int Run();

 private:
  void Setup();
  void Update();
  void Draw();
  void HandleSFMLEvents();
  Scope<sf::RenderWindow> window_;
  Scope<ParticleSystem> particleSystem_;
  bool running_{true};
  sf::Font font_;
  Scope<sf::Text> text_;
  sf::Vector2f lastMousePos_;
  static constexpr sf::Uint32 UPDATE_STEP = 20;
  static constexpr sf::Uint32 MAX_UPDATE_SKIP = 5;
};

}  // namespace app

#endif  // SFMLTEST_APP_HPP
