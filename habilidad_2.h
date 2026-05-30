#pragma once
#include "habilidades.h"

class habilidad_2 : public Habilidades
{
private:
public:
	habilidad_2(); // constructor.

	// override sobreescribe los metodos heredados Habilidades (padre).
	void activar(sf::Vector2f inicio, sf::Vector2f objetivo) override; // activamos la habilidad .
	void subirNivel() override; // subida de nivel de la habilidad.
};