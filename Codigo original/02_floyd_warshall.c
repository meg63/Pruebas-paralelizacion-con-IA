#include <stdio.h>

#define V 4
#define INF 999999

void floydWarshall(int m[V][V], int out[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            out[i][j] = m[i][j];
        }
    }

    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (out[i][k] != INF && out[k][j] != INF && out[i][k] + out[k][j] < out[i][j]) {
                    out[i][j] = out[i][k] + out[k][j];
                }
            }
        }
    }
}

void printMatrix(int matrix[V][V]) {
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (matrix[i][j] == INF)
                printf("INF\t");
            else
                printf("%d\t", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    int m[V][V] = {
        {0, 5, INF, 10},
        {INF, 0, 3, INF},
        {INF, INF, 0, 1},
        {INF, INF, INF, 0}
    };
    int out[V][V];

    floydWarshall(m, out);
    printMatrix(out);

    return 0;
}