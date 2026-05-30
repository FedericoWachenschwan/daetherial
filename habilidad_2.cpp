#include "habilidad_2.h"

habilidad_2::habilidad_2() : Habilidades("Habilidad_2", 5, 1.5f, 0.5f) // valores inventados de momento, cambiarian a medida que avanza el juego.
{
}

// activacion de la habilida tomando en cuenta si el cd esta disponible.
// cd seria cooldown, el tiempo de espera de la habilidad.
void habilidad_2::activar(sf::Vector2f inicio, sf::Vector2f objetivo)
{
	if (_cdListo)
	{
		// aca iria la logica del SFML + sprite + colision con objetivo

		_cdListo = false;
		_cdActual = _cdDuracion;
	}

}

// subida de nivel de la habilidad, aumenta las estadisticas mas relevantes dependiendo el nivel.
void habilidad_2::subirNivel()
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