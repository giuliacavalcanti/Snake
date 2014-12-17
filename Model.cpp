#include <Model.h>
Model::Model(size_t cell, size_t w, size_t h) : gameTimer(), snake(), walls(), food(this),
    bonus(this), cellSize(cell) {
    cellsW = w/cell;
    cellsH = h/cell;
    if (w%cell !=0 || h%cell != 0) {
        emit changeGameAreaSize(cellsW, cellsH);
    }
}
Model::~Model() {
    
}
void Model::initSnake() {
    for (int i = 0;i<3;i++) snake.append(new SnakePart(this));
    snake.at(0)->setCell(QPoint(cellsW/2+1, cellsH/2+1));
    for (int i = 1;i<3;i++) {
        snake.at(i)->setCell(QPoint(snake.at(i-1)->getCell().x()-1, snake.at(i-1)->getCell().y()));
    }
}
void Model::initWalls() {
    for (int i = 0;i<3;i++) walls.append(new Wall(i+1,i+1,this));
}

void Model::keyPressed(QKeyEvent *keyEvent) {
    if (gameStatus != STARTED || !haveMooved) return;
    switch (keyEvent->key()) {
        case Qt::Key_Left:
            if (presentVector == Qt::Key_Right) { return; }
            previousVector = presentVector;
            presentVector = Qt::Key_Left;
            break;
        case Qt::Key_Right:
            if (this->presentVector == Qt::Key_Left) { return; }
            previousVector = presentVector;
            presentVector = Qt::Key_Right;
            break;
        case Qt::Key_Up:
            if (this->presentVector == Qt::Key_Down) { return; }
            presentVector = presentVector;
            presentVector = Qt::Key_Up;
            break;
        case Qt::Key_Down:
            if (this->presentVector == Qt::Key_Up) { return; }
            previousVector = presentVector;
            presentVector = Qt::Key_Down;
            break;
    }
    haveMooved = true;

}
void Model::timerEvent(QTimerEvent *timer) {
    if (gameTimer.timerId() == idGameTimer) {
        size_t x = snake.at(0)->getCell().x();
        size_t y = snake.at(0)->getCell().y();

        QPoint prev;
        prev.setX(x);
        prev.setY(y);
        switch (this->presentVector){
            case Qt::Key_Up:
                snake.at(0)->setCell(QPoint(x, y-1));
                break;
            case Qt::Key_Down:
                snake.at(0)->setCell(QPoint(x, y+1));
                break;
            case Qt::Key_Right:
                snake.at(0)->setCell(QPoint(x+1, y));
                break;
            case Qt::Key_Left:
                snake.at(0)->setCell(QPoint(x-1, y));
                break;
        }

        for (int i=1;i<snake.count();i++) {
            QPoint p(snake.at(i)->getCell());
            snake.at(i)->setCell(p);
            prev = p;
            if (i == snake.count()-1) zeroSnakePos = p;
        }
        haveMooved = true;
        if (isBonusVisible) {
            bonusVisibleTimeLeft--;
            emit updateBonusTimeLeft(bonusVisibleTimeLeft);
            if (bonusVisibleTimeLeft == 0) {
                removeGameElement(BONUS);
                isBonusVisible = false;

            }
        }
        if (!isBonusVisible) bonusTiks++;
        if (!isBonusVisible && bonusTiks == 20) {
            addGameElement(BONUS);
            isBonusVisible = true;
            bonusTiks = 0;
            bonusVisibleTimeLeft = 20;
            emit updateBonusTimeLeft(bonusVisibleTimeLeft);
        }
        Model::ObjectType objType = snake.at(0)->collidesWith(&food, &bonus, &snake, &walls);
//?!!!!!!!!!!!!
        if (objType == Model::WALL || objType == Model::SNAKE_PART) {
            this->finishGame();
        }
        if (objType == Model::FOOD) {
            addGameElement(SNAKE_PART);
            addGameElement(FOOD);
            score+=10;
            emit updateScore(score);
            emit updateAte(++ate);
            emit updateLife(++life);

        }
        if (objType == Model::BONUS) {
            snakeSpeed = NORMAL_SNAKE_SPEED;
            removeGameElement(BONUS);
            emit updateBonusAte(++bonusAte);
            QString msg = "";
            int bonusKind = bonus.data.toInt();
            if (bonusKind == 0) {
                score += 100;
                emit updateScore(score);
                msg = "+100 score";
            } else if (bonusKind == 1) {
                snakeSpeed = NORMAL_SNAKE_SPEED;
                msg = "Normal";
            } else if (bonusKind == 2) {
                snakeSpeed = FAST_SNAKE_SPEED;
                msg = "Fast";
            } else if (bonusKind == 3) {
                snakeSpeed = SLOW_SNAKE_SPEED;
                msg = "Slow";
            }
            emit showBonusTimeLeft(msg);
            gameTimer.start(snakeSpeed, this);
            idGameTimer = gameTimer.timerId();
        }


    }
}
void Model::startGame() {
    idGameTimer = 0;
    gameStatus = STOPPED;
    presentVector = Qt::Key_Right;
    previousVector = Qt::Key_Right;
    haveMooved = false;

    bonusTiks = 0;
    isBonusVisible = false;
    bonusVisibleTimeLeft = 20;

    snakeSpeed = NORMAL_SNAKE_SPEED;

    score = 0;
    bonusAte = 0;
    ate = 0;
    life = 1;

    initWalls();
    initSnake();


    addGameElement(FOOD);


    gameTimer.start(snakeSpeed, this);
    idGameTimer = gameTimer.timerId();

    gameStatus = STARTED;
}
void Model::pauseGame() {
    gameTimer.stop();
    gameStatus = PAUSED;
}
void Model::resumeGame() {
    gameTimer.start(snakeSpeed, this);
    idGameTimer = gameTimer.timerId();
    gameStatus = STARTED;
}
void Model::finishGame() {
    gameStatus = STOPPED;
    gameTimer.stop();
    emit finishGame(score);
}

