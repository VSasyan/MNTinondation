#include "polyligne.h"

//constructeur vide
Polyligne::Polyligne(){
    points = QList<qglviewer::Vec>();
}

Polyligne::Polyligne(qglviewer::Vec pointE, qglviewer::Vec pointF){
    points = QList<qglviewer::Vec>();
    points.push_back(pointE);
    points.push_back(pointF);
}

Polyligne::~Polyligne(){
}

int Polyligne::getSize() {
    return points.size();
}

qglviewer::Vec Polyligne::getPoint(int i, Recentreur recentreur) {
    qglviewer::Vec point = qglviewer::Vec(points.at(i));
    point *= recentreur.pas;
    point += recentreur.delta;
    return point;
}

QList<qglviewer::Vec> *Polyligne::getPoints(){
    return &points;
}

bool Polyligne::testerAjout(qglviewer::Vec pointE, qglviewer::Vec pointF){

    if (points.isEmpty()) {
        return true;
    }
    if(pointE == points[0]){
        return true;
    }
    if(pointE == points.back()){
        return true;
    }
    if(pointF == points[0]){
        return true;
    }
    if(pointF == points.back()){
        return true;
    }
    return false;
}

void Polyligne::ajoutPoints(qglviewer::Vec pointE, qglviewer::Vec pointF){
    if (points.isEmpty()) {
        points.push_back(pointE);
        points.push_back(pointF);
    } else if(pointE == points[0]){
        points.push_front(pointF);
    } else if(pointE == points.back()){
        points.push_back(pointF);
    } else if(pointF == points[0]){
        points.push_front(pointE);
    } else if(pointF == points.back()){
        points.push_back(pointE);
    }
}

void Polyligne::fusionnerPoly(QList<qglviewer::Vec> *points2){
    if(points.first() == points2->first()){ //mettre p2 devant en partant du début (fin p2 devient début polyligne)
        points2->pop_front();
        std::reverse(points2->begin(), points2->end());
        points2->operator +=(points);
        while (points2->size()!=0){
            points2->pop_front();
        }
    }
    else if(points.first() == points2->back()){ //mettre p2 devant en partant de la fin (début p2 devient début polyligne)
        points2->pop_back();
        points2->operator +=(points); //devant
        while (points2->size()!=0){
            points2->pop_back();
        }
    }
    else if(points.last() == points2->first()){ //mettre p2 derrière en partant du début (fin p2 devient début polyligne)
        points2->pop_front();
        points.operator +=(*points2); //à la suite
        while (points2->size()!=0){
            points2->pop_front();
        }
    }
    else if(points.last() == points2->back()){ //mettre p2 derrière en partant de la fin (début p2 devient début polyligne)
        points2->pop_back();
        points.operator +=(*points2);
        std::reverse(points2->begin(), points2->end());
        while (points2->size()!=0){
            points2->pop_back();
        }
    }
}

