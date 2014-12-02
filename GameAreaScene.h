#ifndef GAMEAREASCENE_H
#define GAMEAREASCENE_H
#include <QtWidgets>
#include <time.h>
class GameAreaScene : public QGraphicsScene {
public:
    GameAreaScene(int w, int h) ;
    ~GameAreaScene();
    void drawLevel(QVector<QRect> blocks);
    void drawSnake(QPoint head);
    void setUI(QLCDNumber *scoreNumber, QLCDNumber * ateNumber, QLCDNumber *bonusAteNumber, QLCDNumber *lifeNumber,
               QLCDNumber *bonusTimeLeftNumber, QLabel *bonusTimeLeftLabel);
    void startGame();
    void pauseGame();
    void resumeGame();
    void finishGame();
    void keyPress(QKeyEvent *keyEvent);

    enum GameStatus {
        STOPPED,
        STARTED,
        PAUSED
    };
    GameStatus getGameStatus();
private:
    enum MoveVector {
        UP,
        DOWN,
        RIGHT,
        LEFT
    };
    enum GameElement {
        BONUS,
        SNAKE,
        FOOD
    };

    const int NORMAL_SNAKE_SPEED = 750;
    const int FAST_SNAKE_SPEED = 250;
    const int SLOW_SNAKE_SPEED = 1250;
    QGraphicsItem* collaps(QGraphicsItem *item);
    void addGameElement(GameElement gameElement);
    void removeGameElement(GameElement gameElement);
    void showMsg(QString msg);

    QVector<QGraphicsRectItem*> *levelBlocks;
    QVector<QGraphicsPixmapItem*> *snake;

    QBasicTimer *gameTimer;

    QLCDNumber *scoreNumber, *ateNumber, *bonusAteNumber, *lifeNumber, *bonusTimeLeftNumber;
    QLabel *bonusTimeLeftLabel;

    QGraphicsPixmapItem *bonusItem, *foodItem;
    QGraphicsRectItem *msgRectItem;
    QGraphicsTextItem *msgItem;

    int score, ate, bonusAte, life, snakeSpeed;
    int bonusTiks, bonusVisibleTimeLeft;
    int idGameTimer;
    int snakeHeight;
    bool isBonusVisible, haveMooved;
    QPoint zeroSnakePos;
    GameStatus gameStatus;
    MoveVector previousVector, presentVector;

protected:
    virtual void timerEvent(QTimerEvent *timer);
};

#endif // GAMEAREASCENE_H
