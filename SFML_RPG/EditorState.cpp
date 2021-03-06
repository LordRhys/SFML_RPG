#include "stdafx.h"
#include "EditorState.h"



// Initializer Functions
void EditorState::initVariables()
{
  this->textureRect = sf::IntRect(0, 0, static_cast<int>(this->stateData->gridSize), static_cast<int>(this->stateData->gridSize));
}

void EditorState::initBackground()
{
  
}

void EditorState::initFonts()
{
  if (!this->font.loadFromFile("Fonts/CelticHand.ttf"))
  {
    throw("ERROR::EDITOR_STATE::COULD NOT LOAD FONT");
  }
}

void EditorState::initText()
{
  this->cursorText.setFont(this->font);
  this->cursorText.setFillColor(sf::Color::White);
  this->cursorText.setCharacterSize(12);
  this->cursorText.setPosition(this->mousePosView.x, this->mousePosView.y);
}

void EditorState::initKeybinds()
{
  std::ifstream ifs("Config/editorstate_keybinds.ini");

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

void EditorState::initPauseMenu()
{
  this->pmenu = new PauseMenu(*this->window, this->font);

  this->pmenu->addButton("QUIT", 800.f, "Quit");
  this->pmenu->addButton("SAVE", 600.f, "Save");
  this->pmenu->addButton("LOAD", 400.f, "Load");
}

void EditorState::initButtons()
{
  
}

void EditorState::initGui()
{
  this->sidebar.setSize(sf::Vector2f(80.f, static_cast<float>(this->stateData->gfxSettings->resolution.height)));
  this->sidebar.setFillColor(sf::Color(50, 50, 50, 100));
  this->sidebar.setOutlineColor(sf::Color(200, 200, 200, 150));
  this->sidebar.setOutlineThickness(1.f);

  this->selectorRect.setSize(sf::Vector2f(this->stateData->gridSize, this->stateData->gridSize));

  this->selectorRect.setFillColor(sf::Color(255, 255, 255, 150));
  this->selectorRect.setOutlineThickness(1.f);
  this->selectorRect.setOutlineColor(sf::Color::Green);

  this->selectorRect.setTexture(this->tileMap->getTileSheet());
  this->selectorRect.setTextureRect(this->textureRect);

  this->textureSelector = new gui::TextureSelector(
    20.f, 20.f, 500.f, 500.f, 
    this->gridSize, this->tileMap->getTileSheet(),
    this->font, "TS");
}

void EditorState::initTileMap()
{
  this->tileMap = new TileMap(this->stateData->gridSize, 10, 10, "Resources/Images/Tiles/tilesheet1.png");
}

EditorState::EditorState(StateData* state_data)
  : State(state_data)
{
  this->initVariables();
  this->initBackground();
  this->initFonts();
  this->initText();
  this->initKeybinds();
  this->initPauseMenu();
  this->initButtons();
  this->initTileMap();
  this->initGui();
}


EditorState::~EditorState()
{
  auto it = this->buttons.begin();
  for (it = this->buttons.begin(); it != this->buttons.end(); ++it)
  {
    delete it->second;
  }

  delete this->pmenu;

  delete this->tileMap;

  delete this->textureSelector;
}

// Functions
void EditorState::updateInput(const float& dt)
{
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key(this->keybinds.at("CLOSE"))) && this->getKeytime())
  {
    if (!this->paused)
      this->pauseState();
    else
      this->unpauseState();
  }
}

void EditorState::updateEditorInput(const float& dt)
{
  // Add a tile to the tilemap
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && this->getKeytime())
  {
    if (!this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)))
    {
      if (!this->textureSelector->getActive())
      {
        this->tileMap->addTile(this->mousePosGrid.x, this->mousePosGrid.y, 0, this->textureRect);
      }
      else
      {
        this->textureRect = this->textureSelector->getTextureRect();
      }
    }        
  }
  // Remove a tile to the tilemap
  else if(sf::Mouse::isButtonPressed(sf::Mouse::Right) && this->getKeytime())
  {
    if (!this->sidebar.getGlobalBounds().contains(sf::Vector2f(this->mousePosWindow)))
    {
      if (!this->textureSelector->getActive())
        this->tileMap->removeTile(this->mousePosGrid.x, this->mousePosGrid.y, 0);
    }
  }
}

void EditorState::updateButtons()
{
  // Updates all the buttons in the state and handles their functionality. 
  for (auto& it : this->buttons)
  {
    it.second->update(this->mousePosView);
  }  
}

void EditorState::updateGui(const float& dt)
{
  this->textureSelector->update(dt, this->mousePosWindow);

  if (!this->textureSelector->getActive())
  {
    this->selectorRect.setTextureRect(this->textureRect);
    this->selectorRect.setPosition(this->mousePosGrid.x * this->stateData->gridSize,
      this->mousePosGrid.y * this->stateData->gridSize);
  }  

  this->cursorText.setPosition(this->mousePosView.x +100.f, this->mousePosView.y - 50.f);
  std::stringstream ss;
  ss << this->mousePosView.x << " " << this->mousePosView.y << 
    "\n" << this->mousePosGrid.x << " " << this->mousePosGrid.y <<
    "\n" << " " << this->textureRect.left << " " << this->textureRect.top;
  this->cursorText.setString(ss.str());  
}

void EditorState::updatePauseMenuButtons()
{
  // Quit the editor
  if (this->pmenu->isButtonPressed("QUIT"))
    this->endState();

  // Save the tilemap
  if (this->pmenu->isButtonPressed("SAVE"))
    this->tileMap->saveToFile("test.slmp");

  // Load the tilemap
  if (this->pmenu->isButtonPressed("LOAD"))
    this->tileMap->loadFromFile("test.slmp");
}

void EditorState::update(const float& dt)
{
  this->updateMousePositions();
  this->updateKeytime(dt);
  this->updateInput(dt);

  if (!this->paused) // Unpaused update
  {
    this->updateButtons();
    this->updateGui(dt);
    this->updateEditorInput(dt);
  }
  else  // Paused update
  {
    this->pmenu->update(this->mousePosView);
    this->updatePauseMenuButtons();
  }  
}

void EditorState::renderButtons(sf::RenderTarget& target)
{
  for (auto& it : this->buttons)
  {
    it.second->render(target);
  }
}

void EditorState::renderGui(sf::RenderTarget& target)
{
  if(!this->textureSelector->getActive())
    target.draw(this->selectorRect);

  this->textureSelector->render(target);
  target.draw(this->cursorText);

  target.draw(this->sidebar);
}

void EditorState::render(sf::RenderTarget* target)
{
  if (!target)
    target = this->window;

  this->tileMap->render(*target);

  this->renderButtons(*target);
  this->renderGui(*target);

  if (this->paused)  // pause menu render
  {
    this->pmenu->render(*target);
  }  
}
