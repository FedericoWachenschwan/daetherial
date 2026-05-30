#pragma once
#include <SFML/Graphics.hpp>
#include "map.h"
#include "Colisionable.h" // 🌟 1. Incluimos el contrato de colisiones
#include "Inventario.h"
#include "habilidad_1.h"

// Clase que representa al personaje controlado por el jugador
// 🌟 2. Hacemos que herede de Colisionable de forma pública
class Personaje : public Colisionable {
private:
    sf::Sprite sprite_del_personaje;    // Objeto que muestra la imagen del personaje en pantalla
    sf::Texture textura_abajo;          // Imagen cuando mira hacia abajo
    sf::Texture textura_arriba;         // Imagen cuando mira hacia arriba
    sf::Texture textura_derecha;        // Imagen cuando mira hacia la derecha
    sf::Texture textura_izquierda;      // Imagen cuando mira hacia la izquierda
    float velocidad = 0.f;              // Cuántos píxeles se mueve por frame, empieza en 0
    Inventario _inventario;                // Inventario del personaje
	habilidad_1 primeraHabilidad;       // Primera habilidad del personaje
public:
    // Constructor: se ejecuta automáticamente al crear el objeto Personaje
    Personaje();

    // Acceso a la mochila del personaje
    Inventario& getInventario() { return _inventario; }
    const Inventario& getInventario() const { return _inventario; }

    // FEDE AGREGO ESTAS FUNCIONES PARA QUE EL PERSONAJE PUEDA COMUNICARSE CON EL MAPA Y POR LO TANTO PUEDA COLISIONAR
    // 🛡️ El personaje ahora recibe el mapa por referencia para poder colisionar ("NO es NO")
    void manejarInput(Map& mapa, sf::RenderWindow& ventana);

    // Devuelve las coordenadas X e Y actuales del personaje
    sf::Vector2f getPosicion();

    // Dibuja el personaje en la ventana
    void dibujar(sf::RenderWindow& ventana);

	// Funcion debug para chequear colisiones AABB dibujando la hitbox real del personaje (caja 16x16) en 
    void dibujarDebug(sf::RenderWindow& ventana);

    // =================================================================================================================================
    // 🌟 3. EL CONTRATO OBLIGATORIO: getBounds() para el sistema AABB
    // =================================================================================================================================
    // Devolvemos la hitbox real del personaje: caja 16x16 ubicada en la base del sprite (los pies).
    // Esto hace que las colisiones sean más precisas que usar todo el rectángulo del sprite (64x64).
    sf::FloatRect getBounds() const override {
        sf::Vector2f pos = sprite_del_personaje.getPosition();
        float hitboxX = pos.x + 24.f; // (64 - 16) / 2
        float hitboxY = pos.y + 48.f; // base del sprite
        return sf::FloatRect(hitboxX, hitboxY, 16.f, 16.f);
    }

    // 🌟 DEBUG: Para centrar el sprite con la hitbox en caliente
    void ajustarOrigenSprite(float x, float y);

    void actualizarHabilidades(float dt);
};