#include "Personaje.h"
#include <iostream>
#include "InputManager.h"
#include <cmath> 

// ============================================================================
// CONSTRUCTOR: Inicialización y configuración del indicador de rango
// ============================================================================
Personaje::Personaje() {
    if (!_textura.loadFromFile("assets/maguito_main.png")) {
        std::cerr << "❌ Error: No se pudo cargar la hoja de sprites LPC." << std::endl;
        return;
    }
    _sprite.setTexture(_textura);
    _sprite.setPosition(100.f, 100.f);

	// 🌟 Estadisticas base del personaje heredades de EntidadViva
    _velocidad = 170.f;
    _aceleracion = 10.f; // Que tan rapido alcanza la velocidad máxima
    _desaceleracion = 8.f; // Que tan rapido frena al soltar el movimiento (debe ser mayor que la aceleración para que no se sienta pegajoso)
    _vidaMaxima = 100;
    _vidaActual = _vidaMaxima;
    _danio = 50;
    _cooldownAtaque = 0.5f;

    // 🌟 CONFIGURACIÓN ESTÉTICA DEL ANILLO DE RANGO (Rojo/Celeste transparente)
    _circuloRango.setRadius(_radioAlcance);
    _circuloRango.setFillColor(sf::Color(0, 0, 0, 50));
    _circuloRango.setOutlineColor(sf::Color::White);
    _circuloRango.setOutlineThickness(0.0f);
    _circuloRango.setOrigin(_radioAlcance, _radioAlcance);     // Origen clavado al centro

    actualizarSpriteRect();
}

// ============================================================================
// MANEJAR INPUT: El filtro principal de acciones y movimiento
// ============================================================================
void Personaje::manejarInput(const InputManager& input, Map& mapa, sf::RenderWindow& ventana, bool uiCapturaMouse, float dt) {
    
    // 🌟 1. EL ESCUDO DEL DASH: Si está dasheando, solo calculamos colisiones y SALIMOS.
    if (_estadoActual == EstadoPersonaje::DASH) {
        // Movemos al personaje con la velocidad bestial, chequeando la pared
        resolverColisiones(_velocidadActual * dt, mapa);
        return; // ¡CORTAMOS ACÁ! No dejamos que la física normal lo frene ni que cambie el estado a WALK
    }

    // 2. FILTRO ABSOLUTO: Si está casteando, herido, muerto, o en medio de un DASH, ignoramos el input normal
    if (_estadoActual == EstadoPersonaje::SPELLCAST ||
        _estadoActual == EstadoPersonaje::HURT ||
        _estadoActual == EstadoPersonaje::MUERTO ||
        uiCapturaMouse) return;

    // 3. CONTROL DE COOLDOWN DEL DASH (Lo restamos cada frame)
    if (_cooldownDash > 0.f) _cooldownDash -= dt;

    sf::Vector2f direccion = input.getDireccionMovimiento();

    // 4. EL DISPARADOR DEL DASH (Interceptamos el input antes de la física de inercia)
    if (input.quiereCorrer() && _cooldownDash <= 0.f) {
        // Solo puede dashear si se está intentando mover hacia algún lado
        if (direccion.x != 0.f || direccion.y != 0.f) {
            _estadoActual = EstadoPersonaje::DASH;
            _tiempoDash = _DuracionDash;
            _cooldownDash = 1.5f;
            // Impulso inicial bestial (4 veces la velocidad base, ajustalo a gusto)
            _velocidadActual = direccion * (_velocidad * 4.0f);
            return;
        }
    }

    // ========================================================================
    // TUS FÍSICAS ORIGINALES (Intactas)
    // ========================================================================

    // 🌟 _velocidad viene heredada de EntidadViva
    sf::Vector2f movimiento = direccion * _velocidad;

    // EL MULTIPLICADOR DIAGONAL TRADICIONAL (Aplica el freno matemático exacto)
    if (direccion.x != 0.f && direccion.y != 0.f) {
        movimiento *= 0.7071f;
    }

    // 1. FILTRO DE VELOCIDAD: Aplica aceleración y desaceleración para suavizar el movimiento
    sf::Vector2f velocidadObjetivo = movimiento;

    // 2. FILTRO DE INERCIA: Aplica aceleración para alcanzar la velocidad objetivo...
    if (direccion.x != 0.f || direccion.y != 0.f) {
        // ACELERACIÓN: Nos acercamos fluidamente a la velocidad máxima
        _velocidadActual.x += (velocidadObjetivo.x - _velocidadActual.x) * _aceleracion * dt;
        _velocidadActual.y += (velocidadObjetivo.y - _velocidadActual.y) * _aceleracion * dt;
    }
    else {
        // DESACELERACIÓN: Frenado progresivo hacia el cero absoluto
        _velocidadActual.x += (0.f - _velocidadActual.x) * _desaceleracion * dt;
        _velocidadActual.y += (0.f - _velocidadActual.y) * _desaceleracion * dt;

        // Umbral de corte: Si la velocidad es insignificante, la clavamos en cero para evitar micro-desplazamientos
        if (std::hypot(_velocidadActual.x, _velocidadActual.y) < 10.f) {
            _velocidadActual = { 0.f, 0.f };
        }
    }

    // Actualizamos la mirada y la intención de movimiento de forma inteligente
    determinarEstadoYDireccion(_velocidadActual);
    // Procesamos el intento de apuntar, cancelar o disparar la magia
    procesarHabilidades(input, ventana, uiCapturaMouse);

    sf::Vector2f movimientoEsteFrame = _velocidadActual * dt;
    // 🌟 Ejecutamos las colisiones AABB contra el mapa (Llama a la función de la clase madre)
    resolverColisiones(movimientoEsteFrame, mapa);
}

