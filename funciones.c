#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones.h"


const float LIMITE_ACEPTABLE[4] = {250.0, 50.0, 100.0, 25.0};
const float LIMITE_MAXIMO[4] = {350.0, 70.0, 150.0, 35.0};



float calcularPromedio(float datos[], int dias) {
    float suma = 0;
    for (int i = 0; i < dias; i++) {
        suma += datos[i];
    }
    return dias > 0 ? suma / dias : 0;
}

float predecirNivel(float datos[], int dias, float temperatura, float viento, float humedad) {
    float suma = 0.0;
    int cuenta = 0;

    for (int i = 0; i < dias; i++) {
        if (datos[i] > 0) {
            suma += datos[i];
            cuenta++;
        }
    }

    if (cuenta == 0) {
        return 0.0;
    }

    float promedio = suma / cuenta;
    float ajuste = (temperatura * 0.05) - (viento * 0.02) + (humedad * 0.005);
    return promedio + ajuste;
}



void cargarDatos(Contaminacion zonas[]) {
    FILE *archivo = fopen("datos2h.txt", "r");
    if (!archivo) {
        perror("Error al abrir el archivo");
        exit(EXIT_FAILURE);
    }

    char linea[256];
    fgets(linea, sizeof(linea), archivo);

    int diasLeidos[NUM_ZONAS] = {0};

    while (fgets(linea, sizeof(linea), archivo)) {
        char fecha[12], zona[50];
        float co2, so2, no2, pm25;

        if (sscanf(linea, "%[^,], %[^,], %f, %f, %f, %f", fecha, zona, &co2, &so2, &no2, &pm25) == 6) {
            int zonaIndex = -1;

            for (int i = 0; i < NUM_ZONAS; i++) {
                if (strcmp(zonas[i].zona, zona) == 0) {
                    zonaIndex = i;
                    break;
                }
            }

            if (zonaIndex == -1) {
                for (int i = 0; i < NUM_ZONAS; i++) {
                    if (zonas[i].zona[0] == '\0') {
                        zonaIndex = i;
                        strncpy(zonas[i].zona, zona, sizeof(zonas[i].zona) - 1);
                        break;
                    }
                }
            }

            if (zonaIndex != -1 && diasLeidos[zonaIndex] < DIAS) {
                int dia = diasLeidos[zonaIndex];
                strcpy(zonas[zonaIndex].fecha, fecha);
                zonas[zonaIndex].co2[dia] = co2;
                zonas[zonaIndex].so2[dia] = so2;
                zonas[zonaIndex].no2[dia] = no2;
                zonas[zonaIndex].pm25[dia] = pm25;
                diasLeidos[zonaIndex]++;
            }
        }
    }

    fclose(archivo);
}

void ingresarFactoresClimaticos(float *temperatura, float *viento, float *humedad) {
    printf("Ingrese la temperatura actual (C): ");
    scanf("%f", temperatura);
    printf("Ingrese la velocidad del viento actual (km/h): ");
    scanf("%f", viento);
    printf("Ingrese el nivel de humedad actual (%%): ");
    scanf("%f", humedad);
}



void monitoreoContaminacionActual(Contaminacion zonas[]) {
    printf("\n--- Monitoreo de Contaminacion Actual ---\n");
    for (int i = 0; i < NUM_ZONAS; i++) {
        if (zonas[i].zona[0] != '\0') {
            float co2 = calcularPromedio(zonas[i].co2, DIAS);
            float so2 = calcularPromedio(zonas[i].so2, DIAS);
            float no2 = calcularPromedio(zonas[i].no2, DIAS);
            float pm25 = calcularPromedio(zonas[i].pm25, DIAS);
            
            printf("Zona: %s\n", zonas[i].zona);
            printf("Promedio CO2: %.2f ppm, Promedio SO2: %.2f ppb, Promedio NO2: %.2f ppb, Promedio PM2.5: %.2f ug/m3\n", 
                    co2, so2, no2, pm25);
            mostrarAlertas(co2, so2, no2, pm25);
        }
    }
}

void prediccionFuturo(Contaminacion zonas[], float temperatura, float viento, float humedad) {
    printf("\n--- Prediccion de Niveles Futuros ---\n");
    for (int i = 0; i < NUM_ZONAS; i++) {
        if (zonas[i].zona[0] != '\0') {
            float prediccionCO2 = predecirNivel(zonas[i].co2, DIAS, temperatura, viento, humedad);
            float prediccionSO2 = predecirNivel(zonas[i].so2, DIAS, temperatura, viento, humedad);
            float prediccionNO2 = predecirNivel(zonas[i].no2, DIAS, temperatura, viento, humedad);
            float prediccionPM25 = predecirNivel(zonas[i].pm25, DIAS, temperatura, viento, humedad);

            printf("Zona: %s\n", zonas[i].zona);
            printf("Prediccion CO2: %.2f ppm, Prediccion SO2: %.2f ppb, Prediccion NO2: %.2f ppb, Prediccion PM2.5: %.2f ug/m3\n", 
                    prediccionCO2, prediccionSO2, prediccionNO2, prediccionPM25);
            mostrarAlertas(prediccionCO2, prediccionSO2, prediccionNO2, prediccionPM25);
        }
    }
}

