#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include "Item.h" // Para usar el struct ItemReg
#include "ItemManager.h" // Para acceder a la textura maestra y generar registros

// Definimos en qué parte del creador estamos
enum class EstadoCreador {
    SELECCIONANDO_SPRITE,
    LLENANDO_FORMULARIO,
    MOSTRANDO_LISTA
};

class UI_CreadorItems {
private:
    EstadoCreador _estado;
    int _idTexturaActual;
    const int MAX_TEXTURAS = 6080; // Tu mega-spritesheet

    sf::Sprite _spritePreview;
    sf::Font _fuente;

    // --- Variables del Formulario Visual ---
    int _campoActivo; // 0:ID, 1:Nombre, 2:Tipo, 3:Efecto, 4:Precio, 5:Rareza
    std::vector<std::string> _nombresCampos; // Las etiquetas ("ID:", "Nombre:", etc.)
    std::vector<std::string> _inputsUsuario; // Lo que vos tipeás
	std::vector<ItemReg> _listaItems; // Para mostrar la lista de items creados (opcional)
    bool _guardarSolicitado; // Bandera para avisarle al GameManager que termine

public:
    UI_CreadorItems();

    // Actualiza la vista previa del sprite
    void actualizarSprite(const sf::Texture& texturaMaestra);

    // Captura las flechas y el teclado
    void procesarEventos(sf::Event& evento, const ItemManager& itemManager);

    // Dibuja el carrusel o el formulario en pantalla
    void dibujar(sf::RenderWindow& ventana);

    // Métodos para comunicarse con el GameManager
    bool quiereGuardar() const { return _guardarSolicitado; }
    void confirmarGuardado(); // Reinicia el formulario tras guardar
    ItemReg generarRegistro() const; // Empaqueta los textos en el struct real
};