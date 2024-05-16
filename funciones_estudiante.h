#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED

#include "constantes.h"
#include "estructuras.h"

int solucion(int argc, char* argv[]);
void obtenerHeader(FILE *imagen, t_metadata* header);
t_pixel** crearMatriz(const int filas, const int columnas);
void cargarImagenMatriz(t_pixel** matriz, const unsigned int filas, const unsigned int columnas, FILE* imagen, const unsigned int comienzoImagen);
void mostrarMatriz(t_pixel** matriz, const unsigned int ancho, const unsigned int alto);
void liberarMatriz(t_pixel** matriz, const unsigned int alto);
void rotarDerecha90();
void rotarIzquierda90();
void tonalidadAzul();
void tonalidadRoja();
void tonalidadVerde();
void cortarImagen();



#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
