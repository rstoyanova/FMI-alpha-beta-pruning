#include "board.h"

bool Board::CheckMask(char mask[], char a, char b, char c)
{
	bool check[3] = { 0,0,0 };
	
	for (int i = 0; i < SIZE; i++)
	{
		if ((mask[i] == a) && (!check[0]))
			check[0] = 1;
		else if ((mask[i] == b) && (!check[1]))
			check[1] = 1;
		else if ((mask[i] == c) && (!check[2]))
			check[2] = 1;
		else
			return false;
	}
	return true;
}

Board::Board()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
			board[i][j] = 'e';
	}
	pieces_placed = 0;
}

bool Board::FreePosition(int x, int y)
{
	return board[x][y] == 'e';
}

bool Board::IsValidPosition(int x, int y)
{
	return ((x >= 0 && x < SIZE) && (y >= 0 && y < SIZE));
}

bool Board::PlacePiece(char colour, int x, int y)
{
	if ((pieces_placed == 6) || (board[x][y] != 'e'))
		return false;
	else
		board[x][y] = colour;
	pieces_placed++;
	return true;
}

bool Board::MovePiece(char colour, int curr_x, int curr_y, int next_x, int next_y)
{
	if ((board[curr_x][curr_y] == 'e') || (board[next_x][next_y] != 'e') || (pieces_placed != 6) || colour != board[curr_x][curr_y])
		return false;

	board[next_x][next_y] = board[curr_x][curr_y];
	board[curr_x][curr_y] = 'e';
	return true;
}

int* Board::PosiblePlacements()
{
	int* result = new int[18];
	for (int i = 0; i < 18; i++)    
		result[i] = -1;
	
	int ptr = 0;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (FreePosition(i, j))
			{
				result[ptr++] = i;
				result[ptr++] = j;
			}
		}
	}

	return result;
}

int* Board::Posiblemovements(int x, int y)
{
	int* result = new int[16];
	for (int i = 0; i < 16; i++)    
		result[i] = -1;

	int ptr = 0;
	for (int i = (x - 1); i <= (x + 1); i++)
	{
		for (int j = (y - 1); j <= (y + 1); j++)
		{
			if (FreePosition(i, j) && IsValidPosition(i, j))
			{
				result[ptr++] = i;
				result[ptr++] = j;
			}
		}
	}
	return result;
}

void Board::Visualize()
{
	std::cout << std::endl << "                     " << (char)201;
	for (int i = 0; i < SIZE; i++)
		std::cout << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205;
	std::cout << (char)187 << std::endl;

	std::cout << "                     " << (char)186 << "                           " << (char)186 << std::endl;

	for (int i = 0; i < SIZE; i++)
	{
		if (i == 1)
			std::cout << "                     " << (char)186 << "                           " << (char)186 << std::endl;
		std::cout << "                     " << (char)186;
		for (int j = 0; j < SIZE; j++)
		{
			std::cout << "    ";
			if (board[i][j] != 'e')
				std::cout << board[i][j];
			else
				std::cout << " ";
			std::cout << "    ";
		}
		std::cout << (char)186 <<std::endl;
		std::cout << "                     " << (char)186 << "                           " << (char)186 << std::endl;
		if (i == 1)
			std::cout << "                     " << (char)186 << "                           " << (char)186 << std::endl;
	}

	std::cout << "                     " << (char)200;
	for (int i = 0; i < SIZE; i++)
	{
		std::cout << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205 << (char)205;
	}
	std::cout << (char)188;
	std::cout << std::endl;

}

int Board::Heuristic(char max_colour)
{
	char other;
	if (max_colour == 'w')
		other = 'b';
	else
		other = 'w';

	return (2 * (CountGoodLines(max_colour, 2, 1)) + CountGoodLines(max_colour, 1, 1) - 2 * (CountGoodLines(other, 2, 0)) - CountGoodLines(other, 1, 1));

}

int Board::CountGoodLines(char colour, int count, bool arc)
{
	int result = 0;

	char mask[3] = { 'e','e','e' };
	for (int i = 0; i < count; i++)
		mask[i] = colour;

	for (int i = 0; i < SIZE; i++)
	{
		if (CheckMask(mask, board[0][i], board[1][i], board[2][i]))
			result++;
		if (CheckMask(mask, board[i][0], board[i][1], board[i][2]))
			result++;
	}

	if (arc)
	{
		if (CheckMask(mask, board[0][0], board[1][0], board[0][1]))
			result++;
		if (CheckMask(mask, board[0][1], board[0][2], board[1][2]))
			result++;
		if (CheckMask(mask, board[1][0], board[2][0], board[2][1]))
			result++;
		if (CheckMask(mask, board[1][2], board[2][2], board[2][1]))
			result++;
	}

	if (CheckMask(mask, board[0][0], board[1][1], board[2][2]))
		result++;
	if (CheckMask(mask, board[2][0], board[1][1], board[0][2]))
		result++;

	return result;
}

bool Board::ThreeInALine()
{
	return (CountGoodLines('w', 3, 1) > 0 || CountGoodLines('b', 3, 1));
}

bool Board::AllPiecesPlaced()
{
	return pieces_placed == 6;
}

bool Board::Empty()
{
	return pieces_placed == 0;
}

char Board::GetColour(int x, int y)
{
	return board[x][y];
}

void Board::Print()
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < 3; j++)
			std::cout << board[i][j] << " ";
		std::cout << std::endl;
	}
}

