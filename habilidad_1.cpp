#include "habilidad_1.h"
#include <cmath>
#include <iostream>

habilidad_1::habilidad_1() : Habilidades("Bola de fuego", 5, 10.0f, 0.5f) // valores inventados de momento, cambiarian a medida que avanza el juego.
{
	// Estado inicial del proyectil: inactivo y sin distancia recorrida
	_activo = false;                // Flag: no hay proyectil en pantalla
	_velocidad = 400.0f;            // Velocidad en píxeles/segundo
	_distanciaRecorrida = 0.0f;     // Acumulador de distancia para comparar con el rango

	// Carga de la textura del proyectil y configuración del sprite
	if (!_textura.loadFromFile("assets/habilidades/Fire_Spell_Frame_01.png"))
	{
		// Si falla la carga, lo dejamos registrado en la consola
		std::cerr << "Error al cargar la textura de la habilidad 1" << std::endl;
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

// activacion de la habilida tomando en cuenta si el cd esta disponible.
// cd seria cooldown, el tiempo de espera de la habilidad.
void habilidad_1::activar(sf::Vector2f inicio, sf::Vector2f objetivo)
{
	if (_cdListo && !_activo)
	{
		// Posicionamos el origen del proyectil en la posición del personaje (inicio)
		_sprite.setPosition(inicio);
		_distanciaRecorrida = 0.0f; // reiniciamos el contador de distancia

		// Calculamos vector hacia el objetivo y lo normalizamos para obtener dirección
		float diffX = objetivo.x - inicio.x;
		float diffY = objetivo.y - inicio.y;
		float distanciaReal = std::sqrt(diffX * diffX + diffY * diffY);

		if (distanciaReal != 0)
		{
			// Dirección normalizada: magnitud = 1, solo indica sentido
			_direccion.x = diffX / distanciaReal;
			_direccion.y = diffY / distanciaReal;
		}
		else
		{
			// Evitar división por cero: en caso de apuntar exactamente al mismo punto
			_direccion.x = 0;
			_direccion.y = 0;
		}

		// Rotamos el sprite para que apunte en la dirección de movimiento (solo efecto visual)
		float angulo = std::atan2(diffY, diffX) * 180.f / 3.14159f;
		_sprite.setRotation(angulo);

		// Activamos el proyectil y disparamos el cooldown
		_activo = true;        // El proyectil pasará a moverse en actualizar()
		_cdListo = false;      // Marcamos que ya no está listo hasta que el cooldown termine
		_cdActual = _cdDuracion; // Reiniciamos el temporizador del cooldown
	}

}

void habilidad_1::actualizar(float deltaTime)
{
	// Actualizamos el cooldown cada frame para que el tiempo restante se reduzca
	cdActualizar(deltaTime);

	if (_activo)
	{
		// Distancia que avanza el proyectil en este frame (píxeles)
		float avance = _velocidad * deltaTime;

		// Movemos el sprite en dirección normalizada multiplicada por la distancia de avance
		_sprite.move(_direccion.x * avance, _direccion.y * avance);

		// Acumulamos la distancia para compararla con el rango máximo
		_distanciaRecorrida += avance;

		// El rango de la habilidad se almacena en unidades de juego; aquí multiplicamos por 100
		// (esto es una convención del proyecto; ajustar si es necesario)
		if (_distanciaRecorrida >= (_rango * 100.f))
		{
			// Cuando alcanza el rango, desactivamos el proyectil para que deje de dibujarse y moverse
			_activo = false;
		}
	}
}

void habilidad_1::dibujar(sf::RenderWindow& ventana)
{
	if (_activo)
	{
		ventana.draw(_sprite);
	}
}

// subida de nivel de la habilidad, aumenta las estadisticas mas relevantes dependiendo el nivel.
void habilidad_1::subirNivel()
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