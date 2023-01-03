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
    const vector<double> &getCoordX() const { return coords_X; }
    const vector<double> &getCoordY() const { return coords_Y; }
    const vector<double> &getCoordZ() const { return coords_Z; }
    void printDataX();
    void printDataY();
    void printDataZ();

protected:
    vector<double> coords_X;
    vector<double> coords_Y;
    vector<double> coords_Z;
};

#endif
