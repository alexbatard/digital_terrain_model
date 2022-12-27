#ifndef __TERRAIN_DATA_H__
#define __TERRAIN_DATA_H__

#include <list>

using namespace std;

class TerrainData
{
public:
    TerrainData();
    ~TerrainData();
    void addCoordX(double coord_X);
    void addCoordY(double coord_Y);
    void addCoordZ(double coord_Z);
    void printDataX();
    void printDataY();
    void printDataZ();

protected:
    list<double> data_x;
    list<double> data_y;
    list<double> data_z;
};

#endif
