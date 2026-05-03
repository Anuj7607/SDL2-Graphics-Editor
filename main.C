#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

#define WIDTH 900
#define HEIGHT 600

SDL_Window* window;
SDL_Renderer* renderer;
bool running = true;

// Brush settings
SDL_Color currentColor = {255, 0, 0, 255};
int brushSize = 5;

// Tool types
typedef enum { TOOL_BRUSH, TOOL_LINE, TOOL_RECT, TOOL_CIRCLE, TOOL_ERASER } Tool;
Tool currentTool = TOOL_BRUSH;

// Mouse
typedef struct {
    bool down;
    int x, y;
    int startX, startY;
} Mouse;

Mouse mouse = {false,0,0,0,0};

// Button structure
typedef struct {
    SDL_Rect rect;
    char label[20];
    Tool tool;
    bool isToolBtn;
} Button;

Button toolButtons[5];
SDL_Rect sliderTrack = {10, 550, 200, 6};
SDL_Rect sliderKnob;

// Colors
SDL_Rect colorButtons[6];
SDL_Color palette[6] = {
    {255,0,0,255}, {0,255,0,255}, {0,0,255,255},
    {255,255,0,255}, {255,165,0,255}, {255,255,255,255}
};

SDL_Rect clearBtn = {750, 20, 120, 40};

void initUI() {
    // Toolbar tool buttons
    int x = 10;
    char *names[5] = {"BRUSH","LINE","RECT","CIRCLE","ERASER"};

    for(int i=0;i<5;i++){
        toolButtons[i].rect = (SDL_Rect){x, 20, 100, 40};
        snprintf(toolButtons[i].label, 20, "%s", names[i]);
        toolButtons[i].tool = i;
        toolButtons[i].isToolBtn = true;
        x += 110;
    }

    // Color palette
    int cx = 10;
    for(int i=0;i<6;i++){
        colorButtons[i] = (SDL_Rect){cx, 80, 40, 40};
        cx += 50;
    }

    // Brush slider knob
    sliderKnob = (SDL_Rect){10 + brushSize * 4, 543, 12, 20};
}

void drawUI() {
    // Toolbar background
    SDL_SetRenderDrawColor(renderer, 230,230,230,255);
    SDL_Rect topBar = {0,0, WIDTH, 130};
    SDL_RenderFillRect(renderer, &topBar);

    // Draw tool buttons
    for(int i=0;i<5;i++){
        if(currentTool == toolButtons[i].tool)
            SDL_SetRenderDrawColor(renderer, 180,210,255,255);
        else
            SDL_SetRenderDrawColor(renderer, 200,200,200,255);

        SDL_RenderFillRect(renderer, &toolButtons[i].rect);

        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_Surface* surf = SDL_CreateRGBSurfaceWithFormat(0, 100, 40, 32, SDL_PIXELFORMAT_RGBA32);
        SDL_FillRect(surf,NULL,SDL_MapRGBA(surf->format,200,200,200,0));
        SDL_Texture* text = SDL_CreateTextureFromSurface(renderer, surf);
        SDL_FreeSurface(surf);
    }

    // Color buttons
    for(int i=0;i<6;i++){
        SDL_SetRenderDrawColor(renderer,
            palette[i].r, palette[i].g, palette[i].b, 255);
        SDL_RenderFillRect(renderer, &colorButtons[i]);
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
    }

    // Clear button
    SDL_SetRenderDrawColor(renderer,255,120,120,255);
    SDL_RenderFillRect(renderer,&clearBtn);

    // Slider track
    SDL_SetRenderDrawColor(renderer,150,150,150,255);
    SDL_RenderFillRect(renderer,&sliderTrack);

    // Slider knob
    SDL_SetRenderDrawColor(renderer,80,80,250,255);
    SDL_RenderFillRect(renderer,&sliderKnob);
}

bool inside(SDL_Rect r, int x, int y) {
    return (x >= r.x && x <= r.x+r.w && y >= r.y && y <= r.y+r.h);
}

void drawBrush(int x,int y){
    SDL_SetRenderDrawColor(renderer,currentColor.r,currentColor.g,currentColor.b,255);
    SDL_Rect r = {x - brushSize/2, y - brushSize/2, brushSize, brushSize};
    SDL_RenderFillRect(renderer,&r);
}

void drawLine(int x1,int y1,int x2,int y2){ SDL_SetRenderDrawColor(renderer,currentColor.r,currentColor.g,currentColor.b,255); SDL_RenderDrawLine(renderer,x1,y1,x2,y2);} 
void drawRect(int x,int y,int w,int h){ SDL_SetRenderDrawColor(renderer,currentColor.r,currentColor.g,currentColor.b,255); SDL_Rect r={x,y,w,h}; SDL_RenderDrawRect(renderer,&r);} 
void drawCircle(int cx,int cy,int r){ SDL_SetRenderDrawColor(renderer,currentColor.r,currentColor.g,currentColor.b,255); for(int w=-r;w<r;w++){ for(int h=-r;h<r;h++){ if(w*w+h*h<=r*r) SDL_RenderDrawPoint(renderer,cx+w,cy+h); } }}

void handlePress(int mx,int my){
    // Tools
    for(int i=0;i<5;i++){
        if(inside(toolButtons[i].rect,mx,my)){ currentTool = toolButtons[i].tool; return; }
    }

    // Colors
    for(int i=0;i<6;i++){
        if(inside(colorButtons[i],mx,my)){ currentColor = palette[i]; return; }
    }

    // Clear
    if(inside(clearBtn,mx,my)){
        SDL_SetRenderDrawColor(renderer,0,0,0,255);
        SDL_RenderClear(renderer);
        return;
    }

    // Slider
    if(inside(sliderKnob,mx,my)){
        sliderKnob.x = mx - sliderKnob.w/2;
        brushSize = (sliderKnob.x - 10) / 4;
        if(brushSize < 1) brushSize = 1;
        if(brushSize > 50) brushSize = 50;
    }
}

int main(){
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Modern SDL2 Graphics Editor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);

    initUI();

    while(running){
        SDL_Event e;
        while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=false;

            if(e.type==SDL_MOUSEBUTTONDOWN){
                mouse.down=true;
                SDL_GetMouseState(&mouse.x,&mouse.y);
                mouse.startX = mouse.x;
                mouse.startY = mouse.y;
                handlePress(mouse.x,mouse.y);
            }
            if(e.type==SDL_MOUSEBUTTONUP) mouse.down=false;
            if(e.type==SDL_MOUSEMOTION) SDL_GetMouseState(&mouse.x,&mouse.y);
        }

        if(mouse.down && mouse.y > 140){
            if(currentTool == TOOL_BRUSH) drawBrush(mouse.x,mouse.y);
            else if(currentTool == TOOL_ERASER){ SDL_SetRenderDrawColor(renderer,0,0,0,255); drawBrush(mouse.x,mouse.y);}        }

        drawUI();
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
