#pragma once

#include "MainMenuState.h"

class Game
{
private:
  
  // Variables
  GraphicsSettings gfxSettings;
  sf::RenderWindow *window;
  sf::Event sfEvent;

  sf::Clock dtClock;
  float dt;

  std::stack<State*> states;

  std::map<std::string, int> supportedKeys;

  // Initialization
  void initVariables();
  void initGraphicSettings();
  void initWindow();
  void initKeys();
  void initStates();  

public:
  // Constructors / Destructors
  Game();
  virtual ~Game();

  //Functions


  // Regular
  void endApplication();

  // Updates
  void updateDT();
  void updateSFMLEvents();
  void update();

  // Render
  void render();

  // Core
  void run();
};

