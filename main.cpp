#include <SDL.h>
#include <cstdio>

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 800;
const int CANVAS_WIDTH = 800;
const int CANVAS_HEIGHT = 800;
const int CANVAS_X = (SCREEN_WIDTH - CANVAS_WIDTH) / 2;
const int CANVAS_Y = (SCREEN_HEIGHT - CANVAS_HEIGHT) / 2;

#define PX_DEFAULT 6
int pixelSize = PX_DEFAULT;

SDL_Window *gWindow = nullptr;
SDL_Renderer *gRenderer = nullptr;
SDL_Texture *canvasTexture = nullptr;

int get_fps() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    SDL_Window *tempWindow = SDL_CreateWindow("QueryRefreshRate",
                                              SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                              640, 480, SDL_WINDOW_HIDDEN);

    if (tempWindow == nullptr) {
        SDL_Quit();
        return -1;
    }

    SDL_DisplayMode mode;
    if (SDL_GetCurrentDisplayMode(0, &mode) != 0) {
        SDL_DestroyWindow(tempWindow);
        SDL_Quit();
        return -1;
    }

    SDL_DestroyWindow(tempWindow);

    return mode.refresh_rate;
}

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return false;
    }

    gWindow = SDL_CreateWindow("Simple Paint Application",
                               SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (gWindow == nullptr) {
        return false;
    }

    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    if (gRenderer == nullptr) {
        return false;
    }

    canvasTexture =
            SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888,
                              SDL_TEXTUREACCESS_TARGET, CANVAS_WIDTH, CANVAS_HEIGHT);
    SDL_SetRenderTarget(gRenderer, canvasTexture);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderTarget(gRenderer, nullptr);

    return true;
}

/*bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    return false;
  }

  gWindow = SDL_CreateWindow("Simple Paint Application",
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (gWindow == NULL) {
    return false;
  }

  gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
  if (gRenderer == NULL) {
    return false;
  }

  SDL_SetRenderDrawColor(gRenderer, 50, 50, 50, 255);
  SDL_RenderClear(gRenderer);

  SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
  SDL_RenderFillRect(gRenderer, &canvasRect);

  canvasTexture =
      SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_RGBA8888,
                        SDL_TEXTUREACCESS_TARGET, CANVAS_WIDTH, CANVAS_HEIGHT);
  SDL_SetRenderTarget(gRenderer, canvasTexture);
  SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
  SDL_RenderClear(gRenderer);
  SDL_SetRenderTarget(gRenderer, NULL);

  return true;
}*/

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_DestroyTexture(canvasTexture);
    gWindow = nullptr;
    gRenderer = nullptr;

    SDL_Quit();
}

void drawCanvas() {

    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);

    SDL_Rect canvasRect = {CANVAS_X, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT};
    SDL_RenderCopy(gRenderer, canvasTexture, nullptr, &canvasRect);

    SDL_RenderPresent(gRenderer);
}

/*void drawCanvas() {
  SDL_SetRenderDrawColor(gRenderer, 50, 50, 50, 255);
  SDL_RenderClear(gRenderer);

  SDL_Rect canvasRect = {10, CANVAS_Y, CANVAS_WIDTH, CANVAS_HEIGHT};
  SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
  SDL_RenderFillRect(gRenderer, &canvasRect);

  SDL_RenderCopy(gRenderer, canvasTexture, NULL, &canvasRect);

  SDL_RenderPresent(gRenderer);
}*/

void clearCanvas() {

    SDL_SetRenderTarget(gRenderer, canvasTexture);
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    SDL_RenderClear(gRenderer);
    SDL_SetRenderTarget(gRenderer, nullptr);

    SDL_RenderCopy(gRenderer, canvasTexture, nullptr, nullptr);

    SDL_RenderPresent(gRenderer);
}

void draw(int x, int y, SDL_Color color) {
    if (x >= CANVAS_X && x < CANVAS_X + CANVAS_WIDTH && y >= CANVAS_Y &&
        y < CANVAS_Y + CANVAS_HEIGHT) {

        x -= CANVAS_X;
        y -= CANVAS_Y;

        SDL_SetRenderTarget(gRenderer, canvasTexture);
        SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);

        for (int i = x - pixelSize / 2; i < x + pixelSize / 2; i++) {
            for (int j = y - pixelSize / 2; j < y + pixelSize / 2; j++) {
                SDL_RenderDrawPoint(gRenderer, i, j);
            }
        }

        SDL_SetRenderTarget(gRenderer, nullptr);
    }
}

