#include "Image.h"
#include "utils.h"
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image/stb_image_write.h"

void Image_load(Image *img, const char *fname) {
    if((img->data = stbi_load(fname, &img->width, &img->height, &img->channels, 0)) != NULL) {
        img->size = img->width * img->height * img->channels;
        img->allocation_ = STB_ALLOCATED;
        printf("Canales de la imagen cargada: %d\n", img->channels);
    }
}

void Image_create(Image *img, int width, int height, int channels, bool zeroed) {
    size_t size = width * height * channels;
    if(zeroed) {
        img->data = calloc(size, 1);
    } else {
        img->data = malloc(size);
    }

    if(img->data != NULL) {
        img->width = width;
        img->height = height;
        img->size = size;
        img->channels = channels;
        img->allocation_ = SELF_ALLOCATED;
    }
}

void Image_save(const Image *img, const char *fname) {
    // Check if the file name ends in one of the .jpg/.JPG/.jpeg/.JPEG or .png/.PNG
    if(str_ends_in(fname, ".jpg") || str_ends_in(fname, ".JPG") || str_ends_in(fname, ".jpeg") || str_ends_in(fname, ".JPEG")) {
        stbi_write_jpg(fname, img->width, img->height, img->channels, img->data, 100);
    } else if(str_ends_in(fname, ".png") || str_ends_in(fname, ".PNG")) {
        stbi_write_png(fname, img->width, img->height, img->channels, img->data, img->width * img->channels);
    } else {
        ON_ERROR_EXIT(false, "");
    }
}

void Image_free(Image *img) {
    if(img->allocation_ != NO_ALLOCATION && img->data != NULL) {
        if(img->allocation_ == STB_ALLOCATED) {
            stbi_image_free(img->data);
        } else {
            free(img->data);
        }
        img->data = NULL;
        img->width = 0;
        img->height = 0;
        img->size = 0;
        img->allocation_ = NO_ALLOCATION;
    }
}

void Image_to_gray(const Image *orig, Image *gray) {
    ON_ERROR_EXIT(!(orig->allocation_ != NO_ALLOCATION && orig->channels >= 3), "The input image must have at least 3 channels.");
    int channels = orig->channels == 4 ? 2 : 1;
    Image_create(gray, orig->width, orig->height, channels, false);
    ON_ERROR_EXIT(gray->data == NULL, "Error in creating the image");

    for(unsigned char *p = orig->data, *pg = gray->data; p != orig->data + orig->size; p += orig->channels, pg += gray->channels) {
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
        if(orig->channels == 4) {
            *(pg + 1) = *(p + 3);
        }
    }
}

void Image_to_sepia(const Image *orig, Image *sepia) {
    ON_ERROR_EXIT(!(orig->allocation_ != NO_ALLOCATION && orig->channels >= 3), "The input image must have at least 3 channels.");
    Image_create(sepia, orig->width, orig->height, orig->channels, false);
    ON_ERROR_EXIT(sepia->data == NULL, "Error in creating the image");

    // Sepia filter coefficients from https://stackoverflow.com/questions/1061093/how-is-a-sepia-tone-created
    for(unsigned char *p = orig->data, *pg = sepia->data; p != orig->data + orig->size; p += orig->channels, pg += sepia->channels) {
        *pg       = (uint8_t)fmin(0.393 * *p + 0.769 * *(p + 1) + 0.189 * *(p + 2), 255.0);         // red
        *(pg + 1) = (uint8_t)fmin(0.349 * *p + 0.686 * *(p + 1) + 0.168 * *(p + 2), 255.0);         // green
        *(pg + 2) = (uint8_t)fmin(0.272 * *p + 0.534 * *(p + 1) + 0.131 * *(p + 2), 255.0);         // blue        
        if(orig->channels == 4) {
            *(pg + 3) = *(p + 3);
        }
    }
}

void Image_binary(const Image *orig, Image *binary) {
    ON_ERROR_EXIT(!(orig->allocation_ != NO_ALLOCATION && orig->channels >= 1), "La imagen de entrada debe tener al menos 1 canal.");
    
    // crea la imagen
    Image_create(binary, orig->width, orig->height, 1, false);
    ON_ERROR_EXIT(binary->data == NULL, "Error al crear la imagen binaria");
    
   //Calculamos el umbral
    uint8_t threshold = 127;
    
    // Binariza la imagen original
    for(unsigned char *p = orig->data, *pb = binary->data; p != orig->data + orig->size; p += orig->channels, pb++) {
        uint8_t average = (*p + *(p + 1) + *(p + 2)) / 3;
        *pb = (average > threshold) ? 255 : 0; // Si el promedio es mayor al umbral, se asigna 255, de lo contrario 0
    }
}


void Image_negative(const Image *orig, Image *negative) {
    ON_ERROR_EXIT(!(orig->allocation_ != NO_ALLOCATION), "La imagen de entrada debe tener asignación.");
    ON_ERROR_EXIT(!(orig->channels == 1), "La imagen debe estar en escala de grises.");

    // Crea la imagen 
    Image_create(negative, orig->width, orig->height, orig->channels, false);
    ON_ERROR_EXIT(negative->data == NULL, "Error al crear la imagen negativa");


    for (unsigned char *p = orig->data, *pn = negative->data; p != orig->data + orig->size; p++, pn++) {
        *pn = 255 - *p; // calculamos la inversa restando -255 a todos los pixeles
    }
}


void Image_RGB2CMY(const Image *orig, Image *cmy) {
    ON_ERROR_EXIT(!(orig->allocation_ != NO_ALLOCATION), "La imagen de entrada debe tener asignación.");
    Image_create(cmy, orig->width, orig->height, orig->channels, false);
    ON_ERROR_EXIT(cmy->data == NULL, "Error al crear la imagen CMY");

    // Convierte los valores RGB a CMY
    for (unsigned char *p = orig->data, *pcmy = cmy->data; p != orig->data + orig->size; p += orig->channels, pcmy += cmy->channels) {
        *pcmy     = (255 - *p); // C (Cyan) = 255 - R
        *(pcmy + 1) = (255 - *(p + 1)); // M (Magenta) = 255 - G
        *(pcmy + 2) = (255 - *(p + 2)); // Y (Yellow) = 255 - B
        if (orig->channels == 4) {
          *(pcmy + 3) = *(p + 3);
        }
    }
}

void Image_get_pixel_positions(const Image *img, uint8_t **pixel_values, size_t *size_vector) {
    if (img->allocation_ == NO_ALLOCATION || img->channels != 1) {
        fprintf(stderr, "Error: La imagen no es de un solo canal.\n");
        exit(EXIT_FAILURE);
    }

    *size_vector = img->width * img->height;
    *pixel_values = (uint8_t *)malloc(*size_vector * sizeof(uint8_t));
    if (*pixel_values == NULL) {
        fprintf(stderr, "Error: Locacion de memoeria fallida.\n");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < *size_vector; i++) {
        (*pixel_values)[i] = img->data[i];
    }
}


