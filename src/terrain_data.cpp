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
    coords_X.push_back(coord_X);
}

void TerrainData::addCoordY(double coord_Y)
{
    coords_Y.push_back(coord_Y);
}

void TerrainData::addCoordZ(double coord_Z)
{
    coords_Z.push_back(coord_Z);
}

void TerrainData::printDataX()
{
    for (auto const &i : coords_X)
    {
        cout.precision(10);
        cout << i << endl;
    }
}

void TerrainData::printDataY()
{
    for (auto const &i : coords_Y)
    {
        cout.precision(10);
        cout << i << endl;
    }
}

void TerrainData::printDataZ()
{
    for (auto const &i : coords_Z)
    {
        cout.precision(10);
        cout << i << endl;
    }
}
