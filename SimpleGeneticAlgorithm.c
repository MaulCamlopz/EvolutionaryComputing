//
//  SimpleGeneticAlgorithm.c
//  Tarea VIII: Algoritmo Genético Simple(AGS)
//
//  Created by Raúl Camacho on 29/11/20.
//  Copyright © 2020 UACM. All rights reserved.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/* 
    Utilice las siguientes constantes para permitir un mayor 
    número de individuos de la población y 
    número de longitud del genoma para cada individuo.
*/
#define N 15 // Número maximo de individuos permitidos en la población
#define L 15 // Número maximo de longitud del genoma

#define CHILD 2 // Pareja de individuos (descendientes)

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

// Función para imprimir una población
void printPopulation(struct Population population, int nI, int longGen) {
    int i, j;
    printf("[");
    for (i = 0; i < nI; i++) {
        printf("[");
        for (j = 0; j < longGen; j++) {
            printf("%d,",population.genetics[i].gen[j].value);
        }
        printf("],");
    }
    printf("]\n");
}

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
    if (r < pC) {
        position = (rand() % (lG + 1));
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



// nI: Número de individuos de la población
// lG: longitud del Genoma
// pC: probabilidad de Cruza
// pM: probabilidad de mutación
// nG: número de Generaciones
void simpleGeneticAlgorithm(int nI, int lG, double pC, double pM, int nG) {
    int i, j;
    double S;
    struct fitItem fitPopulation;
    struct Population p = generatePopulation(nI, lG);
    printf("Poblacion original:\n");
    printPopulation(p, nI, lG);
    for (i = 0; i < nG; i++) {
        struct Population newP;
        fitPopulation = fitness(p, nI, lG);
        printf("Evaluacion de la pob:\n");
        //imprime las calificaciones
        printf("[");
        for (j = 0; j < nI; j++) {
            printf("%f, ",fitPopulation.array[j].value);
        }
        printf("]\n");
        for (S = 0.0, j = 0; j < nI; j++) {
		    S = S + fitPopulation.array[j].value;
        }
        printf("Calificacion:%f\n",S);
        // En este ciclo se genera una nueva población
        for (j = 0; j < nI; j++) {
            struct Genetics H1, H2;
            int h1, h2;
            h1 = selection(fitPopulation, N);
            h2 = selection(fitPopulation, N);
            H1 = p.genetics[h1];
            H2 = p.genetics[h2];
            struct Children c;
            c.genetics[0] = H1;
            c.genetics[1] = H2;
            c = crossover(c, pC, lG);
            c = mutation(c, pM, lG);
            newP.genetics[j] = c.genetics[0];
            newP.genetics[j+1] = c.genetics[1];
            j++;
        }
        printf("---------------------\n");
        printf("Nueva poblacion (%d): \n",i);
        printPopulation(newP,nI,lG);
        p = newP;
    }
    fitPopulation = fitness(p, nI, lG);
    printf("Evaluacion Final de la pob:\n");
    printf("[");
    for (i = 0; i < nI; i++) {
        printf("%f, ",fitPopulation.array[i].value);
    }
    printf("]\n");
    for (S = 0.0, j = 0; j < nI; j++) {
        S = S + fitPopulation.array[j].value;
    }
    printf("Suma de calificacion final:%f\n",S); 
}

int main(int argc, char const *argv[]) {
    srand (time(NULL)); // Cambia la semilla de la secuencia de números  aleatorios
    int nI = 0;
    int lG = 0;  
    int nG = 0;
    double pC = 0.0;
    double pM = 0.0; 
    printf("Ingrese el numero de individuos de la poblacion: ");
    scanf("%d",&nI);
    printf("Ingrese longitud del genoma: ");
    scanf("%d",&lG);
    printf("Ingrese probabilidad de cruza: ");
    scanf("%lf", &pC);
    printf("Ingrese probabilidad de mutacion: ");
    scanf("%lf", &pM);
    printf("Ingrese el numero de Generaciones: ");
    scanf("%d",&nG);
    printf("Datos registrados:\n");
    printf("nI = %d;\nlG = %d;\npC = %f;\npM = %f;\nnG = %d.\n", nI, lG, pC, pM, nG);
    printf("---------------------\n");
    simpleGeneticAlgorithm(nI, lG, pC, pM, nG);
    return 0;
}
