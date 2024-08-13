#include "chess_model.hpp"

#include <iostream>

int AskUser(sf::RenderWindow &wnd, const std::string &msg, const std::vector<std::string> &buttons);


ChessModel::ChessModel() : m_blacks_move(false), m_board(nullptr)
{
  Reset();

  for (char ft : "pkbrqc") {
    m_textures[std::string(1, ft) + "w"].loadFromFile("assets/" + std::string(1, ft) + "w.png");
    m_textures[std::string(1, ft) + "b"].loadFromFile("assets/" + std::string(1, ft) + "b.png");

    if (ft == 'c')
      break;
  }

  m_textures["movec"].loadFromFile("assets/move_cell.png");
  m_textures["selc"].loadFromFile("assets/selected_cell.png");
}

ChessModel::~ChessModel()
{
  delete[] m_board;
}


void ChessModel::Print()
{
  printf("  ");
  for (char c = 'A'; c <= 'H'; ++c)
    printf("%c ", c);

  printf("\n");

  for (int y = 7; y >= 0; --y) {

    printf("%d ", y + 1);

    for (int x = 0; x < 8; ++x)
      switch (m_board[y * 8 + x].type)
      {
      case NONE:
        printf("- ");
        break;

      case PAWN:
        if (m_board[y * 8 + x].isBlack)
          printf("p ");
        else
         printf("P ");
        
        break;

      case KNIGHT:
        if (m_board[y * 8 + x].isBlack)
          printf("k ");
        else
         printf("K ");
        
        break;

      case BISHOP:
        if (m_board[y * 8 + x].isBlack)
          printf("b ");
        else
         printf("B ");
        
        break;

      case ROOK:
        if (m_board[y * 8 + x].isBlack)
          printf("r ");
        else
         printf("R ");
        
        break;

      case QUEEN:
        if (m_board[y * 8 + x].isBlack)
          printf("q ");
        else
         printf("Q ");
        
        break;

      case KING:
        if (m_board[y * 8 + x].isBlack)
          printf("c ");
        else
         printf("C ");
        
        break;
      }

    printf("%d\n", y + 1);

  }

  printf("  ");
  for (char c = 'A'; c <= 'H'; ++c)
    printf("%c ", c);

  printf("\n");
}

void ChessModel::Draw(sf::RenderWindow &wnd, const std::string &from)
{
  for (int fi = 0; fi < 64; ++fi) {
    
    if (!from.empty()) {
      try {
        if (checkMove(from[0] - 'A', from[1] - '1', fi % 8, fi / 8)) {
          sf::Sprite cell_bg(m_textures["movec"]);
          cell_bg.setPosition({ (fi % 8) * 100.f, (7 - fi / 8) * 100.f });
          wnd.draw(cell_bg);
        }
      }
      catch (...) {
        sf::Sprite cell_bg(m_textures["movec"]);
        cell_bg.setPosition({ (fi % 8) * 100.f, (7 - fi / 8) * 100.f });
        wnd.draw(cell_bg);
      }

      if (from[0] - 'A' == fi % 8 && from[1] - '1' == fi / 8) {
        sf::Sprite cell_bg(m_textures["selc"]);
        cell_bg.setPosition({ (fi % 8) * 100.f, (7 - fi / 8) * 100.f });
        wnd.draw(cell_bg);
      }
    }


    if (m_board[fi].type == NONE)
      continue;
    
    sf::Sprite icon;
    switch (m_board[fi].type) 
    {
    case PAWN:
      if (m_board[fi].isBlack)
        icon.setTexture(m_textures["pb"]);
      else
        icon.setTexture(m_textures["pw"]);

      break;

    case KNIGHT:
      if (m_board[fi].isBlack)
        icon.setTexture(m_textures["kb"]);
      else
        icon.setTexture(m_textures["kw"]);

      break;

    case BISHOP:
      if (m_board[fi].isBlack)
        icon.setTexture(m_textures["bb"]);
      else
        icon.setTexture(m_textures["bw"]);

      break;

    case ROOK:
      if (m_board[fi].isBlack)
        icon.setTexture(m_textures["rb"]);
      else
        icon.setTexture(m_textures["rw"]);

      break;

    case QUEEN:
      if (m_board[fi].isBlack)
        icon.setTexture(m_textures["qb"]);
      else
        icon.setTexture(m_textures["qw"]);

      break;

    case KING:
      if (m_board[fi].isBlack)
        icon.setTexture(m_textures["cb"]);
      else
        icon.setTexture(m_textures["cw"]);

      break;

    default:
      break;
    }

    icon.setPosition({ (fi % 8) * 100.f, (7 - fi / 8) * 100.f });
    wnd.draw(icon);
  }
}