// ============================================================================
// SUB-FUNCIÓN 1: Decide el estado lógico de movimiento y la mirada
// ============================================================================
void Personaje::determinarEstadoYDireccion(sf::Vector2f direccion) {
    if (direccion.x == 0.f && direccion.y == 0.f) {
        if (_estadoActual == EstadoPersonaje::AIMING) {
            _frameActual = 0;
            _tiempoFrame = 0.f;
        }
        else if (_estadoActual != EstadoPersonaje::IDLE) {
            _estadoActual = EstadoPersonaje::IDLE;
            _frameActual = 9;
            _tiempoFrame = 0.f;
        }
        return;
    }

    if (_estadoActual != EstadoPersonaje::AIMING) {
        _estadoActual = EstadoPersonaje::WALK;
    }

	// ALGORITMO PARA QUE NO MIRE EN DIAGONAL: Comparamos la magnitud del impulso horizontal y vertical para decidir la dirección de la mirada
    if (std::abs(direccion.x) > std::abs(direccion.y)) {
        // El impulso horizontal es mayor, fijamos mirada izquierda o derecha
        _direccionActual = (direccion.x > 0.f) ? DireccionLPC::RIGHT : DireccionLPC::LEFT;
    }
    else {
        // El impulso vertical es mayor o igual, fijamos mirada arriba o abajo
        _direccionActual = (direccion.y > 0.f) ? DireccionLPC::DOWN : DireccionLPC::UP;
    }
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
void Personaje::actualizar(float dt, VisualFX& VisualFX) {
    
    // 1. LÓGICA DEL DASH
    if (_estadoActual == EstadoPersonaje::DASH) {
        _tiempoDash -= dt;
        // Generar rastro (mientras dasheamos)
        // Ajustes: spawn más espaciado, opacidad inicial mayor para que se vea a simple vista
        _relojSpawnRastro += dt;
        if (_relojSpawnRastro >= 0.02f) { // 🌟 Más rápido (cada 0.02s) para que la línea sea continua
            VisualFX.agregarRastro(_sprite, sf::Color(0, 255, 255), 500.f, true);
            _relojSpawnRastro = 0.f;
        }
        if (_tiempoDash <= 0.f) {
            _estadoActual = EstadoPersonaje::IDLE;
            _velocidadActual = { 0.f, 0.f };
        }
    }


    // 2. 💀 CONTROL DE MUERTE
    if (this->estaMuerto()) {
        _estadoActual = EstadoPersonaje::MUERTO;

        if (_frameActual >= 5) {
            _frameActual = 5; // Congelamos en el cuadro del piso
            actualizarSpriteRect();
            _bolaDeFuego.actualizar(dt, VisualFX); // Sincronizamos
            return;
        }
    }

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

        if (_estadoActual == EstadoPersonaje::MUERTO) {
            if (_frameActual < 5) _frameActual++; // Cae al piso cuadro por cuadro
        }
        else {
            _frameActual++;
            controlarLimitesYTransiciones();
        }
    }

    actualizarSpriteRect();
    _bolaDeFuego.actualizar(dt, VisualFX);
}

