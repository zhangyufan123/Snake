#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Box.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <FL/fl_message.H>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};

enum Direction { UP, DOWN, LEFT, RIGHT };
enum Level { EASY, MEDIUM, HARD };

class SnakeGame : public Fl_Widget {
public:
    static const int GRID_SIZE = 20;      // 每格像素大小

    SnakeGame(int w, int h, int level);

    void reset();
    static void timer_callback(void* data);
    void move();
    void changeDirection(Direction dir);
    bool checkCollision(const Point& p);
    void spawnFood();

protected:
    void draw() override;
    int handle(int event) override;

private:
    std::vector<Point> snake;
    Point food;
    Direction dir;
    bool gameRunning;
    static const int WIDTH;          // 网格列数
    static const int HEIGHT;         // 网格行数
    int level;
};

const int SnakeGame::WIDTH = 20;
const int SnakeGame::HEIGHT = 20;

SnakeGame::SnakeGame(int w, int h, int level)
    : Fl_Widget(10, 10, w, h), dir(RIGHT), gameRunning(true), level(level) {
    reset();
    Fl::add_timeout(1, timer_callback, this);
}

void SnakeGame::timer_callback(void* data) {
    SnakeGame* game = static_cast<SnakeGame*>(data);
    game->move();
    Fl::repeat_timeout(1, timer_callback, data); // 递归安排下一次
}

void SnakeGame::reset() {
    snake.clear();
    snake.push_back(Point(WIDTH / 2, HEIGHT / 2));
    dir = RIGHT;
    gameRunning = true;
    spawnFood();
}

void SnakeGame::move() {
    if (!gameRunning) return;

    Point head = snake.front();
    switch (dir) {
        case UP: head.y--; break;
        case DOWN: head.y++; break;
        case LEFT: head.x--; break;
        case RIGHT: head.x++; break;
    }

    if (checkCollision(head)) {
        gameRunning = false;
        redraw();
        return;
    }

    snake.insert(snake.begin(), head);
    if (head == food) {
        spawnFood();
    } else {
        snake.pop_back();
    }
    redraw();
}

void SnakeGame::changeDirection(Direction newDir) {
    if ((newDir == UP && dir != DOWN) ||
        (newDir == DOWN && dir != UP) ||
        (newDir == LEFT && dir != RIGHT) ||
        (newDir == RIGHT && dir != LEFT)) {
        dir = newDir;
    }
}

bool SnakeGame::checkCollision(const Point& p) {
    if (p.x < 0 || p.x >= WIDTH || p.y < 0 || p.y >= HEIGHT) {
        return true;
    }
    for (size_t i = 1; i < snake.size(); ++i) {
        if (snake[i] == p) {
            return true;
        }
    }
    return false;
}

void SnakeGame::spawnFood() {
    int fx, fy;
    fx = rand() % WIDTH;
    fy = rand() % HEIGHT;
    food = Point(fx, fy);
}

int SnakeGame::handle(int event) {
    if (event == FL_KEYDOWN) {
        switch (Fl::event_key()) {
            case FL_Up: changeDirection(UP); break;
            case FL_Down: changeDirection(DOWN); break;
            case FL_Left: changeDirection(LEFT); break;
            case FL_Right: changeDirection(RIGHT); break;
        }
        return 1;
    }
    return Fl_Widget::handle(event);
}

void SnakeGame::draw() {
    fl_color(FL_WHITE);
    fl_rectf(x(), y(), w(), h());

    //绘制网格
    fl_color(FL_LIGHT2);
    for (int i = 0; i <= WIDTH; ++i) {
        fl_line(x() + i * GRID_SIZE, y(), x() + i * GRID_SIZE, y() + h());
    }
    for (int i = 0; i <= HEIGHT; ++i) {
        fl_line(x(), y() + i * GRID_SIZE, x() + w(), y() + i * GRID_SIZE);
    }

    // 绘制蛇
    fl_color(FL_GREEN);
    for (const Point& segment : snake) {
        fl_rectf(x() + segment.x * GRID_SIZE, y() + segment.y * GRID_SIZE, GRID_SIZE, GRID_SIZE);
    }

    // 绘制食物
    fl_color(FL_RED);
    fl_rectf(x() + food.x * GRID_SIZE, y() + food.y * GRID_SIZE, GRID_SIZE, GRID_SIZE);

    if (!gameRunning) {
        fl_color(FL_BLACK);
        fl_draw("Game Over!", x() + w() / 2 - 40, y() + h() / 2);
        reset();
        redraw();
    }
}

int main() {
    cout << "Enter the width, height, and level (0=EASY, 1=MEDIUM, 2=HARD): ";
    int width, height, level;
    cin >> width >> height >> level;
    Fl_Window window(width * 20 + 20, height * 20 + 20, "Snake");
    SnakeGame game(width * 20, height * 20, static_cast<Level>(level));
    window.end();
    window.show();
    return Fl::run();
}