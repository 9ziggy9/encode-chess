#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
  BLACK_RESIGN = 0, WHITE_RESIGN,
  BLACK_PAWN, BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP,
  BLACK_QUEEN, BLACK_KING,
  WHITE_PAWN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP,
  WHITE_QUEEN, WHITE_KING
} Piece;

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
} Square;

char to0x88(const char *square) {
  char rank = square[0] - 'a';
  char file = square[1] - '1';
  return (char)(file << 4) + rank;
}

int main(void) {
  char input[3];
  printf("> ");
  scanf("%3[^\n]s", input);
  printf("%x\n", to0x88(input));
  printf("%d\n", to0x88(input) == E4);
  printf("\n");
  return 0;
}
