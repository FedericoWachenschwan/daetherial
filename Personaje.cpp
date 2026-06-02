#include "Personaje.h"
#include <iostream>
#include "InputManager.h"
#include <cmath> 

// ============================================================================
// CONSTRUCTOR: Inicialización y configuración del indicador de rango
// ============================================================================
Personaje::Personaje() {
    // 🌟 _sprite viene heredado de EntidadViva
    if (!textura_completa_lpc.loadFromFile("assets/maguito_main.png")) {
        std::cerr << "❌ Error: No se pudo cargar la hoja de sprites LPC." << std::endl;
        return;
    }
    _sprite.setTexture(textura_completa_lpc);
    _sprite.setPosition(100.f, 100.f);

    // 🌟 CONFIGURACIÓN ESTÉTICA DEL ANILLO DE RANGO (Rojo/Celeste transparente)
    _circuloRango.setRadius(_radioAlcance);
    _circuloRango.setFillColor(sf::Color(255, 0, 0, 60));
    _circuloRango.setOutlineColor(sf::Color::Red);
    _circuloRango.setOutlineThickness(1.0f);
    _circuloRango.setOrigin(_radioAlcance, _radioAlcance);     // Origen clavado al centro

    actualizarSpriteRect();
}

// ============================================================================
// MANEJAR INPUT: El filtro principal de acciones y movimiento
// ============================================================================
void Personaje::manejarInput(const InputManager& input, Map& mapa, sf::RenderWindow& ventana, bool uiCapturaMouse) {

    // 1. FILTRO ABSOLUTO: Si está casteando o muerto, se congela por completo SIEMPRE
    if (_estadoActual == EstadoPersonaje::SPELLCAST || _estadoActual == EstadoPersonaje::HURT) return;

    sf::Vector2f direccion = input.getDireccionMovimiento();
    // 🌟 _velocidad viene heredada de EntidadViva
    sf::Vector2f movimiento = direccion * _velocidad;

    // EL MULTIPLICADOR DIAGONAL TRADICIONAL (Aplica el freno matemático exacto)
    if (direccion.x != 0.f && direccion.y != 0.f) {
        movimiento *= 0.7071f;
    }

    // Actualizamos la mirada y la intención de movimiento de forma inteligente
    determinarEstadoYDireccion(direccion);

    // Procesamos el intento de apuntar, cancelar o disparar la magia
    procesarHabilidades(input, ventana, uiCapturaMouse);

    // 🌟 Ejecutamos las colisiones AABB contra el mapa (Llama a la función de la clase madre)
    resolverColisiones(movimiento, mapa);
}

// ============================================================================
// SUB-FUNCIÓN 1: Decide el estado lógico de movimiento y la mirada
// ============================================================================
void Personaje::determinarEstadoYDireccion(sf::Vector2f direccion) {
    if (direccion.x == 0.f && direccion.y == 0.f) {
        if (_estadoActual != EstadoPersonaje::AIMING) {
            _estadoActual = EstadoPersonaje::IDLE;
        }
        else {
            _frameActual = 0;
            _tiempoFrame = 0.f;
        }
        return;
    }

    if (_estadoActual != EstadoPersonaje::AIMING) {
        _estadoActual = EstadoPersonaje::WALK;
    }

    if (direccion.y < 0.f)      _direccionActual = DireccionLPC::UP;
    else if (direccion.y > 0.f) _direccionActual = DireccionLPC::DOWN;

    if (direccion.x > 0.f)      _direccionActual = DireccionLPC::RIGHT;
    else if (direccion.x < 0.f) _direccionActual = DireccionLPC::LEFT;
}

// ============================================================================
// SUB-FUNCIÓN 2: Procesa el interruptor (Toggle) y el lanzamiento de la magia
// ============================================================================
void Personaje::procesarHabilidades(const InputManager& input, sf::RenderWindow& ventana, bool uiCapturaMouse) {
    if (uiCapturaMouse) return;

    if (input.quiereSaltar()) {
        if (_estadoActual == EstadoPersonaje::AIMING) {
            _estadoActual = EstadoPersonaje::IDLE;
            _frameActual = 0;
        }
        else {
            _estadoActual = EstadoPersonaje::AIMING;
            _frameActual = 0;
        }
    }

    if (_estadoActual == EstadoPersonaje::AIMING && input.quiereAtacar()) {
        _estadoActual = EstadoPersonaje::SPELLCAST;
        _frameActual = 0;
        _tiempoFrame = 0.f;

        sf::Vector2i mousePantalla = input.getPosicionMouse();
        sf::Vector2f mouseMundo = ventana.mapPixelToCoords(mousePantalla);

        // 🌟 Usamos getPosicion() de la clase madre
        sf::Vector2f posPersonaje = this->getPosicion();
        if (std::abs(mouseMundo.x - posPersonaje.x) > std::abs(mouseMundo.y - posPersonaje.y)) {
            _direccionActual = (mouseMundo.x > posPersonaje.x) ? DireccionLPC::RIGHT : DireccionLPC::LEFT;
        }
        else {
            _direccionActual = (mouseMundo.y > posPersonaje.y) ? DireccionLPC::DOWN : DireccionLPC::UP;
        }

        _bolaDeFuego.activar(posPersonaje, mouseMundo, _radioAlcance);
    }
}

