#ifndef MODEL_H
#define MODEL_H
#include <QtWidgets>
#include <time.h>
class Model : public QObject {
    Q_OBJECT
public:
    Model(size_t cell, size_t w, size_t h);
    ~Model();
    enum GameStatus {
        STOPPED,
        STARTED,
        PAUSED
    };
    enum ObjectType {
        SNAKE_PART,
        FOOD,
        BONUS,
        WALL
    };


   /* class GameField {
    public:
        bool isOccupied(size_t i, size_t j) {
            return gameField.at(i).at(j);
        }
    private:
        QVector<QVector<bool> > gameField;
        void setGameObject(GameObject obj) {
            gameField.at(obj.getPosition().y()).at(obj.getPosition().x()) = true;
        }
        
    };*/

private:
    class GameObject {
    public:
        GameObject(ObjectType objectType): gameType(objectType), cell(0, 0) { }
        ObjectType gameType;
        QPoint getCell() { return cell; }
    protected:
        QPoint cell;
        bool isCollapsed(GameObject *obj1, GameObject *obj2) {
            if (obj1->cell == obj2->cell) return true;
            return false;
        }
    };
    class Bonus;
    class SnakePart;
    class Wall;

    /*class Collapsed {
    public:
        Collapsed(GameObject *obj);
        GameObject *collapsedWith(QVector<GameObject *> listObj);
    private:
        GameObject *obj;
        bool isCollapsed(GameObject *obj1, GameObject *obj2);
    };*/
    class Food : public GameObject {
    public:
        Food(Model *model) : GameObject(FOOD), addScore(100), model(model) { }
        size_t addScore;
        bool isCollides(Bonus *bonus, QVector<SnakePart *> *snake, QVector<Wall *> *walls);
        void setCell(QPoint cell) {
            this->cell = cell;
            emit model->updateFood(cell);
        }
    private:
        Model *model;
    };
    class Bonus : public GameObject {
    public:
        Bonus(Model *model) : GameObject(BONUS), addScore(0), data("0"), model(model) {
            setCell(QPoint(-1, -1));
        }
        size_t addScore;
        QString data;
        bool isCollides(Food *food, QVector<SnakePart *> *snake, QVector<Wall *> *walls);
        void setCell(QPoint cell) {
            this->cell = cell;
            emit model->updateBonus(cell);
        }
    private:
        Model *model;
    };
    class SnakePart : public GameObject {
    public:
        SnakePart(Model *model) : GameObject(SNAKE_PART), number(0), firstTimeUpdate(true), model(model) { }
        size_t number;
        ObjectType collidesWith(Food *food, Bonus *bonus, QVector<SnakePart *> *snake, QVector<Wall *> *walls);
        void setCell(QPoint cell) {
            this->cell = cell;
            firstTimeUpdate = false;
            emit model->updateSnake(firstTimeUpdate, cell);
        }
    private:
        bool firstTimeUpdate;
        Model *model;
    };
    class Wall : public GameObject {
    public:
        Wall(int x, int y, Model *model) : GameObject(WALL), model(model) {
            cell = QPoint(x, y);
            emit model->updateWall(cell);
        }
    private:
        Model *model;

    };

    void addGameElement(ObjectType gameElement);
    void removeGameElement(ObjectType gameElement);
    void initWalls();
    void initSnake();
    
    const size_t NORMAL_SNAKE_SPEED = 750;
    const size_t FAST_SNAKE_SPEED = 250;
    const size_t SLOW_SNAKE_SPEED = 1250;
    
    size_t score, ate, bonusAte, life;
    size_t bonusTiks, bonusVisibleTimeLeft, snakeSpeed;
    int idGameTimer;
    size_t cellSize, cellsW, cellsH;
    bool isBonusVisible, haveMooved;
    QPoint zeroSnakePos;
    GameStatus gameStatus;
    int previousVector, presentVector;

    QVector<SnakePart*> snake;
    QVector<Wall*> walls;
    Food food;
    Bonus bonus;

    
    QBasicTimer gameTimer;




public slots:
    void keyPressed(QKeyEvent *keyEvent);

    void startGame();
    void pauseGame();
    void resumeGame();
    void finishGame();

protected:
    virtual void timerEvent(QTimerEvent *timer);

signals:
    void updateScore(size_t);
    void updateAte(size_t);
    void updateBonusAte(size_t);
    void updateLife(size_t);
    void changeGameAreaSize(size_t, size_t);
    void updateBonusTimeLeft(size_t);
    void showBonusTimeLeft(QString);
    void finishGame(size_t);
    void updateFood(QPoint);
    void updateBonus(QPoint);
    void updateSnake(size_t, QPoint);
    void updateWall(QPoint);


};

#endif // MODEL_H
