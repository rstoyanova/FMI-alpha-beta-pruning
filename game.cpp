#include "game.h"

const int MIN_INT = -2147483646;
const int MAX_INT = 2147483646;

void Game::WhoIsFirst()
{
	time_t curr_time;
	curr_time = time(NULL);
	tm* tm_local = localtime(&curr_time);
	int mins = tm_local->tm_min;

	if (mins % 2)
	{
		std::cout << "\n              You are first!\n";
		first = "human";
	}
	else
	{
		std::cout << "\n              The AI is first!\n";
		first = "AI";
	}
}

Board Game::MiniMax(Board board, int depth, int alpha, int beta, bool maximazingPlayer, char max_col)
{
	if (board.ThreeInALine() || depth <= 0)
		return board;

	char curr_pl;
	if (maximazingPlayer)
		curr_pl = max_col;
	else
	{
		if (max_col == 'w')
			curr_pl = 'b';
		else
			curr_pl = 'w';
	}

	Board* next_moves = NextMoves(board, curr_pl);
	Board temp;
	Board result;
	int pos = 0;

	if (maximazingPlayer)
	{
		int max_eval = MIN_INT;


		while ((!next_moves[pos].Empty()) && pos < 30)
		{
			temp = MiniMax(next_moves[pos], depth - 1, alpha, beta, 0, max_col);
			if (temp.Heuristic(max_col) > max_eval)                                 
			{
				max_eval = temp.Heuristic(max_col);
				result = temp;
			}
			
			if (alpha < temp.Heuristic(max_col))
				alpha = temp.Heuristic(max_col);

			if (beta <= alpha)
				break;
			pos++;
		}
		delete[] next_moves;
		return result;
	}
	else
	{
		int min_eval = MAX_INT;

		while ((!next_moves[pos].Empty()) && pos < 30)
		{
			temp = MiniMax(next_moves[pos], depth - 1, alpha, beta, 1, max_col);
			if (temp.Heuristic(max_col) < min_eval)
			{
				min_eval = temp.Heuristic(max_col);
				result = temp;
			}

			if (beta > temp.Heuristic(max_col))
				beta = temp.Heuristic(max_col);

			if (beta <= alpha)
				break;
			pos++;
		}
		delete[] next_moves;
		return result;	
	}
}

char Game::ColourOfTheFirst()
{
	if (first == "human")
		return human_colour;
	else
	{
		if (human_colour == 'w')
			return 'b';
		else
			return 'w';
	}
}

Game::Game()
{
	human_colour = 'n';
	first = "";
}

Game::Game(const Board board, char colour, std::string f)
{
	this->board = board;
	human_colour = colour;
	first = f;
}

int GetX(int pos)
{
	if ((pos == 1) || (pos == 2) || (pos == 3))
		return 0;
	else if ((pos == 4) || (pos == 5) || (pos == 6))
		return 1;
	else
		return 2;
}

int GetY(int pos)
{
	if ((pos == 1) || (pos == 4) || (pos == 7))
		return 0;
	else if ((pos == 2) || (pos == 5) || (pos == 8))
		return 1;
	else
		return 2;
}

void Game::Start()
{
	for (int i = 0; i < 120; i++)
		std::cout << "*";
	std::cout << "\n                                                         ROTA\n";
	std::cout << std::endl;
	std::cout << "                                             Hi there! Time to play!\n    Choose a colour (white\\black): ";
	std::string colour;
	bool correct = 0;
	while (!correct)
	{
		std::getline(std::cin, colour);
		if (colour == "white")
		{
			human_colour = 'w';
			correct = 1;
		}
		else if (colour == "black")
		{
			human_colour = 'b';
			correct = 1;
		}
		else
			std::cout << "          Incorect input!\n    Choose a correct colour (white\\black): ";
	}
	
	WhoIsFirst();

	if (first == "AI")
	{
		time_t curr_time;
		curr_time = time(NULL);
		tm* tm_local = localtime(&curr_time);
		int posx = (tm_local->tm_min + tm_local->tm_hour) % 3;
		int posy = (((tm_local->tm_mon) % posx) + tm_local->tm_wday) % 3;
		
		if (human_colour == 'w')
			board.PlacePiece('b', posx, posy);
		else
			board.PlacePiece('w', posx, posy);
		Visualize();
	}
}

void Game::Instructions()
{
	std::cout << "                     " << "   _____________________" << std::endl <<"                        ";
	for (int i = 1; i < 10; i++)
	{
		std::cout << " | " << i << " | ";
		if (i % 3 == 0)
			std::cout << std::endl << "                        _____________________"<<std::endl<<"                        ";
	}

	if (!board.AllPiecesPlaced())
		std::cout << "\n        Choose a position to place a piece! \n\n";
	else
		std::cout << "\n        Choose a piece and the position u want to move it to! \n\n";

}

