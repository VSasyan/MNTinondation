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

bool Polyligne::ajoutPoints(qglviewer::Vec pointE, qglviewer::Vec pointF){

    if (points.isEmpty()) {
    points.push_back(pointE);
    points.push_back(pointF);
            return false;
    }
    if(pointE == points[0]){
        points.push_front(pointF);
            return false;
    }
    if(pointE == points.back()){
        points.push_back(pointF);
            return false;
    }
    if(pointF == points[0]){
        points.push_front(pointE);
            return false;
    }
    if(pointF == points.back()){
        points.push_back(pointE);
            return false;
    }
}

void Polyligne::fusionnerPoly(QList<qglviewer::Vec> *points2){
    if(points.first() == points2->first()){ //mettre p2 devant en partant du début (fin p2 devient début polyligne)
        points2->pop_front();
        std::reverse(points2->begin(), points2->end());
        points2->operator +=(points);
        while (points2->size()!=0){
            //points.push_front(points2->first());
            points2->pop_front();
        }
    }
    else if(points.first() == points2->back()){ //mettre p2 devant en partant de la fin (début p2 devient début polyligne)
        points2->pop_back();
        points2->operator +=(points); //devant
        while (points2->size()!=0){
            //points.push_front(points2->back());
            points2->pop_back();
        }
    }
    else if(points.last() == points2->first()){ //mettre p2 derrière en partant du début (fin p2 devient début polyligne)
        points2->pop_front();
        points.operator +=(*points2); //à la suite
        while (points2->size()!=0){
            //points.push_back(points2->first());
            points2->pop_front();
        }
    }
    else if(points.last() == points2->back()){ //mettre p2 derrière en partant de la fin (début p2 devient début polyligne)
        points2->pop_back();
        points.operator +=(*points2);
        std::reverse(points2->begin(), points2->end());
        while (points2->size()!=0){
            //points.push_back(points2->back());
            points2->pop_back();
        }
    }
}
