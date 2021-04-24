#ifndef BOARD_HEADER_INCLUDED
#define BOARD_HEADER_INCLUDED
#include <iostream>

const int SIZE = 3;

class Board
{
private:
	char board[SIZE][SIZE];
	int pieces_placed;

private:
	bool CheckMask(char mask[], char a, char b, char c);
	
public:
	Board();

	bool PlacePiece(char colour, int x, int y);
	bool MovePiece(char colour, int curr_x, int curr_y, int next_x, int next_y);

	int* PosiblePlacements();
	int* Posiblemovements(int x, int y);
	
	int Heuristic(char max_colour);
	int CountGoodLines(char colour, int count, bool arc);

	bool FreePosition(int x, int y);
	bool IsValidPosition(int x, int y);
	bool ThreeInALine();
	bool AllPiecesPlaced();
	bool Empty();

	char GetColour(int x, int y);

	void Visualize();
	void Print();
};

#endif // !BOARD_HEADER_INCLUDED
