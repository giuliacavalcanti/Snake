#include "GameAreaScene.h"
#include <QMessageBox>
GameAreaScene::GameAreaScene(int w, int h) {
    this->setSceneRect(0,0,w,h);

    qDebug()<<"Created game scene";

}
GameAreaScene::~GameAreaScene() {

}
void GameAreaScene::startGame() {
    idGameTimer = 0;
    gameStatus = STOPPED;
    presentVector = RIGHT;
    previousVector = RIGHT;

    bonusTiks = 0;
    isBonusVisible = false;

    bonusItem = this->addPixmap(QPixmap(":/images/images/Bonus.png"));
    bonusItem->setData(0, "Bonus");
    bonusItem->setVisible(false);
    bonusVisibleTimeLeft = 20;

    foodItem = this->addPixmap(QPixmap(":/images/images/Food.jpg"));
    foodItem->setData(0, "Food");
    addGameElement(FOOD);

    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(2);
    QBrush brush(Qt::green);
   // brush.setStyle();
    this->msgRectItem = this->addRect(0, 0, this->width()/4, this->height()/2, pen,  brush);
    //this->msgRectItem->set
    this->msgItem = this->addText("", QFont("Times", 10));
    this->msgItem->setParentItem(this->msgRectItem);
    this->msgItem->setVisible(false);
    this->msgRectItem->setVisible(false);

    this->snakeSpeed = NORMAL_SNAKE_SPEED;
    this->gameTimer = new QBasicTimer();
    this->gameTimer->start(this->snakeSpeed, this);
    this->idGameTimer = this->gameTimer->timerId();
    this->gameStatus = STARTED;

    this->score = 0;
    this->bonusAte = 0;
    this->ate = 0;
    this->life = 1;

    this->haveMooved = false;

}
void GameAreaScene::pauseGame() {
    this->gameTimer->stop();
    this->gameStatus = PAUSED;
}
void GameAreaScene::resumeGame() {
    this->gameTimer->start(this->snakeSpeed, this);
    this->idGameTimer = this->gameTimer->timerId();
    this->gameStatus = STARTED;
}
void GameAreaScene::finishGame() {
    this->gameStatus = STOPPED;
    this->gameTimer->stop();
}
void GameAreaScene::timerEvent(QTimerEvent *timer) {
    if (gameTimer->timerId() == this->idGameTimer) {
        qreal x = snake->at(0)->x();
        qreal y = snake->at(0)->y();


        QPoint prev;
        QPixmap pix;
        prev.setX(x);
        prev.setY(y);
        switch (this->presentVector){
            case UP:
                pix.load(":/images/images/topHead");
                snake->at(0)->setPos(x, y - pix.height());
                snake->at(0)->setPixmap(pix);
                break;
            case DOWN:
                pix.load(":/images/images/downHead");
                snake->at(0)->setPos(x, y + pix.height());
                snake->at(0)->setPixmap(pix);
                break;
            case RIGHT:
                pix.load(":/images/images/rightHead");
                snake->at(0)->setPos(x + pix.width(), y);
                snake->at(0)->setPixmap(pix);
                break;
            case LEFT:
                pix.load(":/images/images/leftHead");
                snake->at(0)->setPos(x - pix.width(), y);
                snake->at(0)->setPixmap(pix);
                break;
        }

        for (int i=1;i<snake->count();i++) {
            QPoint p(snake->at(i)->x(), snake->at(i)->y());
            snake->at(i)->setPos(prev.x(), prev.y());
            prev = p;
            if (i == snake->count()-1) zeroSnakePos = p;
        }
        this->haveMooved = true;
        if (this->isBonusVisible) {
            this->bonusVisibleTimeLeft--;
            this->bonusTimeLeftNumber->display(this->bonusVisibleTimeLeft);
            if (this->bonusVisibleTimeLeft == 0) {
                this->bonusTimeLeftLabel->setVisible(false);
                this->bonusTimeLeftNumber->setVisible(false);
                this->removeGameElement(BONUS);
                this->isBonusVisible = false;

            }
        }
        if (!this->isBonusVisible) this->bonusTiks++;
        if (!this->isBonusVisible && this->bonusTiks == 20) {
            this->bonusTimeLeftLabel->setText(QObject::tr("Бонус исчезнет через"));
            this->addGameElement(BONUS);
            this->isBonusVisible = true;
            this->bonusTiks = 0;
            this->bonusVisibleTimeLeft = 20;
            this->bonusTimeLeftLabel->setVisible(true);
            this->bonusTimeLeftNumber->setVisible(true);
            this->bonusTimeLeftNumber->display(this->bonusVisibleTimeLeft);
        }
        QGraphicsItem *re = collaps(snake->at(0));

        if (re->data(0)=="Wall" || re->data(0)=="Snake") {
            this->finishGame();
            this->showMsg("You lose\n Score = "+QString::number(this->score));
          /*  for (int i = 0;i<snake->length()-1;i++) {
                snake->at(i)->setPos(snake->at(i+1)->x(), snake->at(i+1)->y());
            }
            this->removeGameElement(SNAKE);
            this->life--;
            if (this->life == 0) {
                this->finishGame();
            }*/
        }
        if (re->data(0) == "Food") {
            this->addGameElement(SNAKE);
         //   this->removeGameElement(FOOD);
            this->addGameElement(FOOD);
            this->score+=10;
            this->scoreNumber->display(this->score);
            this->ate++;
            this->ateNumber->display(this->ate);
            this->life++;
            this->lifeNumber->display(this->life);

        }
        if (re->data(0) == "Bonus") {
            this->snakeSpeed = NORMAL_SNAKE_SPEED;
            this->removeGameElement(BONUS);
            this->bonusAte++;
            this->bonusAteNumber->display(this->bonusAte);
            QString msg = "";
            int bonusKind = re->data(1).toInt();
            if (bonusKind == 0) {
                this->score += 100;
                this->scoreNumber->display(this->score);
                msg = "+100 score";
            } else if (bonusKind == 1) {
                this->snakeSpeed = NORMAL_SNAKE_SPEED;
                msg = "Normal";
            } else if (bonusKind == 2) {
                this->snakeSpeed = FAST_SNAKE_SPEED;
                msg = "Fast";
            } else if (bonusKind == 3) {
                this->snakeSpeed = SLOW_SNAKE_SPEED;
                msg = "Slow";
            }
          //  this->bonusTimeLeftLabel->setVisible(false);
            this->bonusTimeLeftLabel->setText(msg);
            this->bonusTimeLeftNumber->setVisible(false);
            this->gameTimer->start(this->snakeSpeed, this);
            this->idGameTimer = this->gameTimer->timerId();
        }


    }

}
void GameAreaScene::keyPress(QKeyEvent *keyEvent) {
    if (this->gameStatus != STARTED || !this->haveMooved) return;
    switch (keyEvent->key()) {
        case Qt::Key_Left:
            qDebug()<<"clicked left";
            if (this->presentVector == RIGHT) { return; }
            this->previousVector = this->presentVector;
            this->presentVector = LEFT;
            break;
        case Qt::Key_Right:
            qDebug()<<"clicked right";
            if (this->presentVector == LEFT) { return; }
            this->previousVector = this->presentVector;
            this->presentVector = RIGHT;
            break;
        case Qt::Key_Up:
            qDebug()<<"clicked up";
            if (this->presentVector == DOWN) { return; }
            this->presentVector = this->presentVector;
            this->presentVector = UP;
            break;
        case Qt::Key_Down:
            qDebug()<<"clicked down";
            if (this->presentVector == UP) { return; }
            this->previousVector = this->presentVector;
            this->presentVector = DOWN;
            break;
    }
    this->haveMooved = true;
}


