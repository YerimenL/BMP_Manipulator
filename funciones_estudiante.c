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
#include <string.h>
#include "constantes.h"
#include "funciones_estudiante.h"
#include "estructuras.h"
#define ARGUMENTOS_INSUFICIENTES 15

#define TAM_HEADER 54

int solucion(int argc, char* argv[])
{
    /*
    if (argc < 3) {
        printf("Uso: %s <imagen.bmpcopiar un> <funcion1> <funcion2> ...\n", arg3 v[0]);
        return ARGUMENTOS_INSUFICIENTES;
    };
    */

    FILE *imgOrig;
    t_metadata header;
    t_metadata *pHeader=&header;
    unsigned char headerCompleto[TAM_HEADER];

    //imagen = fopen(argv[argc-1],"r");//
    imgOrig = fopen("unlam2.bmp","rb");

    if(!imgOrig){
        printf("Ocurrio un problema al intentar abrir archivo BMP.\n");
        return ARCHIVO_NO_ENCONTRADO;
    };


    obtenerInfoHeader(imgOrig, pHeader, headerCompleto);

    modificarTonalidad(imgOrig, headerCompleto, pHeader,0);
    modificarTonalidad(imgOrig, headerCompleto, pHeader,1);
    modificarTonalidad(imgOrig, headerCompleto, pHeader,2);
    modificarContraste(imgOrig, headerCompleto, pHeader,3);
    modificarContraste(imgOrig, headerCompleto, pHeader,4);
    dejarNegativo(imgOrig, headerCompleto, pHeader);
    aplicarByN(imgOrig, headerCompleto, pHeader);
    rotarDerecha90(imgOrig, headerCompleto, pHeader);
    rotarIzquierda90(imgOrig, headerCompleto, pHeader);
    recortarImagen(imgOrig, headerCompleto, pHeader);
    achicarImagen(imgOrig, headerCompleto, pHeader);
    aplicarMonocromatico(imgOrig, headerCompleto, pHeader);






    fclose(imgOrig);



    return TODO_OK;
};

void obtenerInfoHeader(FILE* imgOrig, t_metadata* header, unsigned char* headerCompleto){

    /*obtengo todo el header completo para tenerlo en un vector*/
    fseek(imgOrig,0,SEEK_SET);
    fread(headerCompleto,sizeof(unsigned char),TAM_HEADER,imgOrig);

    //guardo informacion esencial del header para usarla
    memcpy(&header->tamArchivo,headerCompleto+2,sizeof(unsigned int));
    memcpy(&header->comienzoImagen,headerCompleto+10,sizeof(unsigned int));
    memcpy(&header->tamEncabezado,headerCompleto+14,sizeof(unsigned int));
    memcpy(&header->ancho,headerCompleto+18,sizeof(unsigned int));
    memcpy(&header->alto,headerCompleto+22,sizeof(unsigned int));
    memcpy(&header->profundidad,headerCompleto+28,sizeof(unsigned short));

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

    fseek(imagen,comienzoImagen,SEEK_SET);

    for(i=0;i<filas;i++){
        for(j=0;j<columnas;j++){

            fread(&matriz[i][j].pixel,sizeof(unsigned char),3,imagen);
    }};
};

