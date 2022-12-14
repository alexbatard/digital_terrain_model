#ifndef __TERRAIN_DATA_H__
#define __TERRAIN_DATA_H__

#include <fstream>

using namespace std;

class TerrainData
{
public:
    TerrainData();
    ~TerrainData();
    double getLat() const;
    double getLon() const;
    double getAlt() const;
    double getX(int size);
    double getY(int size);
    void set_X_Y_coords();

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
