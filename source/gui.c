#include <3ds.h>
#include <citro2d.h>

#define BUTTON_HEIGHT 40
#define BUTTON_WIDTH 200

typedef struct {
    int x, y, w, h;
    const char* label;
    u32 color;
} Button;

void drawButton(Button *b) {
    C2D_DrawRectSolid(b->x, b->y, 0, b->w, b->h, b->color);
}

int buttonTouched(Button *b, touchPosition *touch) {
    return (touch->px >= b->x && touch->px <= b->x + b->w &&
            touch->py >= b->y && touch->py <= b->y + b->h);
}

Button makeButton(int x, int y, const char* label, u32 color) {
    Button b;
    b.x = x; b.y = y;
    b.w = BUTTON_WIDTH; b.h = BUTTON_HEIGHT;
    b.label = label; b.color = color;
    return b;
}
