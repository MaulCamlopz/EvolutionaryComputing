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

#define M 3 // Número de individuos en la población
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
        printf("- ");
        for (int j = 0, k = 0, queen = 0, position = 0; j < n*n; j++) { 
            if (p.genetics[i].gen[j].value) {
                k = queen * (n - 1);
                position = j - queen - k;
                printf("%d,",position);
                switch (position) {
                    case 0:
                        printf("(first)");
                        break;
                    case (n-1):
                        printf("(last)");
                        break;
                    default:
                        printf("medium");
                        break;
                }
                // if (!p.genetics[i].gen[j-1].value && !p.genetics[i].gen[j+1].value && !p.genetics[i].gen[j+n].value) {
                //     k++;
                //     fit.array[i].value = k;
                // }
                queen++;
            }
        }
        printf("\n");
    }
    return fit;
}

int main() {
    struct Population p = generatePopulation(M,N);
    struct fitQ fit = fit_NReinas(p, N, M);

    // Imprime la población
    for (int i = 0; i < M; i++) {
        printf("[");
        for (int j = 0; j < N*N; j++) {
            printf("%d,",p.genetics[i].gen[j].value);
        }
        printf("]\n");
    }
    printf("\n");

    return 0;
}