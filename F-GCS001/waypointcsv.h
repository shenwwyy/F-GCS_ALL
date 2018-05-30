#ifndef WAYPOINTCSV_H
#define WAYPOINTCSV_H

#include <QObject>

class WayPointCSV : public QObject
{
    Q_OBJECT
public:
    explicit WayPointCSV(QObject *parent = 0);



typedef struct {
      bool   circle;
      qint8  group;
      qint32 totalpoint;
      qint32 currentpoint;
      double longitude;
      double latitude;
      float  altitude;
      float  velocity;
      float  radiu;
      qint8  action;
}_waypoint;



    void Save(_waypoint *WayPoint);
    void Read(quint8 Group, qint32 PointNumber, _waypoint *WayPoint);


signals:

};

#endif // WAYPOINTCSV_H
