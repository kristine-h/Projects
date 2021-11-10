#include "TicTacToe.h"

void TicTacToe::PlayTheGame() {
	int row;
	int column;
	while (true) {
		ShowTheBoard();
		std::cout << "Player " << x_or_o_ << " enter the row(1-3) you want to place the " << x_or_o_ << ": _\b";
		std::cin >> row;
		if (!(IsTheRowValid(row))) { 
			std::cout << "Sorry, the row must be between (1-3).\n"; 
			break;
	       	}
		std::cout << "Player " << x_or_o_ << " enter the column(1-3) you want to place the " << x_or_o_ << ": _\b";
		std::cin >> column;
		if (!(IsTheColumnValid(column))) { 
			std::cout << "Sorry, the column must be between (1-3).\n"; 
			break; 
		}
		if (!(IsThePieceFree(row, column))) { 
			std::cout << "\nYour step is invalid. The piece is not free.\n\n";
			break;
		}
		UpdateTheBoard(row, column);
		if (DidAPlayerWin()) { 
			std::cout << "\nTHE PLAYER " << x_or_o_ << " WON THE GAME.\n"; 
			ShowTheBoard(); 
			break;
		} else if (GameEndedInADraw()) { std::cout << "THE GAME ENDED IN A DRAW.\n"; break; }
		if ( x_or_o_ == 'x') { 
			x_or_o_ = 'o';
		} else x_or_o_ = 'x';
	}
}

void TicTacToe::ShowTheBoard() {
        std::cout << "The Board\n\n" << board_[0][0] << " | " << board_[0][1] << " | " << board_[0][2] << std::endl <<
                                        board_[1][0] << " | " << board_[1][1] << " | " << board_[1][2] << std::endl << 
                                        board_[2][0] << " | " << board_[2][1] << " | " << board_[2][2] << std::endl << std::endl;
}

bool TicTacToe::IsTheRowValid(int row) {
	if (row > 0 && row < 4) { return true; }
	return false;
}

bool TicTacToe::IsTheColumnValid(int column) {
        if (column > 0 && column < 4) { return true; }
        return false;
}

bool TicTacToe::DidAPlayerWin() {
	if ( ((board_[0][0] == x_or_o_) && (board_[1][0] == x_or_o_) && (board_[2][0] == x_or_o_)) || 
	     ((board_[0][1] == x_or_o_) && (board_[1][1] == x_or_o_) && (board_[2][1] == x_or_o_)) ||
	     ((board_[0][2] == x_or_o_) && (board_[1][2] == x_or_o_) && (board_[2][2] == x_or_o_)) ||
	     ((board_[0][0] == x_or_o_) && (board_[0][1] == x_or_o_) && (board_[0][2] == x_or_o_)) ||
	     ((board_[1][0] == x_or_o_) && (board_[1][1] == x_or_o_) && (board_[1][2] == x_or_o_)) ||
	     ((board_[2][0] == x_or_o_) && (board_[2][1] == x_or_o_) && (board_[2][2] == x_or_o_)) ||
	     ((board_[0][0] == x_or_o_) && (board_[1][1] == x_or_o_) && (board_[2][2] == x_or_o_)) ||
	     ((board_[0][2] == x_or_o_) && (board_[1][1] == x_or_o_) && (board_[2][0] == x_or_o_))) {
		return true;
	}
	return false;
}

bool TicTacToe::IsThePieceFree(int row, int column) {
	if (board_[row - 1][column - 1] != ' ') {
		return false;
	} 
	return true;
}

void TicTacToe::UpdateTheBoard(int row, int column) {
	if (x_or_o_ == 'x') {
		board_[row - 1][column - 1] = 'x';
	} else board_[row - 1][column - 1] = 'o';
}

bool TicTacToe::GameEndedInADraw() {
	if (pieces_ == 9) { return true; }
	return false;
}
