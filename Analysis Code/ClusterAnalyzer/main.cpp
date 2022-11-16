#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

// o,n,c,s
// inputs + outputs
std::vector<std::vector<double>> O;
std::vector<std::vector<double>> N;
std::vector<std::vector<double>> C;
std::vector<std::vector<double>> S;

double averageDist; //declared here so averageDistanceoverPairs can access
std::vector<int> vec; //declared here so main can access

double minDistance(double x1, double y1, double z1, double x2, double y2, double z2) //
{
    //YOU HAVE TO CHANGE THE BOX DIMENSIONS. YOU CAN FIND THEM IN LOG.LAMMPS IN THE RESTART AT THE TOP. PLEASE CHANGE. THIS ASSUMES BOX X1 = BOX Y1 AND SUCH.
    double box1 = 0.823567;
    double box2 = 32.9664;
    double boxLength = box2 - box1;
    double minX = std::min(abs(x2 - x1), std::min(abs(x2 - x1 + boxLength), std::abs(x2 - x1 - boxLength)));
    double minY = std::min(abs(y2 - y1), std::min(abs(y2 - y1 + boxLength), std::abs(y2 - y1 - boxLength)));
    double minZ = std::min(abs(z2 - z1), std::min(abs(z2 - z1 + boxLength), std::abs(z2 - z1 - boxLength)));
    
    return sqrt(minX * minX + minY * minY + minZ * minZ);
}

double averageDistance(std::vector<std::vector<double>> cluster)
{
    double sum = 0;
    double size = cluster.size();
    for (int i = 0; i < size; i++)
    {
        for (int j = i; j < size; j++)
        {
            sum += minDistance(cluster[i][0], cluster[i][1], cluster[i][2], cluster[j][1], cluster[j][2], cluster[j][0]);
        }
    }

    return sum / ((size * size - size) / 2); //divide by size choose 2;
}

double calculatePrimacy() {
    vec = {O.size(), N.size(), C.size(), S.size()};
    sort(vec.begin(), vec.end());
    return (double) vec[vec.size() - 1] / vec[vec.size() - 2];
}

double averageDistanceOverPairs(std::vector<std::vector<double>> cluster)
{
    double size = cluster.size();
    double averageDist = averageDistance(cluster);
    return ((size * size - size) / 2);

}

int main()
{
 
    std::ifstream inf{"input.txt"};

    if (!inf)
    {
        std::cerr << "you did dum dum input";
    }

    while (inf)
    {
        char cluster;
        inf >> cluster;

        std::vector<double> position;
        double x,y,z;
        inf >> x;
        inf >> y;
        inf >> z;

        position.push_back(x);
        position.push_back(y);
        position.push_back(z);

        if (cluster == 'O') {
        O.push_back(position);
        }
        else if (cluster == 'N') {
        N.push_back(position);
        }
        else if (cluster == 'C') {
        C.push_back(position);
        }
        else if (cluster == 'S') {
        S.push_back(position);
        }
    }

    inf.close();

    ofstream outFile ("ClusterAnalyzer_25f.txt"); //CHANGE THIS

    outFile << "Primacy " << calculatePrimacy() << '\n';

    outFile << "Cluster MoleculeNum AverageDist AverageDistOverPairs" << '\n';

    outFile << "O " << O.size() << ' ' << averageDistance(O) << " " << averageDistanceOverPairs(O) << '\n';
    outFile << "N " << N.size() << ' ' << averageDistance(N) << " " << averageDistanceOverPairs(N) << '\n';
    outFile << "C " << C.size() << ' ' << averageDistance(C) << ' ' << averageDistanceOverPairs(C) << '\n';
    outFile << "S " << S.size() << ' ' << averageDistance(S) << ' ' << averageDistanceOverPairs(S) << '\n';

}

