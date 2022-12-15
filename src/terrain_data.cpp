#include "terrain_data.h"
#include <iostream>

using namespace std;

TerrainData::TerrainData()
{
}

TerrainData::~TerrainData()
{
}

double TerrainData::getLat() const
{
    return m_lat;
}

double TerrainData::getLon() const
{
    return m_lon;
}

double TerrainData::getAlt() const
{
    return m_alt;
}

double TerrainData::getX() const
{
    return m_x;
}

double TerrainData::getY() const
{
    return m_y;
}

void TerrainData::set_X_Y_coords(PJ_COORD cartesian_coord)
{
    m_x = cartesian_coord.xy.x;
    m_y = cartesian_coord.xy.y;

    // Print the coordinates
    // cout.precision(10);
    // cout << "X: " << m_x << endl;
    // cout << "Y: " << m_y << endl;
}

istream &operator>>(istream &input, TerrainData &data)
{
    input >> data.m_lat >> data.m_lon >> data.m_alt;
    return input;
}

ostream &operator<<(ostream &output, const TerrainData &data)
{
    output << data.m_lat << " " << data.m_lon << " " << data.m_alt;
    return output;
}
