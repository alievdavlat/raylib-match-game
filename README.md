# Raylib Match-3 Game

A match-3 puzzle game written in C with [raylib](https://www.raylib.com/).

![Gameplay](docs/screenshot.png)

## Play the game

**Windows:** [download the latest release](https://github.com/alievdavlat/raylib-match-game/releases/latest), unzip it and run `match-game.exe`. No installation needed.

Windows may show "Windows protected your PC" on first launch because the game is not code-signed. Click **More info**, then **Run anyway**.

## How to play

1. Click a tile to select it.
2. Click a neighbouring tile to swap them.
3. Line up 3 or more equal tiles horizontally or vertically to score.
4. Matched tiles disappear, the tiles above fall down and new tiles fill the gaps. If the falling tiles form a new line, it scores too.

A swap that makes no match is undone. Press `Esc` to quit.

## Features

- 8×8 board with horizontal and vertical match detection
- Cascading matches when falling tiles line up
- Frame-rate independent falling animation
- Score counter with a bounce effect and floating "+10" popups
- Background music and sound effects

## Project structure

```text
src/
├── main.c      window setup and the main loop
├── game.c      mouse input and the game state machine
├── board.c     board generation, match detection, gravity, swapping
├── score.c     score, popups and the score animation
├── audio.c     music and sound effects
└── render.c    all drawing

resources/      background image, font and audio
scripts/        release packaging
```

Each module has a header that lists what other files may use; internal state is `static`. The main loop is three calls per frame: input, update, draw.

## Building from source

Requires [MSYS2](https://www.msys2.org/) installed at `C:\msys64`. In the **MSYS2 UCRT64** terminal:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-raylib
cmake --preset msys2
cmake --build --preset msys2
./build/msys2/match-game.exe
```

To create the release zip (`dist/raylib-match-game-windows.zip`), run from PowerShell:

```powershell
powershell -ExecutionPolicy Bypass -File scripts\package.ps1
```

## What I learned

I started from a raylib tutorial and then kept working on the code past it:

- Tracked down a crash caused by writing outside the board array (`board[-5]`) with gdb.
- Turned on `-Wall -Wextra`, which caught `if (find_matches)` being used without `()`, so the check was always true.
- Replaced a single 450-line `main.c` with separate modules, using headers, `extern` and `static`.
- Made animations use `GetFrameTime()` so they run at the same speed at any frame rate.
- Packaged a Windows build that runs without any development tools installed.

## Possible improvements

- Move limit or timer, and a game-over screen
- High score saved to a file
- Bonus points for 4 and 5 tile matches
- Restart key and a pause menu

## Assets

- Font: [04b03](http://www.04.jp.org/) by Yuji Oshimoto, free for personal and commercial use.
- Background image, music and sound effects were generated procedurally for this project.

## Author

Davlatbek Aliev · [GitHub](https://github.com/alievdavlat)
