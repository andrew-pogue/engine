#pragma once
#include "layout.hh"

class MainMenu : Row {
public:

    MainMenu()
        : panel_()
        , fonts_{"TITLE", "BUTTON"}
        , play_(false)
    {
        panel_.create<Text>(fonts_.title, WHITE, "MAIN MENU");
        auto options = panel_.create<Column>(50.0f);
        options.padding.y = 10.0f;
        options.spacing = 10.0f;
        {   auto btn = options.create<Row>();
            btn.flags.border = true;
            btn.border = { WHITE, 2.0f };
            btn.create<Text>(fonts_.button, WHITE, "PLAY");
            btn.on_click = play; }
        {   auto btn = options.create<Row>();
            btn.flags.border = true;
            btn.border = { WHITE, 2.0f };
            btn.create<Text>(fonts_.button, WHITE, "LOAD");
            btn.on_click = load; }
        {   auto btn = options.create<Row>();
            btn.flags.border = true;
            btn.border = { WHITE, 2.0f };
            btn.create<Text>(fonts_.button, WHITE, "SETTINGS");
            btn.on_click = settings; }
        {   auto btn = options.create<Row>();
            btn.flags.border = true;
            btn.border = { WHITE, 2.0f };
            btn.create<Text>(fonts_.button, WHITE, "EXIT");
            btn.on_click = exit; }
    }

    bool update(double dt) override { return play_; }

private:

    void play()     {}
    void load()     {}
    void settings() {}
    void exit()     { play_ = false; }

    Column panel_;
    struct {
        Font title;
        Font button;
    } fonts_;
    bool play_;

    Keyboard keyboard_(display);
    Mouse mouse_(display);
    Font font_{"assets/font/PressStart2P-Regular.ttf", 36};

};