Board* NextMoves(Board board, char colour_player)
{
	Board* result = new Board[30];
	int ptr = 0;

	if (board.AllPiecesPlaced())
	{
		for (int i = 0; i < SIZE * 3; i++)
		{
			if (board.GetColour(GetX(i), GetY(i)) == colour_player)
			{
				int* next = board.Posiblemovements(GetX(i), GetY(i));
				int pos = 0;
				while (next[pos] != -1)
				{
					result[ptr] = board;
					result[ptr].MovePiece(colour_player, GetX(i), GetY(i), next[pos], next[pos + 1]);
					pos+=2;
					ptr++;
				}
				delete[] next;
			}
		}
	}
	else
	{
		int* next = board.PosiblePlacements();
		int pos = 0;

		while (next[pos] != -1)
		{
			result[ptr] = board;
			result[ptr].PlacePiece(colour_player, next[pos], next[pos+1]);
			pos+=2;
			ptr++;
		}
		delete[] next;
	}
	return result;
}

int* EvaluateNextMoves(Board* possibilities, char max_colour)
{
	int pos = 0;
	int* result = new int[30];
	
	while ((!possibilities[pos].Empty()) && (pos < 30))
	{
		result[pos+1] = possibilities[pos].Heuristic(max_colour);
		pos++;
	}
	
	result[0] = pos;
	return result;
}

int ReturnIndxOfMax(int* vals)
{
	int max = 1;
	for (int i = 2; i <= vals[0]; i++)
	{
		if (vals[i] > max)
			max = i;
	}
	return max - 1;
}

int ReturnIndxOfMin(int* vals)
{
	int min = 1;
	for (int i = 2; i <= vals[0]; i++)
	{
		if (vals[i] < min)
			min = i;
	}
	return min - 1;
}

void Game::HumanPlay()
{
	bool cont = true;

	if (board.AllPiecesPlaced())
	{
		int curr = 0;
		int next = 0;
		do
		{
			std::cout << "                -> ";
			std::cin >> curr;
			if (curr < 1 || curr > 9)
			{
				std::cout << "          !Invalid input!\n      Choose a number between 1 and 9: ";
				continue;
			}
			else if ((board.FreePosition(GetX(curr), GetY(curr))) || (board.GetColour(GetX(curr), GetY(curr)) != human_colour))
			{
				std::cout << "          !Invalid input!\n       Try again: ";
				continue;
			}
			else
				cont = false;
		} while (cont);

		cont = true;
		do
		{
			std::cout << "        move to -> ";
			std::cin >> next;
			if (next < 1 || next >9)
			{
				std::cout << "         !Invalid input!\n      Choose a number between 1 and 9: ";
				continue;
			}
			else if (!board.FreePosition(GetX(next), GetY(next)))
			{
				std::cout << "         This position is taken!\n      Try again: ";
				continue;
			}
			else
				cont = false;

		} while (cont);

		board.MovePiece(human_colour, GetX(curr), GetY(curr), GetX(next), GetY(next));
	}
	else
	{
		do
		{
			int pos = 0;
			std::cout << "                -> ";
			std::cin >> pos;
			if (pos < 1 || pos >9)
			{
				std::cout << "         Invalid input!\n      Please reenter: ";
				continue;
			}
			else if (!board.FreePosition(GetX(pos), GetY(pos)))
			{
				std::cout << "         This position is taken!\n      Choose again: ";
				continue;
			}
			else
			{
				board.PlacePiece(human_colour, GetX(pos), GetY(pos));
				cont = false;
			}
		} while (cont);
	}
	Visualize();
}

void Game::AIPlay()
{
	std::cout << "\n\n\n\n\n\n\n\n\n\n            The computer chose: ";

	char col = ' ';
	if (human_colour == 'w')
		col = 'b';
	else
		col = 'w';

	Board* next_moves = NextMoves(board, col);
	Board* buffer = new Board[30];
	int pos = 0;
	bool max_comp = (first != "AI");

	while (!next_moves[pos].Empty() && pos < 30)
	{
		buffer[pos] = MiniMax(next_moves[pos], 8, MIN_INT, MAX_INT, max_comp, ColourOfTheFirst());
		pos++;
	}

	int* eval_final = EvaluateNextMoves(buffer, ColourOfTheFirst());
	int indx = 0;
	if (max_comp)
		indx = ReturnIndxOfMin(eval_final);
	else
		indx = ReturnIndxOfMax(eval_final);
	
	board = next_moves[indx];
	board.Visualize();
}

bool Game::GameOver()
{
	return board.ThreeInALine();
}

void Game::Visualize()
{
	board.Visualize();
}
