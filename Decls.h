#ifndef DECLS_H
#define DECLS_H

// Args.h
void usage(void);
uint parseUint(char *str, const uint len);
bool sameStrLower(char *a, char *b);
ArgType parseArgType(char *arg);
Length parseLen(char *arg);
uint parseBombs(char *arg);
BoardType parseBoardType(char *arg);
Arg parseArg(char *arg);
Board boardArgs(int argc, char **argv);

// Board.h
void printBoard(const Board *board, const bool decals);
void printDecalsPos(const Board *board, const Coord pos);
uint printCleared(const uint cleared);
void printTile(const Tile tile);
Board boardDefault(void);
void boardCpy(Board *dst, Board *src);
void boardFree(Board *board);
Tile** boardTileAlloc(const Length len);
void boardAlloc(Board *board);
bool validTilePos(const Coord pos, const Length bounds);
uint adjBombs(const Board *board, const Coord pos);
uint adjTileState(const Board *board, const Coord pos, const TileState state);
uint floodFill(Board *board, const Coord pos);
uint floodAdj(Board *board, const Coord pos);
void boardCalcNums(Board *board);
void boardResetFirstClick(Board *board);
void boardResetTileStates(Board *board);
uint boardRemaining(Board *board);
void boardRngBombs(Board *board);
void* boardPlaceBombsThread(void *voidData);
bool boardPlaceBombs(Board *board);

// Solve.h
bool clear121(Board *board, const Coord pos);
uint flagAdj(Board *board, const Coord pos);
uint setAdj(Board *board, const Coord pos, const TileState state);
uint clearAdj(Board *board, const Coord pos);
Board* resetQuests(Board *board);
bool isAbsurd(Board *board);
bool reductioAdAbsurdum(Board *board);
bool satSolve(Board *board);
bool solve(Board *board);

// Save.h
Length boardFileLength(File *file);
Tile tileUnpack(char c);
char tilePack(const Tile tile);
File* saveNewFile(Board *board);
bool boardSave(Board *board);

#endif /* end of include guard: DECLS_H */
