#include "BolaDeFuego.h"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "VisualFX.h"

// ============================================================================
// CONSTRUCTOR: Inicializa las estadísticas base de la Bola de Fuego
// ============================================================================
BolaDeFuego::BolaDeFuego() : Habilidades("Bola de fuego", 5, 10.0f, 0.5f) // Nombre, daño, rango, cooldown
{

	_activo = false;                // El proyectil inicia apagado
	_velocidad = 400.0f;            // Velocidad en píxeles/segundo
	_distanciaRecorrida = 0.0f;     // Contador de distancia acumulada

	// Carga de la textura del proyectil y configuración del sprite
	if (!_textura.loadFromFile("assets/habilidades/Fire_Spell_Frame_01.png"))
	{
		std::cerr << "❌ Error: No se pudo cargar la textura de BolaDeFuego." << std::endl;
	}
	else
	{
		// Asociamos la textura al sprite y centramos el origen para rotarlo correctamente
		_sprite.setTexture(_textura);
		_sprite.setOrigin(_textura.getSize().x / 2.0f, _textura.getSize().y / 2.0f);

		// Escalamos el sprite a un tamaño visual coherente (48 px de ancho)
		float _tamanioHabilidad = 48.0f;
		float _factorEscala = _tamanioHabilidad / _textura.getSize().x;
		_sprite.setScale(_factorEscala, _factorEscala);
	}
}

// ============================================================================
// ACTIVAR: Inicializa el proyectil en el mundo apuntando al cursor
// ============================================================================
void BolaDeFuego::activar(sf::Vector2f inicio, sf::Vector2f objetivo, float rangoPixeles)
{
	if (_cdListo && !_activo)
	{

		_sprite.setPosition(inicio);
		_distanciaRecorrida = 0.0f; // Reiniciamos el contador de distancia
        _rango = rangoPixeles;     // Actualizamos el rango dinámico según el personaje



		float diffX = objetivo.x - inicio.x;
		float diffY = objetivo.y - inicio.y;
		
        // Calculamos el vector hacia el objetivo y lo normalizamos
		float distanciaReal = std::hypot(diffX, diffY);

        if (distanciaReal > 0.001f)
		{
			_direccion.x = diffX / distanciaReal;
			_direccion.y = diffY / distanciaReal;
		}
		else
		{
			// Disparo por defecto a la derecha si hace clic en su propio centro
			_direccion.x = 1.0f;
			_direccion.y = 0.0f;
		}

		float angulo = std::atan2(diffY, diffX) * 180.f / 3.14159f;
		_sprite.setRotation(angulo);

		_activo = true;
		_cdListo = false;
		_cdActual = _cdDuracion;
	}
}

// ============================================================================
// ACTUALIZAR: Mueve el proyectil y gestiona sus ciclos de vida y cooldowns
// ============================================================================
void BolaDeFuego::actualizar(float dt, VisualFX& vfx) {

	// 0. GESTIÓN DEL COOLDOWN (Para poder volver a disparar)
	if (!_cdListo) {
		_cdActual -= dt;
		if (_cdActual <= 0.f) {
			_cdActual = 0.f;
			_cdListo = true;
		}
	}

	// 1. LÓGICA DE LA BOLA (Solo si está activa/volando)
	if (_activo) {

		// A) Emitir partículas de rastro
		_relojSpawnRastro += dt;
		if (_relojSpawnRastro >= 0.015f) {
			vfx.agregarRastro(_sprite, sf::Color(255, 120, 0), 600.f, true);
			float dispersionY = (rand() % 100 - 50) * 2.f; // entre 100 y -100
			sf::Vector2f velChispa = -_direccion * (_velocidad * 0.4f) + sf::Vector2f(0.f, dispersionY);
			vfx.agregarParticulaDinamica(*_sprite.getTexture(), _sprite.getPosition(), velChispa, sf::Color(255, 200, 0), 800.f, true);
			_relojSpawnRastro = 0.f;
		}
		// B) Mover la bola físicamente
		float pasoEfectivo = _velocidad * dt;
		_sprite.move(_direccion * pasoEfectivo);
		_distanciaRecorrida += pasoEfectivo;
		// C) Destruir la bola si superó su rango máximo (no le pegó a nada)
		if (_distanciaRecorrida >= _rango) {
			desactivar(); // O simplemente _activo = false;
		}
	}
}

// ============================================================================
// DIBUJAR: Renderiza el sprite en la ventana de juego
// ============================================================================
void BolaDeFuego::dibujar(sf::RenderWindow& ventana) {
	
	// 2. Depsues dibujamos el sprite encima (si esta activa
	if (_activo) {
		ventana.draw(_sprite, sf::BlendAdd);
	}
}


// ============================================================================
// SUBIR NIVEL: Incrementa las estadísticas base de la magia
// ============================================================================
void BolaDeFuego::subirNivel()
{
	Habilidades::subirNivel();

	switch (_nivel)
	{
	case 2:
		_danio += 5;
		break;
	case 3:
		_danio += 5;
		_rango += 0.5f;
		break;
	case 4:
		_danio += 5;
		_cdDuracion -= 0.1f;
		break;
	case 5:
		_danio += 10;
		_rango += 1.0f;
		_cdDuracion -= 0.3f;
		break;
	default:
		break;
	}
}