#include <iostream>
#include <fstream>
#include <liblas/liblas.hpp>
#include <unordered_map>
#include <vector>
#include <cmath>

using namespace std;
using namespace liblas;

// ChatGPT code;

int main(int argc, char* argv[])
{
    // Check if the correct number of arguments are passed
    if (argc < 3) {
        cout << "Usage: lascombine input1.las input2.las ... output.las" << endl;
        return 1;
    }

    // Create a map to store the point groups
    unordered_map<string, vector<Point>> pointGroups;

    // Loop through all the input files
    for (int i = 1; i < argc - 1; i++) {
        // Open the input file
        ifstream ifs;
        ifs.open(argv[i], ios::in | ios::binary);
        if (!ifs) {
            cout << "Error opening input file: " << argv[i] << endl;
            continue;
        }

        // Create a LAS reader object
        LASReader reader(ifs);

        // Loop through all the points in the input file
        while (reader.ReadNextPoint()) {
            Point point = reader.GetPoint();

            // Divide the point into 100m x 100m grids based on the X and Y axes
            double x = point.GetX();
            double y = point.GetY();
            int xIndex = floor(x / 100);
            int yIndex = floor(y / 100);

            // Create a key for the point group
            string key = to_string(xIndex) + "," + to_string(yIndex);

            // Add the point to the point group
            pointGroups[key].push_back(point);
        }

        // Close the input file
        ifs.close();
    }

    // Open the output file
    ofstream ofs;
    ofs.open(argv[argc - 1], ios::out | ios::binary);
    if (!ofs) {
        cout << "Error opening output file." << endl;
        return 1;
    }

    // Create a LAS header object
    Header header;
    header.SetPointRecordsCount(0);
    header.SetScale(0.01, 0.01, 0.01);
    header.SetOffset(0, 0, 0);

    // Create a LAS writer object
    LASWriter writer(ofs, header);

    // Loop through all the point groups
    for (auto& [key, points] : pointGroups) {
        // Update the header to reflect the new point group
        header.SetPointRecordsCount(header.GetPointRecordsCount() + points.size());

        // Write the points to the output file
        for (Point& point : points) {
            writer.WritePoint(point);
        }
    }

    // Close the output file
    ofs.close();

    return 0;
}