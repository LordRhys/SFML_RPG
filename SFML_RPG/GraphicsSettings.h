#pragma once
class GraphicsSettings
{
private:

public:
  // Variables
  std::string title;
  sf::VideoMode resolution;
  bool fullscreen;
  bool verticalSync;
  unsigned framerateLimit;
  sf::ContextSettings contextSettings;
  std::vector<sf::VideoMode> videoModes;

  GraphicsSettings();
  virtual ~GraphicsSettings();

  // Functions
  void saveToFile(const std::string path);
  void loadFromFile(const std::string path);
};

