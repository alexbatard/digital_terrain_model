#include "terrain_data.h"
#include <math.h>

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

double TerrainData::getX(int size)
{
    double x = fmod((size * (180 + getLon()) / 360), (size + (size / 2)));
    return x;
}

double TerrainData::getY(int size)
{
    double lat_rad = getLat() * M_PI / 180;
    double merc_n = log(tan((M_PI / 4) + (lat_rad / 2)));
    double y = (size / 2) - (size * merc_n / (2 * M_PI));
    return y;
}

istream &operator>>(istream &input, TerrainData &data)
{
    char space;
    input >> data.m_lat >> space >> data.m_lon >> space >> data.m_alt;
    return input;
}

ostream &operator<<(ostream &output, const TerrainData &data)
{
    output << data.m_lat << " " << data.m_lon << " " << data.m_alt;
    return output;
}
