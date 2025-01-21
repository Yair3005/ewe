#define NUM_ZONAS 5
#define DIAS 30

#include <stdio.h>


typedef struct {
    char zona[50];
    char fecha[12];
    float co2[DIAS];
    float so2[DIAS];
    float no2[DIAS];
    float pm25[DIAS];
} Contaminacion;


void cargarDatos(Contaminacion zonas[]);
void ingresarFactoresClimaticos(float *temperatura, float *viento, float *humedad);
float calcularPromedio(float datos[], int dias);
float predecirNivel(float datos[], int dias, float temperatura, float viento, float humedad);
void monitoreoContaminacionActual(Contaminacion zonas[]);
void prediccionFuturo(Contaminacion zonas[], float temperatura, float viento, float humedad);
void calculoPromediosHistoricos(Contaminacion zonas[]);
void mostrarAlertas(float co2, float so2, float no2, float pm25);
void generarReporte(FILE *reporte, Contaminacion zonas[], float temperatura, float viento, float humedad);
void generarRecomendaciones(float co2, float so2, float no2, float pm25);




