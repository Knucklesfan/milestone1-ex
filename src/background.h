/*


Knuxfan's background.h
(old bad, SDL2 only version! New opengl version coming soon.)


*/
#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <iostream>
#include <vector>
#include <array>
#include <algorithm>    // std::sort
#include <cstring>
#include <SDL2/SDL_mixer.h>

class bg
{
    
	public:
        double incrementsx[10];
        double incrementsy[10];
        double layerposx[10];
        double layerposy[10];
        int layers = 0;
        int sinelayer = 0;
        int snwidth = 0;
        int snheight = 0;
        int snwid = 0;
        int rate = 0;
        bool sine = 0;
        int fglayer = 0;
        double angle = 0.0;
        double rotation;
	    void logic(double deltatime);
        std::string name;
        std::string path;
        std::string creator;
        std::string vers;
        Mix_Music* music;
        std::string songname;
        std::string artist;
        void render(SDL_Renderer* renderer, bool layer);
        std::vector<SDL_Texture*> textures;
        SDL_Texture* thumbnail;
        bg(std::string path, bool folder, SDL_Renderer* renderer);
        bg();

    private:
        int maxwidth, maxheight;
    	void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center);
        void drawTexture(SDL_Renderer* renderer, SDL_Texture* texture, int x, int y, double angle, double scale, bool center, int srcx, int srcy, int srcw, int srch);
        bool hasEnding(std::string const& fullString, std::string const& ending);
        void generateSurfaces(std::string path, SDL_Renderer* renderer);
        static bool compareFunction (std::string a, std::string b) {return a<b;} 
        void drawLayer(SDL_Renderer* renderer, SDL_Texture* texture, int tempx, int tempy, int multiplerx, int multiplery, int width, int height, bool wavy, int wavywidth, int sinewidth, int sineheight, double sinepos);
        
};
