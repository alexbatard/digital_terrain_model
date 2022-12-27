#include "point_data.h"
#include "terrain_data.h"
#include <proj.h>
#include <fstream>
#include <iostream>
#include <list>

using namespace std;

// void printList(list<double> const &list)
// {
//     for (auto const &i : list)
//     {
//         cout.precision(10);
//         cout << i << endl;
//     }
// }

void getXYZData(const string &filename, PointData point, TerrainData terrain, PJ *P, PJ_COORD geo_coord, PJ_COORD cartesian_coord)
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        exit(1);
    }

    while (file >> point)
    {
        geo_coord.lpzt.lam = point.getLon();
        geo_coord.lpzt.phi = point.getLat();
        geo_coord.lpzt.z = 0.;

        cartesian_coord = proj_trans(P, PJ_FWD, geo_coord);

        point.setXYCoords(cartesian_coord);

        terrain.addCoordX(point.getX());
        terrain.addCoordY(point.getY());
        terrain.addCoordZ(point.getAlt());
    }
    terrain.printDataX();
 

    file.close();
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Error! create_raster takes 2 arguments: data file path & image size in px" << endl;
        exit(1);
    }

    // input variables
    string filename = argv[1];
    int size = stoi(argv[2]);

    // data variables
    PointData point;
    TerrainData terrain;

    // proj
    PJ *P = proj_create_crs_to_crs(PJ_DEFAULT_CTX,
                               "+proj=longlat +datum=WGS84",
                               "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                               NULL);
    PJ *norm = proj_normalize_for_visualization(PJ_DEFAULT_CTX, P);
    PJ_COORD geo_coord, cartesian_coord;

    proj_destroy(P);
    P = norm;

    // get XYZ coordinates
    getXYZData(filename, point, terrain, P, geo_coord, cartesian_coord);

    proj_destroy(P);

    return 0;
}