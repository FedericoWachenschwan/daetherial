#include "Enemy.h"
#include <cmath>
#include <iostream>

Enemy::Enemy(sf::Vector2f posInicial, Map* mapa, const std::string& rutaTextura)
{
    _mapaRef = mapa;

    if (!_textura.loadFromFile(rutaTextura)) {
        std::cout << "❌ Error: No se pudo cargar la textura LPC del enemigo: " << rutaTextura << std::endl;
    }
    _sprite.setTexture(_textura);
    _sprite.setOrigin(32.f, 32.f);
    setPosicion(posInicial);

    // STATS DE MINION (ajustables a gusto)
    _vidaMaxima = 30;
    _vidaActual = _vidaMaxima;
    _danio = 10;
    _velocidad = 70.f; // Más lento que el Mago (170.f)
    _cooldownAtaque = 2.f; // Cooldown para atacar
    actualizarSpriteRect();
}

void Enemy::actualizar(float dt) {
    sf::Vector2f posAnterior = _sprite.getPosition();
    sf::Vector2f vectorDireccion = _posicionObjetivo - posAnterior;
    float distancia = std::hypot(vectorDireccion.x, vectorDireccion.y);

    if (distancia > 5.f) {
        vectorDireccion /= distancia;
        sf::Vector2f movimiento = vectorDireccion * _velocidad * dt;

        // 🚀 MAGIA: Le pedimos al PADRE que se encargue de chocar
        this->aplicarMovimientoConColisiones(movimiento, *_mapaRef);
    }

    // El resto de la lógica de animación queda igual, pero ahora
    // el sprite YA SE MOVIÓ correctamente gracias al Padre.
    sf::Vector2f direccionMovimiento = _sprite.getPosition() - posAnterior;
    // 4. EL CUERPO: Ejecutamos la lógica LPC de animación
    determinarEstadoYDireccion(direccionMovimiento);

    _tiempoFrame += dt;
    if (_tiempoFrame >= _velocidadAnimacion) {
        _tiempoFrame = 0.f;
        _frameActual++;
        controlarLimitesYTransiciones();
    }

    actualizarSpriteRect();
}

void Enemy::determinarEstadoYDireccion(sf::Vector2f direccion) {
    // Si no se movió casi nada, está quieto
    if (std::abs(direccion.x) < 0.1f && std::abs(direccion.y) < 0.1f) {
        _estadoActual = EstadoPersonaje::IDLE;
        return;
    }

    _estadoActual = EstadoPersonaje::WALK;

    // ALGORITMO DE MIRADA (Igual que tu Mago)
    if (std::abs(direccion.x) > std::abs(direccion.y)) {
        _direccionActual = (direccion.x > 0.f) ? DireccionLPC::RIGHT : DireccionLPC::LEFT;
    }
    else {
        _direccionActual = (direccion.y > 0.f) ? DireccionLPC::DOWN : DireccionLPC::UP;
    }
}

void Enemy::controlarLimitesYTransiciones() {
    if (_estadoActual == EstadoPersonaje::IDLE) {
        _maxFrames = 1;
        _frameActual = 0; // Clavamos el frame de respiración/quieto
    }
    else if (_estadoActual == EstadoPersonaje::WALK) {
        _maxFrames = 9;
        if (_frameActual >= _maxFrames) _frameActual = 0;
    }
}

void Enemy::actualizarSpriteRect() {
    int filaMatriz = 0;
    EstadoPersonaje estadoAnim = _estadoActual;

    // Si está IDLE, usamos la fila de WALK pero clavamos el frame en 0
    if (estadoAnim == EstadoPersonaje::IDLE) {
        estadoAnim = EstadoPersonaje::WALK;
    }

    filaMatriz = static_cast<int>(estadoAnim) + static_cast<int>(_direccionActual);

    int columna = _frameActual;
    if (_estadoActual == EstadoPersonaje::IDLE) {
        columna = 0; // Frame base de estar parado
    }

    _sprite.setTextureRect(sf::IntRect(columna * 64, filaMatriz * 64, 64, 64));
}

void Enemy::dibujar(sf::RenderWindow& ventana) {
    EntidadViva::dibujar(ventana);
}

sf::FloatRect Enemy::getBounds() const {
    sf::FloatRect cajaOriginal = _sprite.getGlobalBounds();
    float margenIzquierda = 15.f;
    float margenDerecha = 15.f;
    float margenArriba = 15.f;
    float margenAbajo = 15.f;

    return sf::FloatRect(
        cajaOriginal.left + margenIzquierda,
        cajaOriginal.top + margenArriba,
        cajaOriginal.width - margenIzquierda - margenDerecha,
        cajaOriginal.height - margenArriba - margenAbajo);
}