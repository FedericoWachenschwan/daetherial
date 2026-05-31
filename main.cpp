#include "GameManager.h"
#include <cstdlib>


//COSITAS PARA CORREGIR::
// 
// ------------------------------------------------------------------------------------------------------------------
// CORREGIDO EL BUG DE LAS COLISIONES, SE CAMBIO LA LOGICA PARA COLISIONAR SE UTILIZA AABB, COMO BRIAN EXPLICO
// ASIMISMO SE CREO LA CLASE ABSTRACTA COLISIONABLE, DE LA CUAL HEREDAN LOS OBJETOS QUE PUEDEN COLISIONAR, 
// SE IMPLEMENTA EL METODO VIRTUAL PURO getBounds() PARA OBTENER LOS BORDES DE LOS OBJETOS Y REALIZAR LAS COLISIONES
// SE IMPLEMENTA INVENTARIO E INVENTARIO UI, SE CREAN LAS CLASES Item, Consumible, Mueble, Recurso, Equipamiento, 
// ASI COMO EL INVENTARIO Y SU INTERFAZ GRAFICA
// 
// 
// COSITAS PARA IMPLEMENTAR:
// Refactorizar mascota, Enemy y Personaje, con EntidadViva, para evitar código repetido y mejorar la organización del proyecto
// Sistema de Guardado /serializacion para guardar el progreso del jugador (nivel, inventario, posición, etc) y cargarlo después
// Implementar la lectura de IDs especiales en el CSV para el agua (sistema de pesca) y las escaleras con efecto 2.5D (subir/bajar pisos)
//-------------------------------------------------------------------------------------------------------------------


int main() {
	system("chcp 65001 > nul"); // Configuramos la consola para UTF-8 y ocultamos el mensaje de cambio de código (Emojis)
    GameManager game; // Instanciamos el GameManager del diagrama
    game.run();       // Ejecutamos el motor

    return 0;
}