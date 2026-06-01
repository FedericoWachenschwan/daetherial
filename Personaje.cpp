#include "Personaje.h"
#include <iostream>
#include "InputManager.h"
#include <cmath> 

// ============================================================================
// CONSTRUCTOR: Inicialización y configuración del indicador de rango
// ============================================================================
Personaje::Personaje() {
    if (!textura_completa_lpc.loadFromFile("assets/maguito_main.png")) {
        std::cerr << "❌ Error: No se pudo cargar la hoja de sprites LPC." << std::endl;
        return;
    }
    sprite_del_personaje.setTexture(textura_completa_lpc);
    sprite_del_personaje.setPosition(100.f, 100.f);

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
    sf::Vector2f movimiento = direccion * velocidad;

    // EL MULTIPLICADOR DIAGONAL TRADICIONAL (Aplica el freno matemático exacto)
    if (direccion.x != 0.f && direccion.y != 0.f) {
        movimiento *= 0.7071f;
    }

    // Actualizamos la mirada y la intención de movimiento de forma inteligente
    determinarEstadoYDireccion(direccion);


    // Procesamos el intento de apuntar, cancelar o disparar la magia
    procesarHabilidades(input, ventana, uiCapturaMouse);

    // Ejecutamos las colisiones AABB contra el mapa
    resolverColisiones(movimiento, mapa);
}

// ============================================================================
// SUB-FUNCIÓN 1: Decide el estado lógico de movimiento y la mirada
// ============================================================================
void Personaje::determinarEstadoYDireccion(sf::Vector2f direccion) {
    // Si no hay inputs de movimiento retenidos...
    if (direccion.x == 0.f && direccion.y == 0.f) {
        // Si venía apuntando y se frena, mantenemos el estado AIMING activo
        if (_estadoActual != EstadoPersonaje::AIMING) {
            _estadoActual = EstadoPersonaje::IDLE;
        }
        else {
            // 🌟 FIX: Si está apuntando pero NO se mueve, congelamos la animación
            _frameActual = 0;
            _tiempoFrame = 0.f;
        }
        return;
    }

    // Si hay movimiento y no venía apuntando, es una caminata normal
    if (_estadoActual != EstadoPersonaje::AIMING) {
        _estadoActual = EstadoPersonaje::WALK;
    }

    // Mapeo clásico de hacia dónde miran los ojos del maguito según las teclas
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

    // 1. INTERRUPTOR DE APUNTADO: Presionar la tecla conmuta entre encender y apagar el rango
    if (input.quiereSaltar()) {
        if (_estadoActual == EstadoPersonaje::AIMING) {
            _estadoActual = EstadoPersonaje::IDLE; // Cancelación voluntaria
            _frameActual = 0;
        }
        else {
            _estadoActual = EstadoPersonaje::AIMING; // Encendido del anillo
            _frameActual = 0;
        }
    }

    // 2. CONFIRMAR DISPARO: Clic izquierdo mientras apunta
    if (_estadoActual == EstadoPersonaje::AIMING && input.quiereAtacar()) {
        _estadoActual = EstadoPersonaje::SPELLCAST;
        _frameActual = 0;
        _tiempoFrame = 0.f;

        // 🌟 CORRECCIÓN DEL DESFASE: Traduce los píxeles de pantalla a coordenadas de la View del mapa
        sf::Vector2i mousePantalla = input.getPosicionMouse();
        sf::Vector2f mouseMundo = ventana.mapPixelToCoords(mousePantalla);

        // Volteamos al personaje hacia donde hizo clic para que tire la magia de frente
        sf::Vector2f posPersonaje = this->getPosicion();
        if (std::abs(mouseMundo.x - posPersonaje.x) > std::abs(mouseMundo.y - posPersonaje.y)) {
            _direccionActual = (mouseMundo.x > posPersonaje.x) ? DireccionLPC::RIGHT : DireccionLPC::LEFT;
        }
        else {
            _direccionActual = (mouseMundo.y > posPersonaje.y) ? DireccionLPC::DOWN : DireccionLPC::UP;
        }

        // 🌟 LOGICA POLIMÓRFICA FINAL: Se lanza con los 3 parámetros de la arquitectura sólida
        _bolaDeFuego.activar(posPersonaje, mouseMundo, _radioAlcance);
    }
}