void ChessModel::Move(const std::string &from, const std::string &to, sf::RenderWindow &wnd)
{
  int x_from  = from[0] - 'A',
      y_from  = from[1] - '1',
      x_to    = to  [0] - 'A',
      y_to    = to  [1] - '1';

  if (
    x_from < 0  || x_from >= 8  ||
    y_from < 0  || y_from >= 8  ||
    x_to < 0    || x_to >= 8    ||
    y_to < 0    || y_to >= 8
  )
    return;

  try {
    if (checkMove(x_from, y_from, x_to, y_to)) {
      m_board[y_to * 8 + x_to] = m_board[y_from * 8 + x_from];
      m_board[y_from * 8 + x_from] = { false, NONE, 0 };

      m_blacks_move = !m_blacks_move;
      ++m_board[y_to * 8 + x_to].moves_count;

      m_last_move = from + to;
    }
  }
  catch (const std::string &exception) {
    m_board[y_to * 8 + x_to] = m_board[y_from * 8 + x_from];
    m_board[y_from * 8 + x_from] = { false, NONE, 0 };

    ++m_board[y_to * 8 + x_to].moves_count;

    m_last_move = from + to;


    if (exception[0] == 'k')
      m_board[(exception[2] - '1') * 8 + exception[1] - 'A'] = { false, NONE, 0 };

    if (exception[0] == 'c') {
      if (exception[1] == 'l') {
        m_board[7 * m_blacks_move * 8 + 2] = { m_blacks_move, KING, 1 };
        m_board[7 * m_blacks_move * 8 + 3] = { m_blacks_move, ROOK, 1 };

        m_board[7 * m_blacks_move * 8] = { false, NONE, 0 };
        m_board[7 * m_blacks_move * 8 + 4] = { false, NONE, 0 };
      }
      else {
        m_board[7 * m_blacks_move * 8 + 6] = { m_blacks_move, KING, 1 };
        m_board[7 * m_blacks_move * 8 + 5] = { m_blacks_move, ROOK, 1 };

        m_board[7 * m_blacks_move * 8 + 4] = { false, NONE, 0 };
        m_board[7 * m_blacks_move * 8 + 7] = { false, NONE, 0 };
      }
    }

    m_blacks_move = !m_blacks_move;
  }

  for (int x = 0; x < 8; ++x)
    if (m_board[m_blacks_move * 7 * 8 + x].type == PAWN) {
      for (int i = 0; i < 1; ++i) {
        switch (AskUser(wnd, "Choose new figure", { "Queen", "Rook", "Bishop", "Knight" }))
        {
        case 3:
          m_board[m_blacks_move * 7 * 8 + x].type = KNIGHT;
          break;

        case 2:
          m_board[m_blacks_move * 7 * 8 + x].type = BISHOP;
          break;

        case 1:
          m_board[m_blacks_move * 7 * 8 + x].type = ROOK;
          break;

        case 0:
          m_board[m_blacks_move * 7 * 8 + x].type = QUEEN;
          break;


        default:
          --i;
          break;
        }
      }
    }
}

int ChessModel::CheckGameRes()
{
  for (int fi1 = 0; fi1 < 64; ++fi1) {
    if (m_board[fi1].type == NONE || m_board[fi1].isBlack != m_blacks_move)
      continue;

    for (int fi2 = 0; fi2 < 64; ++fi2) {
      try {
        if (checkMove(fi1 % 8, fi1 / 8, fi2 % 8, fi2 / 8))
          return 0;
      }
      catch (...) {
        return 0;
      }
    }
  }

  int king_x, king_y;
  for (int fi = 0; fi < 64; ++fi)
    if (m_board[fi].type == KING && m_board[fi].isBlack == m_blacks_move) {
      king_x = fi % 8;
      king_y = fi / 8;
      break;
    }

  m_blacks_move = !m_blacks_move;

  for (int fi = 0; fi < 64; ++fi) {
    try {
      if (checkMove(fi % 8, fi / 8, king_x, king_y))
        return 1 + m_blacks_move;
    }
    catch (...) {
      return 1 + m_blacks_move;
    }
  }

  return 3;
}

