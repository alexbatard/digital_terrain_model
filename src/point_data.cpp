#include "point_data.h"
#include <iostream>

using namespace std;

PointData::PointData()
{
}

PointData::~PointData()
{
}

double PointData::getLat() const
{
    return m_lat;
}

double PointData::getLon() const
{
    return m_lon;
}

double PointData::getAlt() const
{
    return m_alt;
}

double PointData::getX() const
{
    return m_x;
}

double PointData::getY() const
{
    return m_y;
}

void PointData::setXYCoords(PJ_COORD cartesian_coord)
{
    m_x = cartesian_coord.xy.x;
    m_y = cartesian_coord.xy.y;

    // Print the coordinates
    // cout.precision(10);
    // cout << "X: " << m_x << endl;
    // cout << "Y: " << m_y << endl;
}

istream &operator>>(istream &input, PointData &data)
{
    input >> data.m_lat >> data.m_lon >> data.m_alt;
    return input;
}

ostream &operator<<(ostream &output, const PointData &data)
{
    output << data.m_lat << " " << data.m_lon << " " << data.m_alt;
    return output;
}
