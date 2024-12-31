# Trading Strategy Simulator

A **Trading Strategy Simulator** that enables backtesting various trading strategies, including Linear Regression, MACD, RSI, ADX, and Pair Trading, using stock market data from the National Stock Exchange (NSE).

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [How It Works](#how-it-works)
- [Requirements](#requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Configuration](#configuration)
- [File Descriptions](#file-descriptions)
- [Examples](#examples)
- [Future Work](#future-work)
- [Contributing](#contributing)

---

## Overview

The project is designed for quantitative traders and data scientists to test and optimize trading strategies. It combines **C++** for performance-heavy computations and **Python** for data retrieval and visualization.

---

## Features

- **Multiple Strategies:** Implement strategies like:
  - Basic Monotonic Trading
  - Moving Averages (DMA, DMA++)
  - Linear Regression
  - MACD, RSI, and ADX-based strategies
  - Pair Trading

- **Customizable Parameters:** Fine-tune thresholds and input variables to suit your strategy.

- **NSE Stock Data Integration:** Fetch real-time or historical data for Indian equities.

- **Backtesting and Visualization:** Generate backtesting metrics and plot results.

---

## How It Works

1. **Data Fetching:** Historical stock data is fetched using the NSE API.
2. **Simulation:** The core logic, written in C++, processes the data based on the selected strategy.
3. **Visualization:** Backtest results are visualized using Python.

---

## Requirements

- **Operating System:** Linux, macOS, or Windows
- **Tools:**
  - C++ Compiler (e.g., GCC)
  - Python (3.8+)
  - Pandas Library
  - `jugaad-data` for NSE stock data
  - Matplotlib for plotting

---

## Installation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/your-repo/trading-strategy-simulator.git
   cd trading-strategy-simulator
   ```

2. **Install Python Dependencies:**
   ```bash
   pip install -r requirements.txt
   ```

3. **Set up the C++ Compiler:**
   Ensure `g++` is installed and configured.

---

## Usage

### Running the Simulation
1. **Generate CSV Data:**
   ```bash
   make csv strategy=PAIRS symbol1=SBIN symbol2=ADANIENT start_date=01/01/2023 end_date=30/12/2024
   ```

2. **Run the Backtest:**
   ```bash
   make run strategy=PAIRS
   ```

3. **Visualize Results:**
   ```bash
   make plot
   ```

4. **Automated Workflow:**
   Run all steps in one command:
   ```bash
   make soup
   ```

---

## Configuration

Modify parameters directly in the `Makefile` or pass them as arguments:

- `strategy`: Trading strategy (e.g., PAIRS, MACD, RSI)
- `symbol`: Primary stock symbol
- `n`: Lookback period for strategies
- `p`: Threshold percentage
- `oversold_threshold`: RSI oversold value
- `overbought_threshold`: RSI overbought value

For a complete list of parameters, see the `Makefile`.

---

## File Descriptions

- **main.cpp:** Core simulation logic.
- **generate_csv.py:** Fetch stock data from NSE.
- **plot.py:** Generate visualization of backtest results.
- **Makefile:** Automates the workflow (data generation, simulation, plotting).

---

## Examples

### Pair Trading
```bash
make soup strategy=PAIRS symbol1=SBIN symbol2=ADANIENT start_date=01/01/2023 end_date=30/12/2024
```

### RSI Strategy
```bash
make soup strategy=RSI symbol=SBIN oversold_threshold=30 overbought_threshold=70
```

### Linear Regression
```bash
make soup strategy=LINEAR_REGRESSION symbol=SBIN train_start_date=01/05/2023 train_end_date=01/09/2024
```

---

## Future Work

- Add support for international stock exchanges.
- Integrate with other APIs like Alpha Vantage or Yahoo Finance.
- Implement real-time trading support.
- Expand visualization options with interactive dashboards.

---

## Contributing

Contributions are welcome! Please open issues or submit pull requests to improve the project.

---

## License

--