void calculoPromediosHistoricos(Contaminacion zonas[]) {
    printf("\n--- Calculo de Promedios Historicos ---\n");

    
    const float LIMITE_OMS_CO2 = 400.0; 
    const float LIMITE_OMS_SO2 = 20.0;   
    const float LIMITE_OMS_NO2 = 40.0;   
    const float LIMITE_OMS_PM25 = 10.0;  

    for (int i = 0; i < NUM_ZONAS; i++) {
        if (zonas[i].zona[0] != '\0') {
            float promedioCO2 = calcularPromedio(zonas[i].co2, DIAS);
            float promedioSO2 = calcularPromedio(zonas[i].so2, DIAS);
            float promedioNO2 = calcularPromedio(zonas[i].no2, DIAS);
            float promedioPM25 = calcularPromedio(zonas[i].pm25, DIAS);

            printf("Zona: %s\n", zonas[i].zona);
            printf("Promedio Historico CO2: %.2f ppm, Promedio Historico SO2: %.2f ppb, Promedio Historico NO2: %.2f ppb, Promedio Historico PM2.5: %.2f ug/m3\n", 
                    promedioCO2, promedioSO2, promedioNO2, promedioPM25);

            
            if (promedioCO2 > LIMITE_OMS_CO2) {
                printf("  Alerta: El nivel de CO2 supera el limite de la OMS.\n");
            }
            if (promedioSO2 > LIMITE_OMS_SO2) {
                printf("  Alerta: El nivel de SO2 supera el limite de la OMS.\n");
            }
            if (promedioNO2 > LIMITE_OMS_NO2) {
                printf("  Alerta: El nivel de NO2 supera el limite de la OMS.\n");
            }
            if (promedioPM25 > LIMITE_OMS_PM25) {
                printf("  Alerta: El nivel de PM2.5 supera el limite de la OMS.\n");
            }

            
            mostrarAlertas(promedioCO2, promedioSO2, promedioNO2, promedioPM25);
        }
    }
}





void mostrarAlertas(float co2, float so2, float no2, float pm25) {
    if (co2 > LIMITE_ACEPTABLE[0] || so2 > LIMITE_ACEPTABLE[1] || 
        no2 > LIMITE_ACEPTABLE[2] || pm25 > LIMITE_ACEPTABLE[3]) {
        printf("- Alerta: Contaminacion por encima del limite aceptable.\n");
    }

    if (co2 > LIMITE_MAXIMO[0] || so2 > LIMITE_MAXIMO[1] || 
        no2 > LIMITE_MAXIMO[2] || pm25 > LIMITE_MAXIMO[3]) {
        printf("- Peligro: Contaminacion extrema. Se recomienda acciones inmediatas.\n");
    }

  
    if (co2 > LIMITE_MAXIMO[0] || so2 > LIMITE_MAXIMO[1] || 
        no2 > LIMITE_MAXIMO[2] || pm25 > LIMITE_MAXIMO[3]) {
        printf("- Recomendaciones: Reduccion del trafico vehicular, cierre temporal de industrias, suspension de actividades al aire libre.\n");
    }
}


void generarReporte(FILE *reporte, Contaminacion zonas[], float temperatura, float viento, float humedad) {
    fprintf(reporte, "Reporte de contaminacion y predicciones:\n");
    for (int i = 0; i < NUM_ZONAS; i++) {
        if (zonas[i].zona[0] != '\0') {
            float promedioCO2 = calcularPromedio(zonas[i].co2, DIAS);
            float promedioSO2 = calcularPromedio(zonas[i].so2, DIAS);
            float promedioNO2 = calcularPromedio(zonas[i].no2, DIAS);
            float promedioPM25 = calcularPromedio(zonas[i].pm25, DIAS);

            fprintf(reporte, "Zona: %s\n", zonas[i].zona);
            fprintf(reporte, "Promedio CO2: %.2f ppm, Promedio SO2: %.2f ppb, Promedio NO2: %.2f ppb, Promedio PM2.5: %.2f ug/m3\n", 
                    promedioCO2, promedioSO2, promedioNO2, promedioPM25);
        }
    }
}

void generarRecomendaciones(float co2, float so2, float no2, float pm25) {
    int recomendacion = 0; // Variable para controlar si se muestra alguna recomendacion

    if (co2 > LIMITE_ACEPTABLE[0] || so2 > LIMITE_ACEPTABLE[1] || 
        no2 > LIMITE_ACEPTABLE[2] || pm25 > LIMITE_ACEPTABLE[3]) {
        printf("- Alerta: Contaminacion por encima del limite aceptable.\n");
        printf("- Recomendaciones: Reduzca el trafico vehicular y evite actividades al aire libre.\n");
        recomendacion = 1; 
    }

    if (co2 > LIMITE_MAXIMO[0] || so2 > LIMITE_MAXIMO[1] || 
        no2 > LIMITE_MAXIMO[2] || pm25 > LIMITE_MAXIMO[3]) {
        printf("- Peligro: Contaminacion extrema. Se recomienda acciones inmediatas.\n");
        printf("- Recomendaciones: Cierre temporal de industrias y suspension de actividades al aire libre.\n");
        recomendacion = 1; 
    }

    
    if (!recomendacion) {
        printf("- El clima esta limpio, cuidalo.\n");
    }
}
