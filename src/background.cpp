#include "background.h"
#include <string>
#include <vector>
#include <algorithm>    // std::sort
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <iostream>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <cmath>
#include <SDL2/SDL_mixer.h>

bg::bg() {}
bg::bg(std::string path, bool folder, SDL_Renderer* renderer) {
    std::string filepath = DATA_PREFIX "/backgrounds/" + path + "/theme.xml";
    if(folder) {
        filepath = path + "/theme.xml";
    }

    rapidxml::file<> xmlFile(filepath.c_str());
    rapidxml::xml_document<> doc;
    doc.parse<0>(xmlFile.data());
    layers = atoi(doc.first_node("layers")->value());

    std::string p = DATA_PREFIX "/backgrounds/" + path;
    if(folder) {
        p = path;
    }
    
    generateSurfaces(p, renderer); //DOES THIS CODE EVEN WORK??? WHOOOO KNOWWWSSS?!?!?!?!

    
    name = doc.first_node("name")->value();
    creator = doc.first_node("creator")->value();
    vers = doc.first_node("vers")->value();
    songname = doc.first_node("musicname")->value();
    artist = doc.first_node("musicartist")->value();
    rotation = 0;

    if (doc.first_node("rotation") != NULL) {
        rotation = atoi(doc.first_node("rotation")->value());
    }
    if (doc.first_node("sine") != NULL) {
        sine = true;
        sinelayer = atoi(doc.first_node("sine")->value());
        snheight = atoi(doc.first_node("sineheight")->value());
        snwidth = atoi(doc.first_node("sinewidth")->value());
        snwid = atoi(doc.first_node("sinelayerheight")->value());\
        rate = atoi(doc.first_node("sinerate")->value());
    }


    if (doc.first_node("fglayer") != NULL) {
        std::cout << "fglayer detected\n";
        fglayer = atoi(doc.first_node("fglayer")->value());
    }
    if (doc.first_node("thumbnail") != NULL) {
        std::cout << "thumbnail detected\n";
        std::string thmbpath = DATA_PREFIX "/backgrounds/" + path + "/";
        thmbpath += doc.first_node("thumbnail")->value();
        thumbnail = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(thmbpath.c_str()));
    }
    else {
        std::string thmbpath = DATA_PREFIX "/backgrounds/nullbg.bmp";
        thumbnail = SDL_CreateTextureFromSurface(renderer, SDL_LoadBMP(thmbpath.c_str()));

    }

    int array[10];
    std::cout << "LAYERS: " << layers;
    for(int i = 0; i < layers; i++) {

        std::string sr = "layer";
        sr += std::to_string(i);
        std::cout << "TESTING::: " << sr << "\n";
        incrementsx[i] = atoi(doc.first_node(sr.c_str())->value());
        std::string sy = "layer";
        sy += std::to_string(i);
        sy += "y";
        incrementsy[i] = atoi(doc.first_node(sy.c_str())->value());
        //std::cout << "INFORMATION!!!: " << atoi(doc.first_node(sy.c_str())->value()) << "\n";
    }

    std::string muspath = DATA_PREFIX "/backgrounds/" + path + "/";
    if(folder) {
        muspath = path + "/";
    }

    muspath += doc.first_node("music")->value();
    music = Mix_LoadMUS(muspath.c_str());
    if (!music) {
        printf("Failed to load music at %s: %s\n", muspath, SDL_GetError());
    }

    std::cout << "Finished loading: " << name << "\n";
    doc.clear();
}

void  bg::generateSurfaces(std::string path, SDL_Renderer* renderer) {
    std::vector<SDL_Surface*> surfaces;
    for(int i = 0; i < layers; i++) {
        char buff[12];
        snprintf(buff, sizeof(buff), "%02d", i);
        std::string temppath = path + "/" + buff + ".bmp";

        SDL_Surface* temp = SDL_LoadBMP(temppath.c_str());
        if (!temp) {
            printf("Failed to load image at %s: %s\n", temppath, SDL_GetError());
        }
        surfaces.push_back(temp);
        printf("Successfully loaded image at %s\n", temppath.c_str());
    }
    for (SDL_Surface* surf : surfaces) {
        SDL_Texture* temp = SDL_CreateTextureFromSurface(renderer, surf);
        if(temp != NULL) {
            textures.push_back(temp);
            printf("pushed texture!!");
            SDL_FreeSurface(surf);
        }
        else {
            fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
        }
    }

}
void bg::render(SDL_Renderer* renderer, bool layer) {
    //OKAY EXPLAINATION FOR MY ACTIONS:
    //dear whoever is reading this:
    //this code is a bad practice.

    int addition = 0;
    int max = layers;
    if (fglayer > 0) {
        max = fglayer;
        if (layer) {
            max = layers;
            addition = fglayer;
        }
    }
    else if (layer && fglayer == 0) {
        return;
    }

    for (int i = addition; i < max; i++) {
        int width, height;
        SDL_QueryTexture(textures[i], NULL, NULL, &width, &height);
        double tempx = 0;
        double tempy = 0; //yuck
        int multiplerx = 1; //this is really bad practice but it's currently 11pm and i wanna feel accomplished
        int multiplery = 1;
        //std::cout << incrementsx[i] << i << "\n";
        if (incrementsx[i] != 0) {
            tempx = fmod(layerposx[i], width); //ew
        }
        if (incrementsy[i] != 0) {
            tempy = fmod(layerposy[i], height); //GROSS CODE
        }
        if (layerposx[i] > 0) {
            multiplerx = -1;
        }
        if (layerposy[i] > 0) {
            multiplery = -1;
        }
        bool dothis = sine && i == sinelayer;
        drawLayer(renderer, textures[i],tempx,tempy,multiplerx,multiplery,width,height,
        dothis,
        snwid, //wave width in pixels
        snwidth, //sine width
        snheight, //sine height
        angle //increment. 
        );
        }
}
void bg::logic(double deltatime)
{
    angle += deltatime / rate;
    //std::cout << angle << "\n";

    for(int i = 0; i < layers; i++) {
        if(incrementsx[i] != 0) {
            layerposx[i] -= (deltatime)/(incrementsx[i]);
        }
        if(incrementsy != 0) {
            layerposy[i] -= (deltatime)/(incrementsy[i]);
        }

    }
}

