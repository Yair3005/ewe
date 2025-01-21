#include <stdio.h>
#include <stdlib.h>
#include "funciones.h"

int main() {
    Contaminacion zonas[NUM_ZONAS] = {0};
    float temperatura, viento, humedad;

    cargarDatos(zonas);

    FILE *reporte = fopen("reporte.txt", "a");
    if (!reporte) {
        perror("Error al abrir el archivo de reporte");
        exit(EXIT_FAILURE);
    }

    int opcion;
    do {
        printf("\nMenu:\n");
        printf("1. Monitoreo de contaminacion actual\n");
        printf("2. Prediccion de niveles futuros\n");
        printf("3. Calculo de promedios historicos\n");
        printf("4. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                monitoreoContaminacionActual(zonas);
                break;
            case 2:
                ingresarFactoresClimaticos(&temperatura, &viento, &humedad);
                prediccionFuturo(zonas, temperatura, viento, humedad);
                break;
            case 3:
                calculoPromediosHistoricos(zonas);
                break;
            case 4:
                printf("Saliendo del programa...\n");
                break;
            default:
                printf("Opcion no valida. Intente nuevamente.\n");
                break;
        }
    } while (opcion != 4);

    generarReporte(reporte, zonas, temperatura, viento, humedad);

    fclose(reporte);
    return 0;
}
