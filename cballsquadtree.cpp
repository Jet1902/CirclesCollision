#include "cballsquadtree.h"
#include "ccolorball.h"

#include <QTime>
#include <QDebug>

CBallsQuadTree::CBallsQuadTree()
{

}

bool CBallsQuadTree::insert(CColorBall *pBall)
{
    if(!m_boundary.contains(pBall->position()))
        return false;

    if(m_vBallPtrs.size() < m_maxNum    // Если количество хранимых шариков меньше максимального
       && m_vNodes.size() == 0)         // и мы еще не поделились
    {
        m_vBallPtrs.push_back(pBall);
        return true;
    }

    if(m_vNodes.size() == 0)
        subdivide();

    for(CBallsQuadTree& node : m_vNodes)
        if(node.insert(pBall))
            return true;

    Q_ASSERT(false);
    return false;
}

QVector<CColorBall *> CBallsQuadTree::queryRange(const QRectF &range)
{
    QVector<CColorBall *> pointsInRange;
    if(!range.intersects(m_boundary))
        return pointsInRange;

    if(m_vNodes.empty())        // Если мы не поделилимь
    {
        for(int p = 0; p < m_vBallPtrs.size(); p++)
            if(range.contains(m_vBallPtrs[p]->position()))
                pointsInRange.push_back(m_vBallPtrs[p]);
        return  pointsInRange;
    }

    for (CBallsQuadTree& node : m_vNodes)
        pointsInRange.append(node.queryRange(range));

    return pointsInRange;
}

void CBallsQuadTree::paint(QPainter &painter, QBallsWidget& w) const
{
    if(m_vNodes.size())
        foreach(const CBallsQuadTree& node, m_vNodes)
            node.paint(painter, w);
    painter.drawRect(w.rectToLocal(m_boundary));
}

void CBallsQuadTree::subdivide()
{
    QRectF quad(m_boundary.topLeft(), m_boundary.size() / 2);

    quad.moveTopRight(m_boundary.topRight());
    m_vNodes.push_back(CBallsQuadTree(quad));

    quad.moveBottomRight(m_boundary.bottomRight());
    m_vNodes.push_back(CBallsQuadTree(quad));

    quad.moveBottomLeft(m_boundary.bottomLeft());
    m_vNodes.push_back(CBallsQuadTree(quad));

    quad.moveTopLeft(m_boundary.topLeft());
    m_vNodes.push_back(CBallsQuadTree(quad));

    foreach(CColorBall* pBall, m_vBallPtrs)
        for(CBallsQuadTree& node : m_vNodes)
            if(node.insert(pBall))
                break;

    m_vBallPtrs.clear();
}
