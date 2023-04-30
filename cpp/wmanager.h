#ifndef WMANAGER_H
#define WMANAGER_H

#include <QObject>
// #include <QQuickWidget>
#include <QQuickView>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QQmlEngine>
#include <QQmlContext>

#include <QDebug>
#include <QStringList>
#include <QPoint>
#include <vector>
#include "gamelife.h"

class WManager : public QObject
{
    Q_OBJECT

public:
    explicit WManager(QObject *parent = 0);

    // Permet d'avoir une seule instance
    static WManager *getInstance() {
        if(NULL == _singleton) {
            _singleton = new WManager ;
        }
        return _singleton ;
    }

    void load();

    void makeQMLtab(QString nomFichierQMLsansExtension);

    void displayInitialInformations();

    //...........................................................................

    void updateQML_ListView(QString nomModele, QStringList sl);

    Q_INVOKABLE void updatePoint(QPoint);
    Q_INVOKABLE void play(int iterations = 10000);
    Q_INVOKABLE void reset();
    Q_INVOKABLE void rand();
    Q_INVOKABLE void stepByStep();
    Q_INVOKABLE void back();
    Q_INVOKABLE void resize(int lines, int columns);

    Q_INVOKABLE void save(QString file);
    Q_INVOKABLE void load(QString file);

    Q_INVOKABLE void setTemporisation(double inter);

private:
    void initPlateau(int lines = 10, int columns = 10);

protected:
    std::vector<QQuickView*> quickViews;

    QString getRepertoireProjet(bool trace = false);
    
    QVector<QQmlContext *>          m_QMLcontexts;
    QQmlContext*                    m_qmlContext;

signals:

public slots:
    void update();

private:
    GameLife m_glife;

    QTimer * m_timer;

    int m_iterationsMax;
    int m_iterations;

    static WManager *_singleton ;
};

#endif // WMANAGER_H


