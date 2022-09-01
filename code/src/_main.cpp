#include <cstdio>
#include <cstring>

#include "translator.h"

int main(int argc, char* argv[]) {
  if (argc != 2) {
    printf("Comando errado!\n");
    printf("Argumento do nome do arquivo faltando!\n");
    printf("\n");
    printf("Tente de novo!\n");
    printf("\n");
    return 0;
  }

  translateCode(argv[1]);

  return 0;
}
