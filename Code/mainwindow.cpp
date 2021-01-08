#include <QGraphicsView>
#include <QTimer>
#include <QColor>

#include "constants.h"
#include "gamecontroller.h"
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      scene(new QGraphicsScene(this)),
      view(new QGraphicsView(scene, this)),
      game(new GameController(*scene, *view, this))
{
    setCentralWidget(view);
    resize(800, 800);
    setFixedSize(800,800);

    initScene();
    initSceneBackground();

    QTimer::singleShot(0, this, SLOT(adjustViewSize()));
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::adjustViewSize()
{
    view->fitInView(scene->sceneRect(), Qt::KeepAspectRatioByExpanding);
}

void MainWindow::initScene()
{
    scene->setSceneRect(-SCENE_SIZE, -SCENE_SIZE, SCENE_SIZE * 2, SCENE_SIZE * 2);
}

void MainWindow::initSceneBackground()
{
    view->setBackgroundBrush(QColor(0xBF,0xEF,0xFF));
}