void GameAreaScene::drawLevel(QVector<QRect> blocks) {
    QBrush brush(QColor(255, 255, 255), QPixmap(":/images/images/fon.jpg"));
    QPen pen(Qt::NoPen);
    this->addRect(0, 0, this->width(), this->height(), pen, brush);

    QBrush brush2(QColor(255, 255, 255), QPixmap(":/images/images/Wall.jpg"));
    this->levelBlocks = new QVector<QGraphicsRectItem*>();
    for (int i=0;i<blocks.size();i++) {
        this->levelBlocks->append(this->addRect(blocks.at(i), pen, brush2));
        this->levelBlocks->at(i)->setData(0,"Wall");
    }
}

void GameAreaScene::drawSnake(QPoint head) {
    snake = new QVector<QGraphicsPixmapItem*>();
    QPixmap pix(":/images/images/Pit.png");
    int w = pix.width();
    this->snakeHeight = pix.height();
    snake->append(addPixmap(QPixmap(":/images/images/rightHead.png")));
    snake->at(0)->setData(0, "SnakeHead");
    snake->at(0)->setPos(head.x(), head.y());
    for (int i = 1;i<3;i++) {
        snake->append(addPixmap(pix));
        snake->at(i)->setData(0, "Snake");
        snake->at(i)->setPos(snake->at(i-1)->x()-w, head.y());
    }
}

