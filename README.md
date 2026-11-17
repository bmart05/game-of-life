# Conway's Game of Life
This is a simple implementation of Conway's Game of Life in **C++** and **SDL3**. The Game of Life is a simple cellular automata based on these four rules:
1. Any live cell with fewer than two live neighbours dies, as if by underpopulation.
2. Any live cell with two or three live neighbours lives on to the next generation.
3. Any live cell with more than three live neighbours dies, as if by overpopulation.
4. Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.

In my implementation, cells colour themselves based on their age, starting out at red and slowly transitioning to purple. This allows you to better see how cells interact, by seeing where new ones have been created, where older ones are moving, and still life cells.

_You can read more about this [here](https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life)_
## Controls
* `Space` to pause and unpause the simulation
* `C` to cycle through generations manually
* `R` to reset the grid
* `Up Arrow`, `Down Arrow` to change simulation speed
* `Left click` on cells to toggle them on and off
## How to Use
Compile the program using `make`. You can then open `.rle` files using the following command
``` sh
./conway <path-to-file>
```
## Screenshots
<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/0cc39232-0413-4bb9-8904-9030754ea33c" />

_Analog watch example_

<img width="1600" height="1200" alt="image" src="https://github.com/user-attachments/assets/33881c30-7db7-45ce-9930-f36d8bbacd62" />

_Glider loop example_

