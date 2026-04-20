#!/usr/bin/env python3

from __future__ import annotations

import argparse
import math
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt


DEFAULT_FUNCTIONS = ["sin", "cos", "tan", "ln", "log_3", "log_5", "complex"]


def run_exec(exec_path: Path, function: str, start: float, stop: float, step: float) -> tuple[list[float], list[float]]:
    cmd = [
        str(exec_path),
        "--function",
        function,
        "--start",
        str(start),
        "--stop",
        str(stop),
        "--step",
        str(step),
    ]

    proc = subprocess.run(cmd, check=False, text=True, capture_output=True)
    if proc.returncode != 0:
        raise RuntimeError(
            f"exec failed for function '{function}' with code {proc.returncode}\n"
            f"stderr:\n{proc.stderr}"
        )

    xs: list[float] = []
    ys: list[float] = []

    for line in proc.stdout.splitlines():
        line = line.strip()
        if not line or line.startswith("X,"):
            continue

        parts = [p.strip() for p in line.split(",")]
        if len(parts) != 2:
            continue

        try:
            x = float(parts[0])
            y = float(parts[1])
        except ValueError:
            continue

        xs.append(x)
        ys.append(y)

    return xs, ys


def filter_discontinuities(xs: list[float], ys: list[float]) -> tuple[list[float], list[float]]:
    if not xs:
        return xs, ys

    out_x: list[float] = []
    out_y: list[float] = []

    prev_x = None
    prev_y = None

    jump_threshold = 20.0
    mag_threshold = 5.0
    slope_threshold = 300.0

    for x, y in zip(xs, ys):
        if not math.isfinite(y):
            out_x.append(x)
            out_y.append(math.nan)
            prev_x = None
            prev_y = None
            continue

        if prev_x is None or prev_y is None:
            out_x.append(x)
            out_y.append(y)
            prev_x = x
            prev_y = y
            continue

        dx = x - prev_x
        dy = y - prev_y
        slope = abs(dy / dx) if dx != 0 else math.inf

        large_jump = abs(dy) > jump_threshold and abs(prev_y) > mag_threshold and abs(y) > mag_threshold
        sign_flip_high = prev_y * y < 0 and abs(prev_y) > mag_threshold and abs(y) > mag_threshold
        too_steep = slope > slope_threshold and (abs(prev_y) > mag_threshold or abs(y) > mag_threshold)

        if large_jump or sign_flip_high or too_steep:
            out_x.append((prev_x + x) / 2.0)
            out_y.append(math.nan)

        out_x.append(x)
        out_y.append(y)
        prev_x = x
        prev_y = y

    return out_x, out_y


def plot_function(function: str, xs: list[float], ys: list[float], output_dir: Path, start: float, stop: float) -> Path:
    filtered_x, filtered_y = filter_discontinuities(xs, ys)

    plt.figure(figsize=(10, 6))
    plt.plot(filtered_x, filtered_y, linewidth=1.6)
    plt.title(f"{function}(x)")
    plt.xlabel("x")
    plt.ylabel("y")
    plt.grid(True, alpha=0.3)
    plt.xlim(start, stop)
    plt.ylim(-5, 5)

    output_path = output_dir / f"{function}.png"
    plt.tight_layout()
    plt.savefig(output_path, dpi=180)
    plt.close()
    return output_path


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate function plots using ./build/app/exec output")
    parser.add_argument("--exec-path", default="./build/app/exec", help="Path to prebuilt executable")
    parser.add_argument("--output-dir", default="plots", help="Directory for output images")
    parser.add_argument("--start", type=float, default=-5.0, help="Range start")
    parser.add_argument("--stop", type=float, default=5.0, help="Range stop")
    parser.add_argument("--step", type=float, default=0.00001, help="Sampling step")
    parser.add_argument(
        "--functions",
        nargs="*",
        default=DEFAULT_FUNCTIONS,
        help="Functions to plot (default: all)",
    )
    return parser.parse_args()


def main() -> int:
    args = parse_args()

    exec_path = Path(args.exec_path)
    if not exec_path.exists():
        raise FileNotFoundError(f"Executable not found: {exec_path}")

    if args.step == 0:
        raise ValueError("--step must not be 0")

    output_dir = Path(args.output_dir)
    output_dir.mkdir(parents=True, exist_ok=True)

    for function in args.functions:
        xs, ys = run_exec(exec_path, function, args.start, args.stop, args.step)
        if not xs:
            raise RuntimeError(f"No data returned for function '{function}'")
        out = plot_function(function, xs, ys, output_dir, args.start, args.stop)
        print(f"saved: {out}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
