#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QTimer>
#include <QGraphicsItem>
#include <QGraphicsEffect>

class QGraphicsScene;
class QKeyEvent;
class QGraphicsView;

class Snake;
class Food;
class Wall;
class Button;

class GameController : public QObject
{
    Q_OBJECT
public:
    GameController(QGraphicsScene &scene, QGraphicsView &view, QObject *parent = 0);
    ~GameController();

    void snakeAteFood(Snake *snake, Food *food);
    void snakeHitWall(Snake *snake, Wall *wall);
    void snakeAteItself(Snake *snake);
    int sceneColor = 3;
    int snakeColor = 2;

public slots:
    void pause();
    void resume();
    void gameOver();

protected:
     bool eventFilter(QObject *object, QEvent *event);

private:
    void handleKeyPressed(QKeyEvent *event);
    void handleKeyReleased(QKeyEvent *event);
    void addNewFood();
    void addButton(Button *button);
    void addCloud();
    void toStartMenu();
    void skinScene();
    void toStart();
    void Quit();
    void sceneSkipList();
    void snakeSkipList();
    void setSceneColor(Button *c);
    void setSnakeColor(Button *c);
    void setWall();
    void quit();
    QTimer timer;
    QTimer foodTimer;
    QGraphicsScene &scene;
    QGraphicsView &view;
    Snake *snake;
    Button *pauseButton = 0;
    Button *gameOverButton = 0;
    Button *startButton = 0;
    Button *skinButton = 0;
    Button *quitButton = 0;
    Button *SceneSkinButton = 0;
    Button *SnakeSkinButton = 0;
    Button *SceneSkipListButton[4];
    Button *SnakeSkipListButton[4];
    Button *backButton = 0;
    Button *backButtoninSceneSkip = 0;
    Button *backButtoninSnakeSkip = 0;
    Button *desButton = 0;
    Button *yesButton = 0;
    Button *noButton = 0;
    Button *MenuButton = 0;
    int foodcount;

    bool Pause;
    bool start;
    bool inScene;
};

#endif // GAMECONTROLLER_H
