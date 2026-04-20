#pragma once

#include <cmath>
#include <cstddef>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace test::stub {

class CsvStubTable {
   public:
    CsvStubTable() = default;

    explicit CsvStubTable(const std::string &path) { load(path); }

    void load(const std::string &path) {
        rows_.clear();

        std::ifstream in(path);
        if (!in.is_open()) {
            throw std::runtime_error("Cannot open CSV stub file: " + path);
        }

        std::string line;
        while (std::getline(in, line)) {
            if (line.empty() || line.rfind("X,", 0) == 0) {
                continue;
            }

            const auto comma = line.find(',');
            if (comma == std::string::npos) {
                continue;
            }

            const std::string x_str = line.substr(0, comma);
            const std::string y_str = line.substr(comma + 1);
            rows_.emplace_back(std::stod(x_str), std::stod(y_str));
        }
    }

    double lookup(double x) const {
        constexpr double EPS = 1e-6;
        for (const auto &[kx, ky] : rows_) {
            if (std::isnan(kx) && std::isnan(x)) {
                return ky;
            }
            if (std::isinf(kx) && std::isinf(x) && std::signbit(kx) == std::signbit(x)) {
                return ky;
            }
            if (std::isfinite(kx) && std::isfinite(x) && std::abs(kx - x) <= EPS) {
                return ky;
            }
        }
        return std::numeric_limits<double>::quiet_NaN();
    }

   private:
    std::vector<std::pair<double, double>> rows_;
};

}  // namespace test::stub
