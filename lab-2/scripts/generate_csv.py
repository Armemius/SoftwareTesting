#!/usr/bin/env python3

from __future__ import annotations

import argparse
import subprocess
from pathlib import Path


DEFAULT_FUNCTIONS = ["sin", "cos", "tan", "ln", "log_3", "log_5", "complex"]


def run_exec(exec_path: Path, function: str, start: float, stop: float, step: float) -> str:
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

    return proc.stdout


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description="Generate CSV files for functions using ./build/app/exec")
    parser.add_argument("--exec-path", default="./build/app/exec", help="Path to prebuilt executable")
    parser.add_argument("--output-dir", default="csv", help="Directory for output CSV files")
    parser.add_argument("--start", type=float, default=-5.0, help="Range start")
    parser.add_argument("--stop", type=float, default=5.0, help="Range stop")
    parser.add_argument("--step", type=float, default=0.1, help="Sampling step")
    parser.add_argument(
        "--functions",
        nargs="*",
        default=DEFAULT_FUNCTIONS,
        help="Functions to export (default: all)",
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
        raw_output = run_exec(exec_path, function, args.start, args.stop, args.step)
        output_path = output_dir / f"{function}.csv"
        output_path.write_text(raw_output, encoding="utf-8")
        print(f"saved: {output_path}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
