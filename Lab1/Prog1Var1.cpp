#include <iostream>
#include <vector>

struct Point {
    double x;
    double y;
};

struct FilteredPoints {
    std::vector<Point> insidePoints;
    std::vector<Point> outsidePoints;
};

// Функция, определяющей, находится ли точка внутри области
bool isPointInside(const Point& p, double a) {
    //Точки, удовлетворяющие условиям (p.x + p.y < 5 * a / 2) && (p.x * p.y > a * a)
    return (p.x + p.y < 5 * a / 2.0) && (p.x * p.y > a * a);
}


FilteredPoints filterPoints(
    const std::vector<Point>& points,
    double a  // Параметр для области
) {
    FilteredPoints result;

    for (const auto& point : points) {
        if (isPointInside(point, a)) { 
            result.insidePoints.push_back(point);
        } else {
            result.outsidePoints.push_back(point);
        }
    }

    return result;
}

int main() {
    // Задаем вектор
    std::vector<Point> points = {{2.5, 1.7}, {5.0, 7.0}, {1.8, 2.3}, {3.0, 1.0}, {15.0, 8.0}};

    // Определяем значение параметра 'a'
    double a = 2.0;

    FilteredPoints filtered = filterPoints(points, a);

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



