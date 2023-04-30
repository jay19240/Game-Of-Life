#include "wmanager.h"
#include <QApplication>

#include <QDir>
#include <QFileDialog>
#include <QTimer>
#include <iostream>

#include "gamelife.h"


WManager::WManager(QObject *parent) : QObject(parent), m_iterations(0)
{
    qDebug() << "WManager::instance construite ";

    m_timer = new QTimer(this);
}

void WManager::load()
{
    QList<QString> fichiersQML;
    fichiersQML << QString("qml");

    for( int t=0 ; t<1 ; t++)
    {
        makeQMLtab(fichiersQML.at(t));
        std::cout << "tab " << t << " ready." << std::endl;
    }
}

void WManager::makeQMLtab(QString nomFichierQMLsansExtension)
{
    QQuickView* view = new QQuickView();
    quickViews.push_back(view);

    view->setResizeMode( QQuickView::SizeRootObjectToView);

    // Mise en mémoire des objets pour communiquer avec le QML (avant le chargement de la page)
    m_QMLcontexts << view->engine()->rootContext();

    m_qmlContext = m_QMLcontexts.at(0);
    m_qmlContext->setContextProperty("Context", this->getInstance());
    //

    // Initialisation plateau
    initPlateau();

    //
    QString repertoireProjet = getRepertoireProjet();

    QString fichierQML = repertoireProjet + QString("/qml/") + nomFichierQMLsansExtension + QString(".qml");
    std::cout  << "charge le fichier QML : " << fichierQML.toLatin1().constData() << std::endl;

    // Chargement du fichier QML
    view->setSource( QUrl::fromLocalFile(fichierQML) ) ;
    view->show();
}

QString WManager::getRepertoireProjet(bool trace)//false
{
    QString repertoireduFichierApplication( qApp->applicationFilePath() );
    if( trace)
        std::cout << "qApp->applicationFilePath() = "<< repertoireduFichierApplication.toLatin1().constData() << std::endl;

    QDir fichier(repertoireduFichierApplication);
    fichier.cdUp() ;
    fichier.cdUp() ;
    if( trace)
        std::cout << "qApp->applicationName() = "<< qApp->applicationName().toLatin1().constData() << std::endl;

    fichier.cd(qApp->applicationName());
    QString repertoireProjet  = fichier.absolutePath() ;
    if( trace)
        std::cout  << "repertoireProjet : " << repertoireProjet.toLatin1().constData() << std::endl;

    return repertoireProjet;
}

void WManager::displayInitialInformations()
{
    getRepertoireProjet(true);
}

//.............................................................

void WManager::updateQML_ListView(QString nomModele, QStringList sl)
{
    m_qmlContext = m_QMLcontexts.at(0);
    m_qmlContext->setContextProperty(nomModele, QVariant::fromValue( sl ) );
}

void WManager::updatePoint(QPoint pt)
{
    int cellVal = 1 - m_glife.getCell(pt.y(), pt.x());

    m_glife.setCell(pt.y(), pt.x(), cellVal);
    updateQML_ListView("modelPlateau", m_glife.serialize());
}

void WManager::play(int iterationsMax)
{
    m_iterations = 0;
    m_iterationsMax = iterationsMax;

    connect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
}

void WManager::update()
{
    if (m_iterations < m_iterationsMax && m_glife.hasCellAlived())
    {
        m_glife.play();

        updateQML_ListView("modelPlateau", m_glife.serialize());

        m_iterations++;
    }
    else
    {
        disconnect(m_timer, SIGNAL(timeout()), this, SLOT(update()));
    }
}

void WManager::reset()
{
    m_glife.reset();
    m_iterations = 0;

    updateQML_ListView("modelPlateau", m_glife.serialize());
}

void WManager::rand()
{
    m_glife.random();
    m_iterations = 0;

    updateQML_ListView("modelPlateau", m_glife.serialize());
}

void WManager::stepByStep()
{
    m_glife.play();
    updateQML_ListView("modelPlateau", m_glife.serialize());
}

void WManager::back()
{
    m_glife.back();
    updateQML_ListView("modelPlateau", m_glife.serialize());
}

void WManager::resize(int lines, int columns)
{
    m_glife.resizeGrid(lines, columns);
    updateQML_ListView("modelPlateau", m_glife.serialize());
}

void WManager::save(QString file)
{
    m_glife.save(file);
}

void WManager::load(QString file)
{
    m_glife.load(file);
    updateQML_ListView("modelPlateau", m_glife.serialize());
}

void WManager::setTemporisation(double inter)
{
    m_timer->start(inter * 1000);
}

void WManager::initPlateau(int lines, int columns)
{
    m_iterations = 0;

    m_glife.initialisation(lines, columns);
    updateQML_ListView("modelPlateau", m_glife.serialize());
}
