/*
    Integrantes del grupo.
    Si alguno de los integrantes del grupo dejara la materia, completar de todos modos sus datos, aclarando que no entrega.
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------
    Apellido:
    Nombre:
    DNI:
    Entrega:
    -----------------

    Comentarios (opcionales) que deseen hacer al docente sobre el TP:

*/
#include <stdio.h>
#include <stdlib.h>
#include "constantes.h"
#include "funciones_estudiante.h"
#include "estructuras.h"

int solucion(int argc, char* argv[])
{
    /*
    if (argc < 3) {
        printf("Uso: %s <imagen.bmpcopiar un> <funcion1> <funcion2> ...\n", arg3 v[0]);
        return ARGUMENTOS_INSUFICIENTES;
    };
    */

    FILE *imagen;
    t_metadata header;
    t_metadata* pHeader=&header;
    t_pixel** matrizImg;

    //imagen = fopen(argv[argc-1],"r");//
    imagen = fopen("unlam2.bmp","r");

    if(!imagen){
        return ARCHIVO_NO_ENCONTRADO;
    };

    obtenerHeader(imagen, pHeader);

    matrizImg = crearMatriz(pHeader->alto, pHeader->ancho);


    cargarImagenMatriz(matrizImg,pHeader->alto,pHeader->ancho,imagen,pHeader->comienzoImagen);
   fclose(imagen);
    mostrarMatriz(matrizImg, pHeader->ancho, pHeader->alto);

    liberarMatriz(matrizImg, pHeader->alto);


    return TODO_OK;
}

void obtenerHeader(FILE* imagen, t_metadata* header){

    fseek(imagen,2, SEEK_SET);
    fread(&header->tamArchivo, sizeof(int),1,imagen);
    fseek(imagen,14, SEEK_SET);
    fread(&header->tamEncabezado, sizeof(int),1,imagen);
    fseek(imagen,10, SEEK_SET);
    fread(&header->comienzoImagen, sizeof(int),1,imagen);
    fseek(imagen,22, SEEK_SET);
    fread(&header->alto, sizeof(int),1,imagen);
    fseek(imagen,18, SEEK_SET);
    fread(&header->ancho, sizeof(int),1,imagen);
    fseek(imagen,28, SEEK_SET);
    fread(&header->profundidad, sizeof(short),1,imagen);

    };

t_pixel** crearMatriz(const int filas, const int columnas){

    t_pixel** matriz = (t_pixel**)malloc(filas * sizeof(t_pixel*));

    if(matriz == NULL) {
        printf("Error al asignar memoria para las filas.\n");
        return NULL;
    };

    for(int i = 0; i < filas; i++) {
        matriz[i] = (t_pixel*)malloc(columnas * sizeof(t_pixel));
        if(matriz[i] == NULL) {
            printf("Error al asignar memoria para la fila %d.\n", i);
            // Liberar la memoria que si se pudo reservar
            for(int j = 0; j < i; j++) {
                free(matriz[j]);
            };
            free(matriz);
            return NULL;
        };
    };

    return matriz;
};

void cargarImagenMatriz(t_pixel** matriz, const unsigned int filas, const unsigned int columnas, FILE* imagen, const unsigned int comienzoImagen){

    int i=0,j=0;
    unsigned char color;

    fseek(imagen,comienzoImagen,SEEK_SET);

    for(i=0;i<filas;i++){
        for(j=0;j<columnas;j++){

            fread(&color,sizeof(char),1,imagen);
            matriz[i][j].pixel[0]=color;
            fread(&color,sizeof(char),1,imagen);
            matriz[i][j].pixel[1]=color;
            fread(&color,sizeof(char),1,imagen);
            matriz[i][j].pixel[2]=color;
        };
    };
};

void mostrarMatriz(t_pixel** matriz, const unsigned int ancho, const unsigned int alto) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            printf("(%d,%d,%d) \t", matriz[i][j].pixel[0], matriz[i][j].pixel[1], matriz[i][j].pixel[2]);
        };
        printf("\n"); // Nueva línea para la siguiente fila
    };
};

void liberarMatriz(t_pixel** matriz, const unsigned int alto){
    for (int fila = 0; fila < alto; fila++) {
        free(matriz[fila]);
    };
    free(matriz);
};

