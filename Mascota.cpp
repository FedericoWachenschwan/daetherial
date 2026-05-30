#include "Mascota.h"
#include <cmath>

// Constructor: prepara la mascota antes de que empiece el juego
Mascota::Mascota() {
    // La textura es el archivo PNG cargado en memoria (la imagen cruda)
    // El sprite toma esa textura y la muestra en pantalla en una posición
    // Sin conectarlos, el sprite no sabe qué imagen dibujar
    if (!textura_abajo.loadFromFile("assets/cat_abajo.png")) {
        return;
    }
    if (!textura_arriba.loadFromFile("assets/cat_arriba.png")) {
        return;
    }
    if (!textura_derecha.loadFromFile("assets/cat_derecha.png")) {
        return;
    }
    if (!textura_izquierda.loadFromFile("assets/cat_izquierda.png")) {
        return;
    }

    // Por defecto la mascota mira hacia abajo al iniciar
    sprite_mascota.setTexture(textura_abajo);

    // El frame del gato es 32x32 píxeles, lo escalamos para que se vea bien en pantalla
    sprite_mascota.setScale(1.f, 1.f);

    // Coloca la mascota al lado del personaje en la posición inicial
    sprite_mascota.setPosition(200.f, 100.f);

    // Define cuántos píxeles avanza la mascota por frame (más lento que el personaje)
    velocidad = 2.f;

    // Define a cuántos píxeles de distancia la mascota deja de moverse
    distancia_maxima = 40.f;
}

// Mueve la mascota hacia el personaje y cambia la textura según la dirección
void Mascota::seguir(sf::Vector2f posicion_del_personaje) {
    // Obtiene dónde está la mascota ahora mismo
    sf::Vector2f posicion_de_mascota = sprite_mascota.getPosition();

    // Calcula cuántos píxeles separan a la mascota del personaje en X e Y
    float diferencia_x = posicion_del_personaje.x - posicion_de_mascota.x;
    float diferencia_y = posicion_del_personaje.y - posicion_de_mascota.y;

    // Calcula la distancia real entre ambos usando el teorema de Pitágoras (a²+b²=c²)
    float distancia_real = std::sqrt(diferencia_x * diferencia_x + diferencia_y * diferencia_y);

    // Solo se mueve si está más lejos que la distancia máxima
    if (distancia_real > distancia_maxima) {
        // Divide la diferencia por la distancia para obtener una dirección de longitud 1
        // Esto asegura que siempre se mueva a la misma velocidad sin importar el ángulo
        float direccion_x = diferencia_x / distancia_real;
        float direccion_y = diferencia_y / distancia_real;

        // Mueve la mascota en la dirección del personaje a la velocidad definida
        sprite_mascota.move(direccion_x * velocidad, direccion_y * velocidad);

        // Cambia la textura según hacia dónde se mueve la mascota
        // Compara el valor absoluto de X e Y para saber si se mueve más horizontal o vertical
        if (std::abs(diferencia_x) > std::abs(diferencia_y)) {
            // Se mueve más en X que en Y: va hacia los lados
            if (diferencia_x > 0) {
                // Diferencia positiva en X = personaje está a la derecha
                sprite_mascota.setTexture(textura_derecha);
                sprite_mascota.setScale(1.f, 1.f);
            }
            else {
                // Diferencia negativa en X = personaje está a la izquierda
                sprite_mascota.setTexture(textura_izquierda);
                sprite_mascota.setScale(1.f, 1.f);
            }
        }
        else {
            // Se mueve más en Y que en X: va hacia arriba o abajo
            if (diferencia_y > 0) {
                // Diferencia positiva en Y = personaje está abajo (en SFML Y crece hacia abajo)
                sprite_mascota.setTexture(textura_abajo);
                sprite_mascota.setScale(1.f, 1.f);
            }
            else {
                // Diferencia negativa en Y = personaje está arriba
                sprite_mascota.setTexture(textura_arriba);
                sprite_mascota.setScale(1.f, 1.f);
            }
        }
    }
}

// Dibuja el sprite de la mascota en la ventana
void Mascota::dibujar(sf::RenderWindow& ventana) {
    ventana.draw(sprite_mascota);
}