// ============================================================================
// SUB-FUNCIÓN 3: Físicas de colisión AABB por ejes y Pixel Snapping
// ============================================================================
void Personaje::resolverColisiones(sf::Vector2f movimiento, Map& mapa) {
    // 🧱 EJE X
    if (movimiento.x != 0.f) {
        sprite_del_personaje.move(movimiento.x, 0.f);

        for (const auto& bloque : mapa.getBloquesSolidos()) {
            if (this->chequearColision(bloque)) {
                // 🌟 FIX: Rollback y redondeo SOLO si hay colisión
                sprite_del_personaje.move(-movimiento.x, 0.f);
                sf::Vector2f posActual = sprite_del_personaje.getPosition();
                sprite_del_personaje.setPosition(std::round(posActual.x), posActual.y);
                break;
            }
        }
    }

    // 🧱 EJE Y
    if (movimiento.y != 0.f) {
        sprite_del_personaje.move(0.f, movimiento.y);

        for (const auto& bloque : mapa.getBloquesSolidos()) {
            if (this->chequearColision(bloque)) {
                // 🌟 FIX: Rollback y redondeo SOLO si hay colisión
                sprite_del_personaje.move(0.f, -movimiento.y);
                sf::Vector2f posActual = sprite_del_personaje.getPosition();
                sprite_del_personaje.setPosition(posActual.x, std::round(posActual.y));
                break;
            }
        }
    }
}

// ============================================================================
// ACTUALIZAR: El motor temporal de los relojes de animación y lógicas hijas
// ============================================================================
void Personaje::actualizar(float dt) {
    // Si está apuntando, el círculo visual se clava abajo de sus pies en tiempo real
    if (_estadoActual == EstadoPersonaje::AIMING) {
        _circuloRango.setPosition(this->getPosicion());
    }

    // Regulamos el tiempo de refresco de los frames según la acción
    float limiteTiempoFrame = _velocidadAnimacion;
    if (_estadoActual == EstadoPersonaje::IDLE) {
        limiteTiempoFrame = 0.5f; // Medio segundo de respiración en IDLE
    }

    _tiempoFrame += dt;
    if (_tiempoFrame >= limiteTiempoFrame) {
        _tiempoFrame = 0.f;
        _frameActual++;
        controlarLimitesYTransiciones();
    }

    actualizarSpriteRect(); // Recorta la textura del PNG que corresponda
    _bolaDeFuego.actualizar(dt); // Actualiza la física y cooldown de la magia
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
        // Permite un ciclo de hasta 9 frames para mover los pies si patrullás apuntando
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
            _estadoActual = EstadoPersonaje::IDLE; // Vuelve a la normalidad al terminar de lanzar
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

    // IDLE y AIMING se paran físicamente sobre la fila visual de caminata (WALK)
    EstadoPersonaje estadoAnim = _estadoActual;
    if (_estadoActual == EstadoPersonaje::IDLE || _estadoActual == EstadoPersonaje::AIMING) {
        estadoAnim = EstadoPersonaje::WALK;
    }

    if (estadoAnim == EstadoPersonaje::HURT) {
        filaMatriz = 20; // Fila única de muerte
    }
    else {
        // FÓRMULA MAESTRA LPC: (ID de Acción * 4) + ID de Dirección
        filaMatriz = static_cast<int>(estadoAnim) * 4 + static_cast<int>(_direccionActual);
    }

    // Forzamos el frame 0 estático únicamente si el mago está verdaderamente quieto en IDLE
    int columna = _frameActual;
    if (_estadoActual == EstadoPersonaje::IDLE) {
        columna = 0;
    }

    sprite_del_personaje.setTextureRect(sf::IntRect(columna * 64, filaMatriz * 64, 64, 64));
    sprite_del_personaje.setOrigin(32.f, 32.f); // Mantiene el eje centrado en el medio del cuerpo
}

sf::Vector2f Personaje::getPosicion() const { return sprite_del_personaje.getPosition(); }

// ============================================================================
// DIBUJAR: Renderizado en capas ordenadas
// ============================================================================
void Personaje::dibujar(sf::RenderWindow& ventana) {
    // 🎯 CAPA 1 (Fondo): El anillo se estampa primero para que quede abajo de los pies
    if (_estadoActual == EstadoPersonaje::AIMING) {
        ventana.draw(_circuloRango);
    }

    // CAPA 2 (Medio): El sprite del personaje
    ventana.draw(sprite_del_personaje);

    // CAPA 3 (Frente): Los proyectiles mágicos activos
    _bolaDeFuego.dibujar(ventana);
}

void Personaje::dibujarDebug(sf::RenderWindow& ventana) {
    sf::FloatRect limites = this->getBounds();
    sf::RectangleShape rectDebug(sf::Vector2f(limites.width, limites.height));
    rectDebug.setPosition(limites.left, limites.top);
    rectDebug.setFillColor(sf::Color(0, 255, 0, 100));
    rectDebug.setOutlineColor(sf::Color::Green);
    rectDebug.setOutlineThickness(-1.f);
    ventana.draw(rectDebug);
}

void Personaje::ajustarOrigenSprite(float x, float y) {
    sf::Vector2f origenActual = sprite_del_personaje.getOrigin();
    sprite_del_personaje.setOrigin(origenActual.x + x, origenActual.y + y);
}