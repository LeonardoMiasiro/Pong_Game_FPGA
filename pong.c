#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "address_map_arm.h"

// Definições
#define SCREEN_WIDTH  320
#define SCREEN_HEIGHT 240
#define BAR_WIDTH     5
#define BAR_HEIGHT    30
#define BALL_RADIUS   3
#define WHITE         0xFFFF
#define BLACK         0x0000

// Estado do jogo
int ball_x = SCREEN_WIDTH / 2;
int ball_y = SCREEN_HEIGHT / 2;
int ball_dx = 2;
int ball_dy = 2;
int ball_speed_counter = 0;

int bar1_y = 100;
int bar2_y = 100;

int score1 = 0;
int score2 = 0;

// Funções auxiliares
void video_text(int x, int y, char *text);
void video_box(int x1, int y1, int x2, int y2, short color);
void draw_circle(int cx, int cy, int r, short color);
void draw_game_objects();
void clear_game_objects();
void update_ball();
void read_inputs();
void draw_score();

// Main
int main() {
    video_box(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, BLACK); // Limpa a tela

    int frame_counter = 0;
    const int frame_threshold = 4;

    while (1) {
        clear_game_objects();
        read_inputs();

        if (frame_counter >= frame_threshold) {
            update_ball();
            frame_counter = 0;
        } else {
            frame_counter++;
        }

        draw_game_objects();
        draw_score();

        volatile int d;
        for (d = 0; d < 10000; d++);
    }

    return 0;
}

// =================== Funções do jogo ===================

void update_ball() {
    ball_x += ball_dx;
    ball_y += ball_dy;

    // Colisão com topo/fundo
    if (ball_y - BALL_RADIUS <= 0 || ball_y + BALL_RADIUS >= SCREEN_HEIGHT)
        ball_dy = -ball_dy;

    // Colisão com barras
    if ((ball_x - BALL_RADIUS <= 5 + BAR_WIDTH &&
         ball_y >= bar1_y && ball_y <= bar1_y + BAR_HEIGHT) ||
        (ball_x + BALL_RADIUS >= SCREEN_WIDTH - 5 - BAR_WIDTH &&
         ball_y >= bar2_y && ball_y <= bar2_y + BAR_HEIGHT)) {
        ball_dx = -ball_dx;
        ball_speed_counter++;

        if (ball_speed_counter >= 10) {
            if (ball_dx > 0) ball_dx++;
            else ball_dx--;
            if (ball_dy > 0) ball_dy++;
            else ball_dy--;
            ball_speed_counter = 0;
        }
    }

    // Saiu da tela: pontuação
    if (ball_x < 0) {
        score2++;
        ball_x = SCREEN_WIDTH / 2;
        ball_y = SCREEN_HEIGHT / 2;
        ball_dx = 2;
        ball_dy = 2;
        ball_speed_counter = 0;
    }
    if (ball_x > SCREEN_WIDTH) {
        score1++;
        ball_x = SCREEN_WIDTH / 2;
        ball_y = SCREEN_HEIGHT / 2;
        ball_dx = -2;
        ball_dy = 2;
        ball_speed_counter = 0;
    }
}

void read_inputs() {
    volatile uint32_t *key_ptr = (uint32_t *)KEY_BASE;
    uint32_t key = ~(*key_ptr) & 0xF;

    // Player 1
    if ((key & 0x1) && bar1_y > 0)
        bar1_y -= 1;
    if ((key & 0x2) && bar1_y + BAR_HEIGHT < SCREEN_HEIGHT)
        bar1_y += 1;

    // Player 2
    if ((key & 0x4) && bar2_y > 0)
        bar2_y -= 1;
    if ((key & 0x8) && bar2_y + BAR_HEIGHT < SCREEN_HEIGHT)
        bar2_y += 1;
}

void draw_game_objects() {
    // Barras
    video_box(5, bar1_y, 5 + BAR_WIDTH, bar1_y + BAR_HEIGHT, WHITE);
    video_box(SCREEN_WIDTH - 5 - BAR_WIDTH, bar2_y,
              SCREEN_WIDTH - 5, bar2_y + BAR_HEIGHT, WHITE);

    // Bola redonda
    draw_circle(ball_x, ball_y, BALL_RADIUS, WHITE);
}

void clear_game_objects() {
    // Limpa barras
    video_box(5, bar1_y, 5 + BAR_WIDTH, bar1_y + BAR_HEIGHT, BLACK);
    video_box(SCREEN_WIDTH - 5 - BAR_WIDTH, bar2_y,
              SCREEN_WIDTH - 5, bar2_y + BAR_HEIGHT, BLACK);

    // Limpa bola
    draw_circle(ball_x, ball_y, BALL_RADIUS, BLACK);
}

// =================== VGA - Texto ===================

void video_text(int x, int y, char *text) {
    int offset;
    volatile char *char_buffer = (char *)FPGA_CHAR_BASE;
    offset = (y << 7) + x;
    while (*text) {
        *(char_buffer + offset) = *text;
        ++text;
        ++offset;
    }
}

void draw_score() {
    char text[40];
    sprintf(text, "P1: %d   P2: %d", score1, score2);
    video_text(30, 1, text);
}

// =================== VGA - Gráficos ===================

void video_box(int x1, int y1, int x2, int y2, short color) {
    int pixel_ptr;
    int row, col;

    volatile short *pixel_buf = (short *)FPGA_PIXEL_BUF_BASE;

    for (row = y1; row <= y2; row++) {
        for (col = x1; col <= x2; col++) {
            pixel_ptr = (row << 9) + col;
            *(pixel_buf + pixel_ptr) = color;
        }
    }
}

void draw_circle(int cx, int cy, int r, short color) {
    int x, y;
    volatile short *pixel_buf = (short *)FPGA_PIXEL_BUF_BASE;
    for (y = -r; y <= r; y++) {
        for (x = -r; x <= r; x++) {
            if (x * x + y * y <= r * r) {
                int px = cx + x;
                int py = cy + y;
                if (px >= 0 && px < SCREEN_WIDTH && py >= 0 && py < SCREEN_HEIGHT) {
                    int offset = (py << 9) + px;
                    *(pixel_buf + offset) = color;
                }
            }
        }
    }
}
