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
  // 0x88 encoding scheme: https://en.wikipedia.org/wiki/0x88
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

SquareEncoding to_0x88(const char rk, const char fl) {
  SquareEncoding rank = rk - 'a';
  SquareEncoding file = fl - '1';
  return (file << 4) + rank;
}

PieceEncoding to_piece_byte(const char piece) { return (PieceEncoding) piece; }

MoveEncoding encode_move(const char *input) {
  return (MoveEncoding) {
    .piece = to_piece_byte(input[0]),
    .square = to_0x88(input[1], input[2])
  };
}

GameEncoding new_game() { return (GameEncoding) {.turn = 1}; }

size_t append_move(GameEncoding *g, MoveEncoding m) {
  assert(g->turn <= sizeof(g->ms) / sizeof(g->ms[0]) && "Too many moves");
  g->ms[g->turn] = m;
  return ++g->turn;
}

void print_encoding(const GameEncoding *g) {
  size_t turn = 1;
  printf("\n");
  while(turn < g->turn) {
    printf("(0x%02x)(0x%02x)",
	   g->ms[turn].piece,
	   g->ms[turn].square);
    turn++;
  }
  printf("\n");
}

typedef enum {
  HEX = 0,
  ASCII,
} StrFormat;

static void str_alloc_guard(char *p) {
  if (p == NULL) {
    perror("Failed to allocate memory to string.\n");
    exit(EXIT_FAILURE);
  }
}

char *to_square_str(const SquareEncoding s) {
  char *sq_str = malloc(3*sizeof(char));
  str_alloc_guard(sq_str);
  char rank = (((char) s) >> 4) + '1';
  char file = (((char) s) & 7) + 'a';
  sq_str[0] = file;
  sq_str[1] = rank;
  sq_str[3] = '\0';
  return sq_str;
}

char *to_hex_str(const GameEncoding *g) {
  // "7034" -> 1. e4
  // "70344E55" -> 1. e4 Nf3
  // Allocate 2 bytes for each encoding, + 1 for null terminator
  size_t num_bytes = 4 * (sizeof(char) * g->turn - 1) + 1;
  char *str = malloc(num_bytes);
  str_alloc_guard(str);
  for (size_t i = 0; i < g->turn - 1; i++) {
    MoveEncoding m = g->ms[i+1];
    sprintf(str + 4 * i, "%02x%02x", m.piece, m.square);
  }
  return str;
}

char *to_ascii_str(const GameEncoding *g) {
  size_t num_bytes = 2 * (sizeof(char) * g->turn - 1) + 1;
  char *str = malloc(num_bytes);
  printf("String size: %zu\n", num_bytes);
  str_alloc_guard(str);
  for (size_t i = 0; i < g->turn -1; i++) {
    MoveEncoding m = g->ms[i+1];
    sprintf(str + 2 * i, "%c%c", m.piece, m.square);
  }
  return str;
}

char *to_str(GameEncoding *g, StrFormat fmt) {
  switch(fmt) {
  case HEX: return to_hex_str(g);
  case ASCII: return to_ascii_str(g);
  default:
    fprintf(stderr, "to_str: This should be unreachable.\n");
    exit(EXIT_FAILURE);
  }
}

// SINGLE SOURCE OF TRUTH FOR DEALLOCATING STRING MEMORY!
// BEWARE OF DOUBLE FREEING PLZ
void write_to_file(GameEncoding *g, StrFormat fmt) {
  char *str = to_str(g, fmt);
  FILE *f;
  f = fopen("out.txt", "w");
  if (f == NULL) {
    fprintf(stderr, "Error opening file: out.txt.\n");
    exit(EXIT_FAILURE);
  }
  fprintf(f, "%s", str);
  fclose(f);
  free(str);
  str = NULL;
}

void fgets_exit_gracefully() {
  // if fgets encounters EOF or error, it return null we use
  // feof and ferror respectively to test these conditions
  if (feof(stdin)) {
    fprintf(stdout, "End of file reached.\n");
    exit(EXIT_SUCCESS);
  } else if (ferror(stdin)) {
    fprintf(stderr, "Error: fgets reading NULL");
    exit(EXIT_FAILURE);
  }
}

int main(void) {
  GameEncoding game = new_game();
  append_move(&game, (MoveEncoding) {WHITE_PAWN, E4});
  append_move(&game, (MoveEncoding) {BLACK_PAWN, E5});
  append_move(&game, (MoveEncoding) {WHITE_PAWN, D4});
  append_move(&game, (MoveEncoding) {BLACK_KNIGHT, F6});
  append_move(&game, (MoveEncoding) {BLACK_QUEEN, F8});
  append_move(&game, (MoveEncoding) {WHITE_BISHOP, A6});
  print_encoding(&game);
  write_to_file(&game, ASCII);
  return 0;
}

#define MAX_INPUT_LENGTH 5
int main2(void) {
  char input[MAX_INPUT_LENGTH];
  size_t read_length;
  GameEncoding game = new_game();
  while (1) {
    printf("Make move: ");
    if (fgets(input, MAX_INPUT_LENGTH, stdin) == NULL) {
      printf("EXITING FOR STRING TEST\n");
      printf("%s\n", to_str(&game, ASCII));
      fgets_exit_gracefully();
    }
    read_length = strlen(input);
    if (read_length > 0 && input[read_length - 1] == '\n') {
      // Remove \n and terminate the string
      input[read_length - 1] = '\0'; 
    }
    printf("You entered: %s\n", input);
    append_move(&game, encode_move(input));
    print_encoding(&game);
    /* printf("%s\n", to_str(&game, CSTR)); */
  }
  return 0;
}
