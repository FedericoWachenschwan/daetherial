#include "UI_Inventario.h"
#include <iostream>
#include <string>

UI_Inventario::UI_Inventario() {
	_estaAbierto = false;
	_tamanioSlot = 50.f;
	_margen = 10.f;

	// Variables de debug para mover el HUD en caliente (ajustar en tiempo real desde el GameManager)
	_desfaseX = 0.f;
	_desfaseY = 0.f;
	_origenX = 0.f;
	_origenY = 0.f;

	// Diseño del rectángulo base
	_slotFondo.setSize(sf::Vector2f(_tamanioSlot, _tamanioSlot));
	_slotFondo.setFillColor(sf::Color(40, 40, 40, 200));
	_slotFondo.setOutlineColor(sf::Color::White);
	_slotFondo.setOutlineThickness(2.f);

	// 🌟 NUEVO: Cargamos la fuente para los números
	if (!_fuente.loadFromFile("assets/NorthEternal-yYl4V.otf")) { // FUENTE
		std::cout << "❌ Error: No se encontró la fuente para el inventario." << std::endl;
	}

	_textoCantidad.setFont(_fuente);
	_textoCantidad.setCharacterSize(14); // Letra chiquita
	_textoCantidad.setFillColor(sf::Color::White);
	// Le ponemos un bordecito negro al texto para que se lea siempre, sea cual sea el fondo
	_textoCantidad.setOutlineColor(sf::Color::Black);
	_textoCantidad.setOutlineThickness(1.f);
}


// Métodos para que el GameManager incremente o decremente los valores

void UI_Inventario::ajustarPosicion(float x, float y) {
	_desfaseX += x;
	_desfaseY += y;
	// Imprimimos en consola en tiempo real para "robar" los números después
	std::cout << "📍 HUD Pos -> DesfaseX: " << _desfaseX << " | DesfaseY: " << _desfaseY << std::endl;
}

void UI_Inventario::ajustarOrigen(float x, float y) {
	_origenX += x;
	_origenY += y;
	std::cout << "🎯 HUD Origen -> X: " << _origenX << " | Y: " << _origenY << std::endl;
}

void UI_Inventario::dibujar(sf::RenderWindow& ventana, const Inventario& mochila) {
	if (!_estaAbierto) return;
	sf::View vistaOriginal = ventana.getView();
	ventana.setView(ventana.getDefaultView());

	// 🌟 1. APLICAR EL ORIGEN al rectángulo de fondo ANTES de calcular posiciones
	_slotFondo.setOrigin(_origenX, _origenY);

	const std::vector<Item*>& items = mochila.getSlots();
	int cantidadSlotsVisibles = 5;


	int indiceSeleccionado = mochila.getIndiceSeleccionado();
	float anchoTotal = (cantidadSlotsVisibles * _tamanioSlot) + ((cantidadSlotsVisibles - 1) * _margen);

	// 🌟 2. SUMAR LOS DESFASES a la posición inicial (startX y startY)
	float startX = ((ventana.getSize().x - anchoTotal) / 2.f) + _desfaseX;
	float startY = (ventana.getSize().y - _tamanioSlot - 20.f) + _desfaseY;

	for (int i = 0; i < cantidadSlotsVisibles; i++) {

		float posX = startX + i * (_tamanioSlot + _margen);

		// 1. Dibujamos la caja gris de fondo vacía siempre
		_slotFondo.setPosition(posX, startY);

		// ==========================================
		// 🌟 EL SISTEMA DE RESALTADO VISUAL
		// ==========================================
		if (i == indiceSeleccionado) {
			_slotFondo.setOutlineThickness(3.f); // Borde más grueso
			_slotFondo.setOutlineColor(sf::Color::Green); // Color de resaltado
		}
		else {
			_slotFondo.setOutlineThickness(2.f); // Borde normal que definiste en el constructor
			_slotFondo.setOutlineColor(sf::Color::White); // Color original
		}
		ventana.draw(_slotFondo);

		// 2. Si hay un ítem guardado en este slot, lo dibujamos arriba
		if (i <  static_cast<int> (items.size()) && items[i] != nullptr) {

			// Pedimos una copia del dibujito del ítem
			sf::Sprite spriteItem = items[i]->getSprite();

			// Lo acomodamos un poquito adentro de la caja (le sumo 9 píxeles para centrar un sprite de 32x32 en una caja de 50x50)
			spriteItem.setPosition(posX + 9.f, startY + 9.f);
			ventana.draw(spriteItem);

			// 3. Dibujar el número solo si hay más de 1 (como en Minecraft)
			int cantidad = items[i]->getCantidad();
			if (cantidad > 1) {
				_textoCantidad.setString(std::to_string(cantidad));

				// Lo anclamos abajo a la derecha del cuadradito
				_textoCantidad.setPosition(posX + _tamanioSlot - 20.f, startY + _tamanioSlot - 20.f);
				ventana.draw(_textoCantidad);
			}
		}
	}

	// 🎥 Restauramos la cámara normal para no romper el mapa
	ventana.setView(vistaOriginal);
}

