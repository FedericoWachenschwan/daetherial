#include "Personaje.h"
#include <iostream>
#include "InputManager.h"
#include <cmath> 

Personaje::Personaje() {
    if (!_textura.loadFromFile("assets/maguito_main.png")) {
        std::cerr << "ERROR: NO SE PUDO CARGAR LA HOJA DE SPRITES." << std::endl;
        return;
    }
    _sprite.setTexture(_textura);
    _sprite.setPosition(100.f, 100.f);

    _velocidad = 170.f;
    _aceleracion = 10.f;
    _desaceleracion = 8.f;
    _vidaMaxima = 100;
    _vidaActual = _vidaMaxima;
    _danio = 50;
    _cooldownAtaque = 0.5f;

    _circuloRango.setRadius(_radioAlcance);
    _circuloRango.setFillColor(sf::Color(0, 0, 0, 50));
    _circuloRango.setOutlineColor(sf::Color::White);
    _circuloRango.setOutlineThickness(0.0f);
    _circuloRango.setOrigin(_radioAlcance, _radioAlcance);

    actualizarSpriteRect();
}

///====================================================
///                     ORO DEL JUGADOR
///====================================================
int  Personaje::getOro() const { return _oro; }
void Personaje::setOro(int nuevo_oro_del_jugador) { _oro = nuevo_oro_del_jugador; }

///====================================================
///                     VIDA DEL JUGADOR
///====================================================
int Personaje::getVida() const { return _vidaActual; }
int Personaje::getVidaMaxima() const { return _vidaMaxima; }
void Personaje::setVida(int nueva_vida_del_personaje) {
    if (nueva_vida_del_personaje > _vidaMaxima) {
        _vidaActual = _vidaMaxima; // No puede pasar del máximo
    }
    else {
        _vidaActual = nueva_vida_del_personaje;
    }
}

///====================================================
///                     MANÁ DEL JUGADOR
///====================================================
int Personaje::getMana() const { return _mana_actual; }
int Personaje::getManaMaXima() const { return _mana_maxima; }
void Personaje::setMana(int nuevo_mana_del_personaje) {
    if (nuevo_mana_del_personaje > _mana_maxima) {
        _mana_actual = _mana_maxima; // No puede pasar del máximo
    }
    else if (nuevo_mana_del_personaje < 0) {
        _mana_actual = 0; // No puede bajar de cero
    }
    else {
        _mana_actual = nuevo_mana_del_personaje;
    }
}

void Personaje::manejarInput(const InputManager& input, Map& mapa, sf::RenderWindow& ventana, bool uiCapturaMouse, float dt) {

    if (_estadoActual == EstadoPersonaje::DASH) {
        resolverColisiones(_velocidadActual * dt, mapa);
        return;
    }

    if (_estadoActual == EstadoPersonaje::SPELLCAST ||
        _estadoActual == EstadoPersonaje::HURT ||
        _estadoActual == EstadoPersonaje::MUERTO ||
        uiCapturaMouse) return;

    if (_cooldownDash > 0.f) _cooldownDash -= dt;

    sf::Vector2f direccion = input.getDireccionMovimiento();

    if (input.quiereCorrer() && _cooldownDash <= 0.f) {
        if (direccion.x != 0.f || direccion.y != 0.f) {
            _estadoActual = EstadoPersonaje::DASH;
            _tiempoDash = _DuracionDash;
            _cooldownDash = 1.5f;
            _velocidadActual = direccion * (_velocidad * 4.0f);
            return;
        }
    }

    sf::Vector2f movimiento = direccion * _velocidad;

    if (direccion.x != 0.f && direccion.y != 0.f) {
        movimiento *= 0.7071f;
    }

    sf::Vector2f velocidadObjetivo = movimiento;

    if (direccion.x != 0.f || direccion.y != 0.f) {
        _velocidadActual.x += (velocidadObjetivo.x - _velocidadActual.x) * _aceleracion * dt;
        _velocidadActual.y += (velocidadObjetivo.y - _velocidadActual.y) * _aceleracion * dt;
    }
    else {
        _velocidadActual.x += (0.f - _velocidadActual.x) * _desaceleracion * dt;
        _velocidadActual.y += (0.f - _velocidadActual.y) * _desaceleracion * dt;

        if (std::hypot(_velocidadActual.x, _velocidadActual.y) < 10.f) {
            _velocidadActual = { 0.f, 0.f };
        }
    }

    determinarEstadoYDireccion(_velocidadActual);
    procesarHabilidades(input, ventana, uiCapturaMouse);

    sf::Vector2f movimientoEsteFrame = _velocidadActual * dt;
    resolverColisiones(movimientoEsteFrame, mapa);
}

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

    if (std::abs(direccion.x) > std::abs(direccion.y)) {
        _direccionActual = (direccion.x > 0.f) ? DireccionLPC::RIGHT : DireccionLPC::LEFT;
    }
    else {
        _direccionActual = (direccion.y > 0.f) ? DireccionLPC::DOWN : DireccionLPC::UP;
    }
}

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

        ///=========================================================///
        ///   CHEQUEO DE MANÁ - Si no tiene suficiente no puede lanzar
        ///=========================================================///
        int costo_de_mana_por_hechizo = 20; // Cada hechizo cuesta 20 de maná

        if (_mana_actual < costo_de_mana_por_hechizo) {
            std::cout << "NO TENES MANA SUFICIENTE PARA LANZAR EL HECHIZO. MANA ACTUAL: " << _mana_actual << std::endl;
            return; // Sale sin lanzar el hechizo ni cambiar el estado
        }

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

        _mana_actual = _mana_actual - costo_de_mana_por_hechizo; // Restamos el maná al lanzar
        _bolaDeFuego.activar(posPersonaje, mouseMundo, _radioAlcance);
    }
}

