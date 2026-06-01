#include "BolaDeFuego.h"
#include <cmath>
#include <iostream>

// ============================================================================
// CONSTRUCTOR: Inicializa las estadísticas base de la Bola de Fuego
// ============================================================================
BolaDeFuego::BolaDeFuego() : Habilidades("Bola de fuego", 5, 10.0f, 0.5f) // Nombre, daño, rango, cooldown
{
	// Estado inicial del proyectil: inactivo y sin distancia recorrida
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
		// Posicionamos el origen del proyectil en la posición del personaje
		_sprite.setPosition(inicio);
		_distanciaRecorrida = 0.0f; // Reiniciamos el contador de distancia
		_rango = rangoPixeles; // Actualizamos el rango dinámico según el personaje


		// Calculamos el vector hacia el objetivo y lo normalizamos
		float diffX = objetivo.x - inicio.x;
		float diffY = objetivo.y - inicio.y;
		
		// 🌟 FIX 1: std::hypot evita la pérdida de precisión al hacer zoom
		float distanciaReal = std::hypot(diffX, diffY);

		if (distanciaReal > 0.001f) // 🌟 FIX 2: Tolerancia mínima para evitar bugs si el mouse está pegado al mago
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
void BolaDeFuego::actualizar(float deltaTime)
{
	// Reducimos el temporizador del cooldown frame a frame
	cdActualizar(deltaTime);

	if (_activo)
	{
		// Espacio que debe avanzar el proyectil en este frame
		float avance = _velocidad * deltaTime;

		// Desplazamos el sprite en el espacio bidimensional
		_sprite.move(_direccion.x * avance, _direccion.y * avance);

		// Acumulamos la distancia total que se alejó desde el origen
		_distanciaRecorrida += avance;

		// Si alcanza el rango límite de la habilidad, la apagamos
		if (_distanciaRecorrida >= (_rango))
		{
			_activo = false;
		}
	}
}

// ============================================================================
// DIBUJAR: Renderiza el sprite en la ventana de juego
// ============================================================================
void BolaDeFuego::dibujar(sf::RenderWindow& ventana)
{
	if (_activo)
	{
		ventana.draw(_sprite);
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