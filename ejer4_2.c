#include "Image.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <mpi.h>
#include <omp.h>

void calcular_histograma(const uint8_t *pixel_values, size_t size_vector, int *histograma, size_t size_histograma) {
    // Inicializar el histograma con ceros
    #pragma omp parallel for
    for (size_t i = 0; i < size_histograma; ++i) {
        histograma[i] = 0;
    }

    // Calcular la frecuencia de cada valor de píxel
    #pragma omp parallel for
    for (size_t i = 0; i < size_vector; ++i) {
        #pragma omp atomic
        histograma[pixel_values[i]]++;
    }
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    Image img;
    Image_load(&img, "sky_gray.png");
    if (img.data == NULL) {
        printf("Error al cargar la imagen.\n");
        MPI_Finalize();
        return 1;
    }

    // Obtiene el tamaño total del vector de la imagen
    size_t tam_vector = img.width * img.height;

    // calcula el tamaño de cada parte que se enviará a cada proceso, aquí tambien consideramos el residuo
    size_t M = tam_vector / size;
    size_t residue = tam_vector % size;
    if (rank < residue) {
        M++;
    }

    // Vector para almacenar la parte de la imagen que se procesará en cada proceso
    uint8_t *subvector = (uint8_t *)malloc(M * sizeof(uint8_t));

    // Distribuye el vector entre los procesos
    MPI_Scatter(img.data, M, MPI_UNSIGNED_CHAR, subvector, M, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);

    // Calcula el histograma parcial en cada proceso
    int histograma_local[256] = {0};
    calcular_histograma(subvector, M, histograma_local, sizeof(histograma_local) / sizeof(histograma_local[0]));

    // Recolectar los histogramas parciales reuniendo y totalizando
    int histograma_global[256] = {0};
    MPI_Reduce(histograma_local, histograma_global, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
/*
//Con esto imprimimos el histograma en consola
    if (rank == 0) {
        printf("Histograma global:\n");
        for (int i = 0; i < 256; ++i) {
            printf("Valor %d: %d\n", i, histograma_global[i]);
        }
    }*/
    
    if (rank == 0) {
        FILE *file = fopen("histograma.txt", "w");
        if (file == NULL) {
            printf("Error al abrir el archivo.\n");
            MPI_Finalize();
            return 1;
        }

        // Escribir el histograma en el archivo
        fprintf(file, "Histograma global:\n");
        for (int i = 0; i < 256; ++i) {
            fprintf(file, "Valor %d: %d\n", i, histograma_global[i]);
        }

        fclose(file);
    }

    Image_free(&img);
    free(subvector);

    MPI_Finalize();
    return 0;
}

