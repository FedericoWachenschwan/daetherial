#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "MaquinaDeEstados.h"
#include "map.h"
#include "menu.h"
#include "Personaje.h"
#include "Mascota.h"
#include "Camara.h"
#include "Niebla.h"
#include "Golem.h"
#include "Enemigo.h"
#include "Item.h"
#include "FabricaDeItems.h"
#include "AdministradorDeObjetos.h"
#include "UI_Inventario.h"
#include "DebugManager.h"
#include "InputManager.h"
#include "Cursor_Visual.h"
#include "UI_CreadorItems.h"
#include "VisualFX.h"
#include "Tienda.h"

///=================================================================///
///   GAME_MANAGER - Guarda TODAS las piezas del juego en un solo
///   lugar. Las funciones de MaquinaDeEstados.cpp leen y escriben
///   estas variables directamente segun la pantalla actual
///=================================================================///
class GameManager {
public:

    static const int CANTIDAD_MAXIMA_DE_MARCIANITOS = 30;

    ///=============================================================///
    ///   VENTANA, TIEMPO Y PANTALLA ACTUAL
    ///=============================================================///
    sf::RenderWindow ventana;
    sf::Clock reloj;
    PantallaDelJuego pantalla_actual;

    ///=============================================================///
    ///   ENTRADA Y HERRAMIENTAS DE DEBUG
    ///=============================================================///
    InputManager input;
    DebugManager debug;
    Cursor_Visual cursor;

    ///=============================================================///
    ///   MUNDO
    ///=============================================================///
    Camara camara;
    Map mapa;
    Niebla niebla;
    VisualFX efectos_visuales;

    ///=============================================================///
    ///   PERSONAJES VIVOS
    ///=============================================================///
    Personaje personaje;
    Mascota mascota;
    Golem golem;

    ///=============================================================///
    ///   POOL DE MARCIANITOS (OBJECT POOL)
    ///=============================================================///
    Enemigo marcianitos[CANTIDAD_MAXIMA_DE_MARCIANITOS];
    float reloj_de_spawn_de_marcianitos;
    float intervalo_de_spawn_de_marcianitos;

    ///=============================================================///
    ///   OBJETOS, ITEMS Y TIENDA
    ///=============================================================///
    FabricaDeItems fabrica_de_items;
    AdministradorDeObjetos administrador_de_objetos;
    Tienda tienda;

    ///=============================================================///
    ///   INTERFAZ
    ///=============================================================///
    UI_Inventario hud_inventario;
    UI_CreadorItems ui_creador_items;
    Menu menu;

    ///=============================================================///
    ///   TEXTOS Y FUENTES
    ///=============================================================///
    sf::Font fuente_de_textos;
    sf::Text texto_de_creditos;
    sf::Text texto_de_lore;

    ///=============================================================///
    ///   ANIMACION DE LA INTRO
    ///=============================================================///
    sf::Texture textura_de_la_intro;
    sf::Sprite sprite_de_la_intro;
    int frame_actual_de_la_intro;
    float tiempo_acumulado_del_frame_de_intro;

    ///=============================================================///
    ///   HISTORIA - Imagenes con audio al comenzar la primera partida
    ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_IMAGENES_DE_HISTORIA = 20;
    sf::Texture texturas_de_la_historia[CANTIDAD_MAXIMA_DE_IMAGENES_DE_HISTORIA];
    sf::Sprite sprite_de_la_historia;
    sf::Music musica_de_la_historia;
    int cantidad_de_imagenes_de_la_historia;
    int imagen_actual_de_la_historia;
    float tiempo_acumulado_en_la_historia;
    bool el_audio_de_la_historia_termino;
    bool la_musica_de_la_historia_se_cargo;
    bool la_historia_ya_empezo_a_reproducirse;
    bool es_la_primera_partida;

    ///=============================================================///
    ///   MUSICA
    ///=============================================================///
    sf::Music musica_ambiente;

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    GameManager();

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    void cambiar_musica(int id_de_la_musica);
    void spawnear_drop_seguro(Item item_a_soltar, float posicion_x, float posicion_y);
    void resolver_colision_entre_personaje_y_golem();
};

///=================================================================///
///   FUNCION SUELTA - Maneja el bucle principal del juego completo
///=================================================================///
void ejecutar(GameManager& gm);