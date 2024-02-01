#include <glad/gl.h>

#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>

int main()
{
  sf::ContextSettings settings;
  settings.majorVersion   = 3;
  settings.minorVersion   = 3;
  settings.attributeFlags = sf::ContextSettings::Attribute::Core;

  auto window = sf::Window{
      {1280u, 720u},
      "SFML App",
      sf::Style::Default,
      settings
  };

  window.setActive();

  GladGLContext gl;
  gladLoadGLContext(&gl, reinterpret_cast<GLADloadfunc>(sf::Context::getFunction));

  auto size = window.getSize();
  gl.Viewport(0, 0, size.x, size.y);

  gl.ClearColor(0.2f, 0.2f, 0.2f, 1.0f);

  while (window.isOpen())
  {
    for (auto event = sf::Event{}; window.pollEvent(event);)
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
      else if (event.type == sf::Event::Resized)
      {
        gl.Viewport(0, 0, event.size.width, event.size.height);
      }
    }

    gl.Clear(GL_COLOR_BUFFER_BIT);

    window.display();
  }

  return 0;
}
