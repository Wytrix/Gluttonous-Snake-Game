#include <QEvent>
#include <QGraphicsScene>
#include <QKeyEvent>
#include <QGraphicsEffect>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QGraphicsPixmapItem>
#include <QTime>
#include <QDebug>
#include <QCoreApplication>

#include "gamecontroller.h"
#include "food.h"
#include "snake.h"
#include "button.h"
#include "wall.h"
#include "constants.h"
#include "cloud.h"

const qreal foodMaxNum = 20;

GameController::GameController(QGraphicsScene &scene, QGraphicsView &view, QObject *parent) :
    QObject(parent),
    scene(scene),
    view(view),
    start(false),
    foodcount(1),
    Pause(true),
    inScene(false),
    sceneColor(3),
    snakeColor(2)
{
    timer.start( 1000/60 );

    scene.installEventFilter(this);

    foodTimer.start(3000);

    toStartMenu();
}

GameController::~GameController()
{
}

void GameController::snakeAteFood(Snake *snake, Food *food)
{
    scene.removeItem(food);
    delete food;
    scene.update();
    foodcount -= 1;
    if(foodcount < foodMaxNum / 2)
        connect(&foodTimer, &QTimer::timeout, this, &GameController::addNewFood);
}

void GameController::snakeHitWall(Snake *snake, Wall *wall)
{
    QTimer::singleShot(0,this,SLOT(gameOver()));
}

void GameController::snakeAteItself(Snake *snake)
{
    QTimer::singleShot(0, this, SLOT(gameOver()));
}

void GameController::handleKeyPressed(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_Left:
            if(snake->moveDirection != 2) snake->setMoveDirection(Snake::MoveLeft);
            break;
        case Qt::Key_Right:
            if(snake->moveDirection != 1) snake->setMoveDirection(Snake::MoveRight);
            break;
        case Qt::Key_Up:
            if(snake->moveDirection != 4) snake->setMoveDirection(Snake::MoveUp);
            break;
        case Qt::Key_Down:
            if(snake->moveDirection != 3) snake->setMoveDirection(Snake::MoveDown);
            break;
        case Qt::Key_A:
            if(snake->moveDirection != 2) snake->setMoveDirection(Snake::MoveLeft);
            break;
        case Qt::Key_D:
            if(snake->moveDirection != 1) snake->setMoveDirection(Snake::MoveRight);
            break;
        case Qt::Key_W:
            if(snake->moveDirection != 4) snake->setMoveDirection(Snake::MoveUp);
            break;
        case Qt::Key_S:
            if(snake->moveDirection != 3) snake->setMoveDirection(Snake::MoveDown);
            break;
        case Qt::Key_P:{
            pause();
            break;
            }
        case Qt::Key_Escape:{
            if(Pause == false && inScene) pause();
            else if(Pause == true && inScene) resume();
            break;
            }
        case Qt::Key_R:
            resume();
            break;
        case Qt::Key_Space:
            snake->speed = 1;
            break;
    }
}

void GameController::handleKeyReleased(QKeyEvent *event)
{
    switch (event->key())
    {
        case Qt::Key_Space:
        snake->speed = 2;
    }
}

void GameController::addNewFood()
{
    int x, y;
    int SIZE = SCENE_SIZE - WALL_SIZE;
    do {
        x = (int) (qrand() % (2 * SIZE)) ;
        y = (int) (qrand() % (2 * SIZE)) ;
            x -= SIZE;
            y -= SIZE;
    } while (snake->shape().contains(snake->mapFromScene(QPointF(x , y )))||scene.itemAt(x, y, QTransform()) != 0);

    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    QColor color(qrand() % 256,qrand() % 256,qrand() % 256);

    Food *food = new Food(x , y , color);
    scene.addItem(food);
    foodcount += 1;
    if(foodcount >= foodMaxNum)
        disconnect(&foodTimer, &QTimer::timeout, this, &GameController::addNewFood);
}

void GameController::addButton(Button *button)
{
    scene.addItem(button);
}

void GameController::addCloud()
{
    QString file(":/images/images/yun.png");
    Cloud *cloud = new Cloud(-500,30,500,500,file);
    scene.addItem(cloud);
    cloud = new Cloud(-600,160,600,600,file);
    scene.addItem(cloud);
    cloud = new Cloud(-500,-500,500,500,file);
    scene.addItem(cloud);
    cloud = new Cloud(0,-350,500,500,file);
    scene.addItem(cloud);
    cloud = new Cloud(-100,160,600,600,file);
    scene.addItem(cloud);
}
void GameController::setWall()
{
    for(int i = -SCENE_SIZE; i <= SCENE_SIZE;i += WALL_SIZE + 2)
    {
        Wall *boundWall = new Wall(i - 4,-SCENE_SIZE - 4);
        scene.addItem(boundWall);
        boundWall = new Wall(-SCENE_SIZE - 6, i - 2);
        scene.addItem(boundWall);
        boundWall = new Wall(SCENE_SIZE - WALL_SIZE + 4,i - 2);
        scene.addItem(boundWall);
        boundWall = new Wall(i - 4, SCENE_SIZE - WALL_SIZE + 6);
        scene.addItem(boundWall);
    }
}

