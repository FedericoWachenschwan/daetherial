#include "GameManager.h"

//AGREGAR sfml-audio-d.lib;sfml-audio.lib; en las propiedades del proyecto para que funcione el sonido VINCULADOR>ENTRADA sfml-audio-d.lib;sfml-audio.lib;

//COSITAS PARA CORREGIR::
// 
// ------------------------------------------------------------------------------------------------------------------
// CORREGIDO EL BUG DE LAS COLISIONES, SE CAMBIO LA LOGICA PARA COLISIONAR SE UTILIZA AABB, COMO BRIAN EXPLICO
// ASIMISMO SE CREO LA CLASE ABSTRACTA COLISIONABLE, DE LA CUAL HEREDAN LOS OBJETOS QUE PUEDEN COLISIONAR, 
// SE IMPLEMENTA EL METODO VIRTUAL PURO getBounds() PARA OBTENER LOS BORDES DE LOS OBJETOS Y REALIZAR LAS COLISIONES
// SE IMPLEMENTA INVENTARIO E INVENTARIO UI, SE CREAN LAS CLASES Item, Consumible, Mueble, Recurso, Equipamiento, 
// ASI COMO EL INVENTARIO Y SU INTERFAZ GRAFICA
//-------------------------------------------------------------------------------------------------------------------


int main() {
    GameManager game; // Instanciamos el GameManager del diagrama
    game.run();       // Ejecutamos el motor

    return 0;
}