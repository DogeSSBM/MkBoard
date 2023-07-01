#ifndef TYPES_H
#define TYPES_H

typedef FILE                File;
typedef size_t              st;
typedef ptrdiff_t           dt;

typedef long int            lint;
typedef unsigned char       uchar;
typedef unsigned int        uint;
typedef unsigned long       ul;
typedef unsigned long long  ull;

typedef uint8_t             u8;
typedef uint16_t            u16;
typedef uint32_t            u32;
typedef uint64_t            u64;

typedef int8_t              i8;
typedef int16_t             i16;
typedef int32_t             i32;
typedef int64_t             i64;

#define I8MIN               INT8_MIN
#define I8MAX               INT8_MAX
#define I16MIN              INT16_MIN
#define I16MAX              INT16_MAX
#define I32MIN              INT32_MIN
#define I32MAX              INT32_MAX
#define I64MIN              INT64_MIN
#define I64MAX              INT64_MAX

#define U8MAX               UINT8_MAX
#define U16MAX              UINT16_MAX
#define U32MAX              UINT32_MAX
#define U64MAX              UINT64_MAX
#define PI                  M_PI

typedef struct{
    int x;
    int y;
}Coord, Length, Offset;

typedef enum{DIR_U = 0, DIR_R, DIR_D, DIR_L}Direction;

Coord iC(const int x, const int y)
{
    return (const Coord){.x = x, .y = y};
}

bool coordSame(const Coord pos1, const Coord pos2)
{
    return pos1.x == pos2.x && pos1.y == pos2.y;
}

int coordMin(const Coord coord)
{
    return coord.x < coord.y ? coord.x : coord.y;
}

bool inBound(const int n, const int min, const int max)
{
    return !(n < min || n >= max);
}

Direction dirINV(const Direction dir)
{
    return dir^2u;
}

Direction dirROR(const Direction dir)
{
    return (dir+1)&3u;
}

Direction dirUD(const Direction dir)
{
    return !(dir&1);
}

Direction dirPOS(const Direction dir)
{
    return dir==DIR_R||dir==DIR_D;
}

Coord coordShift(const Coord coord, const Direction dir, const int units)
{
    if(dirUD(dir))
        return (const Coord){.x = coord.x, .y = dirPOS(dir)? coord.y+units : coord.y-units};
    return (const Coord){.x = dirPOS(dir)? coord.x+units : coord.x-units, .y = coord.y};
}

typedef enum{
    S_TILE,
    S_NUM,
    S_FLAG,
    S_QEST,
    S_QEST_SAFE,
    S_N
}TileState;
const char *TileStateStr[S_N] = {
    "TILE",
    "NUM",
    "FLAG",
    "QEST",
    "S_QEST_SAFE",
};
const char TileStateChar[S_N] = {
    '#',
    ' ',
    '>',
    '?',
    'S'
};

typedef struct{
    bool isBomb;
    uint num;
    TileState state;
}Tile;

typedef enum{
    B_RNG,
    B_ADJ,
    B_SAT,
    B_N
}BoardType;
const char *BoardTypeStr[B_N] = {
    "B_RNG",
    "B_ADJ",
    "B_SAT"
};

typedef enum{
    BS_MENU,
    BS_FIRST,
    BS_PLACE,
    BS_PLAY,
    BS_END,
    BS_N
}BoardState;
const char *BoardStateStr[BS_N] = {
    "BS_MENU",
    "BS_FIRST",
    "BS_PLACE",
    "BS_PLAY",
    "BS_END"
};

typedef enum{
    A_LEN,
    A_MIN,
    A_TYP,
    A_CHEAT,
    A_ERR
}ArgType;

typedef struct Arg{
    ArgType type;
    union{
        Length len;
        uint min;
        BoardType typ;
        bool cheat;
    };
}Arg;

typedef struct{
    uint saveNum;
    bool rescale;
    Length winLen;
    Length len;
    uint scale;
    Offset off;
    uint numBombs;
    bool cheat;
    uint lastClickTick;
    Coord lDown;
    Coord rDown;
    BoardState state;
    BoardType type;
    Tile **tile;
}Board;

typedef enum{C_NONE, C_LEFT, C_DOUB, C_RIGHT}ClickType;

typedef struct{
    Board board;
    Coord tpos;
    int index;
}ThreadData;


#endif /* end of include guard: TYPES_H */
