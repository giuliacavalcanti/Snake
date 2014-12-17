#ifndef PREVIEW_H
#define PREVIEW_H
#include <QGraphicsView>
#include <QMessageBox>

class Preview : public QGraphicsView {
    Q_OBJECT
public:
    Preview(QGraphicsScene* scene, int i):QGraphicsView(scene) {
        this->i = i;
    }
    ~Preview(){

    }
    int getI() { return i; }
private:
    int i;
protected:
    virtual void mousePressEvent(QMouseEvent *event){
        emit clicked(this->i);
    }
signals:
    void clicked(int);
};
#endif // PREVIEW_H
