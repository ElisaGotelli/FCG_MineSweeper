#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>
#include <filesystem>

inline sf::Texture Covered_texture;
inline sf::Texture Flag_texture;
inline sf::Texture Normal_Mine_texture;
inline sf::Texture Exploded_Mine_texture;
inline sf::Texture False_Mine_texture;
inline sf::Texture Empty_texture;
inline sf::Texture Number_textures[8];
inline sf::Texture border_textures[6];
inline sf::Texture Clock_textures[10];
inline sf::Texture Click_face_texture;
inline sf::Texture lost_face_texture;
inline sf::Texture smile_face_texture;
inline sf::Texture win_face_texture;
inline sf::Font font; 

// ...existing code...
inline void load_textures_fonts(const std::string& res_base = "../risorse/") {
    
    auto path = [&](const std::string& rel){ return res_base + rel; };

    if (!Covered_texture.loadFromFile(path("texture/cells/cellup.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/cells/cellup.jpg"));

    if (!Flag_texture.loadFromFile(path("texture/cells/cellflag.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/cells/cellflag.jpg"));

    if (!Normal_Mine_texture.loadFromFile(path("texture/cells/cellmine.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/cells/cellmine.jpg"));

    if (!Exploded_Mine_texture.loadFromFile(path("texture/cells/blast.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/cells/blast.jpg"));

    if (!False_Mine_texture.loadFromFile(path("texture/cells/falsemine.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/cells/falsemine.jpg"));

    if (!Empty_texture.loadFromFile(path("texture/cells/celldown.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/cells/celldown.jpg"));

    const char* borders[6] = {
        "texture/border/topbottom.jpg",
        "texture/border/leftright.jpg",
        "texture/border/topleft.jpg",
        "texture/border/topright.jpg",
        "texture/border/bottomleft.jpg",
        "texture/border/bottomright.jpg"
    };
    for (int i = 0; i < 6; i++) {
        if (!border_textures[i].loadFromFile(path(borders[i])))
            throw std::runtime_error(std::string("Impossibile caricare ") + path(borders[i]));
    }

    const char* numbers[8] = {
        "texture/cells/cell1.jpg",
        "texture/cells/cell2.jpg",
        "texture/cells/cell3.jpg",
        "texture/cells/cell4.jpg",
        "texture/cells/cell5.jpg",
        "texture/cells/cell6.jpg",
        "texture/cells/cell7.jpg",
        "texture/cells/cell8.jpg"
    };
    for (int i = 0; i < 8; i++) {
        if (!Number_textures[i].loadFromFile(path(numbers[i])))
            throw std::runtime_error(std::string("Impossibile caricare ") + path(numbers[i]));
    }

    const char* clock_numbers[10] = {
        "texture/clock/clock0.jpg",
        "texture/clock/clock1.jpg",
        "texture/clock/clock2.jpg",
        "texture/clock/clock3.jpg",
        "texture/clock/clock4.jpg",
        "texture/clock/clock5.jpg",
        "texture/clock/clock6.jpg",
        "texture/clock/clock7.jpg",
        "texture/clock/clock8.jpg",
        "texture/clock/clock9.jpg"
    };
    for (int i = 0; i < 10; i++) {
        if (!Clock_textures[i].loadFromFile(path(clock_numbers[i])))
            throw std::runtime_error(std::string("Impossibile caricare ") + path(clock_numbers[i]));
    }

    if (!Click_face_texture.loadFromFile(path("texture/faces/clickface.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/faces/clickface.jpg"));

    if (!lost_face_texture.loadFromFile(path("texture/faces/lostface.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/faces/lostface.jpg"));

    if (!smile_face_texture.loadFromFile(path("texture/faces/smileface.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/faces/smileface.jpg"));

    if (!win_face_texture.loadFromFile(path("texture/faces/winface.jpg")))
        throw std::runtime_error(std::string("Impossibile caricare ") + path("texture/faces/winface.jpg"));

    if (!font.openFromFile(res_base + "DejavuSansMono-5m7L.ttf"))
        throw std::runtime_error(std::string("Impossibile caricare il font: ") + res_base + "DejavuSansMono-5m7L.ttf");
}
// ...existing code...

#endif // TEXTURES_HPP
