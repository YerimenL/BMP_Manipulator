#ifndef FUNCIONES_ESTUDIANTE_H_INCLUDED
#define FUNCIONES_ESTUDIANTE_H_INCLUDED

#include "constantes.h"
#include "estructuras.h"

int solucion(int argc, char* argv[]);
void obtenerInfoHeader(FILE*, t_metadata* , unsigned char*);
int rotarIzquierda90(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader);
int rotarDerecha90(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader);
int modificarTonalidad(FILE* ,unsigned char* ,t_metadata* , unsigned int tono);
void setearNombreArchivo(unsigned int opcion, char *nombreArchivo);
int modificarContraste(FILE* ,unsigned char* ,t_metadata* , unsigned int accion);
int dejarNegativo(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader);
int aplicarByN(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader);
int recortarImagen(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader);
int achicarImagen(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader);
int aplicarMonocromatico(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader);



#endif // FUNCIONES_ESTUDIANTE_H_INCLUDED
