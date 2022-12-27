#include "terrain_data.h"
#include <iostream>

using namespace std;

TerrainData::TerrainData()
{
}

TerrainData::~TerrainData()
{
}

void TerrainData::addCoordX(double coord_X)
{
    data_x.push_back(coord_X);
}

void TerrainData::addCoordY(double coord_Y)
{
    data_y.push_back(coord_Y);
}

void TerrainData::addCoordZ(double coord_Z)
{
    data_z.push_back(coord_Z);
}

void TerrainData::printDataX()
{
    for (auto const &i : data_x)
    {
        cout.precision(10);
        cout << i << endl;
    }
}

void TerrainData::printDataY()
{
    for (auto const &i : data_y)
    {
        cout.precision(10);
        cout << i << endl;
    }
}

void TerrainData::printDataZ()
{
    for (auto const &i : data_z)
    {
        cout.precision(10);
        cout << i << endl;
    }
}
