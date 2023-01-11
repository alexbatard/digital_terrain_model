#ifndef __TERRAIN_DATA_H__
#define __TERRAIN_DATA_H__

#include <vector>

using namespace std;

class TerrainData
{
public:
    TerrainData();
    ~TerrainData();
    void addCoordX(double coord_X);
    void addCoordY(double coord_Y);
    void addCoordZ(double coord_Z);
    const vector<double> &getCoordX() const { return m_coords_X; }
    const vector<double> &getCoordY() const { return m_coords_Y; }
    const vector<double> &getCoordZ() const { return m_coords_Z; }
    void printDataX();
    void printDataY();
    void printDataZ();

protected:
    vector<double> m_coords_X;
    vector<double> m_coords_Y;
    vector<double> m_coords_Z;
};

#endif
