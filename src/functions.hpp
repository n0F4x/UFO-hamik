float hdot(const vec3& p1, const vec3& p2) {
    return p1.x * p2.x + p1.y * p2.y - p1.z * p2.z;
}

float hlength(const vec3& v) {
    return std::sqrt(hdot(v, v));
}

vec3 hnormalize(const vec3& v) {
    return v * (1 / hlength(v));
}

namespace hyp {

// Task 1
vec3 perpendicularRight(const vec3& from, const vec3& direction) {
    return hnormalize(
        cross(vec3{ from.x, from.y, -1 * from.z },
              vec3{ direction.x, direction.y, -1 * direction.z }));
}

// Task 1
vec3 perpendicularLeft(const vec3& from, const vec3& direction) {
    return -1 * perpendicularRight(from, direction);
}

// Task 2
vec3 posBySpeedTime(const vec3& from, const vec3& speed, float time) {
    auto dir = hnormalize(speed);
    auto unitSpeed = hlength(speed);
    return from * std::cosh(time * unitSpeed)
         + dir * std::sinh(time * unitSpeed);
}

// Task 2
vec3 speedBySpeedTime(const vec3& from, const vec3& speed, float time) {
    auto dir = hnormalize(speed);
    auto unitSpeed = hlength(speed);
    return unitSpeed
         * (from * std::sinh(time * unitSpeed)
            + dir * std::cosh(time * unitSpeed));
}

// Task 3
float distance(const vec3& point1, const vec3& point2) {
    return std::acosh(-hdot(point1, point2));
}

// Task 3
vec3 direction(const vec3& from, const vec3& to) {
    return hnormalize(-1 * (from * std::cosh(distance(from, to)) - to)
                      / std::sinh(distance(from, to)));
}

// Task 4
vec3 posByDirDist(const vec3& from, const vec3& direction, float distance) {
    return posBySpeedTime(from, hnormalize(direction), distance);
}

// Task 5
/**
 * @param angle IN RADIANS!!!
 */
vec3 rotateBy(const vec3& point, const vec3& speed, float angle) {
    return speed * std::cos(angle)
         + perpendicularRight(point, speed) * std::sin(angle);
}

// Task 6
vec3 fit(const vec3& p) {
    return vec3{ p.x, p.y, std::sqrt(p.x * p.x + p.y * p.y + 1) };
}

// Task 6
vec3 fitSpeed(const vec3& point, const vec3& speed) {
    return speed + point * hdot(speed, point);
}

}   // namespace hyp