bool Model::Food::isCollides(Bonus *bonus, QVector<SnakePart*> *snake, QVector<Wall*> *walls) {
    if (isCollapsed(this, bonus)) return true;
    for (int j = 0; j<snake->size();j++) {
        if (isCollapsed(this, snake->at(j))) return true;
    }
    for (int j = 0; j<walls->size();j++) {
        if (isCollapsed(this, walls->at(j))) return true;
    }
}
bool Model::Bonus::isCollides(Food *food, QVector<SnakePart *> *snake, QVector<Wall *> *walls) {
    if (isCollapsed(this, food)) return true;
    for (int j = 0; j<snake->size();j++) {
        if (isCollapsed(this, snake->at(j))) return true;
    }
    for (int j = 0; j<walls->size();j++) {
        if (isCollapsed(this, walls->at(j))) return true;
    }
}
Model::ObjectType Model::SnakePart::collidesWith(
        Food *food, Bonus *bonus, QVector<SnakePart *> *snake, QVector<Wall *> *walls) {
    if (isCollapsed(this, bonus)) return Model::BONUS;
    if (isCollapsed(this, food)) return Model::FOOD;
    for (int j = 0; j<walls->size();j++) {
        if (isCollapsed(this, walls->at(j))) return Model::WALL;
    }
    for (int j = 1; j<snake->size();j++) {
        if (isCollapsed(this, snake->at(j))) return Model::SNAKE_PART;
    }


}
void Model::addGameElement(ObjectType gameElement) {
    srand(time(NULL));

    bool ok=false;
    while (ok==false) {
        if (gameElement == BONUS) {
            bonus.data = QString::number(rand()%4);
            bonus.setCell(QPoint(rand()%cellsW, rand()%cellsH));
            ok = true;
            if (bonus.isCollides(&food, &snake, &walls)) {
                ok=false;
            }
        } else if (gameElement == FOOD) {
            food.setCell(QPoint(rand()%cellsW, rand()%cellsH));
            ok = true;
            if (food.isCollides(&bonus, &snake, &walls)) {
                ok=false;
            }
        } else if (gameElement == SNAKE_PART) {
            snake.append(new SnakePart(this));
            snake.last()->setCell(zeroSnakePos);
        }
    }

}
void Model::removeGameElement(Model::ObjectType gameElement) {
    if (gameElement == BONUS) {
        bonus.setCell(QPoint(-1, -1));
    }
  /*  if (gameElement == FOOD) this->foodItem->setVisible(false);
    if (gameElement == SNAKE_PART) {
        this->removeItem(snake->last());
        snake->last()->~QGraphicsPixmapItem();
        snake->removeLast();
    }*/
}

/*
Model::Collapsed::Collapsed(GameObject *obj) {
    this->obj = obj;
}
bool Model::Collapsed::isCollapsed(GameObject *obj1, GameObject *obj2) {
    if (obj1->getPosition() == obj2->getPosition()) return true;
    return false;
}
Model::GameObject* Model::Collapsed::collapsedWith(QVector<GameObject *> listObj) {
    for (int i = 0;i<listObj.size();i++) {
        if (obj->gameType == Model::FOOD) {
            if (listObj.at(i)->gameType == Model::FOOD) {
                continue;
            } else if (listObj.at(i)->gameType == Model::BONUS) {
                if (isCollapsed(obj, listObj.at(i))) return listObj.at(i);
            } else if (listObj.at(i)->gameType == Model::SNAKE_PART) {
                for (int j = 0; j<snake.size();j++) {
                    if (isCollapsed(obj, snake.at(j))) return snake.at(j);
                }
            } else if (listObj.at(i)->gameType == Model::WALL) {
                for (int j = 0; j<walls.size();j++) {
                    if (isCollapsed(obj, walls.at(j))) return walls.at(j);
                }
            }
        } else if (obj->gameType == Model::BONUS) {
            if (listObj.at(i)->gameType == Model::FOOD) {
                if (isCollapsed(obj, listObj.at(i))) return listObj.at(i);
            } else if (listObj.at(i)->gameType == Model::BONUS) {
                continue;
            } else if (listObj.at(i)->gameType == Model::SNAKE_PART) {
                for (int j = 0; j<snake.size();j++) {
                    if (isCollapsed(obj, snake.at(j))) return snake.at(j);
                }
            } else if (listObj.at(i)->gameType == Model::WALL) {
                for (int j = 0; j<walls.size();j++) {
                    if (isCollapsed(obj, walls.at(j))) return walls.at(j);
                }
            }
        } else if (obj->gameType == Model::SNAKE_PART) {
            if (listObj.at(i)->gameType == Model::FOOD) {
                if (isCollapsed(obj, listObj.at(i))) return listObj.at(i);
            } else if (listObj.at(i)->gameType == Model::BONUS) {
                if (isCollapsed(obj, listObj.at(i))) return listObj.at(i);
            } else if (listObj.at(i)->gameType == Model::SNAKE_PART) {
                for (int j = 1; j<snake.size();j++) {
                    if (isCollapsed(obj, snake.at(j))) return listObj.at(i);
                }
            } else if (listObj.at(i)->gameType == Model::WALL) {
                for (int j = 0; j<walls.size();j++) {
                    if (isCollapsed(obj, walls.at(j))) return listObj.at(i);
                }
            }
        }
    }
}
*/
