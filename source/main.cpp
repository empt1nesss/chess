#include "chess_model.hpp"

#include <cstdio>
#include <vector>
#include <thread>
#include <chrono>

#include <SFML/Graphics.hpp>


int AskUser(
  sf::RenderWindow &wnd,
  const std::string &assets_path,
  const std::string &msg, 
  const std::vector<std::string> &buttons
)
{

  sf::RectangleShape bg;
  bg.setFillColor(sf::Color(15, 15, 18));
  bg.setOutlineColor(sf::Color(226, 226, 244));
  bg.setOutlineThickness(2.f);

  sf::Font font;
  font.loadFromFile(assets_path + "FiraCode-Medium.ttf");

  sf::Text msg_text;
  msg_text.setFont(font);
  msg_text.setString(msg);
  msg_text.setCharacterSize(32);
  msg_text.setFillColor(sf::Color(226, 226, 244));
  msg_text.setPosition(
    { 400.f - msg_text.getLocalBounds().width / 2.f, 380.f - msg_text.getLocalBounds().height }
  );


  int bg_height = 200, bg_width = msg_text.getLocalBounds().width + 40;
  int button_height = 40, button_width = 0;

  std::vector<sf::Text> t_buttons;
  for (auto &b : buttons) {
    t_buttons.push_back({});
    (t_buttons.end() - 1)->setFont(font);
    (t_buttons.end() - 1)->setString(b);
    (t_buttons.end() - 1)->setCharacterSize(16);
    (t_buttons.end() - 1)->setFillColor(sf::Color(226, 226, 244));

    if (button_width < (t_buttons.end() - 1)->getLocalBounds().width)
      button_width = (t_buttons.end() - 1)->getLocalBounds().width;
  }

  button_width += 40;
  if (bg_width < button_width * buttons.size() + (buttons.size() - 1) * 10 + 40)
    bg_width = button_width * buttons.size() + (buttons.size() - 1) * 10 + 40;

  bg.setSize({ (float)bg_width, (float)bg_height });

  bg.setPosition({ 400.f - bg_width / 2.f, 400 - bg_height / 2.f });

  std::vector<sf::RectangleShape> buttons_shape;
  for (int b = 0; b < t_buttons.size(); ++b) {
    buttons_shape.push_back(sf::RectangleShape({ (float)button_width, (float)button_height }));
    buttons_shape[b].setOutlineColor(sf::Color(226, 226, 244));
    buttons_shape[b].setOutlineThickness(2.f);

    buttons_shape[b].setPosition({
      400.f - button_width * buttons.size() / 2.f - (buttons.size() - 1) * 5.f + b * (button_width + 10),
      420.f
    });

    t_buttons[b].setPosition({
      400.f - button_width * buttons.size() / 2.f - (buttons.size() - 1) * 5.f +
      b * (button_width + 10) +
      (button_width - t_buttons[b].getLocalBounds().width) / 2.f,

      430.f
    });
  }


  sf::Texture wnd_bg_t;
  wnd_bg_t.create(wnd.getSize().x, wnd.getSize().y);
  wnd_bg_t.update(wnd);

  sf::Sprite wnd_bg;
  wnd_bg.setTexture(wnd_bg_t);

  int res = 0;

  while (res == 0) {
    wnd.clear();

    wnd.draw(wnd_bg);
    wnd.draw(bg);
    wnd.draw(msg_text);

    for (int b = 0; b < buttons_shape.size(); ++b) {
      auto mouse_pos = sf::Mouse::getPosition(wnd);
      if (buttons_shape[b].getGlobalBounds().contains(mouse_pos.x, mouse_pos.y)) {
        buttons_shape[b].setFillColor(sf::Color(226, 226, 244, 255));
        t_buttons[b].setFillColor(sf::Color(15, 15, 18, 255));
        
        if (wnd.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
          res = b + 1;
          break;
        }
      }
      else {
        buttons_shape[b].setFillColor(sf::Color(0, 0, 0, 0));
        t_buttons[b].setFillColor(sf::Color(226, 226, 244, 255));
      }

      wnd.draw(buttons_shape[b]);
      wnd.draw(t_buttons[b]);
    }
    
    wnd.display();
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return res - 1;
}

int main(int argc, char **argv)
{
  std::string assets_path (
    argv[0],
    argv[0] + std::string(argv[0]).find_last_of(
#ifdef _WIN32
      "\\"
#elif defined (__unix__)
      "/"
#endif
    )
  );
  assets_path += "/assets/";

  ChessModel cm(assets_path);

  std::string move_from_bf, move_to_bf;
  int game_res = 0;
  
  sf::Texture t_bg;
  t_bg.loadFromFile(assets_path + "bg.png");
  sf::Sprite bg(t_bg);

  sf::RenderWindow wnd{
    { 800, 800 },
    "Chess!",
    sf::Style::None
  };
  
  bool exit = false;
  while (wnd.isOpen()) {

    for (sf::Event event; wnd.pollEvent(event);) {
      if (event.type == sf::Event::Closed)
        wnd.close();

      // if (event.type == sf::Event::MouseButtonEvent)
    }

    if (wnd.hasFocus() && sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      auto mouse_pos = sf::Mouse::getPosition(wnd);

      if (move_from_bf.empty()) {
        move_from_bf = "A1";
        move_from_bf[0] += (mouse_pos.x / 100);
        move_from_bf[1] += 7 - (mouse_pos.y / 100);
      }
      else {
        move_to_bf = "A1";
        move_to_bf[0] += (mouse_pos.x / 100);
        move_to_bf[1] += 7 - (mouse_pos.y / 100);

        if (move_from_bf != move_to_bf) {
          cm.Move(move_from_bf, move_to_bf, wnd);
          move_from_bf = "";
          game_res = cm.CheckGameRes();
        }

        move_to_bf = "";
      }
    }

    wnd.clear();
    
    wnd.draw(bg);
    
    cm.Draw(wnd, move_from_bf);

    wnd.display();
  
    switch (game_res)
    {
    case 1:
      exit = AskUser(wnd, assets_path, "White wins!", { "Play again", "Exit" });
      break;
    case 2:
      exit = AskUser(wnd, assets_path, "Black wins!", { "Play again", "Exit" });
      break;
    case 3:
      exit = AskUser(wnd, assets_path, "Draw!", { "Play again", "Exit" });
      break;
    default:
      break;
    }

    if (game_res) {
      if (exit)
        return 0;
      else {
        cm.Reset();
        game_res = 0;
        move_from_bf = "";
        move_to_bf = "";
      }
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return 0;
}

