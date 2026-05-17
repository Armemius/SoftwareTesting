#!/usr/bin/env python3
from __future__ import annotations

import csv
from pathlib import Path

import matplotlib.pyplot as plt


ROOT = Path(__file__).resolve().parents[1]
DATA_DIR = ROOT / "data"
PLOTS_DIR = ROOT / "plots"
LIMIT_MS = 670


def read_csv(name: str) -> list[dict[str, str]]:
    with (DATA_DIR / name).open(newline="", encoding="utf-8") as source:
        return list(csv.DictReader(source))


def save_load_throughput() -> None:
    rows = read_csv("load_results.csv")
    configs = [f"Config {row['config']}\n${row['cost_usd']}" for row in rows]
    throughput = [float(row["throughput_rpm"]) for row in rows]
    max_response = [float(row["max_response_ms"]) for row in rows]
    colors = ["#d95f02" if row["result"] == "fail" else "#1b9e77" for row in rows]

    fig, ax1 = plt.subplots(figsize=(9.5, 5.2), dpi=150)
    ax1.bar(configs, throughput, color=colors, alpha=0.86, label="Пропускная способность")
    ax1.set_ylabel("Пропускная способность, запросов/мин")
    ax1.set_ylim(0, 450)
    ax1.grid(axis="y", linestyle="--", linewidth=0.6, alpha=0.45)

    ax2 = ax1.twinx()
    ax2.plot(configs, max_response, color="#3b5ba5", marker="o", linewidth=2.2, label="Max response")
    ax2.axhline(LIMIT_MS, color="#b2182b", linestyle="--", linewidth=1.5, label="Порог 670 мс")
    ax2.set_ylabel("Максимальное время отклика, мс")
    ax2.set_ylim(0, 820)

    handles1, labels1 = ax1.get_legend_handles_labels()
    handles2, labels2 = ax2.get_legend_handles_labels()
    ax1.legend(handles1 + handles2, labels1 + labels2, loc="upper center", ncol=3, frameon=False)
    ax1.set_title("Нагрузочное тестирование: пропускная способность при 400 запросах/мин")
    fig.tight_layout()
    fig.savefig(PLOTS_DIR / "load_throughput.png")
    plt.close(fig)


def save_stress_response() -> None:
    rows = read_csv("stress_results.csv")
    load = [int(row["load_rpm"]) for row in rows]
    avg = [float(row["avg_response_ms"]) for row in rows]
    p95 = [float(row["p95_response_ms"]) for row in rows]
    max_response = [float(row["max_response_ms"]) for row in rows]

    fig, ax = plt.subplots(figsize=(9.5, 5.2), dpi=150)
    ax.plot(load, avg, marker="o", linewidth=2, color="#1b9e77", label="Average")
    ax.plot(load, p95, marker="s", linewidth=2, color="#7570b3", label="95% Line")
    ax.plot(load, max_response, marker="^", linewidth=2, color="#d95f02", label="Maximum")
    ax.axhline(LIMIT_MS, color="#b2182b", linestyle="--", linewidth=1.5, label="Порог 670 мс")
    ax.set_xlabel("Нагрузка, запросов/мин")
    ax.set_ylabel("Время отклика, мс")
    ax.set_title("Стресс-тестирование конфигурации 2")
    ax.grid(True, linestyle="--", linewidth=0.6, alpha=0.45)
    ax.legend(loc="upper left", frameon=False)
    fig.tight_layout()
    fig.savefig(PLOTS_DIR / "stress_response.png")
    plt.close(fig)


def main() -> None:
    PLOTS_DIR.mkdir(exist_ok=True)
    save_load_throughput()
    save_stress_response()


if __name__ == "__main__":
    main()