void GameController::gameOver()
{
    gameOverButton = new Button("GAME OVER",0,0,200,100,26,0x00,0xCE,0xD1);
    addButton(gameOverButton);
    connect(gameOverButton,&Button::ButtonIsReleased,this,&GameController::toStartMenu);
    start = false;
    disconnect(&foodTimer, &QTimer::timeout, this, &GameController::addNewFood);
    disconnect(&timer, SIGNAL(timeout()),&scene, SLOT(advance()));
    scene.update();
}

void GameController::pause()
{
    if(!Pause)
    {
        disconnect(&timer, SIGNAL(timeout()),&scene, SLOT(advance()));
        pauseButton = new Button("Resume",0,-60,160,60,20,0x98,0xFB,0x98);
        addButton(pauseButton);
        Pause = true;
        MenuButton = new Button("Menu",0,60,160,60,20,0xFF,0xD7,0);
        addButton(MenuButton);
        disconnect(&foodTimer, &QTimer::timeout, this, &GameController::addNewFood);
        connect(pauseButton,&Button::ButtonIsReleased,this,&GameController::resume);
        connect(MenuButton,&Button::ButtonIsReleased,this,&GameController::toStartMenu);
    }
}

void GameController::resume()
{
    if(Pause)
    {
        connect(&timer, SIGNAL(timeout()),&scene, SLOT(advance()));
        Pause = false;
        if(pauseButton){
            scene.removeItem(pauseButton);
            pauseButton = NULL;
        }
        if(MenuButton){
            scene.removeItem(MenuButton);
            MenuButton = NULL;
        }
        connect(&foodTimer, &QTimer::timeout, this, &GameController::addNewFood);
    }
}

void GameController::toStartMenu()
{
    scene.clear();
    scene.update();
    addCloud();
    inScene = false;
    startButton = new Button("Press To Start",0,-80,400,200,30,0x98,0xF5,0xFF,255);
    addButton(startButton);
    skinButton = new Button("Change skin",-100,100,180,80,18,0x83,0x6F,0xFF,255);
    addButton(skinButton);
    quitButton = new Button("Quit",100,100,180,80,18,0xFF,0x40,0x40,255);
    addButton(quitButton);
    connect(skinButton,&Button::ButtonIsReleased,this,&GameController::skinScene);
    connect(startButton,&Button::ButtonIsReleased,this,&GameController::toStart);
    connect(quitButton,&Button::ButtonIsReleased,this,&GameController::Quit);
}

void GameController::skinScene()
{
    scene.clear();
    scene.update();
    addCloud();
    SceneSkinButton = new Button("Scene Skin",-100,-40,160,160,18,0xB2,0xDF,0xEE);
    addButton(SceneSkinButton);
    SnakeSkinButton = new Button("Snake Skin",100,-40,160,160,18,0xC1,0xFF,0xC1);
    addButton(SnakeSkinButton);
    backButton = new Button("back",0,120,280,60,18,0xEE,0xEE,0);
    addButton(backButton);
    connect(SceneSkinButton,&Button::ButtonIsReleased,this,&GameController::sceneSkipList);
    connect(SnakeSkinButton,&Button::ButtonIsReleased,this,&GameController::snakeSkipList);
    connect(backButton,&Button::ButtonIsReleased,this,&GameController::toStartMenu);
}

void GameController::sceneSkipList()
{
    scene.clear();
    scene.update();
    addCloud();
    desButton = new Button("Select a Scene Color",0,-180,300,40,18,0xB0,0xE2,0xFF);
    addButton(desButton);
    SceneSkipListButton[0] = new Button("",-180,-40,100,160,18,0xC4,0xC4,0xC4);
    SceneSkipListButton[0]->setHasVal(true);
    addButton(SceneSkipListButton[0]);
    SceneSkipListButton[1] = new Button("",-60,-40,100,160,18,0xFF,0xFF,0);
    addButton(SceneSkipListButton[1]);
    SceneSkipListButton[1]->setHasVal(true);
    SceneSkipListButton[2] = new Button("",60,-40,100,160,18,0xBF,0xEF,0xFF);
    addButton(SceneSkipListButton[2]);
    SceneSkipListButton[2]->setHasVal(true);
    SceneSkipListButton[3] = new Button("",180,-40,100,160,18,0xC1,0xFF,0xC1);
    addButton(SceneSkipListButton[3]);
    SceneSkipListButton[3]->setHasVal(true);
    backButtoninSceneSkip = new Button("back",0,120,280,60,18,0xEE,0xEE,0);
    addButton(backButtoninSceneSkip);
    connect(SceneSkipListButton[0],&Button::sendVal,this,&GameController::setSceneColor);
    connect(SceneSkipListButton[1],&Button::sendVal,this,&GameController::setSceneColor);
    connect(SceneSkipListButton[2],&Button::sendVal,this,&GameController::setSceneColor);
    connect(SceneSkipListButton[3],&Button::sendVal,this,&GameController::setSceneColor);
    connect(backButtoninSceneSkip,&Button::ButtonIsReleased,this,&GameController::skinScene);
}

