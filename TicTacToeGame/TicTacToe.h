#ifndef TICTACTOE_H_
#define TICTACTOE_H_

#include <iostream>

class TicTacToe {
public:
        void PlayTheGame();
private:
        char board_[3][3] = {{' ', ' ', ' '},
                             {' ', ' ', ' '},
                             {' ', ' ', ' '}};
        char x_or_o_{'x'};
	int pieces_{0};
        void ShowTheBoard();
        bool IsTheRowValid(int row);
        bool IsTheColumnValid(int column);
        void UpdateTheBoard(int row, int column);
        bool IsThePieceFree(int row, int column);
        bool DidAPlayerWin();
	bool GameEndedInADraw();
};

#endif //TICTACTOE_H_
