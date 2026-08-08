#pragma once

#include <Arduino.h>

#include <Buttons.h>
#include <Canvas.h>

//==========================================================
// ClockNano Framework
// Menu Framework v1.0
//==========================================================
//
// Menu horizontal para display 32x8.
//
// Navegação:
//
//     +       próximo item
//     -       item anterior
//     OK      selecionar
//
// Entrada:
//
//     OK mantido aproximadamente 2 segundos
//
// Ajuste:
//
//     +       aumenta
//     -       diminui
//     OK      confirma
//
//==========================================================


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
// Resultado produzido pelo Menu
//==========================================================

enum class MenuResultType : uint8_t
{
    None = 0,

    Open,

    Selected,

    Changed,

    Confirmed,

    Back
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
// Estado interno do Menu
//==========================================================

enum class MenuState : uint8_t
{
    Closed,

    Opening,

    Browsing,

    Editing
};


//==========================================================
// Item do Menu
//==========================================================

struct MenuItem
{
    MenuItemId id;

    const char* name;
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
    // Remove todos os itens
    //------------------------------------------------------

    void clear();


    //------------------------------------------------------
    // Adiciona um item
    //------------------------------------------------------

    bool addItem(
        MenuItemId id,
        const char* name);


    //------------------------------------------------------
    // Atualiza o Menu
    //
    // Deve ser chamada quando houver ButtonEvent.
    //------------------------------------------------------

    void update(
        const ButtonEvent& event);


    //------------------------------------------------------
    // Renderiza o Menu no Canvas
    //------------------------------------------------------

    void draw(
        Canvas& canvas);


    //------------------------------------------------------
    // Lê o resultado produzido pelo Menu
    //------------------------------------------------------

    bool readResult(
        MenuResult& result);


    //------------------------------------------------------
    // Retorna estado atual
    //------------------------------------------------------

    MenuState state();


    //------------------------------------------------------
    // Retorna item atualmente selecionado
    //------------------------------------------------------

    MenuItemId selected();


    //------------------------------------------------------
    // Inicia o modo de ajuste do item atual
    //------------------------------------------------------

    void beginEdit(
        int16_t value);


    //------------------------------------------------------
    // Define valor atual do ajuste
    //------------------------------------------------------

    void setValue(
        int16_t value);


    //------------------------------------------------------
    // Retorna valor atual do ajuste
    //------------------------------------------------------

    int16_t value();


    //------------------------------------------------------
    // Fecha o Menu
    //------------------------------------------------------

    void close();

}