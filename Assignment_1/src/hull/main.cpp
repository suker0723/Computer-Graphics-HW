////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;

double inline det(const Point &u, const Point &v) {
    // TODO
    return sqrt((u.real() - v.real()) * (u.real() - v.real()) + (u.imag() - v.imag()) * (u.imag() - v.imag()));
}

struct Compare {
    Point p0; // Leftmost point of the poly
    bool operator()(const Point &p1, const Point &p2) {
        // TODO
        double angle =
                (p1.real() - p0.real()) * (p2.imag() - p0.imag()) - (p2.real() - p0.real()) * (p1.imag() - p0.imag());
        if (angle == 0) {
            return det(p0, p1) < det(p0, p2);
        } else {
            return angle > 0;
        }

    }
};

bool inline salientAngle(Point &a, Point &b, Point &c) {
    // TODO
    if (((b.real() - a.real()) * (c.imag() - a.imag()) - (c.real() - a.real()) * (b.imag() - a.imag())) >= 0) {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////

Polygon convex_hull(std::vector<Point> &points) {
    Compare order;
    // TODO

    order.p0 = Point(0, 0);
    int index = 0;
    for (int i = 0; i < points.size(); i++)//LEFT MOST AND DOWN MOST POINT
    {
        if (points[i].imag() < points[index].imag() ||
            (points[i].imag() == points[i].imag() && points[i].real() < points[i].real())) {
            index = i;
        }
    }
    std::swap(points[0], points[index]);
    order.p0 = points[0];
    std::sort(points.begin() + 1, points.end(), order);
    Polygon hull;
    // TODO
    // use salientAngle(a, b, c) here
    hull.push_back(order.p0);
    hull.push_back(points[1]);
    hull.push_back(points[2]);
    int count = 2;
    for (int i = 3; i < points.size(); ++i) {
        while (count > 0 && salientAngle(hull[count - 1], points[i], hull[count])) {
            --count;
            hull.pop_back();
        }
        hull.push_back(points[i]);
        ++count;
    }
    return hull;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> load_xyz(const std::string &filename) {
    std::vector<Point> points;
    std::ifstream in(filename);
    if (!in.is_open()) {
        throw std::runtime_error("failed to open file " + filename);
    }
    int count;
    in >> count;
    while (true) {
        std::string a, b, c;
        in >> a >> b >> c;
        if (!in) break;
        Point p = Point(std::stod(a), std::stod(b));
        points.push_back(p);

    }
    in.close();


    return points;
}

void save_obj(const std::string &filename, Polygon &poly) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("failed to open file " + filename);
    }
    out << std::fixed;
    for (const auto &v: poly) {
        out << "v " << v.real() << ' ' << v.imag() << " 0\n";
    }
    for (size_t i = 0; i < poly.size(); ++i) {
        out << "l " << i + 1 << ' ' << 1 + (i + 1) % poly.size() << "\n";
    }
    out << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    if (argc <= 2) {
        std::cerr << "Usage: " << argv[0] << " points.xyz output.obj" << std::endl;
    }
    std::vector<Point> points = load_xyz(argv[1]);
    Polygon hull = convex_hull(points);
    save_obj(argv[2], hull);
    return 0;
}
