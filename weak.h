#ifndef WEAK_HEADER
#define WEAK_HEADER

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#define USE_BITSCAN_ASM

// See http://chessprogramming.wikispaces.com/Bitboards.
#define C64(constantU64) constantU64##ULL
#define RANK(pos) ((pos)/8)
#define FILE(pos) ((pos)&7)
#define OPPOSITE(side) ((Side)(1-(side)))
#define POSBOARD(pos) ((BitBoard)(1ULL<<(pos)))
#define POSITION(rank, file) ((Position)((rank)*8 + (file)))

#define FORCE_INLINE inline __attribute__((always_inline))

#define INIT_MOVE_LEN 100

// Perft positions, see http://chessprogramming.wikispaces.com/Perft+Results.

#define PERFT_COUNT 5

#define FEN1 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"
#define FEN3 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -"
#define FEN4 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define FEN4_REVERSED "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1"

enum CastleEvent {
  NoCastleEvent = 0,
  LostKingSideWhite = 1 << 0,
  LostQueenSideWhite = 1 << 1,
  LostKingSideBlack = 1 << 2,
  LostQueenSideBlack = 1 << 3
};

enum CastleSide {
  KingSide,
  QueenSide
};

enum File {
  FileA = 0,
  FileB = 1,
  FileC = 2,
  FileD = 3,
  FileE = 4,
  FileF = 5,
  FileG = 6,
  FileH = 7
};

enum MoveType {
  Normal,
  CastleQueenSide,
  CastleKingSide,
  PromoteKnight,
  PromoteBishop,
  PromoteRook,
  PromoteQueen,
  EnPassant
};

enum Piece {
  Pawn,
  Knight,
  Bishop,
  Rook,
  Queen,
  King,
  MissingPiece
};

enum Position {
  A1, B1, C1, D1, E1, F1, G1, H1,
  A2, B2, C2, D2, E2, F2, G2, H2,
  A3, B3, C3, D3, E3, F3, G3, H3,
  A4, B4, C4, D4, E4, F4, G4, H4,
  A5, B5, C5, D5, E5, F5, G5, H5,
  A6, B6, C6, D6, E6, F6, G6, H6,
  A7, B7, C7, D7, E7, F7, G7, H7,
  A8, B8, C8, D8, E8, F8, G8, H8,
  EmptyPosition
};

enum Rank {
  Rank1 = 0,
  Rank2 = 1,
  Rank3 = 2,
  Rank4 = 3,
  Rank5 = 4,
  Rank6 = 5,
  Rank7 = 6,
  Rank8 = 7
};

enum Side {
  White,
  Black
};

// A bitboard is an efficient representation of the occupancy of a chessboard [0].
// We use little-endian rank-file (LERF) mapping [1].
typedef uint64_t                BitBoard;
typedef enum CastleEvent        CastleEvent;
typedef struct CastleEventSlice CastleEventSlice;
typedef enum CastleSide         CastleSide;
typedef struct CheckStats       CheckStats;
typedef struct CheckStatsSlice  CheckStatsSlice;
typedef struct ChessSet         ChessSet;
typedef struct EnPassantSlice   EnPassantSlice;
typedef struct Game             Game;
typedef struct Move             Move;
typedef struct MoveHistory      MoveHistory;
typedef struct MoveSlice        MoveSlice;
typedef enum MoveType           MoveType;
typedef struct PerftStats       PerftStats;
typedef enum Piece              Piece;
typedef struct PieceSlice       PieceSlice;
typedef enum Position           Position;
typedef enum Rank               Rank;
typedef enum File               File;
typedef struct Set              Set;
typedef enum Side               Side;
typedef struct StringBuilder    StringBuilder;

struct CastleEventSlice {
  CastleEvent *Vals, *Curr;
};

struct CheckStats {
  BitBoard CheckSquares[6], CheckSources, Discovered, Pinned;
  Position DefendedKing, AttackedKing;
};

struct CheckStatsSlice {
  CheckStats *Vals, *Curr;
};

struct EnPassantSlice {
  Position *Vals, *Curr;
};

struct MoveSlice {
  Move *Vals, *Curr;
};

struct PieceSlice {
  int Len, Cap;
  Piece *Vals;
};

struct Set {
  BitBoard EmptySquares, Occupancy;
  BitBoard Boards[6];
};

