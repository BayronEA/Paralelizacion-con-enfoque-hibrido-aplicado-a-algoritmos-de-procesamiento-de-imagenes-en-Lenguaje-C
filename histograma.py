import subprocess
import matplotlib.pyplot as plt

def main():
    subprocess.run(["mpirun", "-np", "4", "ejer4_2"])

# funcion para leer el histograma
def histograma():
 
    histograma = {}
    with open("histograma.txt", "r") as file:
        for line in file:
            if line.startswith("Valor"):
                valor, cantidad = line.split(":")
                histograma[int(valor.split()[1])] = int(cantidad)
    return histograma

# Visualizamos el histograma
def visualizar(histograma):

    valores = list(histograma.keys())
    cantidades = list(histograma.values())
    plt.bar(valores, cantidades)
    plt.title("Histograma de la imagen")
    plt.xlabel("Valor del píxel")
    plt.ylabel("Frecuencia")
    plt.show()

if __name__ == "__main__":
    main()
    histograma = histograma()
    visualizar(histograma)