void GameController::snakeSkipList()
{
    scene.clear();
    scene.update();
    addCloud();
    desButton = new Button("Select a Snake Color",0,-180,300,40,18,0xB0,0xE2,0xFF);
    addButton(desButton);
    SnakeSkipListButton[0] = new Button("",-180,-40,100,160,18,0xFF,0x3E,0x96);
    SnakeSkipListButton[0]->setHasVal(true);
    addButton(SnakeSkipListButton[0]);
    SnakeSkipListButton[1] = new Button("",-60,-40,100,160,18,0xFF,0xFF,0);
    addButton(SnakeSkipListButton[1]);
    SnakeSkipListButton[1]->setHasVal(true);
    SnakeSkipListButton[2] = new Button("",60,-40,100,160,18,0xBF,0xEF,0xFF);
    addButton(SnakeSkipListButton[2]);
    SnakeSkipListButton[2]->setHasVal(true);
    SnakeSkipListButton[3] = new Button("",180,-40,100,160,18,0xC1,0xFF,0xC1);
    addButton(SnakeSkipListButton[3]);
    SnakeSkipListButton[3]->setHasVal(true);
    backButtoninSnakeSkip = new Button("back",0,120,280,60,18,0xEE,0xEE,0);
    addButton(backButtoninSnakeSkip);
    connect(SnakeSkipListButton[0],&Button::sendVal,this,&GameController::setSnakeColor);
    connect(SnakeSkipListButton[1],&Button::sendVal,this,&GameController::setSnakeColor);
    connect(SnakeSkipListButton[2],&Button::sendVal,this,&GameController::setSnakeColor);
    connect(SnakeSkipListButton[3],&Button::sendVal,this,&GameController::setSnakeColor);
    connect(backButtoninSnakeSkip,&Button::ButtonIsReleased,this,&GameController::skinScene);
}
void GameController::setSceneColor(Button *c)
{
    QColor color;
    if(c == SceneSkipListButton[0])
    {
        sceneColor = 1;
        view.setBackgroundBrush(Qt::lightGray);
    }
    else if(c == SceneSkipListButton[1]) {
        sceneColor = 2;
        color.setRgb(0xFF,0xFF,0xE0);
        view.setBackgroundBrush(color);
    }
    else if(c == SceneSkipListButton[2]){
        sceneColor = 3;
        color.setRgb(0xE0,0xFF,0xFF);
        view.setBackgroundBrush(color);
    }
    else if(c == SceneSkipListButton[3]) {
        sceneColor = 4;
        color.setRgb(0xF0,0xFF,0xF0);
        view.setBackgroundBrush(color);
    }
}
void GameController::setSnakeColor(Button *c)
{
    if(c == SnakeSkipListButton[0])
        snakeColor = 1;
    else if(c == SnakeSkipListButton[1])
        snakeColor = 2;
    else if(c == SnakeSkipListButton[2])
        snakeColor = 3;
    else if(c == SnakeSkipListButton[3])
        snakeColor = 4;
}
void GameController::toStart()
{
    scene.clear();
    scene.update();
    snake = new Snake(*this,snakeColor);
    scene.addItem(snake);
    foodcount = 0;
    addNewFood();
    setWall();
    Pause = false;
    start = true;
    inScene = true;
    connect(&timer, SIGNAL(timeout()),&scene, SLOT(advance()));
    connect(&foodTimer, &QTimer::timeout, this, &GameController::addNewFood);
}

void GameController::Quit()
{
    scene.clear();
    scene.update();
    addCloud();
    desButton = new Button("Do You Want to Quit ?",0,-60,360,80,18,0xB2,0xDF,0xEE);
    addButton(desButton);
    yesButton = new Button("Yes",-60,60,100,60,18,0xFF,0x40,0x40);
    addButton(yesButton);
    noButton = new Button("No",60,60,100,60,18,0x7E,0xC0,0xEE);
    addButton(noButton);
    connect(yesButton,&Button::ButtonIsReleased,this,&GameController::quit);
    connect(noButton,&Button::ButtonIsReleased,this,&GameController::toStartMenu);
}

void GameController::quit()
{
    QCoreApplication::quit();
}

bool GameController::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress && start == true) {
        handleKeyPressed((QKeyEvent *)event);
        return true;
    }
    else if (event->type() == QEvent::KeyRelease && start ==true)
    {
        handleKeyReleased((QKeyEvent *)event);
        return true;
    } else {
        return QObject::eventFilter(object, event);
    }
}
