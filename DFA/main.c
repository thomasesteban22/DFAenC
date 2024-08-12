#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estructura para representar una transición
typedef struct {
    char estado_actual[10];
    char simbolo;
    char estado_siguiente[10];
} Transicion;

// Estructura para representar el DFA
typedef struct {
    char estados[10][10];
    int num_estados;
    char alfabeto[10];
    int num_simbolos;
    char estado_inicial[10];
    char estados_aceptacion[10][10];
    int num_estados_aceptacion;
    Transicion transiciones[100];
    int num_transiciones;
} DFA;

// Función para leer el archivo de configuración
int leer_configuracion(DFA *dfa) {
    char filename[] = "C:\\Users\\Thomas Esteban\\CLionProjects\\DFA\\configuracion.txt";

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de configuración");
        return -1;
    }

    // Leer estados
    fscanf(file, "%s", dfa->estados[0]);
    char *token = strtok(dfa->estados[0], ",");
    dfa->num_estados = 0;
    while (token != NULL) {
        strcpy(dfa->estados[dfa->num_estados++], token);
        token = strtok(NULL, ",");
    }

    // Leer alfabeto
    fscanf(file, "%s", dfa->alfabeto);
    dfa->num_simbolos = strlen(dfa->alfabeto);

    // Leer estado inicial
    fscanf(file, "%s", dfa->estado_inicial);

    // Leer estados de aceptación
    fscanf(file, "%s", dfa->estados_aceptacion[0]);
    token = strtok(dfa->estados_aceptacion[0], ",");
    dfa->num_estados_aceptacion = 0;
    while (token != NULL) {
        strcpy(dfa->estados_aceptacion[dfa->num_estados_aceptacion++], token);
        token = strtok(NULL, ",");
    }

    // Leer transiciones
    dfa->num_transiciones = 0;
    char transicion_linea[30];
    while (fscanf(file, "%s", transicion_linea) != EOF) {
        token = strtok(transicion_linea, ",");
        strcpy(dfa->transiciones[dfa->num_transiciones].estado_actual, token);
        token = strtok(NULL, ",");
        dfa->transiciones[dfa->num_transiciones].simbolo = token[0];
        token = strtok(NULL, ",");
        strcpy(dfa->transiciones[dfa->num_transiciones].estado_siguiente, token);
        dfa->num_transiciones++;
    }

    fclose(file);
    return 0;
}

// Función para obtener el siguiente estado dada una transición
char* obtener_siguiente_estado(DFA *dfa, char *estado_actual, char simbolo) {
    for (int i = 0; i < dfa->num_transiciones; i++) {
        if (strcmp(dfa->transiciones[i].estado_actual, estado_actual) == 0 && dfa->transiciones[i].simbolo == simbolo) {
            return dfa->transiciones[i].estado_siguiente;
        }
    }
    return NULL;
}

// Función para verificar si un estado es de aceptación
int es_estado_aceptacion(DFA *dfa, char *estado) {
    for (int i = 0; i < dfa->num_estados_aceptacion; i++) {
        if (strcmp(dfa->estados_aceptacion[i], estado) == 0) {
            return 1;
        }
    }
    return 0;
}

// Función para imprimir las transiciones
void imprimir_transiciones(DFA *dfa) {
    printf("Transiciones del DFA:\n");
    for (int i = 0; i < dfa->num_transiciones; i++) {
        printf("%s -- %c --> %s\n", dfa->transiciones[i].estado_actual, dfa->transiciones[i].simbolo, dfa->transiciones[i].estado_siguiente);
    }
}

int main() {
    DFA dfa;
    char cadena[100];

    // Leer el archivo de configuración
    if (leer_configuracion(&dfa) != 0) {
        return 1;
    }

    // Imprimir las transiciones
    imprimir_transiciones(&dfa);

    // Ingresar la cadena de entrada
    printf("Ingrese la cadena de entrada: ");
    scanf("%s", cadena);

    // Procesar la cadena de entrada con el DFA
    char *estado_actual = dfa.estado_inicial;
    for (int i = 0; i < strlen(cadena); i++) {
        estado_actual = obtener_siguiente_estado(&dfa, estado_actual, cadena[i]);
        if (estado_actual == NULL) {
            printf("Resultado: La cadena fue rechazada.\n");
            return 0;
        }
    }

    // Verificar si el estado final es de aceptación
    if (es_estado_aceptacion(&dfa, estado_actual)) {
        printf("Resultado: La cadena fue aceptada.\n");
    } else {
        printf("Resultado: La cadena fue rechazada.\n");
    }

    return 0;
}
