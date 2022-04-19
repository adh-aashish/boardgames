# BoardGames

BoardGames is the collection of three games namely Buddhichal, Checkers and Chess.

Each game has single player ( vs computer ) and multiplayer mode ( except in chess ).

See the building instructions below to build and play on your own computer.

## Images of Some Screens :

### 1. Main Menu
![](./finalImages/mainmenu.png)
### 2. Buddhichal
![](./finalImages/buddhichal.png)

![](./finalImages/bc3.png)

### 3. Checkers
![](./finalImages/checkers.png)

![](./finalImages/checkers-king.png)

### 4. Chess
![](./finalImages/chess2.png)


### Further images :
The final images of the game can be seen from [here](https://github.com/avisekksarma/BoardGames/tree/main/finalImages)

##### **Building Instructions**

Dependencies: SFML 2.5.1 , cmake

Install SFML as according to your machine.

##### **Using Cmake from commandline**

From our base project directory, type in the following commands:

```
$ rm -rf build
$ cmake CMakeLists.txt -B build
$ cd build
$ make
$ ./BoardGames
```


