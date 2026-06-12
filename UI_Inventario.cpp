#include "UI_Inventario.h"
#include <iostream>
#include <string>

UI_Inventario::UI_Inventario() {
    _estaAbierto = false;
    _tamanioSlot = 50.f;
    _margen = 10.f;

    _desfaseX = 0.f;
    _desfaseY = 0.f;
    _origenX = 0.f;
    _origenY = 0.f;

    _slotFondo.setSize(sf::Vector2f(_tamanioSlot, _tamanioSlot));
    _slotFondo.setFillColor(sf::Color(40, 40, 40, 200));
    _slotFondo.setOutlineColor(sf::Color::White);
    _slotFondo.setOutlineThickness(2.f);

    if (!_fuente.loadFromFile("assets/NorthEternal.otf")) {
        std::cout << "ERROR: NO SE ENCONTRO LA FUENTE PARA EL INVENTARIO." << std::endl;
    }

    _textoCantidad.setFont(_fuente);
    _textoCantidad.setCharacterSize(14);
    _textoCantidad.setFillColor(sf::Color::White);
    _textoCantidad.setOutlineColor(sf::Color::Black);
    _textoCantidad.setOutlineThickness(1.f);
}

void UI_Inventario::ajustarPosicion(float x, float y) {
    _desfaseX += x;
    _desfaseY += y;
    std::cout << "HUD Pos -> DesfaseX: " << _desfaseX << " | DesfaseY: " << _desfaseY << std::endl;
}

void UI_Inventario::ajustarOrigen(float x, float y) {
    _origenX += x;
    _origenY += y;
    std::cout << "HUD Origen -> X: " << _origenX << " | Y: " << _origenY << std::endl;
}

void UI_Inventario::dibujar(sf::RenderWindow& ventana, const Inventario& mochila) {
    if (!_estaAbierto) return;

    sf::View vistaOriginal = ventana.getView();
    ventana.setView(ventana.getDefaultView());

    _slotFondo.setOrigin(_origenX, _origenY);

    const std::vector<Item*>& items = mochila.getSlots();
    int cantidadSlotsVisibles = 5;
    int indiceSeleccionado = mochila.getIndiceSeleccionado();

    float anchoTotal = (cantidadSlotsVisibles * _tamanioSlot) + ((cantidadSlotsVisibles - 1) * _margen);
    float startX = ((ventana.getSize().x - anchoTotal) / 2.f) + _desfaseX;
    float startY = (ventana.getSize().y - _tamanioSlot - 20.f) + _desfaseY;

    for (int i = 0; i < cantidadSlotsVisibles; i++) {

        float posX = startX + i * (_tamanioSlot + _margen);

        _slotFondo.setPosition(posX, startY);

        if (i == indiceSeleccionado) {
            _slotFondo.setOutlineThickness(3.f);
            _slotFondo.setOutlineColor(sf::Color::Green);
        }
        else {
            _slotFondo.setOutlineThickness(2.f);
            _slotFondo.setOutlineColor(sf::Color::White);
        }
        ventana.draw(_slotFondo);

        if (i < static_cast<int>(items.size()) && items[i] != nullptr) {

            sf::Sprite spriteItem = items[i]->getSprite();
            const sf::Texture* textura = spriteItem.getTexture();

            if (textura != nullptr) {
                ///=========================================================///
                ///   ITEM CON TEXTURA - Lo dibujamos normal
                ///=========================================================///
                spriteItem.setPosition(posX + 9.f, startY + 9.f);
                ventana.draw(spriteItem);
            }
            else {
                ///=========================================================///
                ///   ITEM SIN TEXTURA - Dibujamos un rectángulo de color
                ///   según el tipo del item, y su nombre encima
                ///=========================================================///
                sf::RectangleShape rectangulo_del_item; // Rectángulo que reemplaza al sprite
                rectangulo_del_item.setSize(sf::Vector2f(_tamanioSlot - 10.f, _tamanioSlot - 10.f));
                rectangulo_del_item.setPosition(posX + 5.f, startY + 5.f);

                TipoItem tipo_del_item = items[i]->getTipo();

                if (tipo_del_item == TipoItem::Consumible) {
                    rectangulo_del_item.setFillColor(sf::Color(200, 50, 50, 200)); // Rojo para pociones de vida
                }
                else if (tipo_del_item == TipoItem::PocionMana) {
                    rectangulo_del_item.setFillColor(sf::Color(50, 50, 220, 200)); // Azul para pociones de maná
                }
                else {
                    rectangulo_del_item.setFillColor(sf::Color(100, 100, 100, 200)); // Gris para el resto
                }

                ventana.draw(rectangulo_del_item);

                // Nombre del item encima del rectángulo
                sf::Text nombre_del_item_en_slot;
                nombre_del_item_en_slot.setFont(_fuente);
                nombre_del_item_en_slot.setCharacterSize(8);
                nombre_del_item_en_slot.setFillColor(sf::Color::White);
                nombre_del_item_en_slot.setOutlineColor(sf::Color::Black);
                nombre_del_item_en_slot.setOutlineThickness(1.f);
                nombre_del_item_en_slot.setString(items[i]->getNombre());
                nombre_del_item_en_slot.setPosition(posX + 3.f, startY + 3.f);
                ventana.draw(nombre_del_item_en_slot);
            }

            int cantidad = items[i]->getCantidad();
            if (cantidad > 1) {
                _textoCantidad.setString(std::to_string(cantidad));
                _textoCantidad.setPosition(posX + _tamanioSlot - 20.f, startY + _tamanioSlot - 20.f);
                ventana.draw(_textoCantidad);
            }
        }
    }

    ventana.setView(vistaOriginal);
}

