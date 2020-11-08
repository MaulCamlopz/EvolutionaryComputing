//
//  SelectionAlgorithm.c
//  Tarea VI: Algoritmo de selección
//
//  Created by Raúl Camacho on 07/11/20.
//  Copyright © 2020 UACM. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define N 4 // Número de individuos en la población
#define L 3 // Longitud del gen

struct Gen {
    int value;
};

struct Genetics {
    struct Gen gen[L];
};

struct Population {
    struct Genetics genetics[N];
};

struct Item {
    double value;
};

struct fitItem {
    struct Item array[N];
};

// funcion variante del coseno
double function(double x){
    double value = ((5.5)*x)-(3.5);
    double result = ((1 - (pow(value, 2.0))) * (cos(value) + 1) + 2);
    return result;
}


struct Population generatePopulation(int n, int longGen) {
    struct Population p;
    srand (time(NULL)); // Cambia la semilla de la secuencia de números aleatorios
    int i, j;
    for (i = 0; i < n; i++) { // Recorre los genes
        for (j = 0; j < longGen; j++) { // Recorre cada gen
            int num = rand () % (2); // Obtiene un número aleatorio entre 0 y 1
            p.genetics[i].gen[j].value = num; // Asigna un valor al gen
        }
    }
    return p;
}

struct fitItem fit(struct Population p, int n, int longGen) {
	struct fitItem fit;
    int i, j, k;
    for (i = 0; i < n; i++) {
    	double domain = 0.0;
        for (j = 0, k = 1; j < longGen; j++, k++) {
        	double d = p.genetics[i].gen[j].value/pow(2, k);
        	domain = domain + d;
        }
        fit.array[i].value = function(domain);
    }
    return fit;
}

int main() {

    int n = N;
    int longGen = L;
    struct Population p = generatePopulation(n, longGen);
    struct fitItem fitItem = fit(p, n, longGen);
    // Imprime la población
    printf("[");
    int i, j;
    for (i = 0; i < N; i++) {
        printf("[");
        for (j = 0; j < longGen; j++) {
            printf("%d,",p.genetics[i].gen[j].value);
        }
        printf("],");
    }
    printf("]");
    printf("\n");
    //imprime las calificaciones
    printf("[");
    for (i = 0; i < n; i++) {
        printf("%f,",fitItem.array[i].value);
    }
    printf("]");
    
    printf("\n");

    return 0;
}

