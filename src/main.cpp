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
vector<double> points;           // vector of x and y coordinates from the terrain data
vector<double> x_y_z_min_max(6); // x, y and z coordinates min and max values
// vector<double> p_test = {252930, 6.80564e+06};

const int numColors = 9; // number of colors
const double colorValues[numColors] = {0.0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1.0};
const double colors[numColors][3] = {{59, 76, 192},
                                     {68, 90, 204},
                                     {77, 104, 215},
                                     {87, 117, 225},
                                     {98, 130, 234},
                                     {108, 142, 241},
                                     {119, 154, 247},
                                     {130, 165, 251},
                                     {141, 176, 254}};

// Linear interpolation between two colors
void lerpColor(const double t, const double *c1, const double *c2, double *result)
{
    result[0] = int((1.0 - t) * c1[0] + t * c2[0]);
    result[1] = int((1.0 - t) * c1[1] + t * c2[1]);
    result[2] = int((1.0 - t) * c1[2] + t * c2[2]);
}

// Generate a color from the Haxby color map
void haxbyColor(double value, double *result)
{
    if (value <= 0.0)
    {
        result[0] = colors[0][0];
        result[1] = colors[0][1];
        result[2] = colors[0][2];
        return;
    }
    if (value >= 1.0)
    {
        result[0] = colors[numColors - 1][0];
        result[1] = colors[numColors - 1][1];
        result[2] = colors[numColors - 1][2];
        return;
    }

    int idx;
    for (idx = 0; idx < numColors - 1; idx++)
    {
        if (colorValues[idx] <= value && value < colorValues[idx + 1])
        {
            break;
        }
    }

    double t = (value - colorValues[idx]) / (colorValues[idx + 1] - colorValues[idx]);
    lerpColor(t, colors[idx], colors[idx + 1], result);
}

// Get the x, y and z coordinates min and max values (fills the x_y_z_min_max vector)
vector<double> findMinMax(const vector<double> &points, PointData &point)
{
    vector<double> min_max = x_y_z_min_max;
    if (points.size() == 2 && terrain.getCoordZ().size() == 1)

    {
        min_max[0] = points[0];
        min_max[1] = points[0];
        min_max[2] = points[1];
        min_max[3] = points[1];
        min_max[4] = terrain.getCoordZ()[0];
        min_max[5] = terrain.getCoordZ()[0];
    }
    else
    {
        if (point.getX() < min_max[0])
        {
            min_max[0] = point.getX();
        }
        if (point.getX() > min_max[1])
        {
            min_max[1] = point.getX();
        }
        if (point.getY() < min_max[2])
        {
            min_max[2] = point.getY();
        }
        if (point.getY() > min_max[3])
        {
            min_max[3] = point.getY();
        }
        if (point.getAlt() < min_max[4])
        {
            min_max[4] = point.getAlt();
        }
        if (point.getAlt() > min_max[5])
        {
            min_max[5] = point.getAlt();
        }
    }
    return min_max;
}

// Get the x, y and z coordinates from the file
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
        x_y_z_min_max = findMinMax(points, point);
        // cout << point.getX() << " " << point.getY() << " " << point.getAlt() << endl;
        // cout << x_y_z_min_max[0] << " " << x_y_z_min_max[1] << " " << x_y_z_min_max[2] << " " << x_y_z_min_max[3] << " " << x_y_z_min_max[4] << " " << x_y_z_min_max[5] << endl;
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

