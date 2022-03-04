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

// Return true iff [a,b] intersects [c,d], and store the intersection in ans
bool intersect_segment(const Point &a, const Point &b, const Point &c, const Point &d, Point &ans) {
    // TODO
    double a1 = b.imag() - a.imag();
    double b1 = a.real() - b.real();

    double a2 = d.imag() - c.imag();
    double b2 = c.real() - d.real();


    double det = a1 * b2 - a2 * b1;
    if (det == 0) {
        return false;
    } else {

        if ((b.imag() < c.imag() && a.imag() >= c.imag()) || (b.imag() >= c.imag() && a.imag() < c.imag())) {
            double x = (c.imag() - a.imag()) * (b.real() - a.real()) / (b.imag() - a.imag()) + a.real();
            ans = Point(x, c.imag());
            return x >= c.real();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

bool is_inside(const Polygon &poly, const Point &query) {
    // 1. Compute bounding box and set coordinate of a point outside the polygon
    // TODO
    Point outside(poly[0].real(), poly[0].imag());
    for (Point p: poly) {
        if (outside.real() > p.real()) {
            outside.real(p.real());
        }
        if (outside.imag() > p.imag()) {
            outside.imag(p.imag());
        }
    }
    if (outside.real() >= 0 && outside.imag() >= 0) {
        outside.real(0);
        outside.imag(0);
    }
    // 2. Cast a ray from the query point to the 'outside' point, count number of intersections
    // TODO
    int count = 0;
    for (int i = 1; i <= poly.size(); i++) {
        Point cur;
        int second = (i == poly.size()) ? 0 : i;
        if (query.imag() == poly[second].imag() && query.real() == poly[second].real()) {
            return true;
        } else if (intersect_segment(poly[i - 1], poly[second], query, outside, cur)) {
            if (cur.real() == query.real()) {
                return true;
            }
            count++;
        }
    }
    if (count % 2 == 0) {
        return false;
    } else {
        return true;
    }

}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> load_xyz(const std::string &filename) {
    std::vector<Point> points;
    std::ifstream in(filename);
    // TODO
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

Polygon load_obj(const std::string &filename) {
    Polygon obj;
    std::ifstream in(filename);
    // TODO

    if (!in.is_open()) {
        throw std::runtime_error("failed to open file " + filename);
    }
    while (true) {
        std::string a, b, c, d;
        in >> a >> b >> c >> d;
        if (!in || a == "f") break;
        Point p = Point(std::stod(b), std::stod(c));
        obj.push_back(p);
    }
    in.close();
    return obj;
}

void save_xyz(const std::string &filename, const std::vector<Point> &points) {
    // TODO
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("failed to open file " + filename);
    }
    out << points.size() << "\n";
    out << std::fixed;
    for (const auto &v: points) {
        out << v.real() << ' ' << v.imag() << " 0\n";
    }
    out << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
    if (argc <= 3) {
        std::cerr << "Usage: " << argv[0] << " points.xyz poly.obj result.xyz" << std::endl;
    }
    std::vector<Point> points = load_xyz(argv[1]);
    Polygon poly = load_obj(argv[2]);
    std::vector<Point> result;
    for (size_t i = 0; i < points.size(); ++i) {
        if (is_inside(poly, points[i])) {
            result.push_back(points[i]);
        }
    }
    save_xyz(argv[3], result);
    return 0;
}