// ==========================================
// DETECTAR CLIC CASILLERO (Para Hotbar de 5 Slots)
// ==========================================

void UI_Inventario::detectarClicCasillero(sf::Vector2i posicionMouse, Inventario& mochila, const sf::RenderWindow& ventana) {

	// 🌟 LA MAGIA DE SFML: Traducimos el pixel del monitor a la coordenada real de la UI
	sf::Vector2f mouseUI = ventana.mapPixelToCoords(posicionMouse, ventana.getDefaultView());

	// 🐛 CHIVATO 1: Nos avisa que el clic llegó hasta acá
	std::cout << "🖱️ Clic detectado en coordenada UI -> X: " << mouseUI.x << " | Y: " << mouseUI.y << std::endl;

	int cantidadSlotsVisibles = 5;
	float anchoTotal = (cantidadSlotsVisibles * _tamanioSlot) + ((cantidadSlotsVisibles - 1) * _margen);
	float startX = ((ventana.getSize().x - anchoTotal) / 2.f) + _desfaseX;
	float startY = (ventana.getSize().y - _tamanioSlot - 20.f) + _desfaseY;

	for (int i = 0; i < cantidadSlotsVisibles; i++) {
		float posX = startX + i * (_tamanioSlot + _margen);
		sf::FloatRect limitesSlot(posX, startY, _tamanioSlot, _tamanioSlot);

		if (limitesSlot.contains(mouseUI)) {
			std::cout << "🎯 ¡Le embocaste al casillero " << i << "!" << std::endl;

			// 🛡️ Agregamos "&& mochila.getSlots()[i] != nullptr" para asegurar que de verdad haya un objeto físico ahí
			if (i < static_cast<int>(mochila.getSlots().size()) && mochila.getSlots()[i] != nullptr) {
				mochila.setIndiceSeleccionado(i);

				// 🌟 1. Le pedimos el tipo al inventario usando tu nuevo getter
				TipoItem tipo = mochila.getItemTipo(i);

				// 🌟 2. Traducimos el Enum a un string amigable para los humanos
				std::string nombreTipo = "Desconocido";
				if (tipo == TipoItem::Consumible) nombreTipo = "Consumible";
				else if (tipo == TipoItem::Equipamiento) nombreTipo = "Equipamiento";
				else if (tipo == TipoItem::Recurso) nombreTipo = "Recurso";
				else if (tipo == TipoItem::Mueble) nombreTipo = "Mueble";

				// 🌟 3. Ahora sí, lo imprimimos hermoso en la consola
				std::cout << "✅ Y tenia un item de tipo [" << nombreTipo << "]. ¡Seleccionado!" << std::endl;
			}
			else {
				mochila.setIndiceSeleccionado(-1);
				std::cout << "❌ Le diste al casillero, pero estaba vacio." << std::endl;
			}
			return; // Encontramos el slot, cortamos el bucle
		}
	}
}

bool UI_Inventario::mouseSobrePanel(sf::Vector2i posicionMouse) const {
	if (!_estaAbierto) return false;

	// Si está abierto, chequea si el mouse cae adentro del rectangulo del sprite
	return _spriteInventario.getGlobalBounds().contains(
		static_cast<float>(posicionMouse.x),
		static_cast<float>(posicionMouse.y)
	);
}

