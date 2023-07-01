#ifndef BOARD_H
#define BOARD_H

void printBoard(const Board *board, const bool decals)
{
    assertExpr(board->tile);
    for(int y = 0; y < board->len.y; y++){
        for(int x = 0; x < board->len.x; x++){
            if(decals){
                if(board->tile[x][y].state == S_NUM)
                    putchar(board->tile[x][y].num ? '0' + board->tile[x][y].num : ' ');
                else
                    putchar(TileStateChar[board->tile[x][y].state]);
            }else{
                if(board->tile[x][y].isBomb)
                    putchar('B');
                else
                    putchar(board->tile[x][y].num ? '0' + board->tile[x][y].num : ' ');
            }
            putchar(' ');
        }
        putchar('\n');
    }
    putchar('\n');
}

void printDecalsPos(const Board *board, const Coord pos)
{
    assertExpr(board->tile);
    for(int y = 0; y < board->len.y; y++){
        for(int x = 0; x < board->len.x; x++){
            if(x == pos.x && y == pos.y)
                putchar('X');
            else if(board->tile[x][y].state == S_NUM)
                putchar(board->tile[x][y].num ? '0'+board->tile[x][y].num : ' ');
            else
                putchar(TileStateChar[board->tile[x][y].state]);
            putchar(' ');
        }
        putchar('\n');
    }
}

uint printCleared(const uint cleared)
{
    if(cleared)
        printf("Cleared %u tiles.\n", cleared);
    return cleared;
}

void printTile(const Tile tile)
{
    printf("(Tile){\n");
    printf("\t.isBomb = %s,\n", tile.isBomb ? "true" : "false");
    printf("\t.num = %u,\n", tile.num);
    printf("\t.state = %s\n", TileStateStr[tile.state]);
    printf("}\n");
}

Board boardDefault(void)
{
    Board board = {
        .winLen = iC(0, 0),
        .len = iC(30, 16),
        .scale = 0,
        .off = iC(0, 0),
        .numBombs = 99,
        .lDown = iC(-1,-1),
        .rDown = iC(-1,-1),
        .state = BS_FIRST,
        .type = B_SAT
    };
    return board;
}

void boardCpy(Board *dst, Board *src)
{
    assertExpr(dst->tile != NULL && src->tile != NULL);
    assertExpr(coordSame(dst->len, src->len));
    for(int x = 0; x < dst->len.x; x++){
        for(int y = 0; y < dst->len.y; y++){
            dst->tile[x][y] = src->tile[x][y];
        }
    }
}

void boardFree(Board *board)
{
    assertExpr(board->tile != NULL);
    printf("Freeing board: {%i,%i}\n", board->len.x, board->len.y);
    for(int x = 0; x < board->len.x; x++)
        free(board->tile[x]);
    free(board->tile);

    board->tile = NULL;
    printf("Free'd board: {%i,%i}\n", board->len.x, board->len.y);
}

Tile** boardTileAlloc(const Length len)
{
    assertExprMsg(coordMin(len) >= 4, "Cannot boardAlloc anything < {4,4}, len: {%i,%i}", len.x, len.y);
    Tile **tile = calloc(len.x, sizeof(Tile*));
    for(int x = 0; x < len.x; x++)
        tile[x] = calloc(len.y, sizeof(Tile));
    return tile;
}

void boardAlloc(Board *board)
{
    assertExprMsg(!board->tile, "Tried to boardAlloc when board->tile was not NULL");
    board->tile = boardTileAlloc(board->len);
}

bool validTilePos(const Coord pos, const Length bounds)
{
    return inBound(pos.x, 0, bounds.x) && inBound(pos.y, 0, bounds.y);
}

uint adjBombs(const Board *board, const Coord pos)
{
    uint count = 0;
    for(int yo = -1; yo <= 1; yo++){
        for(int xo = -1; xo <= 1; xo++){
            const Coord adj = {.x = pos.x+xo, .y = pos.y+yo};
            count += validTilePos(adj, board->len) && !coordSame(pos, adj) && board->tile[adj.x][adj.y].isBomb;
        }
    }
    return count;
}

uint adjTileState(const Board *board, const Coord pos, const TileState state)
{
    uint count = 0;
    for(int yo = -1; yo <= 1; yo++){
        for(int xo = -1; xo <= 1; xo++){
            const Coord adj = {.x = pos.x+xo, .y = pos.y+yo};
            count += (
                !coordSame(pos, adj) &&
                validTilePos(adj, board->len) &&
                board->tile[adj.x][adj.y].state == state
            );
        }
    }
    return count;
}

uint floodFill(Board *board, const Coord pos)
{
    if(!validTilePos(pos, board->len) || board->tile[pos.x][pos.y].state != S_TILE)
        return 0;
    if(board->tile[pos.x][pos.y].isBomb){
        printBoard(board, false);
        printf("\n");
        printBoard(board, true);
        printf("\n");
        printDecalsPos(board, pos);
        panic("tried to prop bomb at %i,%i\n", pos.x, pos.y);
    }
    board->tile[pos.x][pos.y].state = S_NUM;
    if(board->tile[pos.x][pos.y].num > 0)
        return 1;

    uint cleared = 1;
    for(int yo = -1; yo <= 1; yo++){
        for(int xo = -1; xo <= 1; xo++){
            const Coord adj = {.x = pos.x+xo, .y = pos.y+yo};
            if(
                !coordSame(pos, adj) &&
                validTilePos(adj, board->len) &&
                board->tile[adj.x][adj.y].state == S_TILE
            )
                cleared += floodFill(board, adj);
        }
    }

    return cleared;
}

