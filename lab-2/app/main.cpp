#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>

#include <maths/complex.hpp>
#include <maths/cos.hpp>
#include <maths/ln.hpp>
#include <maths/log.hpp>
#include <maths/sin.hpp>
#include <maths/tan.hpp>

namespace {

using Fn = double (*)(double, size_t);

constexpr auto SIN_FN = static_cast<Fn>(arms::maths::sin);
constexpr auto LN_FN = static_cast<Fn>(arms::maths::ln);

constexpr double cos_from_sin(double x, size_t terms) {
    return arms::maths::cos<double, SIN_FN>(x, terms);
}

constexpr double tan_from_sin_cos(double x, size_t terms) {
    return arms::maths::tan<double, SIN_FN, cos_from_sin>(x, terms);
}

constexpr double log3_from_ln(double x, size_t terms) {
    return arms::maths::log_3<double, LN_FN>(x, terms);
}

constexpr double log5_from_ln(double x, size_t terms) {
    return arms::maths::log_5<double, LN_FN>(x, terms);
}

constexpr double complex_from_base(double x, size_t terms) {
    return arms::maths::complex<double, tan_from_sin_cos, log5_from_ln, LN_FN, log3_from_ln>(x, terms);
}

const std::unordered_map<std::string, Fn> FUNCTION_TABLE = {
    {"sin", SIN_FN},         {"cos", cos_from_sin},   {"tan", tan_from_sin_cos},      {"ln", LN_FN},
    {"log_3", log3_from_ln}, {"log_5", log5_from_ln}, {"complex", complex_from_base},
};

struct Options {
    bool csv = false;
    bool interactive = false;
    std::string function_name;
    std::optional<double> start;
    std::optional<double> stop;
    std::optional<double> step;
};

void print_usage() {
    std::cerr << "Usage:\n"
              << "  exec --function <name> --start <x0> --stop <x1> --step <dx> [--csv]\n"
              << "  exec --function <name> --interactive [--csv]\n"
              << "Functions: sin, cos, tan, ln, log_3, log_5, complex\n";
}

bool parse_double(const std::string &value, double &out) {
    char *end = nullptr;
    out = std::strtod(value.c_str(), &end);
    return end != value.c_str() && *end == '\0';
}

bool parse_args(int argc, char **argv, Options &options) {
    for (int i = 1; i < argc; ++i) {
        const std::string arg = argv[i];

        if (arg == "--csv") {
            options.csv = true;
        } else if (arg == "--interactive") {
            options.interactive = true;
        } else if (arg == "--function") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --function\n";
                return false;
            }
            options.function_name = argv[++i];
        } else if (arg == "--start") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --start\n";
                return false;
            }
            double parsed = 0.0;
            if (!parse_double(argv[++i], parsed)) {
                std::cerr << "Invalid numeric value for --start\n";
                return false;
            }
            options.start = parsed;
        } else if (arg == "--stop") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --stop\n";
                return false;
            }
            double parsed = 0.0;
            if (!parse_double(argv[++i], parsed)) {
                std::cerr << "Invalid numeric value for --stop\n";
                return false;
            }
            options.stop = parsed;
        } else if (arg == "--step") {
            if (i + 1 >= argc) {
                std::cerr << "Missing value for --step\n";
                return false;
            }
            double parsed = 0.0;
            if (!parse_double(argv[++i], parsed)) {
                std::cerr << "Invalid numeric value for --step\n";
                return false;
            }
            options.step = parsed;
        } else {
            std::cerr << "Unknown argument: " << arg << '\n';
            return false;
        }
    }

    if (options.function_name.empty()) {
        std::cerr << "--function is required\n";
        return false;
    }

    if (!FUNCTION_TABLE.contains(options.function_name)) {
        std::cerr << "Unknown function: " << options.function_name << '\n';
        return false;
    }

    if (options.interactive) {
        if (options.start.has_value() || options.stop.has_value() || options.step.has_value()) {
            std::cerr << "--interactive cannot be combined with --start/--stop/--step\n";
            return false;
        }
        return true;
    }

    if (!options.start.has_value() || !options.stop.has_value() || !options.step.has_value()) {
        std::cerr << "Range mode requires --start, --stop and --step\n";
        return false;
    }

    if (*options.step == 0.0) {
        std::cerr << "--step must not be 0\n";
        return false;
    }

    if (*options.start < *options.stop && *options.step < 0.0) {
        std::cerr << "--step must be positive when start < stop\n";
        return false;
    }
    if (*options.start > *options.stop && *options.step > 0.0) {
        std::cerr << "--step must be negative when start > stop\n";
        return false;
    }

    return true;
}

void write_header(std::ostream &output) {
    output << "X,Result(X)\n";
}

void write_row(std::ostream &output, double x, double y) {
    output << std::setprecision(15) << x << ',' << y << '\n';
}

int run_interactive(std::ostream &output, Fn fn) {
    double x = 0.0;
    while (std::cin >> x) {
        write_row(output, x, fn(x, arms::maths::defaults::DEFAULT_TERMS<double>));
    }
    return 0;
}

int run_range(std::ostream &output, Fn fn, double start, double stop, double step) {
    const double eps = std::max(1e-12, std::abs(step) * 1e-9);
    const auto within = [step, stop, eps](double x) {
        if (step > 0.0) {
            return x <= stop + eps;
        }
        return x >= stop - eps;
    };

    size_t guard = 0;
    for (double x = start; within(x); x += step) {
        write_row(output, x, fn(x, arms::maths::defaults::DEFAULT_TERMS<double>));
        if (++guard > 10'000'000) {
            std::cerr << "Too many points generated, check start/stop/step.\n";
            return 1;
        }
    }

    return 0;
}

}  // namespace

int main(int argc, char **argv) {
    Options options;
    if (!parse_args(argc, argv, options)) {
        print_usage();
        return 1;
    }

    const Fn fn = FUNCTION_TABLE.at(options.function_name);

    if (options.csv) {
        const std::string filename = options.function_name + ".csv";
        std::ofstream output(filename);
        if (!output.is_open()) {
            std::cerr << "Failed to open output file: " << filename << '\n';
            return 1;
        }

        write_header(output);

        const int code = options.interactive ? run_interactive(output, fn)
                                             : run_range(output, fn, *options.start, *options.stop, *options.step);

        if (code == 0) {
            std::cout << "Saved results to " << filename << '\n';
        }
        return code;
    }

    write_header(std::cout);
    return options.interactive ? run_interactive(std::cout, fn)
                               : run_range(std::cout, fn, *options.start, *options.stop, *options.step);
}
