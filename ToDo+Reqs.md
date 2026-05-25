# 💡 Lights Game: Development Blueprint

## 🏗️ Phase 1: Core Data Structures (The Logic)
Expected Classes: Point, Line, Graph (or Board), TraversalEngine (optional)

- [ ] Create a Line class. This represents a single arm and holds data like its current angle or direction slot (0 to 5).
- [ ] Create a Point class. It needs variables to remember its overall rotation angle and an array/vector of its attached Line objects.
- [ ] Create a Board or Graph class. This will hold a list/array of all the Points and know which Point is physically next to which Point on the hexagonal grid.
- [ ] Write a DFS (Depth-First Search) function. It should start at the middle Point and check which other Points it can reach through connected Lines to turn their "lit" status on.

## 🎨 Phase 2: The View (The Graphics)
Expected Classes: View, PointShape, LineShape (inheriting sf::Drawable and sf::Transformable)

- [ ] Create the View class that actually opens the SFML RenderWindow.
- [ ] Create a visual LineShape class (or struct) to handle drawing the individual rectangular arm.
- [ ] Create a visual PointShape class. It should draw the center circle and hold its associated LineShapes so they can all be rotated together using SFML's built-in math.
- [ ] Write the math that takes a Point's logical position in your graph and figures out its exact X and Y pixel coordinates on the screen so they visually form a hexagon.

## ⚙️ Phase 3: The Engine (The Loop & Interaction)
Expected Classes: Iterations on the View class

- [ ] Put the standard SFML while(window.isOpen()) game loop inside your View class.
- [ ] Inside the loop, use sf::Event to check if the user clicked the red X to close the window, or if they clicked their mouse buttons.
- [ ] When the mouse is clicked, figure out which Point on the screen was clicked based on the mouse's X and Y coordinates.
- [ ] Rotate the clicked Point in your data, run your DFS to see what lights up, clear the screen, and draw everything again.

## 🎮 Phase 4: Game Manager & Progression
Expected Classes: GameManager, LevelLoader

- [ ] Create a GameManager class. It will hold the current level number and start the game.
- [ ] Write code to read the text file that tells you how many Points there are and which Lines they possess.
- [ ] After every click, check if the number of "lit" Points equals the total number of Points on the board.
- [ ] If the player wins, show a success message on the screen and tell GameManager to load the next level.

---

# 📋 Quick-Reference Requirements

### Board Layout & Visuals
* The player is presented with a hexagonal board on which Points ("lights") are arranged at equal intervals.
* One or more Lines (arms) extend from each Point.
* The maximum number of Lines is 6 for Points not on the edges of the hexagon, and for Points on the edge it is lower, depending on the number of neighbours.
* Your game must be generic and support boards of any size.
* The game will be able to display and work with anything included in the file, provided it does not cause the window to exceed the size of the screen.
* There is no need to implement a menu or high score table.
* It is not mandatory to display a clock.

### Interaction & Gameplay Logic
* Each time a Point is clicked, it rotates by 60 degrees, along with its Lines.
* Clicking the left mouse button rotates clockwise, and clicking the right button rotates anticlockwise.
* It is only necessary to support exiting the game by pressing the red X in the corner.
* When the Lines of adjacent Points point towards each other, they are connected and a link is formed.
* The central light is always on, and any light connected to a lit light also lights up.
* The player’s aim is to light up all the Points on the board.

### Level Progression Constraints
* The game will have three levels, with the board getting bigger from level to level.
* In the first level, the middle row of the hexagon will have 5 Points.
* In the second, 7 Points.
* In the third, 9 Points.
* In all levels, the top and bottom rows have 3 Points.
* For every row further from the centre, there is one Point fewer.
* When the player completes a level, a message will appear confirming their success and we will move on to the next level.
* When the player completes the third level, a message will appear confirming that the game has been successfully completed.