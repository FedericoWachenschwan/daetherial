#pragma once
#include <SFML/Graphics.hpp> // libreria grafica de SFML
#include <SFML/Audio.hpp> // libreria de audio de SFML
#include <memory> // para usar punteros inteligentes (unique_ptr)
#include <fstream> // para leer y escribir archivos
#include "MaquinaDeEstados.h" // enum con las pantallas del juego
#include "map.h" // clase del mapa y colisiones
#include "menu.h" // clase del menu principal
#include "Personaje.h" // clase del personaje jugable
#include "Mascota.h" // clase de la mascota del jugador
#include "Camara.h" // clase que sigue al personaje con la camara
#include "Niebla.h" // efecto de niebla de guerra
#include "Golem.h" // clase del jefe final (Golem)
#include "Enemigo.h" // clase de los enemigos normales
#include "Item.h" // clase de los objetos del mundo
#include "AdministradorDeObjetos.h" // gestiona los items en el piso
#include "UI_Inventario.h" // interfaz grafica del inventario
#include "DebugManager.h" // herramientas de depuracion visual
#include "InputManager.h" // captura teclado y mouse
#include "Cursor_Visual.h" // cursor personalizado en pantalla
#include "VisualFX.h" // efectos visuales del juego
#include "Tienda.h" // clase de la tienda del juego

///=================================================================///
///   LOGRO - Un logro individual: tiene nombre y si fue
///   desbloqueado en esta sesion de juego
///=================================================================///
struct Logro {
    char nombre[64]; // texto descriptivo del logro
    bool desbloqueado; // true si el jugador ya lo consiguio
};

///=================================================================///
///   GAME_MANAGER - Guarda TODAS las piezas del juego en un solo
///   lugar. Las funciones de MaquinaDeEstados.cpp leen y escriben
///   estas variables directamente segun la pantalla actual
///=================================================================///
class GameManager {
public:

    static const int CANTIDAD_MAXIMA_DE_MARCIANITOS = 30; // tope de enemigos simultaneos en el pool
    static const int CANTIDAD_MAXIMA_DE_LOGROS = 5; // cuantos logros tiene el juego

 ///=============================================================///
 ///   VENTANA, TIEMPO Y PANTALLA ACTUAL
 ///=============================================================///
    sf::RenderWindow ventana; // ventana donde se dibuja todo el juego
    sf::Clock reloj; // mide el tiempo entre cada frame
    PantallaDelJuego pantalla_actual; // indica en que pantalla estamos ahora

 ///=============================================================///
 ///   ENTRADA Y HERRAMIENTAS DE DEBUG
 ///=============================================================///
    InputManager input; // gestiona la entrada del teclado y el mouse
    DebugManager debug; // herramienta para mostrar informacion de depuracion
    Cursor_Visual cursor; // cursor grafico personalizado del juego

 ///=============================================================///
 ///   MUNDO
 ///=============================================================///
    Camara camara; // camara que sigue al personaje
    Map mapa; // mapa con tiles y colisiones
    Niebla niebla; // efecto de niebla que oculta zonas no visitadas
    VisualFX efectos_visuales; // efectos visuales como particulas o destellos

 ///=============================================================///
 ///   PERSONAJES VIVOS
 ///=============================================================///
    Personaje personaje; // el heroe que controla el jugador
    Mascota mascota; // la mascota que acompana al personaje
    Golem golem; // el jefe final del juego

 ///=============================================================///
 ///   POOL DE MARCIANITOS (OBJECT POOL)
 ///=============================================================///
    Enemigo marcianitos[CANTIDAD_MAXIMA_DE_MARCIANITOS]; // arreglo fijo de enemigos reutilizables
    float reloj_de_spawn_de_marcianitos; // tiempo acumulado desde el ultimo spawn
    float intervalo_de_spawn_de_marcianitos; // cada cuantos segundos aparece un enemigo nuevo

 ///=============================================================///
 ///   OBJETOS, ITEMS Y TIENDA
 ///=============================================================///
    AdministradorDeObjetos administrador_de_objetos; // gestiona items tirados en el suelo
    Tienda tienda; // tienda donde el jugador compra items

 ///=============================================================///
 ///   INTERFAZ
 ///=============================================================///
    UI_Inventario hud_inventario; // panel visual del inventario del jugador
    Menu menu; // menu principal del juego

