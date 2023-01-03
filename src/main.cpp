#include "point_data.h"
#include "terrain_data.h"
#include <proj.h>
#include <fstream>
#include <iostream>
#include <vector>
#include "delaunator.hpp"

using namespace std;

// data variables
PointData point;
TerrainData terrain;
vector<double> points;
vector<double> p = {252930, 6.80564e+06};

void getXYZData(const string &filename, PJ *P, PJ_COORD geo_coord, PJ_COORD cartesian_coord)
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

        points.push_back(point.getX());
        points.push_back(point.getY());
    }
    file.close();
}

// Check if the point (x, y) is inside the triangle with vertices (xa, ya), (xb, yb), and (xc, yc)
bool isInsideTriangle(double x, double y, double xa, double ya, double xb, double yb, double xc, double yc)
{
    // Compute the barycentric coordinates of the point
    double A = ((yb - yc) * (x - xc) + (xc - xb) * (y - yc)) / ((yb - yc) * (xa - xc) + (xc - xb) * (ya - yc));
    double B = ((yc - ya) * (x - xc) + (xa - xc) * (y - yc)) / ((yb - yc) * (xa - xc) + (xc - xb) * (ya - yc));
    double C = 1 - A - B;

    // Check if the point is inside the triangle
    return A >= 0 && A <= 1 && B >= 0 && B <= 1 && C >= 0 && C <= 1;
}

void interpolation(vector<double> &p, delaunator::Delaunator &d)
{
    // Iterate over all the triangles in the Delaunay triangulation
    for (size_t i = 0; i < d.triangles.size(); i += 3)
    {
        // Get the indices of the vertices of the triangle
        size_t a = d.triangles[i], b = d.triangles[i + 1], c = d.triangles[i + 2];

        // Get the coordinates of the vertices of the triangle
        double xa = d.coords[2 * d.triangles[i]];
        double ya = d.coords[2 * d.triangles[i] + 1];
        double xb = d.coords[2 * d.triangles[i + 1]];
        double yb = d.coords[2 * d.triangles[i + 1] + 1];
        double xc = d.coords[2 * d.triangles[i + 2]];
        double yc = d.coords[2 * d.triangles[i + 2] + 1];

        // cout << "Triangle " << i / 3 << ": " << xa << " " << ya << " " << xb << " " << yb << " " << xc << " " << yc << endl;

        // Check if the point is inside the triangle
        if (isInsideTriangle(p[0], p[1], xa, ya, xb, yb, xc, yc))
        {
            cout.precision(10);
            cout << "Triangle " << i / 3 << ": " << xa << " " << ya << " " << xb << " " << xc << " " << yc << " contains the point" << endl;

            // Get the altitude values of the vertices of the triangle
            double za = terrain.getCoordZ()[i];
            double zb = terrain.getCoordZ()[i + 1];
            double zc = terrain.getCoordZ()[i + 2];

            // Compute the barycentric coordinates of the point
            double A = ((yb - yc) * (p[0] - xc) + (xc - xb) * (p[1] - yc)) / ((yb - yc) * (xa - xc) + (xc - xb) * (ya - yc));
            double B = ((yc - ya) * (p[0] - xc) + (xa - xc) * (p[1] - yc)) / ((yb - yc) * (xa - xc) + (xc - xb) * (ya - yc));
            double C = 1 - A - B;

            // Interpolate the value using the barycentric coordinates
            double zp = A * za + B * zb + C * zc;
            cout << "Altitude values of the vertices: " << za << " " << zb << " " << zc << endl;
            cout << "Interpolated value at point p: " << zp << endl;
        }
    }
}

double determinePixelColor(double pixel_x, double pixel_y)
{
    // TODO
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

    // proj
    PJ *P = proj_create_crs_to_crs(PJ_DEFAULT_CTX,
                                   "+proj=longlat +datum=WGS84",
                                   "+proj=lcc +lat_1=49 +lat_2=44 +lat_0=46.5 +lon_0=3 +x_0=700000 +y_0=6600000 +ellps=GRS80 +towgs84=0,0,0,0,0,0,0 +units=m +no_defs",
                                   NULL);
    PJ *norm = proj_normalize_for_visualization(PJ_DEFAULT_CTX, P);
    PJ_COORD geo_coord, cartesian_coord;

    proj_destroy(P);
    P = norm;

    // get XYZ coordinates and add coordinates to points vector
    getXYZData(filename, P, geo_coord, cartesian_coord);
    proj_destroy(P);

    // Create Delaunator object
    delaunator::Delaunator d(points);

    interpolation(p, d);

    return 0;
}