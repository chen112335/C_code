/*
 * 贪吃蛇小游戏 - 标准C语言实现
 * 编译命令 (MinGW-gcc):  gcc snake.c -o snake -std=c11
 * 运行:  ./snake
 *
 * 操作说明:
 *   ↑ W   上
 *   ↓ S   下
 *   ← A   左
 *   → D   右
 *   空格   暂停/继续
 *   Q     退出游戏
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>           /* MinGW自带: _kbhit(), _getch() */
#include <time.h>            /* clock() 用于帧率控制 */

 /* ============== 游戏常量 ============== */
#define MAP_WIDTH       40   /* 地图宽度 (列数) */
#define MAP_HEIGHT      20   /* 地图高度 (行数) */
#define INIT_LENGTH      4   /* 蛇的初始长度 */
#define INIT_DIR         1   /* 初始方向: 0上 1右 2下 3左 */
#define FRAME_MS       120   /* 每帧间隔毫秒数, 值越小蛇越快 */

/* 方向对应的坐标增量 (y, x) */
static const int DY[] = { -1,  0,  1,  0 };   /* 上 右 下 左 */
static const int DX[] = { 0,  1,  0, -1 };

/* ============== 数据结构 ============== */
typedef struct {
    int x;
    int y;
} Point;

/* ============== 全局变量 ============== */
Point   snake[MAP_WIDTH * MAP_HEIGHT]; /* 用数组存蛇身, 最多填满地图 */
int     snake_len;                     /* 当前蛇身长度 */
int     dir;                           /* 当前方向 */
Point   food;                          /* 食物坐标 */
int     score;                         /* 当前得分 */
int     game_over;                     /* 游戏结束标志 */
int     paused;                        /* 暂停标志 */

/* ============== 终端控制 ============== */

/* 设置光标位置 (ANSI转义序列, row/col 从1开始) */
static void gotoxy(int row, int col) {
    printf("\033[%d;%dH", row, col);
}

static void hide_cursor(void) { printf("\033[?25l"); fflush(stdout); }
static void show_cursor(void) { printf("\033[?25h"); fflush(stdout); }
static void clear_screen(void) { printf("\033[2J");  fflush(stdout); }

/* 在逻辑坐标 (y, x) 处画一个字符 (边框占了第0行/第0列) */
static void draw_char(int y, int x, char ch) {
    gotoxy(y + 1, x + 1);    /* 偏移到边框内 */
    putchar(ch);
    fflush(stdout);
}

/* ============== 游戏辅助 ============== */

/* 随机生成一个不与蛇重叠的食物坐标 */
static void spawn_food(void) {
    int valid;
    do {
        food.x = rand() % MAP_WIDTH;
        food.y = rand() % MAP_HEIGHT;
        valid = 1;
        for (int i = 0; i < snake_len; i++) {
            if (snake[i].x == food.x && snake[i].y == food.y) {
                valid = 0;
                break;
            }
        }
    } while (!valid);
}

/* 画一次完整边框 (只在开始时调用) */
static void draw_frame(void) {
    clear_screen();
    hide_cursor();

    /* 顶边框 */
    printf("+");
    for (int x = 0; x < MAP_WIDTH; x++) printf("-");
    printf("+\n");

    /* 中间主体 */
    for (int y = 0; y < MAP_HEIGHT; y++) {
        printf("|");
        for (int x = 0; x < MAP_WIDTH; x++) printf(" ");
        printf("|\n");
    }

    /* 底边框 */
    printf("+");
    for (int x = 0; x < MAP_WIDTH; x++) printf("-");
    printf("+\n");

    fflush(stdout);
}

/* 更新底部状态栏 */
static void draw_status(void) {
    gotoxy(MAP_HEIGHT + 2, 1);
    printf(" 得分: %-6d", score);
    if (paused) printf("  [ 已暂停 ]");
    printf("   方向:WASD/箭头  暂停:空格  退出:Q   ");
    fflush(stdout);
}

/* ============== 游戏主逻辑 ============== */

static void game_init(void) {
    srand((unsigned)time(NULL));
    snake_len = INIT_LENGTH;
    dir = INIT_DIR;
    score = 0;
    game_over = 0;
    paused = 0;

    /* 蛇初始水平居中, 头朝右 */
    int sy = MAP_HEIGHT / 2;
    int sx = MAP_WIDTH / 2;
    for (int i = 0; i < snake_len; i++) {
        snake[i].x = sx - i;
        snake[i].y = sy;
    }
    spawn_food();
}

