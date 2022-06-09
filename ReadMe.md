# Project Brief

Implement Conway's Game of Life in 64-bit integer-space using the language of your choice.

Imagine a 2D grid - each cell (coordinate) can be either "alive" or "dead". Every generation of the simulation, the system ticks forward. Each cell's value changes according to the following:
If an "alive" cell had less than 2 or more than 3 alive neighbors (in any of the 8 surrounding cells), it becomes dead.
If a "dead" cell had *exactly* 3 alive neighbors, it becomes alive.
Your input is a list of alive (x, y) integer coordinates. They could be anywhere in the signed 64-bit range. This means the board could be very large!

Your program should read the state of the simulation from standard input, run 10 iterations of the Game of Life, and print the result to standard output in Life 1.06 format. 

We're most interested in seeing how you break down the problem and deal with very large integers. Otherwise, feel free to add whatever you like for extra credit. For example, you might add a GUI or animation.

We intentionally leave the specification for the solution vague to see how you make decisions about these kinds of things and how those decisions are reflected in your code.  The most important advice I can give is to make sure your solution represents your best work and to come prepared to discuss what decisions you've made, alternatives you may have rejected (and why), and where you might take the work going forward.

### Additional notes:
Good source of info: https://conwaylife.com/wiki/
Maximum speed of any lifeform is C/2 
(Life forms can only move on every second cycle) This can probably be used to inform the grid size depending on how many iterations we want to perform (up to some upper bound)

### Implementation details
- Input and Output in Game of Life format 1.06
- Game should iterate X amount of times and then return the result game state (Probably write to an output file, may implement visual output if I have enough time)
- Written on UE4.27, Because it's what I've been using for the majority of the last several years of dev work, I've recently setup several projects using it, and to the best of my knowledge Unreal Engine is the platform that Valorant is developed on, so transferrable skills?
- I'm using [this plugin](https://github.com/slowburn-dev/site-gists/tree/main/DcPlugin) to use a headless UE client target. To start with will not be targetting visuals as I have limited time.

### Notes:
- Wonder if there would be a way to just use sets instead of multidimensional arrays for iterating game state, sets would hold all of the live cells, and only cells that have live neighbors can become active...
- Perhaps have an active and inactive set pool? hmmm... would be more difficult to multithread however.
- Wondering if a combination of set / array is best? Set contains all active cells, use set to update neighbor cells's count in array, parse array into new active cells... might cut down total reads/writes?
- THIS IMPLEMENTATION IS NOT PERFECT! INT64's within 128 of MIN / MAX may crash, I'm aware of the issue and the fix but have run out of time to fix this edge case. given the nature of this test being more to test code implementation rather than edge cases, I'm unhappy but willing to accept this issue.

### Getting setup:
```
# Setup for building to the headless target (if you'd like to build locally, required a UE4.27 source installed locally!)
Change the UE4_DIR variable in the bat file to point to your local engine Dir, should be all sorted!

# building the headless target (not required)
Run the build-headless bat file after the above setup

# running the built exe
run: run-headless.bat. can leave it there for example behaviour (loads a glider and runs the simulation for 10 iterations)

# Running with command line args
example (in the project dir): ./run-headless -iterations 50 -InputFilepath C:\dev\example\glider.life -OutputFilepath C:\dev\example\result.life
```

Give Hayden a shout if you have any trouble! :) 

A great writeup if you get stuck: https://slowburn.dev/blog/headless-fun-unreal-engine/

## To Do:
- [x] Get a headless target setup and building.
- [x] Setup reading from / saving to files using Life 1.06 format.
- [x] Get basic Game of Life functionality working without any major optimisations (Step 1 so we're working up froma solid foundation) Should use 2 arrays to swap state from one to another.
- [x] Setup some basic unit tests, should load a handful of patterns and have some expected output to check against (And ensure nothing crashes :P).
- [x] Use a grid based system to allow us to allocate working areas without having to actually handle the entire int64 working space.
- [x] Probably worth to have an input sanitization step. (E.G. remove singluar cells that would die on the first iteration, no need to allocate space for a single cell at x: int64_max, y:int64Max)
- [ ] parallelize the simulation step proccess, should be able to allocate threads to handle a couple of grids and share the previous state between the threads.
