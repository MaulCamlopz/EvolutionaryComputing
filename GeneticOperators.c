//
//  GeneticOperators.c
//  Tarea VII: Operadores geneticos - Cruza y Mutación
//
//  Created by Raúl Camacho on 15/11/20.
//  Copyright © 2020 UACM. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* Utilice las siguientes constantes para cambiar el número de individuos de la población y el número de longitud del genoma para cada individuo. */
#define N 6 // Número de individuos en la población
#define L 3 // Longitud del gen

#define CHILD 2 // Pareja de individuos

struct Gen {
    int value;
};

struct Genetics {
    struct Gen gen[L];
};

struct Children {
    struct Genetics genetics[CHILD];
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

// Función variante del coseno
double function(double x) {
    double value = ((5.5)*x)-(3.5);
    double result = ((1 - (pow(value, 2.0))) * (cos(value) + 1) + 2);
    return result;
}

// Función fitness
struct fitItem fitness(struct Population p, int n, int longGen) {
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

// Función de selección
int selection(struct fitItem fitPopulation, int n) {
    /*
        c -> calificacion ponderada global de la población
        Ca -> calificación acumulada
        S -> suma de calificaciones de la población
    */
    double S, c, Ca, r;
	int i, response;
    response = -1;
	r =  (( rand( ) % 1001 ) / 1000.0f) ;
	for (S = 0.0, i = 0; i < n; i++) {
		S = S + fitPopulation.array[i].value;
    }
    c = r*S;
    for (Ca = 0.0, i = 0; i < n; i++) {
		Ca = Ca + fitPopulation.array[i].value;
		if (Ca > c) {
			response = i;
			break;
		}
    }
    return response;
}

// Función de cruza
struct Children crossover(struct Children sel, double pC, int lG) {
    /*
        sel: pareja de individuos seleccionados
        pC: probabilidad de Cruza
        lG: longitud del Gen
    */
    struct Children c;
    // H1 y H2 estructuras para guardar los hijos de la cruza
    struct Genetics H1, H2;
    double r;
    int position, i;
    r = ((rand() % 1001) / 1000.0f);
    printf("random = %f\n",r);
    printf("probabilidad de Cruza = %f, \n",pC);
    if (r < pC) {
        position = (rand() % (lG + 1));
        printf("Cruza en la posicion: %d\n",position);
        for (i = 0; i < position; i++) {
            H1.gen[i].value = sel.genetics[0].gen[i].value;
            H2.gen[i].value = sel.genetics[1].gen[i].value;
        }
        for (i = position; i < lG; i++) {
            H1.gen[i].value = sel.genetics[1].gen[i].value;
            H2.gen[i].value = sel.genetics[0].gen[i].value;
        }
        c.genetics[0] = H1;
        c.genetics[1] = H2;
    } else {
        return sel;
    }
    return c;
}

// Función de mutación
struct Children mutation(struct Children sel, double pM, int lG) {
    double q = 0.0;
    int i, j = 0;
    for (i = 0; i < CHILD; i++) {
        for (j = 0; j < lG; j++) {
            q = ((rand() % 1001) / 1000.0f);
            if (q < pM) {
                printf("Mutacion\n");
                printf("valor de q: %f, valor de pM: %f\n", q, pM);
                printf("Para %d, en la posicion %d\n", i, j);
                if (sel.genetics[i].gen[j].value == 1) {
                    sel.genetics[i].gen[j].value = 0;
                } else {
                    sel.genetics[i].gen[j].value = 1;
                }
            }
        }
    }
    return sel;
}

// Función para generar una población
struct Population generatePopulation(int n, int longGen) {
    struct Population p;
    int i, j;
    for (i = 0; i < n; i++) { // Recorre los genes
        for (j = 0; j < longGen; j++) { // Recorre cada gen
            int num = rand () % (2); // Obtiene un número aleatorio entre 0 y 1
            p.genetics[i].gen[j].value = num; // Asigna un valor al gen
        }
    }
    return p;
}

int main(int argc, char const *argv[]) {

    srand (time(NULL)); // Cambia la semilla de la secuencia de números  aleatorios
    
    int i, j;
    struct Population p = generatePopulation(N, L);
    struct fitItem fitPopulation = fitness(p, N, L);

    // Imprime la población
    printf("[");
    
    for (i = 0; i < N; i++) {
        printf("[");
        for (j = 0; j < L; j++) {
            printf("%d,",p.genetics[i].gen[j].value);
        }
        printf("],");
    }
    printf("]");
    printf("\n");

    //imprime las calificaciones
    printf("[");
    for (i = 0; i < N; i++) {
        printf("%f, ",fitPopulation.array[i].value);
    }
    printf("]");
    printf("\n");

    struct Genetics H1, H2;
    int h1, h2;
    h1 = selection(fitPopulation, N);
    h2 = selection(fitPopulation, N);
    H1 = p.genetics[h1];
    H2 = p.genetics[h2];

    printf("Elemento seleccionado: %d\n", h1);
    printf("[");
    for (i = 0; i < L; i++) {
        printf("%d,", H1.gen[i].value);
    }
    printf("]");

    printf("\n");
    printf("Elemento seleccionado: %d\n", h2);
    printf("[");
    for (i = 0; i < L; i++) {
        printf("%d,", H2.gen[i].value);
    }
    printf("]");
    printf("\n\n");
    
    struct Children c;
    c.genetics[0] = H1;
    c.genetics[1] = H2;

    c = crossover(c, 0.1, L);
    
    // Imprime la población
    printf("[");
    for (i = 0; i < CHILD; i++) {
        printf("[");
        for (j = 0; j < L; j++) {
            printf("%d,",c.genetics[i].gen[j].value);
        }
        printf("],");
    }
    printf("]");
    printf("\n\n");

    c = mutation(c, 0.1, L);
    
    // Imprime la población
    printf("[");
    for (i = 0; i < CHILD; i++) {
        printf("[");
        for (j = 0; j < L; j++) {
            printf("%d,",c.genetics[i].gen[j].value);
        }
        printf("],");
    }
    printf("]");
    printf("\n");

    return 0;
}
