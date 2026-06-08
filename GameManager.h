#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Colisionable.h"
#include "map.h"
#include "menu.h"
#include "Personaje.h"
#include "Mascota.h"
#include "Camara.h"
#include "Niebla.h"
#include "Enemy.h"
#include "ItemManager.h"
#include "ObjectsManager.h"
#include "UI_Inventario.h"
#include "DebugManager.h"
#include "BolaDeFuego.h"
#include "InputManager.h"
#include "Cursor_Visual.h"
#include "UI_CreadorItems.h"
#include "Tienda.h"

enum GameState {
    MENU,
    JUGANDO,
    CREDITOS,
    CREADOR_ITEMS
};

class GameManager {
private:

    UI_CreadorItems _uiCreadorItems;
    InputManager _input;
    DebugManager _debug;
    sf::RenderWindow _ventana;
    Camara _camara;
    GameState _estado;
    Niebla _niebla;
    sf::Clock _reloj;
    UI_Inventario _hudInventario;
    Menu _menu;
    Map _mapa;
    Personaje _personaje;
    Mascota _mascota;
    Enemy* _golem;
    Cursor_Visual _cursor;

    ///=========================
    ///          TIENDA
    ///=========================
    Tienda* _tienda = nullptr; // Inicializado en nullptr para evitar puntero basura

    ItemManager _itemManager;
    ObjectsManager _objectsManager;
    sf::Font _fontCreditos;
    sf::Text _textoCreditos;

    void procesarEventos();
    void actualizar();
    void renderizar();
    void spawnearDropSeguro(Item* item, float startX, float startY);
    void cambiarMusica(GameState nuevoEstado);
    sf::Music _musicaAmbiente;
    void colisionEntreEntidades(EntidadViva& jugador, EntidadViva& enemigo);

public:
    ~GameManager();
    GameManager();
    void run();
};