// ============================================================================
// ACTUALIZAR: El motor temporal de los relojes de animación y lógicas hijas
// ============================================================================
void Personaje::actualizar(float dt) {
    if (_estadoActual == EstadoPersonaje::AIMING) {
        _circuloRango.setPosition(this->getPosicion());
    }

    float limiteTiempoFrame = _velocidadAnimacion;
    if (_estadoActual == EstadoPersonaje::IDLE) {
        limiteTiempoFrame = 0.5f;
    }

    _tiempoFrame += dt;
    if (_tiempoFrame >= limiteTiempoFrame) {
        _tiempoFrame = 0.f;
        _frameActual++;
        controlarLimitesYTransiciones();
    }

    actualizarSpriteRect();
    _bolaDeFuego.actualizar(dt);
}

// ============================================================================
// CONTROLAR LÍMITES Y TRANSICIONES: Setea los límites de frames de la matriz LPC
// ============================================================================
void Personaje::controlarLimitesYTransiciones() {
    switch (_estadoActual) {

    case EstadoPersonaje::IDLE:
        _maxFrames = 1;
        _frameActual = 0;
        break;

    case EstadoPersonaje::AIMING:
        _maxFrames = 9;
        if (_frameActual >= _maxFrames) _frameActual = 0;
        break;

    case EstadoPersonaje::WALK:
        _maxFrames = 9;
        if (_frameActual >= _maxFrames) _frameActual = 0;
        break;

    case EstadoPersonaje::SPELLCAST:
        _maxFrames = 7;
        if (_frameActual >= _maxFrames) {
            _estadoActual = EstadoPersonaje::IDLE;
            _frameActual = 0;
        }
        break;

    case EstadoPersonaje::HURT:
        _maxFrames = 6;
        if (_frameActual >= _maxFrames) _frameActual = _maxFrames - 1;
        break;
    }
}

// ============================================================================
// ACTUALIZAR SPRITE RECT: El encargado matemático del recorte del PNG
// ============================================================================
void Personaje::actualizarSpriteRect() {
    int filaMatriz = 0;

    EstadoPersonaje estadoAnim = _estadoActual;
    if (_estadoActual == EstadoPersonaje::IDLE || _estadoActual == EstadoPersonaje::AIMING) {
        estadoAnim = EstadoPersonaje::WALK;
    }

    if (estadoAnim == EstadoPersonaje::HURT) {
        filaMatriz = 20;
    }
    else {
        filaMatriz = static_cast<int>(estadoAnim) * 4 + static_cast<int>(_direccionActual);
    }

    int columna = _frameActual;
    if (_estadoActual == EstadoPersonaje::IDLE) {
        columna = 0;
    }

    // 🌟 Usamos _sprite
    _sprite.setTextureRect(sf::IntRect(columna * 64, filaMatriz * 64, 64, 64));
    _sprite.setOrigin(32.f, 32.f);
}

// ============================================================================
// DIBUJAR: Renderizado en capas ordenadas
// ============================================================================
void Personaje::dibujar(sf::RenderWindow& ventana) {
    if (_estadoActual == EstadoPersonaje::AIMING) {
        ventana.draw(_circuloRango);
    }

    // 🌟 Usamos la función de dibujo de la clase madre para renderizar el sprite
    EntidadViva::dibujar(ventana);

    _bolaDeFuego.dibujar(ventana);
}

void Personaje::dibujarDebug(sf::RenderWindow& ventana) const {
    sf::FloatRect limites = this->getBounds();
    sf::RectangleShape rectDebug(sf::Vector2f(limites.width, limites.height));
    rectDebug.setPosition(limites.left, limites.top);
    rectDebug.setFillColor(sf::Color(0, 255, 0, 100));
    rectDebug.setOutlineColor(sf::Color::Green);
    rectDebug.setOutlineThickness(-1.f);
    ventana.draw(rectDebug);
}

void Personaje::ajustarOrigenSprite(float x, float y) {
    // 🌟 Usamos _sprite
    sf::Vector2f origenActual = _sprite.getOrigin();
    _sprite.setOrigin(origenActual.x + x, origenActual.y + y);
}