void mostrarMatriz(t_pixel** matriz, const unsigned int ancho, const unsigned int alto) {
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            printf("(%d,%d,%d)-", matriz[i][j].pixel[0], matriz[i][j].pixel[1], matriz[i][j].pixel[2]);
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

int modificarTonalidad(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader,unsigned int color){

    char nombreArchivo[30];
    setearNombreArchivo(color, nombreArchivo);

    FILE* imgDestino = fopen(nombreArchivo, "wb");
    if(!imgDestino)
    {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    fwrite(headerCompleto,sizeof(unsigned char),TAM_HEADER,imgDestino);

    unsigned int bytesPixel = pHeader->profundidad/8;
    unsigned int bytesRelleno = (4 - ((pHeader->ancho * bytesPixel)%4))%4;

    t_pixel pixel;

    fseek(imgOrig,pHeader->comienzoImagen,SEEK_SET);
    fseek(imgDestino,pHeader->comienzoImagen,SEEK_SET);

    for(unsigned int i=0;i<pHeader->alto;i++ )
    {
        for(unsigned int j=0;j<pHeader->ancho;j++)
        {
            fread(&pixel,sizeof(unsigned char),3,imgOrig);

            if (pixel.pixel[color] * 1.5 > 255) {
                    pixel.pixel[color] = 255;
            } else {
                    pixel.pixel[color] = (unsigned char)(pixel.pixel[color] * 1.5);
            };

            fwrite(&pixel,sizeof(unsigned char),3,imgDestino);
        };
        for(unsigned int k = 0; k < bytesRelleno; k++)
        {
            unsigned char relleno = 0;
            fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
        };
    };

    fclose(imgDestino);
    return TODO_OK;
};

void setearNombreArchivo(unsigned int opcion, char *nombreArchivo) {
    switch (opcion) {
        case 0:
            sprintf(nombreArchivo, "estudiante_tonalidad-azul.bmp");
            break;
        case 1:
            sprintf(nombreArchivo, "estudiante_tonalidad-verde.bmp");
            break;
        case 2:
            sprintf(nombreArchivo, "estudiante_tonalidad-rojo.bmp");
            break;
        case 3:
            sprintf(nombreArchivo, "estudiante_aumentar-contraste.bmp");
            break;
        case 4:
            sprintf(nombreArchivo, "estudiante_reducir-contraste.bmp");
            break;
        default:
            printf("Opción no válida\n");
            break;
    };
};

int modificarContraste(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader,unsigned int accion){

    char nombreArchivo[40];
    setearNombreArchivo(accion, nombreArchivo);
    int valorColor;

    FILE* imgDestino = fopen(nombreArchivo, "wb");
    if(!imgDestino)
    {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    fwrite(headerCompleto,sizeof(unsigned char),TAM_HEADER,imgDestino);

    unsigned int bytesPixel = pHeader->profundidad/8;
    unsigned int bytesRelleno = (4 - ((pHeader->ancho * bytesPixel)%4))%4;

    t_pixel pixel;

    fseek(imgOrig,pHeader->comienzoImagen,SEEK_SET);
    fseek(imgDestino,pHeader->comienzoImagen,SEEK_SET);

    for(unsigned int i=0;i<pHeader->alto;i++ )
    {
        for(unsigned int j=0;j<pHeader->ancho;j++)
        {
            fread(&pixel,sizeof(unsigned char),3,imgOrig);

            for (int i = 0; i < 3; i++) {
                valorColor = pixel.pixel[i];
                if (accion == 3) {
                    if (valorColor < 127) {
                        valorColor = valorColor * 0.75; // Reducir un 25%
                    } else {
                        valorColor = valorColor * 1.25; // Aumentar un 25%
                    }
                } else if (accion == 4) {
                    if (valorColor < 127) {
                        valorColor = valorColor * 1.25; // Aumentar un 25%
                    } else {
                        valorColor = valorColor * 0.75; // Reducir un 25%
                    }
                };

            // Asegurar que el valor esté en el rango 0-255
            if (valorColor > 255) valorColor = 255;
            if (valorColor < 0) valorColor = 0;

            pixel.pixel[i] = (unsigned char)valorColor;
            };

            fwrite(&pixel,sizeof(unsigned char),3,imgDestino);

        for(unsigned int k = 0; k < bytesRelleno; k++)
        {
            unsigned char relleno = 0;
            fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
        };
        };
    };

    fclose(imgDestino);
    return TODO_OK;

};

int dejarNegativo(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader){

    FILE* imgDestino = fopen("estudiante_negativo.bmp", "wb");
    if(!imgDestino)
    {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    fwrite(headerCompleto,sizeof(unsigned char),TAM_HEADER,imgDestino);

    unsigned int bytesPixel = pHeader->profundidad/8;
    unsigned int bytesRelleno = (4 - ((pHeader->ancho * bytesPixel)%4))%4;

    t_pixel pixel;

    fseek(imgOrig,pHeader->comienzoImagen,SEEK_SET);
    fseek(imgDestino,pHeader->comienzoImagen,SEEK_SET);

    for(unsigned int i=0;i<pHeader->alto;i++ )
    {
        for(unsigned int j=0;j<pHeader->ancho;j++)
        {
            fread(&pixel,sizeof(unsigned char),3,imgOrig);

            pixel.pixel[0]=255-pixel.pixel[0]; //calculo el complemento del color para cada componente
            pixel.pixel[1]=255-pixel.pixel[1];
            pixel.pixel[2]=255-pixel.pixel[2];

            fwrite(&pixel,sizeof(unsigned char),3,imgDestino);
        };
        for(unsigned int k = 0; k < bytesRelleno; k++)
        {
            unsigned char relleno = 0;
            fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
        };
    };

    fclose(imgDestino);
    return TODO_OK;
};

int aplicarByN(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader){

    FILE* imgDestino = fopen("estudiante_escala-de-grises.bmp", "wb");
    if(!imgDestino)
    {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    fwrite(headerCompleto,sizeof(unsigned char),TAM_HEADER,imgDestino);

    unsigned int bytesPixel = pHeader->profundidad/8;
    unsigned int bytesRelleno = (4 - ((pHeader->ancho * bytesPixel)%4))%4;

    t_pixel pixel;
    int prom;

    fseek(imgOrig,pHeader->comienzoImagen,SEEK_SET);
    fseek(imgDestino,pHeader->comienzoImagen,SEEK_SET);

    for(unsigned int i=0;i<pHeader->alto;i++ )
    {
        for(unsigned int j=0;j<pHeader->ancho;j++)
        {
            fread(&pixel,sizeof(unsigned char),3,imgOrig);

            prom = (pixel.pixel[0]+pixel.pixel[1]+pixel.pixel[2])/3; //calculo el promedio de las tres componentes

            pixel.pixel[0]=prom;
            pixel.pixel[1]=prom;
            pixel.pixel[2]=prom;

            fwrite(&pixel,sizeof(unsigned char),3,imgDestino);
        };
        for(unsigned int i= 0; i < bytesRelleno; i++)
        {
            unsigned char relleno = 0;
            fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
        };
    };

    fclose(imgDestino);
    return TODO_OK;
}

int rotarDerecha90(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader){

    unsigned char copiaHeader[TAM_HEADER];
    memcpy(copiaHeader,headerCompleto,TAM_HEADER);
    FILE* imgDestino = fopen("estudiante_rotar-derecha.bmp","wb");
    if(!imgDestino)
    {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    };

    memcpy(copiaHeader+18,&pHeader->alto,sizeof(unsigned int));  //Invierto las dimensiones
    memcpy(copiaHeader+22,&pHeader->ancho,sizeof(unsigned int));

    fseek(imgDestino,0,SEEK_SET);
    fwrite(copiaHeader,sizeof(unsigned char),TAM_HEADER,imgDestino);

    unsigned int bytesPixel = pHeader->profundidad/8;
    unsigned int bytesRelleno = (4 - ((pHeader->ancho * bytesPixel)%4))%4;

    t_pixel pixel;
    int nuevaCol, nuevaFil;

    fseek(imgOrig,pHeader->comienzoImagen,SEEK_SET);
    fseek(imgDestino,pHeader->comienzoImagen,SEEK_SET);

    for(unsigned int i=0;i<pHeader->alto;i++ )
    {
        for(unsigned int j=0;j<pHeader->ancho;j++)
        {
            nuevaFil=pHeader->ancho-1-j;
            nuevaCol=i;
            fread(&pixel,sizeof(unsigned char),3,imgOrig);

            //me ubico en la posicion correcta para trasponer la imagen
            fseek(imgDestino, pHeader->comienzoImagen + (nuevaFil*(pHeader->alto * bytesPixel) + nuevaCol*bytesPixel), SEEK_SET);

            fwrite(&pixel,sizeof(unsigned char),3,imgDestino);
        };
        for(unsigned int k= 0; k < bytesRelleno; k++)
        {
            unsigned char relleno = 0;
            fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
        };
    };

    fclose(imgDestino);
    return TODO_OK;

}

int rotarIzquierda90(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader){

    unsigned char copiaHeader[TAM_HEADER];
    memcpy(copiaHeader,headerCompleto,TAM_HEADER);
    FILE* imgDestino = fopen("estudiante_rotar-izquierda.bmp","wb");
    if(!imgDestino)
    {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    };

    memcpy(copiaHeader+18,&pHeader->alto,sizeof(unsigned int));  //Invierto las dimensiones
    memcpy(copiaHeader+22,&pHeader->ancho,sizeof(unsigned int));

    fseek(imgDestino,0,SEEK_SET);
    fwrite(copiaHeader,sizeof(unsigned char),TAM_HEADER,imgDestino);

    unsigned int bytesPixel = pHeader->profundidad/8;
    unsigned int bytesRelleno = (4 - ((pHeader->ancho * bytesPixel)%4))%4;

    t_pixel pixel;
    int nuevaCol, nuevaFil;

    fseek(imgOrig,pHeader->comienzoImagen,SEEK_SET);
    fseek(imgDestino,pHeader->comienzoImagen,SEEK_SET);

    for(unsigned int i=0;i<pHeader->alto;i++ )
    {
        for(unsigned int j=0;j<pHeader->ancho;j++)
        {
            nuevaFil=j;
            nuevaCol=pHeader->alto-1-i;
            fread(&pixel,sizeof(unsigned char),3,imgOrig);

            //me ubico en la posicion correcta para trasponer la imagen
            fseek(imgDestino, pHeader->comienzoImagen + (nuevaFil*(pHeader->alto * bytesPixel) + nuevaCol*bytesPixel), SEEK_SET);

            fwrite(&pixel,sizeof(unsigned char),3,imgDestino);
        };
        for(unsigned int k= 0; k < bytesRelleno; k++)
        {
            unsigned char relleno = 0;
            fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
        };
    };

    fclose(imgDestino);
    return TODO_OK;
}

int recortarImagen(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader){

    FILE* imgDestino = fopen("estudiante_recortar.bmp","wb");
    if(!imgDestino)
    {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    };

    fseek(imgDestino,0,SEEK_SET);
    fwrite(headerCompleto,sizeof(unsigned char),TAM_HEADER,imgDestino);

    unsigned int bytesPixel = pHeader->profundidad/8;
    unsigned int bytesRelleno = (4 - ((pHeader->ancho * bytesPixel)%4))%4;

    t_pixel pixel;

    fseek(imgOrig,pHeader->comienzoImagen,SEEK_SET);
    fseek(imgDestino,pHeader->comienzoImagen,SEEK_SET);

    for(unsigned int i=0;i<pHeader->alto;i++ )
    {
        for(unsigned int j=0;j<pHeader->ancho;j++)
        {
            fread(&pixel,sizeof(unsigned char),3,imgOrig);


            if(i>(unsigned int)(pHeader->alto/2) || j>=(unsigned int)(pHeader->ancho/2))
            {
                pixel.pixel[0]=0;
                pixel.pixel[1]=0;
                pixel.pixel[2]=0;
            };

            fwrite(&pixel,sizeof(unsigned char),3,imgDestino);
        };
        for(unsigned int k= 0; k < bytesRelleno; k++)
        {
            unsigned char relleno = 0;
            fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
        };
    };

    fclose(imgDestino);
    return TODO_OK;
}

int achicarImagen(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader){

    unsigned char copiaHeader[TAM_HEADER];
    memcpy(copiaHeader,headerCompleto,TAM_HEADER);

    FILE* imgDestino = fopen("estudiante_achicar.bmp","wb");
    if(!imgDestino)
    {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    };

    unsigned int nuevoAlto=(pHeader->alto/2);
    unsigned int nuevoAncho=(pHeader->ancho/2);

    memcpy(copiaHeader+18,&nuevoAncho,sizeof(unsigned int));  //calculo las nuevas medidas
    memcpy(copiaHeader+22,&nuevoAlto,sizeof(unsigned int));

    fseek(imgDestino,0,SEEK_SET);
    fwrite(copiaHeader,sizeof(unsigned char),TAM_HEADER,imgDestino);

    unsigned int bytesPixel = pHeader->profundidad/8;
    unsigned int bytesRelleno = (4 - ((pHeader->ancho * bytesPixel)%4))%4;

    t_pixel pixel;

    fseek(imgOrig,pHeader->comienzoImagen,SEEK_SET);
    fseek(imgDestino,pHeader->comienzoImagen,SEEK_SET);

    for(unsigned int i=0;i<pHeader->alto;i++ )
    {
        for(unsigned int j=0;j<pHeader->ancho;j++)
        {
            fread(&pixel,sizeof(unsigned char),3,imgOrig);
            fseek(imgDestino, pHeader->comienzoImagen + ((i/2) * (nuevoAncho * bytesPixel) + (j/2) * bytesPixel), SEEK_SET);
            fwrite(&pixel,sizeof(unsigned char),3,imgDestino);
        };
        for(unsigned int k= 0; k < bytesRelleno; k++)
        {
            unsigned char relleno = 0;
            fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
        };
    };

    fclose(imgDestino);
    return TODO_OK;

}

int aplicarMonocromatico(FILE* imgOrig, unsigned char* headerCompleto, t_metadata* pHeader) {

    unsigned char copiaHeader[TAM_HEADER];
    memcpy(copiaHeader, headerCompleto, TAM_HEADER);




    FILE* imgDestino = fopen("estudiante_monocromo.bmp", "wb");
    if (!imgDestino) {
        printf("Error al crear el archivo BMP de destino.\n");
        return NO_SE_PUEDE_CREAR_ARCHIVO;
    }

    unsigned short nuevaProfundidad = 1;
    // Calcular nuevo tamaño del archivo considerando el ancho en bits
    unsigned int nuevoAnchoBytes = (pHeader->ancho) / 8;  // Redondear hacia arriba al byte más cercano
    unsigned int bytesRelleno = (4 - (nuevoAnchoBytes % 4)) % 4;
    unsigned int nuevoTamImagen = (nuevoAnchoBytes + bytesRelleno) * pHeader->alto;

    unsigned int nuevoTamHeader = pHeader->tamEncabezado + nuevoTamImagen;

    memcpy(copiaHeader + 34, &nuevoTamImagen, sizeof(unsigned int));
    memcpy(copiaHeader + 28, &nuevaProfundidad, sizeof(unsigned short));
    memcpy(copiaHeader + 2, &nuevoTamHeader, sizeof(unsigned int));

    fseek(imgDestino, 0, SEEK_SET);
    fwrite(copiaHeader, sizeof(unsigned char), TAM_HEADER, imgDestino);

    fseek(imgOrig, pHeader->comienzoImagen, SEEK_SET);
    fseek(imgDestino, pHeader->comienzoImagen, SEEK_SET);

    t_pixel pixel;
    unsigned char byte;
    int prom;
    unsigned int contador=0;

    for (unsigned int i = 0; i < pHeader->alto; i++) {
        for (unsigned int j = 0; j < pHeader->ancho; j++) {

            fread(&pixel.pixel, sizeof(unsigned char), 3, imgOrig);

            prom = (pixel.pixel[0] + pixel.pixel[1] + pixel.pixel[2]) / 3;

            if (prom > 127) {
                byte |= (1 << (8-contador%8)); // Establecer el bit en la posición k a 1 si prom > 127
            };

            contador++;

            if(contador % 8 == 0 || j == pHeader->ancho-1){
                fwrite(&byte, sizeof(unsigned char), 1, imgDestino);
                byte = 0;
            };


            if(contador % 45 == 0 || j == pHeader->ancho-1){
                    for(unsigned int k= 0; k < bytesRelleno; k++){
                        unsigned char relleno = 0;
                        fwrite(&relleno, sizeof(unsigned char), 1, imgDestino);
                    };
            };


        };



    };

    fclose(imgDestino);
    return TODO_OK;
}