void Personaje::actualizar(float dt, VisualFX& VisualFX) {

    if (_estadoActual == EstadoPersonaje::DASH) {
        _tiempoDash -= dt;
        _relojSpawnRastro += dt;
        if (_relojSpawnRastro >= 0.02f) {
            VisualFX.agregarRastro(_sprite, sf::Color(0, 255, 255), 500.f, true);
            _relojSpawnRastro = 0.f;
        }
        if (_tiempoDash <= 0.f) {
            _estadoActual = EstadoPersonaje::IDLE;
            _velocidadActual = { 0.f, 0.f };
        }
    }

    if (this->estaMuerto()) {
        _estadoActual = EstadoPersonaje::MUERTO;
        if (_frameActual >= 5) {
            _frameActual = 5;
            actualizarSpriteRect();
            _bolaDeFuego.actualizar(dt, VisualFX);
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
            if (_frameActual < 5) _frameActual++;
        }
        else {
            _frameActual++;
            controlarLimitesYTransiciones();
        }
    }

    actualizarSpriteRect();
    _bolaDeFuego.actualizar(dt, VisualFX);
}

void Personaje::dibujar(sf::RenderWindow& ventana) {
    _bolaDeFuego.dibujar(ventana);
    if (_estadoActual == EstadoPersonaje::AIMING) {
        ventana.draw(_circuloRango);
    }
    EntidadViva::dibujar(ventana);
}

void Personaje::controlarLimitesYTransiciones() {
    switch (_estadoActual) {
    case EstadoPersonaje::IDLE:
        _maxFrames = 11;
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
        _maxFrames = 6;
        if (_frameActual >= _maxFrames) _frameActual = 5;
        break;
    }
}

void Personaje::actualizarSpriteRect() {
    int filaMatriz = 0;
    EstadoPersonaje estadoAnim = _estadoActual;

    if (_estadoActual == EstadoPersonaje::IDLE ||
        _estadoActual == EstadoPersonaje::AIMING ||
        _estadoActual == EstadoPersonaje::DASH) {
        estadoAnim = EstadoPersonaje::WALK;
    }

    if (estadoAnim == EstadoPersonaje::HURT || estadoAnim == EstadoPersonaje::MUERTO) {
        filaMatriz = 20;
    }
    else {
        filaMatriz = static_cast<int>(estadoAnim) * 4 + static_cast<int>(_direccionActual);
    }

    int columna = _frameActual;
    _sprite.setTextureRect(sf::IntRect(columna * 64, filaMatriz * 64, 64, 64));
    _sprite.setOrigin(32.f, 32.f);
}