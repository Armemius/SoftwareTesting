# Лабораторная работа 4

Артефакты:

- `report/report.tex` -- исходник отчёта.
- `report/report.pdf` -- собранный отчёт.
- `tests/load-test.jmx` -- JMeter-план нагрузочного тестирования.
- `tests/stress-test.jmx` -- JMeter-план стресс-тестирования выбранной конфигурации.
- `data/*.csv` -- сводные результаты запусков.
- `plots/*.png` -- графики для отчёта.

Пример запуска нагрузочного теста для второй конфигурации:

```bash
jmeter -n -t tests/load-test.jmx -Jhost=localhost -Jport=8080 -Jconfig=2 -Jthreads=10 -Jthroughput=400 -Jduration=300 -l results/load-config-2.jtl
```

Пример запуска стресс-теста на 14 пользователей:

```bash
jmeter -n -t tests/stress-test.jmx -Jhost=localhost -Jport=8080 -Jthreads=14 -Jthroughput=560 -Jduration=300 -l results/stress-14-users.jtl
```
