#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef enum {
  BLACK_RESIGN  = 0x45, WHITE_RESIGN  = 0x65,
  BLACK_PAWN	= 0x50, BLACK_ROOK    = 0x52,
  BLACK_KNIGHT	= 0x4E, BLACK_BISHOP  = 0x42,
  BLACK_QUEEN	= 0x51, BLACK_KING    = 0x4B,
  WHITE_PAWN	= 0x70, WHITE_ROOK    = 0x72,
  WHITE_KNIGHT	= 0x6E, WHITE_BISHOP  = 0x62,
  WHITE_QUEEN	= 0x71, WHITE_KING    = 0x6B
} PieceEncoding;

typedef enum {
  // 0x88 encoding scheme
  A1 = 0x00, B1, C1, D1, E1, F1, G1, H1,
  A2 = 0x10, B2, C2, D2, E2, F2, G2, H2,
  A3 = 0x20, B3, C3, D3, E3, F3, G3, H3,
  A4 = 0x30, B4, C4, D4, E4, F4, G4, H4,
  A5 = 0x40, B5, C5, D5, E5, F5, G5, H5,
  A6 = 0x50, B6, C6, D6, E6, F6, G6, H6,
  A7 = 0x60, B7, C7, D7, E7, F7, G7, H7,
  A8 = 0x70, B8, C8, D8, E8, F8, G8, H8
} SquareEncoding;

typedef struct MoveEncoding {
  PieceEncoding piece;
  SquareEncoding square;
} MoveEncoding;

typedef struct GameEncoding {
  MoveEncoding ms[255];
  size_t turn;
} GameEncoding;

SquareEncoding to0x88(const char rk, const char fl) {
  SquareEncoding rank = rk - 'a';
  SquareEncoding file = fl - '1';
  return (file << 4) + rank;
}

PieceEncoding toPieceByte(const char piece) { return (PieceEncoding) piece; }

MoveEncoding encodeMove(const char *input) {
  MoveEncoding m = {
    .piece = toPieceByte(input[0]),
    .square = to0x88(input[1], input[2])
  };
  return m;
}

size_t appendMove(GameEncoding *g, MoveEncoding m) {
  assert(g->turn <= sizeof(g->ms) / sizeof(g->ms[0]) && "Too many moves");
  g->ms[g->turn] = m;
  return ++g->turn;
}

#define MAX_INPUT_LENGTH 5
int main(void) {
  char input[MAX_INPUT_LENGTH];
  size_t read_length;
  while (1) {
    printf("Make move: ");
    // if fgets encounters EOF or error, it return null we use
    // feof and ferror respectively to test these conditions
    if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
      if (feof(stdin)) {
	fprintf(stdout, "End of file reached.\n");
	exit(EXIT_SUCCESS);
      } else if (ferror(stdin)) {
	fprintf(stderr, "Error: fgets reading NULL");
	exit(EXIT_FAILURE);
      }
    }
    read_length = strlen(input);
    if (read_length > 0 && input[read_length - 1] == '\n') {
      input[read_length - 1] = '\0'; // Remove \n and terminate the string
    }
    printf("You entered: %s\n", input);
  }
  return 0;
}