void drawCircle(int x, int y, int radius, SDL_Color color) {
    if (x >= CANVAS_X && x < CANVAS_X + CANVAS_WIDTH && y >= CANVAS_Y && y < CANVAS_Y + CANVAS_HEIGHT) {
        x -= CANVAS_X;
        y -= CANVAS_Y;

        SDL_SetRenderTarget(gRenderer, canvasTexture);
        SDL_SetRenderDrawColor(gRenderer, color.r, color.g, color.b, color.a);

        // for (int i = 0; i < 360; i++) {
        //   double angle = i * M_PI / 180;
        //   int circleX = x + radius * cos(angle);
        //   int circleY = y + radius * sin(angle);
        //   SDL_RenderDrawPoint(gRenderer, circleX, circleY);
        // }
        for (int i = -radius; i <= radius; i++) {
            for (int j = -radius; j <= radius; j++) {
                if (i * i + j * j <= radius * radius) {
                    SDL_RenderDrawPoint(gRenderer, x + i, y + j);
                }
            }
        }

        SDL_SetRenderTarget(gRenderer, nullptr);
    }
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char *args[]) {

    int targetFPS = get_fps();

    if (targetFPS == -1) {
        return 1;
    }

    Uint32 ms_per_frame = 1000 / targetFPS;

    if (!init()) {
        return 1;
    }

    bool quit = false;
    int x = 0, y = 0;
    int mode = 1; // 0 rect, 1 crcl

    Uint32 frameStart, frameTime;

    SDL_Event e;
    SDL_Color drawingColor = {0, 0, 0, 255};

    while (!quit) {

        frameStart = SDL_GetTicks();

        while (SDL_PollEvent(&e) != 0) {

            if (e.type == SDL_QUIT) {
                quit = true;
            }

            if (e.type == SDL_MOUSEMOTION &&
                e.motion.state & SDL_BUTTON_LMASK) {
                x = e.motion.x;
                y = e.motion.y;

                if (mode) {
                    drawCircle(x, y, pixelSize / 2, drawingColor);
                } else {
                    draw(x, y, drawingColor);
                }
            } else if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_a:
                        drawingColor = {255, 0, 0, 255};
                        break;
                    case SDLK_s:
                        drawingColor = {0, 255, 0, 255};
                        break;
                    case SDLK_d:
                        drawingColor = {0, 0, 255, 255};
                        break;
                    case SDLK_w:
                        drawingColor = {0, 0, 0, 255};
                        break;
                    default:
                        break;
                }
            }

            if (SDL_GetModState() & KMOD_CTRL) {

                if (e.type == SDL_MOUSEWHEEL) {
                    if (e.wheel.which == 0) {
                        if (e.wheel.y > 0) {
                            pixelSize = (pixelSize < 254) ? pixelSize + 1 : 254;
                        } else if (e.wheel.y < 0) {
                            pixelSize = (pixelSize > 2) ? pixelSize - 1 : 2;
                        }
                    }

                    printf("%d\n", pixelSize);
                }

                if (e.key.keysym.sym == SDLK_r) {
                    pixelSize = PX_DEFAULT;
                    printf("%d\n", pixelSize);
                }

                if (e.key.keysym.sym == SDLK_c) {
                    clearCanvas();
                }
            }

            if (SDL_GetModState() & KMOD_SHIFT) {
                if (e.key.keysym.sym == SDLK_c) {
                    mode = 1;
                    printf("circle\n");
                }
                if (e.key.keysym.sym == SDLK_r) {
                    mode = 0;
                    printf("rectangle\n");
                }
            }
        }

        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_BLEND);
        SDL_SetRenderDrawColor(gRenderer, drawingColor.r, drawingColor.g, drawingColor.b, drawingColor.a);
        SDL_SetRenderDrawBlendMode(gRenderer, SDL_BLENDMODE_NONE);
        SDL_RenderDrawPoint(gRenderer, x, y);

        drawCanvas();

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < ms_per_frame) {
            SDL_Delay(ms_per_frame - frameTime);
        }
    }

    close();
    return 0;
}

/*

KEYBINDINGS:
LMB : draw
W   : black
A   : red
S   : green
D   : blue

CTRL+
  SCROLL+
    UP    : increase px size
    DOWN  : deccrease px size
  C : clear
  R : default px size

SHIFT+
  C : circle brush
  R : rectangle brush

*/
