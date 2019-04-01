#include "GameState.h"


// Initializer Functions
void GameState::initKeybinds()
{
  std::ifstream ifs("Config/gamestate_keybinds.ini");

  if (ifs.is_open())
  {
    std::string key = "";
    std::string key2 = "";

    while (ifs >> key >> key2)
    {
      this->keybinds[key] = this->supportedKeys->at(key2);
    }
  }

  ifs.close();
  
  
}

void GameState::initTextures()
{
  if (!this->textures["PLAYER_IDLE"].loadFromFile("Resources/Images/Sprites/Player/test.png"))
  {
  	throw("ERROR::MAIN_MENU_STATE::FAILERD TO LOAD SPRITE TEXTURE");
  }
}

void GameState::initPlayers()
{
  this->player = new Player(0, 0, this->textures["PLAYER_IDLE"]);
}

// Constructors / Destructors
GameState::GameState(sf::RenderWindow* window, std::map<std::string, int>* supportedKeys, std::stack<State*>* states)
  : State(window, supportedKeys, states)
{
  this->initKeybinds();
  this->initTextures();
  this->initPlayers();
}


GameState::~GameState()
{
  delete this->player;
}

void GameState::updateInput(const float & dt)
{
  // Update player input
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_LEFT"))))
    this->player->move(dt, -1.0f, 0.0f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_RIGHT"))))
    this->player->move(dt, 1.0f, 0.0f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_UP"))))
    this->player->move(dt, 0.0f, -1.0f);
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("MOVE_DOWN"))))
    this->player->move(dt, 0.0f, 1.0f);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))))
    this->endState();
}

void GameState::update(const float& dt)
{
  this->updateMousePositions();
  this->updateInput(dt);

  this->player->update(dt);
}

void GameState::render(sf::RenderTarget* target)
{
  if (!target)
    target = this->window;
  
  this->player->render(target);
  
}
