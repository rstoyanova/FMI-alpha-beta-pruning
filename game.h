#ifndef GAME_HEADER_INCLUDED
#define GAME_HEADER_INCLUDED
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include "board.h"
#include <ctime>

class Game
{
private:
	Board board;
	char human_colour;
	std::string first;

private:
	void WhoIsFirst();	
	
	Board MiniMax(Board board, int depth, int alpha, int beta, bool maximazingPlayer, char max_col);

	char ColourOfTheFirst();

	

public:
	Game();
	Game(const Board board, char colour, std::string f);
	void Start();
	void Instructions();

	void HumanPlay();
	void AIPlay();
	
	bool GameOver();
	void Visualize();
};

int GetX(int pos);
int GetY(int pos);

Board* NextMoves(Board board, char colour_player);
int* EvaluateNextMoves(Board* possibilities, char max_colour);
int ReturnIndxOfMax(int* vals);
int ReturnIndxOfMin(int* vals);

#endif // !GAME_HEADER_INCLUDED