void GameAreaScene::setUI(QLCDNumber *scoreNumber, QLCDNumber *ateNumber, QLCDNumber *bonusAteNumber, QLCDNumber *lifeNumber,
                          QLCDNumber *bonusTimeLeftNumber, QLabel *bonusTimeLeftLabel) {
    this->scoreNumber = scoreNumber;
    this->ateNumber = ateNumber;
    this->bonusAteNumber = bonusAteNumber;
    this->lifeNumber = lifeNumber;
    this->bonusTimeLeftNumber = bonusTimeLeftNumber;
    this->bonusTimeLeftLabel = bonusTimeLeftLabel;

}

QGraphicsItem* GameAreaScene::collaps(QGraphicsItem *item){
    QList <QGraphicsItem*> collisions = this->collidingItems(item);
    foreach (QGraphicsItem *it, collisions) {
        if (it == item) continue;
        return it;
    }
    return NULL;
}
void GameAreaScene::addGameElement(GameElement gameElement) {
    srand(time(NULL));

    bool ok=false;
    while (ok==false) {
        QPointF rect;
        QGraphicsItem *item;
        if (gameElement == BONUS) {
            qDebug()<<"adding BONUS";
            item = this->bonusItem;
            item->setData(1, rand()%4);
        } else if (gameElement == FOOD) {
            qDebug()<<"adding FOOD";
            item = this->foodItem;
        } else if (gameElement == SNAKE) {
            qDebug()<<"adding SNAKE";
            snake->append(this->addPixmap(QPixmap(":/images/images/Pit.png")));
            item = snake->last();
            item->setData(0, "Snake");
        }
        if (gameElement == BONUS || gameElement == FOOD) {
            rect.setX((rand()%(int)(this->width()/this->snakeHeight*this->snakeHeight)));
            rect.setY((rand()%(int)(this->height()/this->snakeHeight*this->snakeHeight)));
        } else if (gameElement == SNAKE) {
            rect = this->zeroSnakePos;
        }

        item->setPos(rect);
        item->setVisible(true);
        QGraphicsItem *re = collaps(item);
        ok=true;

        if (re->data(0)=="Wall" || re->data(0)=="Snake" || re->data(0)=="Food"
                || re->data(0)=="Bonus" || re->data(0)=="SnakeHead") {
            ok=false;
            qDebug()<<"false";
        }
    }

}

void GameAreaScene::removeGameElement(GameElement gameElement) {
    if (gameElement == BONUS) this->bonusItem->setVisible(false);
    if (gameElement == FOOD) this->foodItem->setVisible(false);
    if (gameElement == SNAKE) {
        this->removeItem(snake->last());
        snake->last()->~QGraphicsPixmapItem();
        snake->removeLast();
    }
}
void GameAreaScene::showMsg(QString msg) {
    this->msgItem->setHtml(msg);
    this->msgItem->setPos(this->msgRectItem->boundingRect().width()/2 - this->msgItem->boundingRect().width()/2,
                          this->msgRectItem->boundingRect().height()/2 - this->msgItem->boundingRect().height()/2);
    this->msgRectItem->setPos(this->width()/2 - this->msgRectItem->boundingRect().width()/2,
                              this->height()/2 - this->msgRectItem->boundingRect().height()/2);
    this->msgItem->setVisible(true);
    this->msgRectItem->setVisible(true);
    this->msgItem->setZValue(3);
    this->msgRectItem->setZValue(3);
}
GameAreaScene::GameStatus GameAreaScene::getGameStatus() {
    return this->gameStatus;
}
