#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Item.h"
#include "FabricaDeItems.h"

///=================================================================///
///   ESTADO DEL CREADOR - En que paso del formulario esta
///=================================================================///
enum class EstadoDelCreadorDeItems {
    SELECCIONANDO_SPRITE,
    LLENANDO_FORMULARIO,
    MOSTRANDO_LISTA
};

///=================================================================///
///   UI_CREADOR_ITEMS - Pantalla para crear items nuevos a mano y
///   guardarlos en el archivo .dat
///=================================================================///
class UI_CreadorItems {
private:

    static const int CANTIDAD_DE_CAMPOS_DEL_FORMULARIO = 6;
    static const int CANTIDAD_MAXIMA_DE_TEXTURAS = 6080;

    EstadoDelCreadorDeItems _estado_actual;
    int _id_de_la_textura_actual;

    sf::Sprite _vista_previa_del_sprite;
    sf::Font _fuente;

    int _campo_activo_del_formulario;
    std::string _nombres_de_los_campos[CANTIDAD_DE_CAMPOS_DEL_FORMULARIO];
    std::string _textos_que_escribio_el_usuario[CANTIDAD_DE_CAMPOS_DEL_FORMULARIO];

    RegistroDeItem _lista_de_items_para_mostrar[FabricaDeItems::CANTIDAD_MAXIMA_DE_REGISTROS_A_LEER];
    int _cantidad_de_items_en_la_lista;

    bool _se_solicito_guardar;

    ///=============================================================///
    ///   CONVERTIR TEXTO A NUMERO
    ///=============================================================///
    int convertir_texto_a_numero_entero(const std::string& texto) const;

public:

    ///=============================================================///
    ///   CONSTRUCTOR
    ///=============================================================///
    UI_CreadorItems();

    ///=============================================================///
    ///   GETTERS
    ///=============================================================///
    bool getSe_solicito_guardar() const { return _se_solicito_guardar; }

    ///=============================================================///
    ///   OTROS METODOS
    ///=============================================================///
    void actualizar_sprite_de_vista_previa(const sf::Texture& textura_maestra);
    void procesar_eventos(sf::Event& evento, const FabricaDeItems& fabrica_de_items);
    void dibujar(sf::RenderWindow& ventana_del_juego);
    void confirmar_guardado();
    RegistroDeItem generar_registro_con_los_datos_ingresados() const;
};