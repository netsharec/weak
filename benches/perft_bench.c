/*
  Weak, a chess perft calculator derived from Stockfish.

  Copyright (C) 2004-2008 Tord Romstad (Glaurung author)
  Copyright (C) 2008-2012 Marco Costalba, Joona Kiiski, Tord Romstad (Stockfish authors)
  Copyright (C) 2011-2012 Lorenzo Stoakes

  Weak is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Weak is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>
#include <time.h>
#include "../weak.h"
#include "bench.h"

#if defined(QUICK_BENCH)
#define MAX_DEPTH 6
#else
#define MAX_DEPTH 7
#endif

// Perft positions, see http://chessprogramming.wikispaces.com/Perft+Results.

#define PERFT_COUNT 5

#define FEN1 "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
#define FEN2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq -"
#define FEN3 "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - -"
#define FEN4 "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1"
#define FEN4_REVERSED "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 1"

static char *fens[PERFT_COUNT] = { FEN1, FEN2, FEN3, FEN4, FEN4_REVERSED };
static Game games[PERFT_COUNT];
static int depthCounts[PERFT_COUNT] = { 6, 5, 7, 6, 6 };

void
BenchPerft()
{
  char tmp[200];
  clock_t ticks;
  double elapsed, totalElapsed = 0;
  int i, j;
  int64_t nodes, totalNodes = 0;
  long iters, k;

  // Initialise games.
  for(i = 0; i < PERFT_COUNT; i++) {
    games[i] = ParseFen(fens[i]);
  }

  for(i = 0; i < PERFT_COUNT; i++) {
    // Depth 1-3 tests are too short to be meaningful. Ignore.
    for(j = 4; j <= depthCounts[i] && j <= MAX_DEPTH; j++) {
      elapsed = 0;
      iters = 1;

      while(elapsed < MIN_ELAPSED) {
        ticks = clock();
        for(k = 0; k < iters; k++) {
          nodes = (int64_t)QuickPerft(&games[i], j);
        }
        ticks = clock() - ticks;
        // In ms.
        elapsed = 1000*((double)ticks)/CLOCKS_PER_SEC;
        iters *= 2;
      }
      // Remove final *2.
      iters /= 2;

      totalNodes += nodes;
      totalElapsed += elapsed/iters;

      sprintf(tmp, "Perft Position %d Depth %d", i+1, j);
      OutputBenchResults(tmp, elapsed, iters, nodes);
    }
  }

  printf("Median Perft Performance: %f Mn/s\n", 1E-3*totalNodes/totalElapsed);
}
