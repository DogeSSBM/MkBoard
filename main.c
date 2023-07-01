#include "Includes.h"

int main(int argc, char **argv)
{
    Board board = boardArgs(argc, argv);
    boardPlaceBombs(&board);
    boardSave(&board);
    boardFree(&board);

    return 0;
}
