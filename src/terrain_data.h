#ifndef __TERRAIN_DATA_H__
#define __TERRAIN_DATA_H__

#include <fstream>
#include <proj.h>

using namespace std;

class TerrainData
{
public:
    TerrainData();
    ~TerrainData();
    double getLat() const;
    double getLon() const;
    double getAlt() const;
    double getX() const;
    double getY() const;
    void set_X_Y_coords(PJ_COORD cartesian_coord);

    friend istream &operator>>(istream &input, TerrainData &data);
    friend ostream &operator<<(ostream &output, const TerrainData &data);

protected:
    double m_lat;
    double m_lon;
    double m_alt;
    double m_x;
    double m_y;
};

#endif