void bg::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center) {
    SDL_Rect sprite;
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    int oldwidth = sprite.w;
    int oldheight = sprite.h;
    sprite.w = sprite.w * scale;
    sprite.h = sprite.h * scale;
    if (center) {
        sprite.x = x - oldwidth / 2;
        sprite.y = y - oldheight / 2;
    }
    else {
        sprite.x = x + oldwidth / 2 - sprite.w / 2;
        sprite.y = y + oldheight / 2 - sprite.h / 2;
    }
    SDL_RenderCopyEx(renderer, texture, NULL, &sprite, 0, NULL, SDL_FLIP_NONE);
}

//lol i stole more code from font.h
//I had this great idea for doing waves through like modifying pixel data, and then I realized "oh crap, that'll use up way too much GPU to handle"

//so then i cried

void bg::drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch) {
    SDL_Rect sprite;
    SDL_Rect srcrect = {srcx, srcy, srcw, srch};
    if(SDL_QueryTexture(texture, NULL, NULL, &sprite.w, &sprite.h) < 0) {
        printf("TEXTURE ISSUES!!! \n");
        std::cout << SDL_GetError() << "\n";
    };
    sprite.w = srcw * scale;
    sprite.h = srch * scale;
    if (center) {
        sprite.x = x - srcw / 2;
        sprite.y = y - srch / 2;
    }
    else {
        sprite.x = x + srcw / 2 - sprite.w / 2;
        sprite.y = y + srch / 2 - sprite.h / 2;
    }
    SDL_RenderCopy(renderer, texture, &srcrect, &sprite);
    //since the angle system doesnt even work
    //BUT I SWEAR GUYS ILL GET IT TO WORK EVENTUALLY
}
//anyways so that's the new and totally awesome relevant modern new background system thing that everyone definitely loves


//(edit added pun)
bool bg::hasEnding(std::string const& fullString, std::string const& ending) { //thank you kdt on Stackoverflow, its late at night and you helped me right https://stackoverflow.com/questions/874134/find-out-if-string-ends-with-another-string-in-c
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    }
    else {
        return false;
    }
}

void bg::drawLayer(SDL_Renderer* renderer, SDL_Texture* texture, int tempx, int tempy, int multiplerx, int multiplery, int width, int height, bool wavy, int wavywidth, int sinewidth, int sineheight, double sinepos) {
    if (wavy) {
        for(int i = 0; i < height; i+=wavywidth) {
            double sinex = (sin((sinepos + i) * sinewidth) * sineheight);

            drawTexture(renderer, texture, tempx+sinex, (tempy+i), fmod(angle, 360), 1.0, false,0,i,width,wavywidth);

            drawTexture(renderer, texture, tempx+sinex + (width * multiplerx), (tempy+i) + (height * multiplery), fmod(angle, 360), 1.0, false,0,i,width,wavywidth);
            
            drawTexture(renderer, texture, tempx+sinex, (tempy+i)+(height * multiplery), fmod(angle, 360), 1.0, false,0,i,width,wavywidth);

            drawTexture(renderer, texture, tempx+sinex + (width * multiplerx), tempy+i, fmod(angle, 360), 1.0, false,0,i,width,wavywidth);
        }
    }
    else {
        drawTexture(renderer, texture, tempx, tempy, fmod(angle, 360), 1.0, false);
        drawTexture(renderer, texture, tempx + (width * multiplerx), tempy + (height * multiplery), fmod(angle, 360), 1.0, false);
        drawTexture(renderer, texture, tempx + 0, tempy + (height * multiplery), fmod(angle, 360), 1.0, false);
        drawTexture(renderer, texture, tempx + (width * multiplerx), tempy, fmod(angle, 360), 1.0, false);
    }

}

