#include "stdafx.h"
#include "Game.h"

// Static functions

// Initializer functions
void Game::initVariables()
{
  this->window = nullptr;

  this->dt = 0.0f;
}

void Game::initGraphicSettings()
{
  this->gfxSettings.loadFromFile("Config/graphics.ini");  
}

void Game::initWindow()
{
  // Create an SFML window
  
  if(this->gfxSettings.fullscreen)
    this->window = new sf::RenderWindow(this->gfxSettings.resolution, 
                                        this->gfxSettings.title,
                                        sf::Style::Fullscreen, 
                                        this->gfxSettings.contextSettings);
  else
    this->window = new sf::RenderWindow(this->gfxSettings.resolution,
                                        this->gfxSettings.title,
                                        sf::Style::Titlebar | sf::Style::Close, 
                                        this->gfxSettings.contextSettings);

  this->window->setFramerateLimit(this->gfxSettings.framerateLimit);
  this->window->setVerticalSyncEnabled(this->gfxSettings.verticalSync);
}

void Game::initKeys()
{
  std::ifstream ifs("Config/supported_keys.ini");

  if (ifs.is_open())
  {
    std::string key = "";
    int key_value = 0;

    while (ifs >> key >> key_value)
    {
      this->supportedKeys[key] = key_value;
    }
  }

  ifs.close();

  // DEBUG Remove later
  /*for (auto i: this->supportedKeys)
  {
    std::cout << i.first << " " << i.second << "\n";
  }*/
}

void Game::initStates()
{
  this->states.push(new MainMenuState(this->window, this->gfxSettings, &this->supportedKeys, &this->states));  
}


// Constructors / Destructors
Game::Game()
{
  this->initVariables();
  this->initGraphicSettings();
  this->initWindow();
  this->initKeys();
  this->initStates();  
}

Game::~Game()
{
  delete this->window;

  while (!this->states.empty())
  {
    delete this->states.top();
    this->states.pop();
  }
}



// Functions

void Game::endApplication()
{
  std::cout << "Ending Application!" << "\n";
}

void Game::updateDT()
{
  // Updates the dt variable with the time it takes to update and render one frame.

  this->dt = this->dtClock.restart().asSeconds();

  /*system("cls");
  std::cout << this->dt << "\n" << std::endl;*/
}

void Game::updateSFMLEvents()
{
  sf::Event event;
  while (this->window->pollEvent(this->sfEvent))
  {
    if (this->sfEvent.type == sf::Event::Closed)
      this->window->close();
  }  
}

void Game::update()
{
  this->updateSFMLEvents();

  if (!this->states.empty())
  {
    this->states.top()->update(this->dt);

    if (this->states.top()->getQuit())
    {
      this->states.top()->endState();
      delete this->states.top();
      this->states.pop();
    }
  }
  else  // Application end
  {
    this->endApplication();
    this->window->close();
  }
}

void Game::render()
{
  this->window->clear();
  
  // Render items
  if (!this->states.empty()) 
    this->states.top()->render();

  this->window->display();
}

void Game::run()
{
  while (this->window->isOpen())
  {
    this->updateDT();
    this->update();
    this->render();
  }
}


