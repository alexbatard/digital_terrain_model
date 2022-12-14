#include "terrain_data.h"
#include <math.h>
#include <proj.h>

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

void TerrainData::set_X_Y_coords(){
    PJ *P;
    PJ_COORD a, b;

    P = proj_create_crs_to_crs(PJ_DEFAULT_CTX,
                               "+proj=longlat +datum=WGS84",
                               "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                               NULL);

    a = proj_coord(getLat(), getLon(), 0, 0);
    b = proj_trans(P, PJ_FWD, a);
    m_x = b.xy.x;
    m_y = b.xy.y;

    // Clean up
    proj_destroy(P);
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
