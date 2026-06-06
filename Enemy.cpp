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
    // 1. Guardamos dónde estaba antes de pensar
    sf::Vector2f posAnterior = _sprite.getPosition();

    // 2. NUEVO CEREBRO (Movimiento directo y ultra ligero)
    sf::Vector2f vectorDireccion = _posicionObjetivo - posAnterior;
    float distancia = std::hypot(vectorDireccion.x, vectorDireccion.y);

    // Si no está encima del jugador, que avance hacia él
    if (distancia > 5.f) {
        vectorDireccion /= distancia; // Normalizamos

        // Calculamos cuánto píxeles se quiere mover este frame
        sf::Vector2f movimiento = vectorDireccion * _velocidad * dt;

        // --- MAGIA DE WALL-SLIDING ---
        // Chequeamos Eje X
        sf::FloatRect hitboxX = getBounds();
        hitboxX.left += movimiento.x;
        if (!_mapaRef->hayColision(hitboxX)) {
            _sprite.move(movimiento.x, 0.f);
        }

        // Chequeamos Eje Y
        sf::FloatRect hitboxY = getBounds();
        hitboxY.top += movimiento.y;
        if (!_mapaRef->hayColision(hitboxY)) {
            _sprite.move(0.f, movimiento.y);
        }
    }

    // 3. Calculamos hacia dónde lo movió la IA para saber a dónde tiene que mirar
    sf::Vector2f posNueva = _sprite.getPosition();
    sf::Vector2f direccionMovimiento = posNueva - posAnterior;

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

    filaMatriz = static_cast<int>(estadoAnim) * 4 + static_cast<int>(_direccionActual);

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
    return _sprite.getGlobalBounds();
}