uint floodAdj(Board *board, const Coord pos)
{
    uint count = 0;
    for(int yo = -1; yo <= 1; yo++){
        for(int xo = -1; xo <= 1; xo++){
            const Coord adj = {.x = pos.x+xo, .y = pos.y+yo};
            if(
                !coordSame(pos, adj) &&
                validTilePos(adj, board->len) &&
                board->tile[adj.x][adj.y].state == S_TILE
            ){
                if(board->tile[adj.x][adj.y].isBomb){
                    board->state = BS_MENU;
                    return 0;
                }
                count += floodFill(board, adj);
            }
        }
    }
    return count;
}

void boardCalcNums(Board *board)
{
    for(int y = 0; y < board->len.y; y++)
        for(int x = 0; x < board->len.x; x++)
            board->tile[x][y].num = adjBombs(board, iC(x,y));
}

void boardResetFirstClick(Board *board)
{
    assertExprMsg(board->tile, "Must alloc board->tile before resetting to first click state.");
    for(int y = 0; y < board->len.y; y++){
        for(int x = 0; x < board->len.x; x++){
            board->tile[x][y].state = S_TILE;
            board->tile[x][y].isBomb = false;
            board->tile[x][y].num = 0;
        }
    }
    board->state = BS_FIRST;
}

void boardResetTileStates(Board *board)
{
    assertExprMsg(board->tile, "Must alloc board->tile before resetting tiles.");
    for(int y = 0; y < board->len.y; y++)
        for(int x = 0; x < board->len.x; x++)
            board->tile[x][y].state = S_TILE;
}

uint boardRemaining(Board *board)
{
    uint total = 0;
    for(int y = 0; y < board->len.y; y++)
        for(int x = 0; x < board->len.x; x++)
            total += board->tile[x][y].state != S_NUM;
    return total - board->numBombs;
}

void boardRngBombs(Board *board)
{
    const Coord tpos = board->lDown;
    assertExpr(validTilePos(tpos, board->len));
    boardResetFirstClick(board);
    for(uint i = 0; i < board->numBombs; i++){
        Coord pos;
        do{
            pos.x = rand()%board->len.x;
            pos.y = rand()%board->len.y;
        }while(
            board->tile[pos.x][pos.y].isBomb || (
                inBound(pos.x, tpos.x-1,tpos.x+2) &&
                inBound(pos.y, tpos.y-1,tpos.y+2)
            )
        );
        board->tile[pos.x][pos.y].isBomb = true;
    }
}

_Atomic int done;

void* boardPlaceBombsThread(void *voidData)
{
    ThreadData *data = voidData;
    Board *board = &(data->board);
    for(ull i = 0; i < 100000; i++){
        if(atomic_load(&done) != -1){
            return NULL;
        }
        boardRngBombs(board);
        boardCalcNums(board);
        floodFill(board, data->tpos);
        if(solve(board)){
            atomic_store(&done, data->index);
            data->board = *board;
            return NULL;
        }
    }
    return 0;
}

bool boardPlaceBombs(Board *board)
{
    Coord tpos = board->lDown;
    printf("placing bombs\n");
    assertExpr(board->state == BS_FIRST);
    for(Direction d = 0; d < 4; d++)
        if(!validTilePos(coordShift(tpos, d, 1), board->len))
            tpos = coordShift(tpos, dirINV(d), 1);
    if(board->type != B_RNG){
        atomic_init(&done, -1);
        ThreadData data[THREAD_COUNT] = {0};
        pthread_t thread[THREAD_COUNT] = {0};
        for(int i = 0; i < THREAD_COUNT; i++){
            data[i].board = *board;
            boardAlloc(&(data[i].board));
            boardResetFirstClick(&(data[i].board));
            data[i].tpos = tpos;
            data[i].index = i;
            pthread_create(&(thread[i]), NULL, boardPlaceBombsThread, &(data[i]));
        }
        // int tries[THREAD_COUNT] = {0};
        for(int i = 0; i < THREAD_COUNT; i++){
            pthread_join(thread[i], NULL);
        }
        boardAlloc(board);
        boardCpy(board, &(data[atomic_load(&done)].board));
        for(int i = 0; i < THREAD_COUNT; i++)
            boardFree(&(data[i].board));
    }else{
        boardAlloc(board);
        boardResetFirstClick(board);
    }

    printBoard(board, false);
    printBoard(board, true);
    boardResetTileStates(board);
    floodFill(board, tpos);
    board->state = BS_PLAY;
    printf("Tiles remaining: %u\n", boardRemaining(board));
    return true;
}

#endif /* end of include guard: BOARD_H */