struct MoveHistory {
  CastleEventSlice CastleEvents;
  CheckStatsSlice  CheckStats;
  EnPassantSlice   EnPassantSquares;
  MoveSlice        Moves;
  PieceSlice       CapturedPieces;
};

struct ChessSet {
  BitBoard EmptySquares, Occupancy;
  Set      Sets[2];
};

struct Game {
  bool        CastlingRights[2][2];
  CheckStats  CheckStats;
  ChessSet    ChessSet;
  bool        Debug;
  MoveHistory History;
  Position    EnPassantSquare;
  Side        WhosTurn, HumanSide;
};

struct Move {
  Piece    Piece;
  Position From, To;
  bool     Capture;
  MoveType Type;
};

struct PerftStats {
  uint64_t Count, Captures, EnPassants, Castles, Promotions, Checks, Checkmates;
};

struct StringBuilder {
  // Length is the total number of characters in the builder.
  int Length;
  // cap, len refer to the capacity/length of the number of strings in the builder.
  int cap, len;
  char **strings;
};

static const BitBoard
  CastlingAttackMasks[2][2]      = {{ C64(0x0000000000000060), C64(0x000000000000000c) },
                                    { C64(0x6000000000000000), C64(0x0c00000000000000) }},
  CastlingMasks[2][2]            = {{ C64(0x0000000000000060), C64(0x000000000000000e) },
                                    { C64(0x6000000000000000), C64(0x0e00000000000000) }},
  CentralSquaresMask             =  C64(0x0000003c3c000000),
  EdgeMask                       =  C64(0xff818181818181ff),
  EmptyBoard                     =  C64(0x0000000000000000),
  FileAMask                      =  C64(0x0101010101010101),
  FileBMask                      =  C64(0x0202020202020202),
  FileCMask                      =  C64(0x0404040404040404),
  FileDMask                      =  C64(0x0808080808080808),
  FileEMask                      =  C64(0x1010101010101010),
  FileFMask                      =  C64(0x2020202020202020),
  FileGMask                      =  C64(0x4040404040404040),
  FileHMask                      =  C64(0x8080808080808080),
  FullyOccupied                  = ~C64(0x0000000000000000),
  InitBlackBishops               =  C64(0x2400000000000000),
  InitBlackKing                  =  C64(0x1000000000000000),
  InitBlackKnights               =  C64(0x4200000000000000),
  InitBlackOccupancy             =  C64(0xffff000000000000),
  InitBlackPawns                 =  C64(0x00ff000000000000),
  InitBlackQueens                =  C64(0x0800000000000000),
  InitBlackRooks                 =  C64(0x8100000000000000),
  InitOccupancy                  =  C64(0xffff00000000ffff),
  InitWhiteBishops               =  C64(0x0000000000000024),
  InitWhiteKing                  =  C64(0x0000000000000010),
  InitWhiteKnights               =  C64(0x0000000000000042),
  InitWhiteOccupancy             =  C64(0x000000000000ffff),
  InitWhitePawns                 =  C64(0x000000000000ff00),
  InitWhiteQueens                =  C64(0x0000000000000008),
  InitWhiteRooks                 =  C64(0x0000000000000081),
  NotFileAMask                   = ~C64(0x0101010101010101),
  NotFileBMask                   = ~C64(0x0202020202020202),
  NotFileCMask                   = ~C64(0x0404040404040404),
  NotFileDMask                   = ~C64(0x0808080808080808),
  NotFileEMask                   = ~C64(0x1010101010101010),
  NotFileFMask                   = ~C64(0x2020202020202020),
  NotFileGMask                   = ~C64(0x4040404040404040),
  NotFileHMask                   = ~C64(0x8080808080808080),
  NotRank1Mask                   = ~C64(0x00000000000000ff),
  NotRank2Mask                   = ~C64(0x000000000000ff00),
  NotRank3Mask                   = ~C64(0x0000000000ff0000),
  NotRank4Mask                   = ~C64(0x00000000ff000000),
  NotRank5Mask                   = ~C64(0x000000ff00000000),
  NotRank6Mask                   = ~C64(0x0000ff0000000000),
  NotRank7Mask                   = ~C64(0x00ff000000000000),
  NotRank8Mask                   = ~C64(0xff00000000000000),
  Rank1Mask                      =  C64(0x00000000000000ff),
  Rank2Mask                      =  C64(0x000000000000ff00),
  Rank3Mask                      =  C64(0x0000000000ff0000),
  Rank4Mask                      =  C64(0x00000000ff000000),
  Rank5Mask                      =  C64(0x000000ff00000000),
  Rank6Mask                      =  C64(0x0000ff0000000000),
  Rank7Mask                      =  C64(0x00ff000000000000),
  Rank8Mask                      =  C64(0xff00000000000000);

