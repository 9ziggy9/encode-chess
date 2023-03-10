#include <stdio.h>

typedef enum {
  BLACK_RESIGN = 0, WHITE_RESIGN,
  BLACK_PAWN, BLACK_ROOK, BLACK_KNIGHT, BLACK_BISHOP,
  BLACK_QUEEN, BLACK_KING,
  WHITE_PAWN, WHITE_ROOK, WHITE_KNIGHT, WHITE_BISHOP,
  WHITE_QUEEN, WHITE_KING
} Piece;

typedef enum {
  // Enumerating from 1 is a weird choice, I kind of
  // want for H8 to correspond to 64, aka 0x40
  A1 = 1, A2, A3, A4, A5, A6, A7, A8,
  B1, B2, B3, B4, B5, B6, B7, B8,
  C1, C2, C3, C4, C5, C6, C7, C8,
  D1, D2, D3, D4, D5, D6, D7, D8,
  E1, E2, E3, E4, E5, E6, E7, E8,
  F1, F2, F3, F4, F5, F6, F7, F8,
  G1, G2, G3, G4, G5, G6, G7, G8,
  H1, H2, H3, H4, H5, H6, H7, H8
} Square;

int main(void) {
  printf("H8: %x\n", H8);
  char input[3];
  printf("> ");
  scanf("%3[^\n]s", input);
  printf("\n>>> %s", input);
  return 0;
}
