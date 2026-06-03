#include "Mascota.h"
#include <cmath> // Para std::abs y std::hypot

Mascota::Mascota() {
    if (!textura_abajo.loadFromFile("assets/cat_abajo.png")) return;
    if (!textura_arriba.loadFromFile("assets/cat_arriba.png")) return;
    if (!textura_derecha.loadFromFile("assets/cat_derecha.png")) return;
    if (!textura_izquierda.loadFromFile("assets/cat_izquierda.png")) return;

    // 🌟 Usamos el _sprite heredado
    _sprite.setTexture(textura_abajo);
    _sprite.setScale(1.f, 1.f);
    _sprite.setPosition(200.f, 100.f);

    // 🌟 FIX IMPORTANTE: Como ahora usamos 'dt' (que vale onda 0.016 por frame), 
    // la velocidad no puede ser 2. Hay que subirla a píxeles por segundo reales.
    _velocidad = 120.f;

    distancia_maxima = 40.f;
}

void Mascota::actualizar(float dt) {
    sf::Vector2f posicion_de_mascota = _sprite.getPosition();

    float diferencia_x = _posicionDuenio.x - posicion_de_mascota.x;
    float diferencia_y = _posicionDuenio.y - posicion_de_mascota.y;

    // 🌟 FIX: Aplicamos el std::hypot anti-crasheos
    float distancia_real = std::hypot(diferencia_x, diferencia_y);

    if (distancia_real > distancia_maxima) {
        float direccion_x = diferencia_x / distancia_real;
        float direccion_y = diferencia_y / distancia_real;

        // 🌟 FIX: Multiplicamos por _velocidad y por dt para sincronizar con los FPS
        _sprite.move(direccion_x * _velocidad * dt, direccion_y * _velocidad * dt);

        // Tu lógica original de texturas (intacta porque es excelente)
        if (std::abs(diferencia_x) > std::abs(diferencia_y)) {
            if (diferencia_x > 0) {
                _sprite.setTexture(textura_derecha);
            }
            else {
                _sprite.setTexture(textura_izquierda);
            }
        }
        else {
            if (diferencia_y > 0) {
                _sprite.setTexture(textura_abajo);
            }
            else {
                _sprite.setTexture(textura_arriba);
            }
        }
    }
}

void Mascota::dibujar(sf::RenderWindow& ventana) {
    EntidadViva::dibujar(ventana);
}

// 🌟 HITBOX EXCLUSIVA DEL GATO
sf::FloatRect Mascota::getBounds() const {
    sf::Vector2f pos = _sprite.getPosition();

    // Asumiendo que el gato es de 32x32 y queremos la colisión en su base
    return sf::FloatRect(pos.x, pos.y, 32.f, 32.f);
}