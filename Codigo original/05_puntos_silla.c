#include <stdio.h>

#define FILAS 3
#define COLUMNAS 3

void buscarPuntosSilla(int m[FILAS][COLUMNAS]) {
    printf("out = [ ");
    int primero = 1;

    for (int i = 0; i < FILAS; i++) {
        
        int min_val = m[i][0];
        for (int j = 1; j < COLUMNAS; j++) {
            if (m[i][j] < min_val) {
                min_val = m[i][j];
            }
        }

       
        for (int j = 0; j < COLUMNAS; j++) {
            if (m[i][j] == min_val) {
                int es_max_col = 1;
                for (int k = 0; k < FILAS; k++) {
                    if (m[k][j] > min_val) {
                        es_max_col = 0;
                        break;
                    }
                }
                if (es_max_col) {
                    if (!primero) printf(", ");
                    printf("[%d, %d, %d]", min_val, i, j);
                    primero = 0;
                }
            }
        }
    }
    printf(" ]\n");
}

int main() {
    int m[FILAS][COLUMNAS] = {
        {8, 4, 1},
        {6, 9, 3},
        {7, 5, 2}
    };

    buscarPuntosSilla(m);
    return 0;
}