void ChessModel::Reset()
{
  if (m_board != nullptr)
    delete[] m_board;

  m_board = new Figure[64];
  
  for (int y = 6; y < 8; ++y)
    for (int x = 0; x < 8; ++x)
      m_board[y * 8 + x].isBlack = true;
  
  for (int y = 1; y < 8; y += 5)
    for (int x = 0; x < 8; ++x)
      m_board[y * 8 + x].type = PAWN;

  for (int y = 0; y < 8; y += 7) {
    m_board[y * 8].type = ROOK;

    m_board[y * 8 + 7].type = ROOK;
  }

  for (int y = 0; y < 8; y += 7) {
    m_board[y * 8 + 1].type = KNIGHT;
    m_board[y * 8 + 6].type = KNIGHT;
  }

  for (int y = 0; y < 8; y += 7) {
    m_board[y * 8 + 2].type = BISHOP;
    m_board[y * 8 + 5].type = BISHOP;
  }

  m_board[3].type = QUEEN;
  m_board[4].type = KING;

  m_board[7 * 8 + 3].type = QUEEN;
  m_board[7 * 8 + 4].type = KING;

}


bool ChessModel::checkMove(int x_from, int y_from, int x_to, int y_to, bool check_if_shah)
{
  if (
    m_board[y_from * 8 + x_from].type == NONE             ||
    m_board[y_from * 8 + x_from].isBlack != m_blacks_move
  )
    return false;

  bool res = false;
  std::string exception;

  if (
    m_board[y_from * 8 + x_from].type == KING         &&
    m_board[y_to * 8 + x_to].type == ROOK             &&
    m_board[y_to * 8 + x_to].isBlack == m_blacks_move
  ) {
    if (
      m_board[y_from * 8 + x_from].moves_count ||
      m_board[y_to * 8 + x_to].moves_count
    )
      return false;

    if (x_to == 0) {
      for (int x = 1; x < 4; ++x)
        if (m_board[7 * m_blacks_move * 8 + x].type != NONE)
          return false;

      res = true;
      exception = "cl";
    }
    else if (x_to == 7) {
      for (int x = 5; x < 7; ++x)
        if (m_board[7 * m_blacks_move * 8 + x].type != NONE)
          return false;

      res = true;
      exception = "cs";
    }
    goto CHECK_SHAH;
  }

  if (
    m_board[y_to * 8 + x_to].isBlack == m_blacks_move &&
    m_board[y_to * 8 + x_to].type != NONE
  )
    return false;

  switch (m_board[y_from * 8 + x_from].type)
  {
  case PAWN:
    if (abs(x_to - x_from) > 1 || y_to == y_from || abs(y_to - y_from) > 2)
      break;

    if (y_to > y_from == m_blacks_move)
      break;
    
    if (x_to == x_from) {
      if (m_board[(m_blacks_move ? y_from - 1 : y_from + 1) * 8 + x_to].type != NONE)
        break;

      if (abs(y_to - y_from) == 2) {
        if (m_board[y_from * 8 + x_from].moves_count == 0) {
          if (m_board[(m_blacks_move ? y_from - 2 : y_from + 2) * 8 + x_to].type == NONE)
            res = true;
          
          break;
        }
        else
          break;
      }

      res = true;
      break;
    }
    
    if (abs(y_to - y_from) != 1)
      break;

    if (m_board[y_to * 8 + x_to].type == NONE) {
      if (
        y_from == 4 - m_blacks_move                                             &&
        m_board[(m_last_move[3] - '1') * 8 + m_last_move[2] - 'A'].type == PAWN &&
        m_last_move[1] == ('7' - 5 * m_blacks_move)                             &&
        m_last_move[3] == y_from + '1'                                          &&
        m_last_move[2] == x_to + 'A'
      ) {
        res = true;
        exception = "kA1";
        exception[1] = m_last_move[2];
        exception[2] = m_last_move[3];
      }

      break;
    }

    res = true;

    break;

  case KNIGHT:
    if (
      (abs(x_from - x_to) == 2 && abs(y_from - y_to) == 1) ||
      (abs(x_from - x_to) == 1 && abs(y_from - y_to) == 2)
    )
      res = true;

    break;

  case BISHOP:
    if (abs(x_to - x_from) != abs(y_to - y_from))
      break;

    res = true;

    for (int x = x_from, y = y_from;;) {
      if (x < x_to)
        ++x;
      else
        --x;

      if (y < y_to)
        ++y;
      else
       --y;

      if (x == x_to)
        break;

      if (m_board[y * 8 + x].type != NONE) {
        res = false;
        break;
      }
    }
    break;

  case ROOK:
    res = true;

    if (x_from == x_to)
      for (int y = y_from;;) {
        if (y < y_to)
          ++y;
        else
          --y;

        if (y == y_to)
          break;

        if (m_board[y * 8 + x_to].type != NONE) {
          res = false;
          break;
        }
      }
    else if (y_from == y_to)
      for (int x = x_from;;) {
        if (x < x_to)
          ++x;
        else
          --x;

        if (x == x_to)
          break;

        if (m_board[y_to * 8 + x].type != NONE) {
          res = false;
          break;
        }
      }
    else
      res = false;

    break;

  case QUEEN:
    if (abs(x_to - x_from) == abs(y_to - y_from)) {
      res = true;
    
      for (int x = x_from, y = y_from;;) {
        if (x < x_to)
          ++x;
        else
          --x;

        if (y < y_to)
          ++y;
        else
          --y;

        if (x == x_to)
          break;

        if (m_board[y * 8 + x].type != NONE) {
          res = false;
          break;
        }
      }
    }

    if (res)
      break;

    res = true;

    if (x_from == x_to)
      for (int y = y_from;;) {
        if (y < y_to)
          ++y;
        else
          --y;

        if (y == y_to)
          break;

        if (m_board[y * 8 + x_to].type != NONE) {
          res = false;
          break;
        }
      }
    else if (y_from == y_to)
      for (int x = x_from;;) {
        if (x < x_to)
          ++x;
        else
          --x;

        if (x == x_to)
          break;

        if (m_board[y_to * 8 + x].type != NONE) {
          res = false;
          break;
        }
      }
    else
      res = false;

    break;

  case KING:
    if (abs(x_to - x_from) <= 1 && abs(y_to - y_from) <= 1)
      res = true;

    break;


  default:
    return false;
  }

CHECK_SHAH:

  if (res == false)
    return false;

  if (check_if_shah == false) {
    if (!exception.empty())
      throw exception;

    return true;
  }


  Figure* temp_board = new Figure[64];
  for (int fi = 0; fi < 64; ++fi)
    temp_board[fi] = m_board[fi];

  std::string temp_last_move = m_last_move;
  bool temp_blacks_move = m_blacks_move;

  m_board[y_to * 8 + x_to] = m_board[y_from * 8 + x_from];
  ++m_board[y_to * 8 + x_to].moves_count;
  m_board[y_from * 8 + x_from] = { false, NONE, 0 };

  m_blacks_move = !m_blacks_move;
  m_last_move = "A1A1";
  m_last_move[0] += x_from;
  m_last_move[1] += y_from;
  m_last_move[2] += x_to;
  m_last_move[3] += y_to;

  if (!exception.empty()) {
    if (exception[0] == 'k')
      m_board[(exception[2] - '1') * 8 + exception[1] - 'A'] = { false, NONE, 0 };

    if (exception[0] == 'c') {
      if (exception[1] == 'l') {
        m_board[7 * !m_blacks_move + 2] = { !m_blacks_move, KING, 1 };
        m_board[7 * !m_blacks_move + 3] = { !m_blacks_move, ROOK, 1 };

        m_board[7 * !m_blacks_move] = { false, NONE, 0 };
        m_board[7 * !m_blacks_move + 4] = { false, NONE, 0 };
      }
      else {
        m_board[7 * !m_blacks_move + 6] = { !m_blacks_move, KING, 1 };
        m_board[7 * !m_blacks_move + 5] = { !m_blacks_move, ROOK, 1 };

        m_board[7 * !m_blacks_move + 4] = { false, NONE, 0 };
        m_board[7 * !m_blacks_move + 7] = { false, NONE, 0 };
      }
    }
  }

  int king_x, king_y;
  for (int fi = 0; fi < 64; ++fi)
    if (m_board[fi].type == KING && m_board[fi].isBlack != m_blacks_move) {
      king_x = fi % 8;
      king_y = fi / 8;
      break;
    }

  for (int fi = 0; fi < 64; ++fi)
    if (m_board[fi].type != NONE && m_board[fi].isBlack == m_blacks_move) {
      try {
        if (checkMove(fi % 8, fi / 8, king_x, king_y, false)) {
          res = false;
          break;
        }
      }
      catch (...) {
        res = false;
        break;
      }
    }
  
  for (int fi = 0; fi < 64; ++fi)
    m_board[fi] = temp_board[fi];

  delete[] temp_board;

  m_last_move = temp_last_move;
  m_blacks_move = temp_blacks_move;

  if (!exception.empty())
    throw exception;

  return res;
}