 ///=============================================================///
 ///   FUENTES
 ///=============================================================///
    sf::Font fuente_de_textos; // fuente tipografica cargada desde archivo

 ///=============================================================///
 ///   CREDITOS
 ///=============================================================///
    sf::Text texto_de_creditos; // texto con los creditos del juego

 ///=============================================================///
 ///   ANIMACION DE LA INTRO
 ///=============================================================///
    sf::Texture textura_de_la_intro; // spritesheet con los frames de la intro
    sf::Sprite sprite_de_la_intro; // sprite que muestra el frame actual de la intro
    int frame_actual_de_la_intro; // indice del frame que se esta mostrando
    float tiempo_acumulado_del_frame_de_intro; // tiempo transcurrido en el frame actual

 ///=============================================================///
 ///   HISTORIA - Imagenes con audio al comenzar la primera partida
 ///=============================================================///
    static const int CANTIDAD_MAXIMA_DE_IMAGENES_DE_HISTORIA = 20; // maximo de imagenes de historia soportadas
    sf::Texture texturas_de_la_historia[CANTIDAD_MAXIMA_DE_IMAGENES_DE_HISTORIA]; // imagenes de la historia
    sf::Sprite sprite_de_la_historia; // sprite que muestra la imagen actual de la historia
    sf::Music musica_de_la_historia; // audio que suena durante la historia
    int cantidad_de_imagenes_de_la_historia; // cuantas imagenes se cargaron exitosamente
    int imagen_actual_de_la_historia; // indice de la imagen que se esta mostrando
    float tiempo_acumulado_en_la_historia; // tiempo transcurrido en la imagen actual
    bool el_audio_de_la_historia_termino; // true cuando el audio de la historia llego al final
    bool la_musica_de_la_historia_se_cargo; // true si el archivo de audio se cargo correctamente
    bool la_historia_ya_empezo_a_reproducirse; // true si la historia ya comenzo a reproducirse
    bool es_la_primera_partida; // true si el jugador nunca termino el juego antes

 ///=============================================================///
 ///   MUSICA
 ///=============================================================///
    sf::Music musica_ambiente; // musica de fondo que suena segun la pantalla activa

 ///=============================================================///
 ///   LOGROS Y ESTADISTICAS DE PARTIDA
 ///=============================================================///
    bool partida_ganada; // true si el jugador derroto al Golem
    int enemigos_eliminados; // contador de enemigos muertos en la partida
    bool la_gema_fue_entregada; // true si la Gema Arcana ya aparecio en el suelo esta partida
    float tiempo_final_de_la_partida; // duracion total de la partida en segundos
    sf::Clock reloj_de_partida; // reloj que mide cuanto tiempo lleva la partida
    std::unique_ptr<Logro[]> logros; // arreglo dinamico con todos los logros del juego

 ///=============================================================///
 ///   PORTAL DE VICTORIA - Aparece donde murio el Golem
 ///=============================================================///
    bool portal_de_victoria_activo; // true si el portal de salida esta visible
    sf::Vector2f posicion_del_portal_de_victoria; // coordenadas donde aparece el portal
    float timer_respawn_del_portal_de_victoria; // temporizador antes de que aparezca el portal

 ///=============================================================///
 ///   CONSTRUCTOR
 ///=============================================================///
 // #1
    GameManager(); // inicializa todos los sistemas del juego

 ///=============================================================///
 ///   OTROS METODOS
 ///=============================================================///
 // #2
    void cambiar_musica(int id_de_la_musica); // detiene la musica actual y reproduce la nueva
 // #3
    void spawnear_drop_seguro(Item item_a_soltar, float posicion_x, float posicion_y); // coloca un item en el mundo evitando paredes
 // #4
    void resolver_colision_entre_personaje_y_golem(); // empuja al personaje si choca con el Golem
 // #5
    void guardar_logros(); // escribe los logros en un archivo binario
 // #6
    void cargar_logros(); // lee los logros desde el archivo binario
 // #7
    void reiniciar_partida(); // resetea todo el estado de juego y vuelve al menu
};

///=================================================================///
///   FUNCION SUELTA - Maneja el bucle principal del juego completo
///=================================================================///
void ejecutar(GameManager& gm); // inicia el loop principal: eventos, update y render
