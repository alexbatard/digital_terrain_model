#ifndef __POINT_DATA_H__
#define __POINT_DATA_H__

#include <fstream>
#include <proj.h>

using namespace std;

class PointData
{
public:
    PointData();
    ~PointData();
    double getLat() const;
    double getLon() const;
    double getAlt() const;
    double getX() const;
    double getY() const;
    void setXYCoords(PJ_COORD cartesian_coord);

    friend istream &operator>>(istream &input, PointData &data);
    friend ostream &operator<<(ostream &output, const PointData &data);

protected:
    double m_lat;
    double m_lon;
    double m_alt;
    double m_x;
    double m_y;
};

#endif