FORCE_INLINE void
AppendCastleEvent(CastleEventSlice *slice, CastleEvent castleEvent)
{
  *slice->Curr++ = castleEvent;
}

FORCE_INLINE void
AppendCheckStats(CheckStatsSlice* slice, CheckStats checkStats)
{
  *slice->Curr++ = checkStats;
}

FORCE_INLINE void
AppendEnpassantSquare(EnPassantSlice* slice, Position pos)
{
  *slice->Curr++ = pos;
}

FORCE_INLINE void
AppendMove(MoveSlice *slice, Move move)
{
  *slice->Curr++ = move;
}

#if defined(USE_BITSCAN_ASM)
FORCE_INLINE Position
BitScanBackward(BitBoard bitBoard)
{
  BitBoard posBoard;
  __asm__("bsrq %1, %0": "=r"(posBoard): "rm"(bitBoard));
  return (Position)posBoard;
}

FORCE_INLINE Position
BitScanForward(BitBoard bitBoard)
{
  BitBoard posBoard;
  __asm__("bsfq %1, %0": "=r"(posBoard): "rm"(bitBoard));
  return (Position)posBoard;
}
#else
// bitboard.c
Position BitScanBackward(BitBoard);
Position BitScanForward(BitBoard);
#endif

FORCE_INLINE int
FileDistance(Position from, Position to)
{
  return abs(FILE(from) - FILE(to));
}

FORCE_INLINE bool
SingleBit(BitBoard bitBoard) {
  // This is clever (not my idea!) - if there is only 1 bit, it'll be the msb. (n-1) gives you
  // all the bits below the msb, so if there's anything set there there's more than 1 bit.
  return (bitBoard & (bitBoard - 1)) == EmptyBoard;
}

FORCE_INLINE int
RankDistance(Position from, Position to)
{
  return abs(RANK(from) - RANK(to));
}

FORCE_INLINE Position
PopForward(BitBoard *bitBoard)
{
  Position ret = BitScanForward(*bitBoard);

  *bitBoard ^= POSBOARD(ret);

  return ret;
}

// bishop.c
BitBoard BishopQueenAttackersTo(ChessSet*, Position, BitBoard);
BitBoard BishopAttacksFrom(Position, BitBoard);
BitBoard BishopSquareThreats(Position, BitBoard);

// bitboard.c

// See http://chessprogramming.wikispaces.com/BitScan#bsfbsr

bool Aligned(Position, Position, Position);
BitBoard FlipDiagA1H8(BitBoard);
BitBoard FlipVertical(BitBoard);
void     InitRays(void);
int      PopCount(BitBoard);
bool     PositionOccupied(BitBoard, Position);
BitBoard Rotate90AntiClockwise(BitBoard);
BitBoard Rotate90Clockwise(BitBoard);
BitBoard NortOne(BitBoard);
BitBoard NortRay(Position);
BitBoard EastOne(BitBoard);
BitBoard EastRay(Position);
BitBoard SoutOne(BitBoard);
BitBoard SoutRay(Position);
BitBoard WestOne(BitBoard);
BitBoard WestRay(Position);
BitBoard NoEaOne(BitBoard);
BitBoard NoEaRay(Position);
BitBoard NoWeOne(BitBoard);
BitBoard NoWeRay(Position);
BitBoard SoEaOne(BitBoard);
BitBoard SoEaRay(Position);
BitBoard SoWeOne(BitBoard);
BitBoard SoWeRay(Position);

// chessset.c
BitBoard AllAttackersTo(ChessSet*, Position, BitBoard);
ChessSet NewChessSet(void);
ChessSet NewEmptyChessSet(void);
void     RemovePiece(ChessSet*, Side, Piece, Position);
BitBoard PinnedPieces(ChessSet*, Side, bool);
void     PlacePiece(ChessSet*, Side, Piece, Position);

