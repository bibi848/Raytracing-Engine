#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include "FunctionGroup.h"

void setPixel(SDL_Renderer* renderer, int x, int y, int color[3]) {
    SDL_SetRenderDrawColor(renderer, color[0], color[1], color[2], 255);
    SDL_RenderDrawPoint(renderer, x, y);
}

void saveImage(SDL_Renderer* renderer, const char* filename, const int WIDTH, const int HEIGHT) {
    // Create an SDL surface to hold the pixel data
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, WIDTH, HEIGHT, 32, SDL_PIXELFORMAT_RGBA32);

    if (!surface) {
        std::cerr << "Failed to create surface: " << SDL_GetError() << std::endl;
        return;
    }

    // Read the current renderer contents into the surface
    if (SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_RGBA32, surface->pixels, surface->pitch) < 0) {
        std::cerr << "Failed to read pixels: " << SDL_GetError() << std::endl;
        SDL_FreeSurface(surface);
        return;
    }

    // Save the surface as an image file (e.g., PNG)
    if (IMG_SavePNG(surface, filename) != 0) {
        std::cerr << "Failed to save image: " << IMG_GetError() << std::endl;
    }
    else {
        std::cout << "Image saved as " << filename << std::endl;
    }

    // Clean up
    SDL_FreeSurface(surface);
}

int RenderImage(const int WIDTH, const int HEIGHT, std::vector<std::vector<std::vector<int>>>& PixelArray, bool SaveImage, const char* filename) {
    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    // Create an SDL window
    SDL_Window* window = SDL_CreateWindow("Pixel Renderer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH, HEIGHT,
        SDL_WINDOW_SHOWN);

    if (!window) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // Create a renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!renderer) {
        std::cerr << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Clear screen with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Clear with black color
    SDL_RenderClear(renderer);

    // Loop through the pixels and draw them
    for (int y = 0; y < HEIGHT; ++y) {
        for (int x = 0; x < WIDTH; ++x) {
            setPixel(renderer, x, y, PixelArray[y][x].data()); // Pass RGB data
        }
    }

    // Update screen
    SDL_RenderPresent(renderer);

    // Event loop to keep window open
    bool running = true;
    SDL_Event event;

    while (running) {
        // Handle events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false; // Exit loop if the window is closed
            }
        }
    }

    if (SaveImage) {
        saveImage(renderer, filename, WIDTH, HEIGHT);
    }

    // Cleanup
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}