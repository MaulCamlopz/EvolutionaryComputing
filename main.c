//
//  main.c
//  Representación del dominio del problema de las N-reinas
//
//  Created by Raúl Camacho on 26/10/20.
//  Copyright © 2020 UACM. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define M 2 // Número de individuos en la población
#define N 4 // Número de reinas

struct Gen {
    int value;
};

struct Genetics {
    struct Gen gen[N*N];
};

struct Population {
    struct Genetics genetics[M];
};

struct fitQ {
    struct Gen array[M];
};

int *initQueens(int n) {
    int *v = NULL;
    if ((v = malloc(sizeof(int)*n)) == NULL) {
        printf("Sin memoria suficiente\n");
        return NULL;
    }
    return v;
}

void queenPosition(int *v, int n) {
    for (int i = 0, j = 0; i < n; i++) {
        j = i * (n - 1);
        v[i] = (rand()%n) + i + j;
    }
}

struct Population generatePopulation(int m, int n) {
    struct Population p;
    int *queens = NULL;
    for (int i = 0; i < m; i++) { // Recorre cada individuo
        queens = initQueens(n); // Reinicia las reinas
        queenPosition(queens, n); // Asigna una posición aleatoria a cada reina
        for (int j = 0, k = 0; j < n*n; j++) { // Agrega las reinas al individuo
            if (j == queens[k]) {
                p.genetics[i].gen[j].value = 1;
                k++;
            } else {
                p.genetics[i].gen[j].value = 0;
            }
        }
    }
    return p;
}

struct fitQ fit_NReinas(struct Population p, int n, int m) {
    struct fitQ fit;
    for (int i = 0; i < m; i++) {
        int score = 0;
        for (int j = 0; j < n*n; j++) {
            if (p.genetics[i].gen[j].value) { // Si encuentra una reina retorna verdadero
                int row = j/n;
                int column = j%n;
                printf("\nreina en posicion row %d column %d: ",row,column);
                int positionRow = row*n;
                int positionColumn = (0*n) + (positionRow%n);
                for (int k = 0; k < n; k++) {
                    //revisamos ataques sobre la misma fila
                    if (positionRow != j && p.genetics[i].gen[positionRow].value == 1) {
                        printf("attack in row from %d to %d, ",positionRow,j);
                    } else {
                        score++;
                    }
                    //revisamos ataques sobre la misma columna
                    if (positionColumn != j && p.genetics[i].gen[positionColumn].value == 1) {
                        printf("attack in column from %d to %d, ",positionColumn,j);
                    } else {
                        score++;
                    }
                    positionRow++;
                    positionColumn = ((k+1)*n) + (j % n);
                }
            }
            fit.array[i].value = score;
        }
        printf("\n");
    }
    return fit;
}

int main() {
    struct Population p = generatePopulation(M,N);
    struct fitQ fit = fit_NReinas(p, N, M);

    // Imprime la población
    int q = 0;
    printf("\n");
    for (int i = 0; i < M; i++) {
        printf("Tablero %d:\n",i+1);
        for (int j = 0; j < N*N; j++) {
            printf("%d ",p.genetics[i].gen[j].value);
            if(q < N-1){
                q++;
            } else {
                q = 0;
                printf("\n");
            }
        }
        printf("\n");
    }

    printf("Resultado de funcion fitness:\n");
    printf("[");
        for (int i = 0; i < M; i++) {
            printf("%d,",fit.array[i].value);
        }
        printf("]\n");

    return 0;
}