void UI_Inventario::detectarClicCasillero(sf::Vector2i posicionMouse, Inventario& mochila, const sf::RenderWindow& ventana) {

    sf::Vector2f mouseUI = ventana.mapPixelToCoords(posicionMouse, ventana.getDefaultView());

    std::cout << "CLIC DETECTADO EN COORDENADA UI -> X: " << mouseUI.x << " | Y: " << mouseUI.y << std::endl;

    int cantidadSlotsVisibles = 5;
    float anchoTotal = (cantidadSlotsVisibles * _tamanioSlot) + ((cantidadSlotsVisibles - 1) * _margen);
    float startX = ((ventana.getSize().x - anchoTotal) / 2.f) + _desfaseX;
    float startY = (ventana.getSize().y - _tamanioSlot - 20.f) + _desfaseY;

    for (int i = 0; i < cantidadSlotsVisibles; i++) {
        float posX = startX + i * (_tamanioSlot + _margen);
        sf::FloatRect limitesSlot(posX, startY, _tamanioSlot, _tamanioSlot);

        if (limitesSlot.contains(mouseUI)) {
            std::cout << "LE DISTE AL CASILLERO " << i << std::endl;

            if (i < static_cast<int>(mochila.getSlots().size()) && mochila.getSlots()[i] != nullptr) {
                mochila.setIndiceSeleccionado(i);

                TipoItem tipo = mochila.getItemTipo(i);
                std::string nombreTipo = "Desconocido";
                if (tipo == TipoItem::Consumible)  nombreTipo = "Consumible";
                else if (tipo == TipoItem::Equipamiento) nombreTipo = "Equipamiento";
                else if (tipo == TipoItem::Recurso) nombreTipo = "Recurso";
                else if (tipo == TipoItem::Mueble)  nombreTipo = "Mueble";
                else if (tipo == TipoItem::PocionMana) nombreTipo = "Pocion de Mana";

                std::cout << "ITEM SELECCIONADO DE TIPO [" << nombreTipo << "]." << std::endl;
            }
            else {
                mochila.setIndiceSeleccionado(-1);
                std::cout << "CASILLERO VACIO." << std::endl;
            }
            return;
        }
    }
}

bool UI_Inventario::mouseSobrePanel(sf::Vector2i posicionMouse) const {
    if (!_estaAbierto) return false;
    return _spriteInventario.getGlobalBounds().contains(
        static_cast<float>(posicionMouse.x),
        static_cast<float>(posicionMouse.y)
    );
}