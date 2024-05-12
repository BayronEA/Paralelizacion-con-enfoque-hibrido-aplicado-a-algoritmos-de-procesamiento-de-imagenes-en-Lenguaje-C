# Paralelizacion-con-enfoque-hibrido-aplicado-a-algoritmos-de-procesamiento-de-imagenes-en-Lenguaje-C
Pequeño proyecto en donde se implementan funciones para el procesamiento de imágenes en lenguaje C 

#### Nuestra librería image.h
Hace el llamado del archivo image.c en donde tenemos todas las funciones que nos permiten procesar las imagenes tales como crear imagen, guardar imagen, liberar imagen, convertir a gris, filtro sepia, binarizar la imagen, sacarle el negativo convertir de RGB a CMY
Resultados

- Imagen original
![sky](https://github.com/BayronEA/Paralelizacion-con-enfoque-hibrido-aplicado-a-algoritmos-de-procesamiento-de-imagenes-en-Lenguaje-C/assets/51730885/aa98422f-e710-4d1a-ba58-81b5d5741dd3)

- Binarizada
![sky_binary](https://github.com/BayronEA/Paralelizacion-con-enfoque-hibrido-aplicado-a-algoritmos-de-procesamiento-de-imagenes-en-Lenguaje-C/assets/51730885/a3d494e6-c586-4555-a76b-86be8183e883)

- CMY
![sky_cmy](https://github.com/BayronEA/Paralelizacion-con-enfoque-hibrido-aplicado-a-algoritmos-de-procesamiento-de-imagenes-en-Lenguaje-C/assets/51730885/31f6fc6f-206a-44df-99e0-dd1fa4ac6985)

- Gris
![sky_gray](https://github.com/BayronEA/Paralelizacion-con-enfoque-hibrido-aplicado-a-algoritmos-de-procesamiento-de-imagenes-en-Lenguaje-C/assets/51730885/48007b23-da55-4ef1-8d80-01b3aae5a227)

- Negativo
![sky_negative](https://github.com/BayronEA/Paralelizacion-con-enfoque-hibrido-aplicado-a-algoritmos-de-procesamiento-de-imagenes-en-Lenguaje-C/assets/51730885/4e0eaa5c-4339-4605-9d2f-65bf16f2276a)

- Filtro sepia
![sky_sepia](https://github.com/BayronEA/Paralelizacion-con-enfoque-hibrido-aplicado-a-algoritmos-de-procesamiento-de-imagenes-en-Lenguaje-C/assets/51730885/4541d47d-0f5f-4eff-ac8c-18a97fc4fde2)

#### Ejercicio con programación híbrida
Escribir un programa utilizando OpenMPI, en el cual el proceso 0 lee una imagen (en escala de grises), 
y una vez obtenida su matriz, distribuye la misma por filas entre los procesos activos. Cada proceso 
debe calcular el histograma parcial de su subimagen y enviar los resultados al proceso 0, quien los 
reúne y totaliza, para imprimir el histograma general. Localmente, los procesos deben optimizar el 
cálculo del histograma, utilizando programación paralela con OpenMP.
El proceso 0 además debe escribir el histograma obtenido en un archivo de texto de manera que se 
pueda visualizar en Python. Finalmente escriba un script en Python, que lance el programa paralelo 
escrito en lenguaje C, y luego visualice el histograma obtenido.
