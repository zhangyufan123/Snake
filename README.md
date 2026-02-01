

---

# 🐍 Snake Game – C++ with FLTK

A classic Snake game implemented in **C++** using the **[FLTK (Fast Light Toolkit)](https://www.fltk.org/)** graphical user interface library. Supports customizable grid size and difficulty levels.

---

## 🖼️ Screenshot

*(Optional: Add a screenshot later if desired)*  
The game features:
- Grid-based snake movement
- Keyboard controls (arrow keys)
- Food spawning
- Collision detection (walls & self)
- Three difficulty levels (Easy, Medium, Hard)

---

## 🛠️ Features

- **Customizable board size**: Set width and height at startup.
- **Three difficulty levels**:
  - **Easy**: 0.3 seconds per move
  - **Medium**: 0.2 seconds per move
  - **Hard**: 0.1 seconds per move
- **Real-time rendering** with smooth animation
- **Keyboard input**: Use arrow keys to control direction
- **Automatic restart** on game over (can be modified to manual restart)

---

## 📦 Dependencies

You need the following installed to build and run this program:

- **C++ compiler** (e.g., `g++`)
- **FLTK 1.3 or higher**
- Standard Linux development libraries (`X11`, `pthread`, etc.)

### On Ubuntu/Debian:
```bash
sudo apt update
sudo apt install g++ libfltk1.3-dev
```

### On Fedora/RHEL:
```bash
sudo dnf install gcc-c++ fltk-devel
```

---

## 🧱 Building the Project

1. Clone or download the source code (`Snake.cpp`).
2. Open a terminal in the project directory.
3. Compile with:

```bash
g++ -o snake Snake.cpp $(pkg-config --cflags --libs fltk)
```

> 💡 Using `pkg-config` ensures all required libraries (`X11`, `fontconfig`, etc.) are linked automatically.

---

## ▶️ Running the Game

Run the executable:

```bash
./snake
```

You will be prompted to enter:

```
Enter the width, height, and level (0=EASY, 1=MEDIUM, 2=HARD):
```

Example input:
```
20 15 1
```
→ Creates a 20×15 grid at medium difficulty.

> ⚠️ The actual window size is `width × 20` by `height × 20` pixels (each cell is 20×20 px).

---

## 🎮 Controls

| Key          | Action                     |
|--------------|----------------------------|
| ↑ (Up)       | Move snake upward          |
| ↓ (Down)     | Move snake downward        |
| ← (Left)     | Move snake left            |
| → (Right)    | Move snake right           |

> The game starts moving right automatically.

---

## 📁 Project Structure

```
Snake.cpp       # Main source file (contains full implementation)
README.md       # This file
```

> This is a single-file project for simplicity.

---

## 🔒 License

This project is free and open-source. You may use, modify, and distribute it under the **MIT License**.

---

## 🙌 Acknowledgements

- Built with [FLTK](https://www.fltk.org/) – a lightweight C++ GUI toolkit.
- Inspired by the classic Nokia Snake game.

---

## 🐞 Reporting Issues

Found a bug or have a suggestion? Please open an issue on your local repository or contact the author.

---

> ✨ Enjoy the game! 🐍