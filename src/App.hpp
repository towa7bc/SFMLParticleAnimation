//
// Created by Michael Wittmann on 06/06/2020.
//

#ifndef SFMLTEST_APP_HPP
#define SFMLTEST_APP_HPP

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

namespace app {

class App {
 public:
  void Setup();
  void Update();
  void Draw();
  void HandleEvent();
  int Run();

 private:
  sf::VideoMode video_mode_;
  sf::RenderWindow window_;
  sf::Event event_;
};

}  // namespace app

#endif  // SFMLTEST_APP_HPP