/* 非阻塞键盘轮询 */
static void handle_input(void) {
    while (_kbhit()) {
        int ch = _getch();
        /* 方向键会先返回 0xE0/0x00 再返回方向码 */
        if (ch == 0xE0 || ch == 0) {
            ch = _getch();
            switch (ch) {
            case 72: if (dir != 2) dir = 0; break; /* ↑ */
            case 77: if (dir != 3) dir = 1; break; /* → */
            case 80: if (dir != 0) dir = 2; break; /* ↓ */
            case 75: if (dir != 1) dir = 3; break; /* ← */
            }
        }
        else {
            switch (ch) {
            case 'w': case 'W': if (dir != 2) dir = 0; break;
            case 'd': case 'D': if (dir != 3) dir = 1; break;
            case 's': case 'S': if (dir != 0) dir = 2; break;
            case 'a': case 'A': if (dir != 1) dir = 3; break;
            case ' ': paused = !paused; draw_status(); break;
            case 'q': case 'Q': game_over = 1; break;
            }
        }
    }
}

/* 推进一帧 */
static void update(void) {
    /* 1. 新头坐标 */
    Point head;
    head.x = snake[0].x + DX[dir];
    head.y = snake[0].y + DY[dir];

    /* 2. 撞墙检测 */
    if (head.x < 0 || head.x >= MAP_WIDTH ||
        head.y < 0 || head.y >= MAP_HEIGHT) {
        game_over = 1;
        return;
    }

    /* 3. 撞自己检测 (尾巴下帧会移走, 所以不比较尾巴) */
    for (int i = 0; i < snake_len - 1; i++) {
        if (snake[i].x == head.x && snake[i].y == head.y) {
            game_over = 1;
            return;
        }
    }

    /* 4. 是否吃到食物 */
    int ate = (head.x == food.x && head.y == food.y);

    /* 5. 如果没吃到食物, 尾巴要移走 —— 先把尾巴位置清空 */
    if (!ate) {
        draw_char(snake[snake_len - 1].y, snake[snake_len - 1].x, ' ');
    }
    else {
        score += 10;
        spawn_food();
    }

    /* 6. 整体后移: 身体从后往前拷贝 */
    if (ate) {
        snake_len++;                       /* 加一节, 数组腾出空间 */
    }
    /* 从尾到头依次后移一格, 最后把头放到位置0 */
    for (int i = snake_len - 1; i > 0; i--) {
        snake[i] = snake[i - 1];
    }
    snake[0] = head;

    /* 7. 画出新的头 */
    draw_char(snake[0].y, snake[0].x, '@');
    /* 8. 重新画出身体 (头部已经画过, 从第1节开始) */
    for (int i = 1; i < snake_len; i++) {
        draw_char(snake[i].y, snake[i].x, 'o');
    }
    /* 9. 画食物 */
    draw_char(food.y, food.x, '*');
}

/* 游戏结束提示 */
static void show_game_over(void) {
    gotoxy(MAP_HEIGHT + 2, 1);
    printf(" 游戏结束!  最终得分: %d    按 R 重开 / Q 退出", score);
    fflush(stdout);

    for (;;) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'q' || ch == 'Q') break;
            if (ch == 'r' || ch == 'R') {
                /* 重新开一局 */
                game_init();
                draw_frame();
                draw_status();
                update();  /* 画出初始蛇和食物 */
                return;
            }
        }
    }
}

/* ============== 主函数 ============== */
int main(void) {
    game_init();
    draw_frame();
    draw_status();
    /* 先画出初始蛇和食物 */
    update();

    clock_t last_tick = clock();

    while (!game_over) {
        handle_input();

        if (!paused) {
            clock_t now = clock();
            /* 到达帧间隔才推进, clock() 返回值单位是 CLOCKS_PER_SEC 之一 */
            if ((now - last_tick) * 1000 / CLOCKS_PER_SEC >= FRAME_MS) {
                update();
                draw_status();
                last_tick = now;
            }
        }
    }

    show_game_over();
    show_cursor();
    return 0;
}