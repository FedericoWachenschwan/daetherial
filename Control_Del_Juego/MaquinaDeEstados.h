#pragma once
#include <SFML/Graphics.hpp>

class GameManager;

///=================================================================///
///   PANTALLA_DEL_JUEGO - PATRON MAQUINA DE ESTADOS: en que
///   pantalla del juego estamos ahora. Reemplaza las 6 clases
///   Estado/EstadoIntro/EstadoMenu/etc por un simple enum. Es como
///   un semaforo: en cada momento solo vale UNA de estas opciones
///=================================================================///
enum class PantallaDelJuego {
    INTRO,
    MENU,
    LORE,
    HISTORIA,
    JUGANDO,
    CREADOR_ITEMS,
    CREDITOS
};

void procesar_eventos_segun_la_pantalla(GameManager& gm, sf::Event& evento);
void actualizar_segun_la_pantalla(GameManager& gm, float tiempo_transcurrido);
void renderizar_segun_la_pantalla(GameManager& gm);