// ============================================================================
// DIBUJAR: Renderizado en capas ordenadas
// ============================================================================
void Personaje::dibujar(sf::RenderWindow& ventana) {
    // 1. Dibujamos el proyectil de la bola de fuego
    _bolaDeFuego.dibujar(ventana);

    if (_estadoActual == EstadoPersonaje::AIMING) {
        ventana.draw(_circuloRango);
    }
    // 2. Dibujamos al personaje encima de todo
    EntidadViva::dibujar(ventana);
}

// ============================================================================
// CONTROLAR LÍMITES Y TRANSICIONES: Setea los límites de frames de la matriz LPC
// ============================================================================
void Personaje::controlarLimitesYTransiciones() {
    switch (_estadoActual) {

    case EstadoPersonaje::IDLE:
        _maxFrames = 11;
		// Si el frame se pasa de 10, lo clavamos en el 9 (El último de caminata) para que no se vea tan raro el cambio a idle
        if (_frameActual < 9 || _frameActual >= _maxFrames) {
            _frameActual = 9;
        }
        break;

    case EstadoPersonaje::AIMING:
        _maxFrames = 9;
        if (_frameActual >= _maxFrames) _frameActual = 0;
        break;

    case EstadoPersonaje::WALK:
        _maxFrames = 9;
        if (_frameActual >= _maxFrames) _frameActual = 0;
        break;

    case EstadoPersonaje::DASH:
        // Reutilizamos la animación de WALK para el dash (frames idénticos)
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

    case EstadoPersonaje::MUERTO:
        _maxFrames = 6; // Del frame 0 al 5
        if (_frameActual >= _maxFrames) {
            _frameActual = 5; // Clavado en el piso acostado
        }
        break;
    }
}

// ============================================================================
// ACTUALIZAR SPRITE RECT: El encargado matemático del recorte del PNG
// ============================================================================
void Personaje::actualizarSpriteRect() {
    int filaMatriz = 0;

    EstadoPersonaje estadoAnim = _estadoActual;
    // Durante IDLE y AIMING mostramos la animación de WALK (misma fila)
    // También queremos que DASH reutilice la animación de WALK para que el personaje muestre movimiento durante el impulso
    if (_estadoActual == EstadoPersonaje::IDLE || _estadoActual == EstadoPersonaje::AIMING || _estadoActual == EstadoPersonaje::DASH) {
        estadoAnim = EstadoPersonaje::WALK;
    }

    if (estadoAnim == EstadoPersonaje::HURT || estadoAnim == EstadoPersonaje::MUERTO) {
        filaMatriz = 20;
    }
    else {
        filaMatriz = static_cast<int>(estadoAnim) * 4 + static_cast<int>(_direccionActual);
    }


	int columna = _frameActual; // columna recibe directamente el frame dinamico (9 o 10) para hacer la animacion de respiracion


    // 🌟 Usamos _sprite
    _sprite.setTextureRect(sf::IntRect(columna * 64, filaMatriz * 64, 64, 64));
    _sprite.setOrigin(32.f, 32.f);
}
