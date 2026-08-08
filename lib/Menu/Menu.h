#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#include <Buttons.h>
#include <Canvas.h>

//==========================================================
// Identificação dos itens
//==========================================================

enum class MenuItemId : uint8_t
{
    None = 0,

    Time,
    Date,
    Font,
    Alarm,
    Brightness,
    Contrast,
    About
};

//==========================================================
// Estado do Menu
//==========================================================

enum class MenuState : uint8_t
{
    Closed,

    Browsing,

    Editing
};

//==========================================================
// Resultado do Menu
//==========================================================

enum class MenuResultType : uint8_t
{
    None,

    Open,

    Selected,

    Changed,

    Confirmed
};

//==========================================================
// Configuração da edição
//==========================================================
//
// Esta estrutura não conhece o significado do valor.
//
// Ela somente define as regras:
//
// editable = item pode ser editado
// minimum  = limite inferior
// maximum  = limite superior
// step     = incremento/decremento
//

struct MenuEditConfig
{
    bool editable;

    int16_t minimum;

    int16_t maximum;

    int16_t step;
};

//==========================================================
// Item do Menu
//==========================================================

struct MenuItem
{
    MenuItemId id;

    const char *name;

    MenuEditConfig edit;
};

//==========================================================
// Resultado
//==========================================================

struct MenuResult
{
    MenuResultType type;

    MenuItemId item;

    int16_t value;
};

//==========================================================
// API pública
//==========================================================

namespace Menu
{

    //------------------------------------------------------
    // Inicialização
    //------------------------------------------------------

    bool begin();

    //------------------------------------------------------
    // Limpa todos os itens
    //------------------------------------------------------

    void clear();

    //------------------------------------------------------
    // Adiciona item sem edição
    //
    // Mantém compatibilidade com a API anterior.
    //------------------------------------------------------

    bool addItem(
        MenuItemId id,
        const char *name);

    //------------------------------------------------------
    // Adiciona item com configuração de edição
    //------------------------------------------------------

    bool addItem(
        MenuItemId id,
        const char *name,
        const MenuEditConfig &edit);

    //------------------------------------------------------
    // Recebe eventos dos Buttons
    //------------------------------------------------------

    void update(
        const ButtonEvent &event);

    //------------------------------------------------------
    // Desenha o Menu
    //------------------------------------------------------

    void draw(
        Canvas &canvas);

    //------------------------------------------------------
    // Lê resultado pendente
    //------------------------------------------------------

    bool readResult(
        MenuResult &result);

    //------------------------------------------------------
    // Estado atual
    //------------------------------------------------------

    MenuState state();

    //------------------------------------------------------
    // Item atualmente selecionado
    //------------------------------------------------------

    MenuItemId selected();

    //------------------------------------------------------
    // Inicia edição do valor atual
    //------------------------------------------------------

    void beginEdit(
        int16_t value);

    //------------------------------------------------------
    // Define valor atual
    //------------------------------------------------------

    void setValue(
        int16_t value);

    //------------------------------------------------------
    // Retorna valor atual
    //------------------------------------------------------

    int16_t value();

    //------------------------------------------------------
    // Fecha o Menu
    //------------------------------------------------------

    void close();

}

#endif