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
#include <sstream>  // 添加字符串流支持
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
    const int WIDTH;          // 网格列数，改为实例常量
    const int HEIGHT;         // 网格行数，改为实例常量
    int level;
    double speed;
};

SnakeGame::SnakeGame(int w, int h, int level)
    : Fl_Widget(10, 10, w, h), 
      WIDTH(w/20), 
      dir(RIGHT), 
      gameRunning(true), 
      HEIGHT(h/20),
      level(level) {
    reset();

    switch (level){
        case 0:
            speed = 1;
            break;
        case 1:
            speed = 0.5;
            break;
        case 2:
            speed = 0.2;
            break;
    }

    Fl::add_timeout(speed, timer_callback, this);
    // Add this line to make widget take focus
    Fl::focus(this);
}

void SnakeGame::timer_callback(void* data) {
    SnakeGame* game = static_cast<SnakeGame*>(data);
    game->move();
    Fl::repeat_timeout(game->speed, timer_callback, data); // 递归安排下一次
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
            case 'r': reset();
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
        string msg = "Game Over!";
        //fl_draw("Game Over!", x() + w() / 2 - 40, y() + h() / 2);
        fl_message("%s", msg.c_str());
        reset();
        redraw();
    }
}

int main() {
    cout << "Enter the width, height, and level (0=EASY, 1=MEDIUM, 2=HARD) separated by spaces (press Enter for defaults 20 20 0): ";
    
    string input_line;
    getline(cin, input_line);  // 读取整行输入
    
    int width = 20, height = 20, level = 0;  // 默认值
    
    // 如果输入不为空，则尝试解析
    if (!input_line.empty()) {
        stringstream ss(input_line);
        if (!(ss >> width >> height >> level)) {
            // 解析失败，使用默认值
            cout << "Invalid input format. Using default values: width=" << width 
                 << ", height=" << height << ", level=" << level << endl;
        } else {
            // 解析成功，验证level范围
            if (level < 0 || level > 2) {
                cout << "Invalid level. Using default level: 0 (EASY)" << endl;
                level = 0;
            }
        }
    } else {
        cout << "No input provided. Using default values: width=" << width 
             << ", height=" << height << ", level=" << level << endl;
    }
    
    Fl_Window window(width * 20 + 20, height * 20 + 20, "Snake");
    SnakeGame game(width * 20, height * 20, level);
    window.end();
    window.show();
    return Fl::run();
}