// Interpolate the color of the point (x, y) using the altitudes of the vertices of the triangle
void interpolation(delaunator::Delaunator &d, vector<double> &p, double color[3])
{
    // TODO sort the list of triangles based on the coordinates of their center
    // TODO dichotomic search to find the triangle(s) containing the point (x, y)

    // Iterate over all the triangles in the Delaunay triangulation
    for (size_t i = 0; i < d.triangles.size(); i += 3)
    {
        // Get the indices of the vertices of the triangle
        size_t a = d.triangles[i], b = d.triangles[i + 1], c = d.triangles[i + 2];
        // cout << "Triangle " << i / 3 << ": " << a << " " << b << " " << c << endl;

        // Get the coordinates of the vertices of the triangle
        double xa = d.coords[2 * d.triangles[i]];
        double ya = d.coords[2 * d.triangles[i] + 1];
        double xb = d.coords[2 * d.triangles[i + 1]];
        double yb = d.coords[2 * d.triangles[i + 1] + 1];
        double xc = d.coords[2 * d.triangles[i + 2]];
        double yc = d.coords[2 * d.triangles[i + 2] + 1];

        //     double x_center = (xa + xb + xc) / 3;
        //     double y_center = (ya + yb + yc) / 3;

        // vector<double> p = {x_center, y_center};

        // cout << "Triangle " << i / 3 << ": " << xa << " " << ya << " " << xb << " " << yb << " " << xc << " " << yc << endl;

        // Check if the point is inside the triangle
        if (!isInsideTriangle(p[0], p[1], xa, ya, xb, yb, xc, yc))
        {
            // cout.precision(10);
            // cout << "Triangle " << i / 3 << ": " << xa << " " << ya << " " << xb << " " << xc << " " << yc << " contains the point" << endl;

            // Get the altitude values of the vertices of the triangle
            double za = terrain.getCoordZ()[a];
            double zb = terrain.getCoordZ()[b];
            double zc = terrain.getCoordZ()[c];

            // Compute the barycentric coordinates of the point
            double A = ((yb - yc) * (p[0] - xc) + (xc - xb) * (p[1] - yc)) / ((yb - yc) * (xa - xc) + (xc - xb) * (ya - yc));
            double B = ((yc - ya) * (p[0] - xc) + (xa - xc) * (p[1] - yc)) / ((yb - yc) * (xa - xc) + (xc - xb) * (ya - yc));
            double C = 1 - A - B;

            // Interpolate the value using the barycentric coordinates
            double z_min = x_y_z_min_max[4];
            double z_max = x_y_z_min_max[5];
            double zp = A * za + B * zb + C * zc;
            double zp_norm = (zp - z_max) / (z_min - z_max); // normalize the altitude value
            haxbyColor(zp_norm, color);                      // determine the color of the point based on the altitude value (fills the color variable)
            cout << "Altitude values of the vertices: " << za << " " << zb << " " << zc << endl;
            cout << "Interpolated value at point p: " << zp << endl;
            // return zp;
        }
    }
}

// create the image, looping on all pixels
void create_image(int img_width, delaunator::Delaunator &d)
{
    double px_size = (x_y_z_min_max[1] - x_y_z_min_max[0]) / img_width;  // size of a pixel in the projection
    double img_height = (x_y_z_min_max[3] - x_y_z_min_max[2]) / px_size; // height of the image in pixels
    double color[3];                                                     // color of a pixel in the image
    ofstream image("dtm.ppm", ios::binary);
    if (!image.is_open())
    {
        cout << "Error opening image" << endl;
        exit(1);
    }
    image << "P3" << endl;                           // PPM format
    image << img_width << " " << img_height << endl; // width and height of the image
    image << 255 << endl;

    // loop on all pixels
    for (int i = 0; i < img_width; i++)
    {
        for (int j = 0; j < img_height; j++)
        {
            vector<double> p = {i * px_size + x_y_z_min_max[0], j * px_size + x_y_z_min_max[2]}; // coordinates of the pixel in the projection
            interpolation(d, p, color);
            image << color[0] << " " << color[1] << " " << color[2] << "\n"; // write the color of the pixel in the image
        }
    }
    // for (size_t i = 0; i < d.triangles.size(); i += 3)
    // {
    //     interpolation(d, i, color);
    //     // cout << " " << color[0] << " " << color[1] << " " << color[2] << endl;
    //     image << color[0] << " " << color[1] << " " << color[2] << "\n";
    // }

    image.close();
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
    int img_width = stoi(argv[2]);

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

    // create Delaunator object
    delaunator::Delaunator d(points);

    // create image
    create_image(img_width, d);

    return 0;
}