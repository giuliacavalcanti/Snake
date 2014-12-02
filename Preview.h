#ifndef PREVIEW_H
#define PREVIEW_H
#include <QGraphicsView>
#include <QMessageBox>

class Preview : public QGraphicsView {
    Q_OBJECT
public:
    Preview(QGraphicsScene* scene):QGraphicsView(scene) {

    }
    ~Preview(){

    }

protected:
    virtual void mousePressEvent(QMouseEvent *event){
        emit clicked();

    }
signals:
    void clicked();
};
#endif // PREVIEW_H
