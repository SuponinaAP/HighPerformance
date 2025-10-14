#include <iostream>
#include <vector>
#include <cmath>  // Для sqrt

struct Point {
    double x;
    double y;
};

struct FilteredPoints {
    std::vector<Point> insidePoints;
    std::vector<Point> outsidePoints;
};

bool isPointInside(const Point& p, double q, double p_param) {
    //  y^2 <= 2px + p^2 (парабола, открытая вправо)
    //  y^2 >= -2qx + q^2 (парабола, открытая влево)
    return (p.y * p.y <= 2 * p_param * p.x + p_param * p_param) &&
           (p.y * p.y >= -2 * q * p.x + q * q);
}

FilteredPoints filterPoints(
    const std::vector<Point>& points,  
    double q, double p_param 
) {
    FilteredPoints result;

    for (const auto& point : points) {
        if (isPointInside(point, q, p_param)) { 
            result.insidePoints.push_back(point);
        } else {
            result.outsidePoints.push_back(point);
        }
    }

    return result;
}

int main() {
    std::vector<Point> points = {{0.5, 0.5},{0.8, 1.0},{0.8, 0.8},{1.2, 0.0},{0.0, 1.0}};

    // Определяем значения параметров 'q' и 'p_param'
    double q = 2.0; 
    double p_param = 1.0; 

    FilteredPoints filtered = filterPoints(points, q, p_param);

    // Вывод точек, находящихся внутри области
    std::cout << "Точки внутри области:\n";
    for (const auto& p : filtered.insidePoints) {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }

    // Вывод точек, находящихся вне области
    std::cout << "\nТочки вне области:\n";
    for (const auto& p : filtered.outsidePoints) {
        std::cout << "(" << p.x << ", " << p.y << ")\n";
    }

    return 0;
}

