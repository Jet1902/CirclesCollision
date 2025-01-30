#ifndef QBALLSWIDGET_H
#define QBALLSWIDGET_H

#include <QWidget>

class QBallsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QBallsWidget(QWidget *parent = nullptr);

    qreal xToLocal(qreal x) const;
    qreal yToLocal(qreal y) const;

    QPointF pntToLocal(const QPointF pnt) const;
    QRectF rectToLocal(const QRectF rc) const;

    qreal xToWorld(qreal x) const;
    qreal yToWorld(qreal y) const;

    QPointF pntToWorld(const QPointF pnt) const;

signals:

public slots:
    void onBallsUpdated();

protected:
    int hitTest(const QPointF& pos);    // Возвращает индекс первого попавшегося шарика, в который попадают коондинаты pos

private:
    QRectF worldRect;   // Область, занимаемая миров в нашем виджете

    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
};

#endif // QBALLSWIDGET_H
