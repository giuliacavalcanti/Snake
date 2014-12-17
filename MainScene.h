#ifndef MAINSCENE_H
#define MAINSCENE_H
#include <QGraphicsScene>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QGraphicsProxyWidget>
#include "Preview.h"
//#include "GameScene.h"
#include "GameAreaScene.h"

class MainScene : public QGraphicsScene {
    Q_OBJECT
public:
    MainScene(int w, int h);
    ~MainScene();

private:

    int selectedLevel;
    void drawBG(int w, int h);
    QWidget* getPreLevel(int lvl);
    QVector<QVector<QRect> > maps, smallMaps;
    QVector<QPoint> snakeHead;
    void loadMaps();
    void drawPreviews();
    void releaseBG();
    void releasePreviews();
    void releaseGameScene();

    QVector<Preview*> *v;

    QGraphicsRectItem *bgItem;
    QGraphicsPixmapItem *headerItem;
    QVector<QGraphicsScene*> *previewScenes;
    QVector<QWidget*> *previewWidgets;
    QVector<QGraphicsProxyWidget*> *previewProxyWidgets;


    //__
    QGraphicsView *gameView;
    GameAreaScene *gameScene;
    QGraphicsProxyWidget *gameProxyWidget;
    QWidget *gameWidget;

    QGridLayout *bgLayout, *rightLayout, *leftLayout, *centerLayout;
    QPushButton *buttonBack, *buttonPause;
    QWidget *menuWidget;
    QGraphicsProxyWidget *menuProxyWidget;
    QLCDNumber *scoreNumber, *ateNumber, *bonusAteNumber, *lifeNumber, *bonusTimeLeftNumber;
    QLabel *scoreLabel, *ateLabel, *bonusAteLabel, *lifeLabel, *bonusTimeLeftLabel;



public slots:
    void drawGameScene(int i);
    void drawTitleScene(int w, int h);

private slots:
    void closeGameScene();
    void pauseOrResumeGame();

protected:
    virtual void keyPressEvent(QKeyEvent *keyEvent);


};


#endif // MAINSCENE_H
