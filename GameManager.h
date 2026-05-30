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
#include "ItemManager.h"    // 🌟 Nuevo: La fábrica de ítems
#include "ObjectsManager.h" // 🌟 Nuevo: Para manejar el mapa
#include "UI_Inventario.h"  // 🌟 Nuevo: La interfaz del inventario
#include "DebugManager.h"   // 🌟 Nuevo: Para activar el modo debug y mover cosas en caliente

// Estados del juego
enum GameState {
    MENU,
    JUGANDO,
    CREDITOS
};

class GameManager {
private:
	// Debug Manager para controlar el modo debug y mover cosas en caliente
    DebugManager _debug;
    
    // Ventana principal
    sf::RenderWindow _ventana;

    // Cámara
    Camara _camara;

    // Estado actual del juego
    GameState _estado;
	Niebla _niebla; // Capa de niebla para el efecto visual
	sf::Clock _reloj; // Reloj para medir el tiempo entre frames

    // Objetos del juego
    UI_Inventario _hudInventario;
    Menu _menu;
    Map _mapa;
    Personaje _personaje;
    Mascota _mascota;
	Enemy _golem;

    // 🌟 NUEVOS ATRIBUTOS: Tus dos nuevos motores de objetos
    ItemManager _itemManager;
    ObjectsManager _objectsManager;

    // 🔹 Elementos para pantalla de créditos
    sf::Font _fontCreditos;   // Fuente del texto
    sf::Text _textoCreditos;  // Texto a mostrar

    // Métodos del Game Loop
    void procesarEventos();
    void actualizar();
    void renderizar();
   
	// Funcion intermediaria para spawnear un ítem de forma segura (sin colisionar con el personaje, mapa u otros objetos)
    // 🌟 Nuestro nuevo método de Rejection Sampling para loot
    void spawnearDropSeguro(Item* item, const sf::Texture& textura, float startX, float startY);

	// Función para cambiar la música de ambiente según el estado del juego (opcional pero recomendado para mejorar la inmersión)
    void cambiarMusica(GameState nuevoEstado);

    sf::Music _musicaAmbiente;



public:
    GameManager();
    void run();
};