// game.c
CheckStats  CalculateCheckStats(Game*);
bool        Checkmated(Game*);
void        DoCastleKingSide(Game*);
void        DoCastleQueenSide(Game*);
bool        GivesCheck(Game*, Move*);
void        InitEngine(void);
void        DoMove(Game*, Move*);
CheckStats  NewCheckStats(void);
Game        NewEmptyGame(bool, Side);
Game        NewGame(bool, Side);
MoveHistory NewMoveHistory(void);
bool        PseudoLegal(Game*, Move*, BitBoard);
bool        Stalemated(Game*);
void        ToggleTurn(Game*);
void        Unmove(Game*);

// king.c
void     InitKing(void);
BitBoard KingAttackersTo(ChessSet*, Position);
BitBoard KingAttacksFrom(Position);

// knight.c
void     InitKnight(void);
BitBoard KnightAttacksFrom(Position);
BitBoard KnightAttackersTo(ChessSet *, Position);

// magic.c
void InitMagics(void);

// movegen.c
void AllMoves(MoveSlice*, Game*);

// parser.c
Game ParseFen(char*);

// pawn.c
BitBoard AllPawnCaptureSources(ChessSet*, Side);
BitBoard AllPawnCaptureTargets(ChessSet*, Side);
BitBoard AllPawnPushSources(ChessSet*, Side);
void     InitPawn(void);
BitBoard PawnAttackersTo(ChessSet*, Position);
BitBoard PawnAttacksFrom(Position, Side);
BitBoard PawnCaptureSources(ChessSet*, Side, BitBoard);
BitBoard PawnCaptureTargets(ChessSet*, Side, BitBoard);
BitBoard PawnPushSources(ChessSet*, Side, BitBoard);

// perft.c
PerftStats Perft(Game*, int);
uint64_t   QuickPerft(Game*, int);

// rook.c
BitBoard RookAttacksFrom(Position, BitBoard);
BitBoard RookQueenAttackersTo(ChessSet *, Position, BitBoard);
BitBoard RookSquareThreats(Position, BitBoard);

// set.c
Set   NewBlackSet(void);
Set   NewEmptySet(void);
Set   NewWhiteSet(void);
Piece PieceAt(Set*, Position);
void  SetPlacePiece(Set*, Piece, Position);
void  SetRemovePiece(Set*, Piece, Position);

// slices.c
void             AppendPiece(PieceSlice*, Piece);
int              LenCastleEvents(CastleEventSlice*);
int              LenMoves(MoveSlice*);
CastleEventSlice NewCastleEventSlice(void);
CheckStatsSlice  NewCheckStatsSlice(void);
EnPassantSlice   NewEnPassantSlice(void);
MoveSlice        NewMoveSlice(Move*);
PieceSlice       NewPieceSlice(void);
CastleEvent      PopCastleEvent(CastleEventSlice*);
CheckStats       PopCheckStats(CheckStatsSlice*);
Position         PopEnPassantSquare(EnPassantSlice*);
Move             PopMove(MoveSlice*);
Piece            PopPiece(PieceSlice*);

// stringer.c
char  CharPiece(Piece);
char* StringBitBoard(BitBoard);
char* StringChessSet(ChessSet*);
char* StringMove(Move*);
char* StringPerft(PerftStats*);
char* StringPiece(Piece);
char* StringPosition(Position);
char* StringSide(Side);

// util.c
void*         allocate(size_t, size_t);
void*         allocateZero(size_t, size_t);
void          release(void*);
void          panic(char*, ...);
void          AppendString(StringBuilder *, char*);
char*         BuildString(StringBuilder*, bool);
int           Max(int, int);
StringBuilder NewStringBuilder(void);
void          ReleaseStringBuilder(StringBuilder*);
void          SetUnbufferedOutput(void);

// Array containing BitBoard of positions between two specified squares, as long as
// they are on the same rank/file/diagonal. This is exclusive of the from and to squares.
BitBoard Between[64][64];

// Array containing lookups determining whether any sliding attack is possible
// between the two positions.
bool CanSlideAttack[64][64];

// Array containing distances between positions on the board.
int Distance[64][64];

// Array containing attacks for a specified piece and position on an empty BitBoard.
// We only calculate this for sliding pieces.
BitBoard EmptyAttacks[5][64];

#endif
