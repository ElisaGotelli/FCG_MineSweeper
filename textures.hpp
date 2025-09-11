#ifndef TEXTURES_HPP
#define TEXTURES_HPP

#include <SFML/Graphics.hpp>
#include <stdexcept>
#include <string>

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
inline sf::Texture smile_face_down_texture;
inline sf::Texture win_face_texture;

inline void load_textures() {
    if (!Covered_texture.loadFromFile("risorse/texture/cells/cellup.jpg"))
        throw std::runtime_error("Impossibile caricare cellup.jpg");

    if (!Flag_texture.loadFromFile("risorse/texture/cells/cellflag.jpg"))
        throw std::runtime_error("Impossibile caricare cellflag.jpg");

    if (!Normal_Mine_texture.loadFromFile("risorse/texture/cells/cellmine.jpg"))
        throw std::runtime_error("Impossibile caricare cellmine.jpg");

    if (!Exploded_Mine_texture.loadFromFile("risorse/texture/cells/blast.jpg"))
        throw std::runtime_error("Impossibile caricare blast.jpg");

    if (!False_Mine_texture.loadFromFile("risorse/texture/cells/falsemine.jpg"))
        throw std::runtime_error("Impossibile caricare falsemine.jpg");

    if (!Empty_texture.loadFromFile("risorse/texture/cells/celldown.jpg"))
        throw std::runtime_error("Impossibile caricare celldown.jpg");

    const char* borders[6] = {
        "risorse/texture/border/topbottom.jpg",
        "risorse/texture/border/leftright.jpg",
        "risorse/texture/border/topleft.jpg",
        "risorse/texture/border/topright.jpg",
        "risorse/texture/border/bottomleft.jpg",
        "risorse/texture/border/bottomright.jpg"
    };
    for (int i = 0; i < 6; i++) {
        if (!border_textures[i].loadFromFile(borders[i]))
            throw std::runtime_error(std::string("Impossibile caricare ") + borders[i]);
    }

    const char* numbers[8] = {
        "risorse/texture/cells/cell1.jpg",
        "risorse/texture/cells/cell2.jpg",
        "risorse/texture/cells/cell3.jpg",
        "risorse/texture/cells/cell4.jpg",
        "risorse/texture/cells/cell5.jpg",
        "risorse/texture/cells/cell6.jpg",
        "risorse/texture/cells/cell7.jpg",
        "risorse/texture/cells/cell8.jpg"
    };
    for (int i = 0; i < 8; i++) {
        if (!Number_textures[i].loadFromFile(numbers[i]))
            throw std::runtime_error(std::string("Impossibile caricare ") + numbers[i]);
    }

    const char* clock_numbers[10] = {
        "risorse/texture/clock/clock0.jpg",
        "risorse/texture/clock/clock1.jpg",
        "risorse/texture/clock/clock2.jpg",
        "risorse/texture/clock/clock3.jpg",
        "risorse/texture/clock/clock4.jpg",
        "risorse/texture/clock/clock5.jpg",
        "risorse/texture/clock/clock6.jpg",
        "risorse/texture/clock/clock7.jpg",
        "risorse/texture/clock/clock8.jpg",
        "risorse/texture/clock/clock9.jpg"
    };
    for (int i = 0; i < 10; i++) {
        if (!Clock_textures[i].loadFromFile(clock_numbers[i]))
            throw std::runtime_error(std::string("Impossibile caricare ") + clock_numbers[i]);
    }

}

#endif // TEXTURES_HPP
