#pragma once
#include <SFML/Graphics.hpp> // tipos graficos de SFML como sf::Event

class GameManager; // declaracion anticipada: evita incluir GameManager.h en este header

///=================================================================///
///   PANTALLA_DEL_JUEGO - PATRON MAQUINA DE ESTADOS: en que
///   pantalla del juego estamos ahora. Reemplaza las 6 clases
///   Estado/EstadoIntro/EstadoMenu/etc por un simple enum. Es como
///   un semaforo: en cada momento solo vale UNA de estas opciones
///=================================================================///
enum class PantallaDelJuego {
    INTRO, // pantalla de animacion introductoria del juego
    MENU, // menu principal con opciones de inicio
    HISTORIA, // secuencia de imagenes con audio al empezar primera partida
    JUGANDO, // pantalla principal del juego en accion
    CREDITOS, // pantalla con los integrantes del grupo
    LOGROS // pantalla que muestra los logros desbloqueados
};

void procesar_eventos_segun_la_pantalla(GameManager& gm, sf::Event& evento); // enruta eventos segun pantalla activa
void actualizar_segun_la_pantalla(GameManager& gm, float tiempo_transcurrido); // actualiza logica segun pantalla activa
void renderizar_segun_la_pantalla(GameManager& gm); // dibuja en pantalla segun pantalla activa
