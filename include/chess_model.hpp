#ifndef CHESS_MODEL
#define CHESS_MODEL


#include <string>
#include <map>
#include <SFML/Graphics.hpp>

class ChessModel
{
public:
  
  ChessModel();
  ~ChessModel();

  void  Print       ();
  void  Draw        (sf::RenderWindow &wnd, const std::string &from);
  void  Move        (const std::string &from, const std::string &to, sf::RenderWindow &wnd);
  int   CheckGameRes();
  void  Reset       ();

private:

  enum FigureType
  {
    NONE,
    PAWN,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING
  };

  struct Figure
  {
    bool        isBlack           = false;
    FigureType  type              = NONE;
    size_t      moves_count       = 0;
  };


  Figure* m_board;

  bool        m_blacks_move;
  std::string m_last_move;

  std::map<std::string, sf::Texture> m_textures;


  bool checkMove(int x_from, int y_from, int x_to, int y_to, bool check_if_shah=true);

};


#endif // !CHESS_MODEL

