// let the machine do the work
#include<iostream>

#include<fstream>

#include<sstream>

#include<vector>

#include<math.h>

#include<queue>

using namespace std;

int n {
  4
}, x {
  3
}, p {
  2
}, max_hold_days {
  2147483640
}, oversold_threshold {}, overbought_threshold {}, adx_threshold {}, iterator_for_day_one {}, threshold;
double c1 {
  2
}, c2 {
  0.2
};
string strategy {
  "PAIRS"
}, start_date {
  "2023-05-01"
}, end_date {
  "2023-05-11"
}, train_start_date {
  "2023-01-10"
}, train_end_date {
  "2023-01-15"
}, symbol {}, symbol1 {}, symbol2 {};

bool findInvMatGaussJordan(vector < vector < double >> & mat, int order) {
  double temp;
  for (int i = 0; i < order; i++) {
    for (int j = 0; j < 2 * order; j++) {
      if (j == (i + order))
        mat[i][j] = 1;
    }
  }
  for (int i = order - 1; i > 0; i--) {
    if (mat[i - 1][0] < mat[i][0]) {
      vector < double > temp = mat[i];
      mat[i] = mat[i - 1];
      mat[i - 1] = temp;
    }
  }
  for (int i = 0; i < order; i++) {
    for (int j = 0; j < order; j++) {
      if (j != i) {
        temp = mat[j][i] / mat[i][i];
        for (int k = 0; k < 2 * order; k++) {
          mat[j][k] -= mat[i][k] * temp;
        }
      }
    }
  }
  for (int i = 0; i < order; i++) {
    temp = mat[i][i];
    for (int j = 0; j < 2 * order; j++) {
      mat[i][j] = mat[i][j] / temp;
    }
  }
  double determinant = 1;
  for (int i = 0; i < order; i++) {
    determinant *= mat[i][i];
  }
  if (std::abs(determinant) < 1e-6) return false;

  return true;
}
// Function to calculate dot product
double dot_product(const std::vector < double > & v1,
  const std::vector < double > & v2) {
  double product = 0;
  for (int i = 0; i < v1.size(); i++)
    product += v1[i] * v2[i];
  return product;
}
// Function to perform Linear Regression using Normal Equation
vector < vector < double >> transpose(const vector < vector < double >> & matrix) {
  vector < vector < double >> output(int(matrix[0].size()), vector < double > (int(matrix.size())));
  for (int i = 0; i < int(matrix[0].size()); i++) {
    for (int j = 0; j < int(matrix.size()); j++) {
      output[i][j] = matrix[j][i];
    }
  }
  return output;
}

vector < vector < double >> multiply(const vector < vector < double >> & m1,
  const vector < vector < double >> & m2, vector < vector < double >> & answer) {
  for (int i = 0; i < m1.size(); i++) {
    for (int j = 0; j < m2[0].size(); j++) {
      double temp {};
      for (int k = 0; k < m1[0].size(); k++) {
        temp += m1[i][k] * m2[k][j];
      }
      answer[i][j] = temp;
    }
  }
  return answer;
}

vector < double > NormalEquation(std::vector < std::vector < double >> & X, std::vector < double > & y) {
  // Solve the system of linear equations XTX * beta = XTy for beta
  // ...
  // x is nx8 and y is nx1
  // 8xn nx1
  //         8x8 8x1

  // return beta;
  auto X_transpose = transpose(X);
  vector < vector < double >> inverse(8, vector < double > (16, 0));
  auto XTX = multiply(X_transpose, X, inverse);
  bool result = findInvMatGaussJordan(inverse, 8);
  if (!result) return vector < double > (1, -1);
  vector < vector < double >> _y(y.size(), vector < double > (1));
  vector < vector < double >> XTy(8, vector < double > (1));
  for (int i = 0; i < y.size(); i++) _y[i][0] = y[i];
  multiply(X_transpose, _y, XTy);
  vector < double > answer(8);
  for (int i = 0; i < 8; i++) {
    double temp {};
    for (int j = 0; j < 8; j++) temp += inverse[i][j + 8] * XTy[j][0];
    answer[i] = temp;
  }
  return answer;
}

string corrected_date(string date) {
  return date.substr(8, 2) + '/' + date.substr(5, 2) + '/' + date.substr(0, 4);
}

double spread_for_the_day(const int & current_index,
  const vector < pair < double, double >> & stock_data_for_pair) {
  return stock_data_for_pair[current_index].first - stock_data_for_pair[current_index].second;
}

int condition_for_pair(const int & current_index,
  const vector < pair < double, double >> & stock_data_for_pair) {
  double mean {}, std {};
  for (int i = 0; i < n; i++) {
    mean += spread_for_the_day(current_index + i, stock_data_for_pair);
  }
  mean /= n;
  for (int i = 0; i < n; i++) std += pow((spread_for_the_day(current_index + i, stock_data_for_pair) - mean), 2);
  std = sqrt(std / n);
  double z_score {
    (spread_for_the_day(current_index, stock_data_for_pair) - mean) / std
  };
  if (z_score > threshold) return 1;
  else if (z_score < -threshold) return -1;
  else return 0;
}

int monotonic_basic(const int & current_index,
  const vector < pair < string, double >> & stock_data) {
  // condition check for increasing
  bool correct {
    true
  };
  for (int i = current_index; i < current_index + n; i++) {
    if (stock_data[i].second <= stock_data[i + 1].second) {
      correct = false;
      break;
    }
  }
  if (correct) return 1;
  correct = true;
  // condition check for decreasing
  for (int i = current_index; i < current_index + n; i++) {
    if (stock_data[i].second >= stock_data[i + 1].second) {
      correct = false;
      break;
    }
  }
  if (correct) return -1;
  // else neither increasing or decreasing
  return 0;
}

double MACD(const int & current_index,
  const vector < pair < string, double >> & stock_data) {
  double short_EWM {}, long_EWM {}, MACDi {};
  const int days_short {
    12
  }, days_long {
    26
  }, days_signal {
    9
  };
  const double alpha_short {
    2.0 / (days_short + 1)
  }, alpha_long {
    2.0 / (days_long + 1)
  }, alpha_signal {
    2.0 / (days_signal + 1)
  };

  short_EWM = stock_data[iterator_for_day_one].second;
  for (int i = iterator_for_day_one; i > current_index; i--) {
    short_EWM = alpha_short * (stock_data[i - 1].second - short_EWM) + short_EWM;
  }
  long_EWM = stock_data[iterator_for_day_one].second;
  for (int i = iterator_for_day_one; i > current_index; i--) {
    long_EWM = alpha_long * (stock_data[i - 1].second - long_EWM) + long_EWM;
  }
  MACDi = short_EWM - long_EWM;
  return MACDi;
}

int SIGNAL(const int & current_index,
  const vector < pair < string, double >> & stock_data) {
  const double alpha {
    2.0 / 10
  };
  double EWM {
    MACD(iterator_for_day_one, stock_data)
  }, EWM_today {};
  EWM_today = MACD(current_index, stock_data);
  for (int i = iterator_for_day_one; i > current_index; i--) {
    EWM = alpha * (MACD(i - 1, stock_data) - EWM) + EWM;
  }
  if (EWM_today > EWM) return 1;
  else if (EWM > EWM_today) return -1;
  else return 0;
}

int standard_deviation(const int & current_index,
  const vector < pair < string, double >> & stock_data) {
  double mean {}, psd {}, current_price {
    stock_data[current_index].second
  };
  // calculating the mean of the previous n days;
  for (int i = current_index; i <= current_index + n; i++) mean += stock_data[i].second;
  mean /= (n + 1);
  //calculating the standard deviation
  // psd is p*sd
  for (int i = current_index; i <= current_index + n; i++) psd += pow(((stock_data[i].second) - mean), 2);
  psd = sqrt(psd / (n + 1));
  psd *= p;

  if (current_price - mean >= psd) return 1;
  else if (mean - current_price >= psd) return -1;
  else return 0;
}

int better_DMA(const int & current_index,
  const vector < pair < string, double >> & stock_data) {
  double ER {
    stock_data[current_index].second - stock_data[current_index + n].second
  }, temp_denominator {};
  for (int i = current_index; i < current_index + n; i++) temp_denominator += fabs(stock_data[i].second - stock_data[i + 1].second);
  if (temp_denominator != 0) ER /= temp_denominator;
  else return 0; // Skip if denominator is 0
  double AMA_prev {
    stock_data[current_index].second
  }, AMA_current {}, SF_prev {
    0.5
  }, SF_current {};
  for (int i = current_index; i < current_index + n; i++) {
    double temp_SF_current {
      SF_current
    }, temp_AMA_current {
      AMA_current
    };
    SF_current = SF_prev + c1 * ((((2 * ER) / (1 + c2)) - 1) / (((2 * ER) / (1 + c2)) + 1) - SF_prev);
    SF_prev = temp_SF_current;
    AMA_current = AMA_prev + SF_current * (stock_data[i].second - AMA_prev);
    AMA_prev = temp_AMA_current;
  }
  double percentage {
    100 * (stock_data[current_index].second / AMA_current) - 1
  };
  if (percentage >= p) return 1;
  else if (percentage <= -p) return -1;
  else return 0;
}

int RSI(const int & current_index,
  const vector < pair < string, double >> & stock_data) {
  double avg_gain {}, avg_loss {}, rs, rsi;

  for (int i = 0; i < n; ++i) {
    double price_diff = stock_data[current_index + i].second - stock_data[current_index + i + 1].second;
    if (price_diff >= 0) avg_gain += price_diff;
    else avg_loss -= price_diff;
  }
  avg_gain /= n;
  avg_loss /= n;

  rs = avg_gain / avg_loss;
  rsi = 100 - (100 / (1 + rs));

  if (rsi < oversold_threshold) return 1; // Buy signal
  else if (rsi > overbought_threshold) return -1; // Sell signal
  else return 0; // No action
}

double true_range(const int & current_index,
  const vector < pair < string, double >> & stock_data, vector < pair < double, double >> stock_data_for_ADX) {
  return max(fabs(stock_data_for_ADX[current_index].second - stock_data_for_ADX[current_index].first),
    max(fabs(stock_data_for_ADX[current_index].second - stock_data[current_index + 1].second),
      fabs(stock_data_for_ADX[current_index].first - stock_data[current_index + 1].second)));
}

double ATR_formula(const int & current_index,
  const vector < pair < string, double >> & stock_data, vector < pair < double, double >> stock_data_for_ADX) {
  double ATR_current {
    true_range(current_index, stock_data, stock_data_for_ADX)
  };
  for (int i = iterator_for_day_one; i > current_index; i--) {
    ATR_current = (2.0 / (1 + n)) * (true_range(i - 1, stock_data, stock_data_for_ADX) - ATR_current) + ATR_current;
  }
  return ATR_current;
}

double DI_plus_formula(const int & current_index,
  const vector < pair < string, double >> & stock_data, vector < pair < double, double >> stock_data_for_ADX) {
  double DI_current {
    max(0.0, stock_data_for_ADX[current_index].second - stock_data_for_ADX[current_index + 1].second) / ATR_formula(current_index, stock_data, stock_data_for_ADX)
  };
  for (int i = iterator_for_day_one; i > current_index; i--) {
    DI_current = (2.0 / (1 + n)) * (max(0.0, stock_data_for_ADX[i - 1].second - stock_data_for_ADX[i].second) / ATR_formula(i - 1, stock_data, stock_data_for_ADX) - DI_current) + DI_current;
  }
  return DI_current;
}

double DI_minus_formula(const int & current_index,
  const vector < pair < string, double >> & stock_data, vector < pair < double, double >> stock_data_for_ADX) {
  double DI_current {
    max(0.0, stock_data_for_ADX[current_index].first - stock_data_for_ADX[current_index + 1].first) / ATR_formula(current_index, stock_data, stock_data_for_ADX)
  };
  for (int i = iterator_for_day_one; i > current_index; i--) {
    DI_current = (2.0 / (1 + n)) * (max(0.0, stock_data_for_ADX[i - 1].first - stock_data_for_ADX[i].first) / ATR_formula(i - 1, stock_data, stock_data_for_ADX) - DI_current) + DI_current;
  }
  return DI_current;
}

double ADX_formula(const int & current_index,
  const vector < pair < string, double >> & stock_data, vector < pair < double, double >> stock_data_for_ADX) {
  double DI_plus, DI_minus, DX;
  DI_plus = DI_plus_formula(current_index, stock_data, stock_data_for_ADX);
  DI_minus = DI_minus_formula(current_index, stock_data, stock_data_for_ADX);
  // Calculate Directional Index (DX)
  DX = (100.0 * fabs(DI_plus - DI_minus) / (DI_plus + DI_minus));
  return DX;
}

int ADX(const int & current_index,
  const vector < pair < string, double >> & stock_data, vector < pair < double, double >> stock_data_for_ADX) {
  double EWM_ADX {
    ADX_formula(current_index, stock_data, stock_data_for_ADX)
  };
  for (int i = iterator_for_day_one; i > current_index; i--) {
    EWM_ADX = (2.0 / (1 + n)) * (ADX_formula(i - 1, stock_data, stock_data_for_ADX) - EWM_ADX) + EWM_ADX;
  }
  if (EWM_ADX > adx_threshold) return 1;
  else if (EWM_ADX < adx_threshold) return -1;
  else return 0;
}

double expected_closing_price_by_linear_regression(const int & current_index,
  const vector < pair < string, double >> & stock_data,
    const vector < double > & parameters_for_linear_regression,
      const vector < vector < double >> & stock_data_for_ML) {
  double return_price {};
  return_price += parameters_for_linear_regression[0] * 1;
  return_price += parameters_for_linear_regression[1] * stock_data[current_index + 1].second;
  return_price += parameters_for_linear_regression[2] * stock_data_for_ML[current_index + 1][0];
  return_price += parameters_for_linear_regression[3] * stock_data_for_ML[current_index + 1][1];
  return_price += parameters_for_linear_regression[4] * stock_data_for_ML[current_index + 1][2];
  return_price += parameters_for_linear_regression[5] * stock_data_for_ML[current_index + 1][3];
  return_price += parameters_for_linear_regression[6] * stock_data_for_ML[current_index + 1][4];
  return_price += parameters_for_linear_regression[7] * stock_data_for_ML[current_index][0];
  return return_price;
}

int Linear_Regression(const int & current_index,
  const vector < pair < string, double >> & stock_data,
    const vector < double > & parameters_for_linear_regression,
      const vector < vector < double >> & stock_data_for_ML) {
  double expected_price = expected_closing_price_by_linear_regression(current_index, stock_data, parameters_for_linear_regression, stock_data_for_ML);
  int p_ = (((100.0 * expected_price) / stock_data[current_index].second) - 100.0);
  if (p_ >= p) return 1;
  else if (p_ <= -p) return -1;
  else return 0;
}

int condition(const int & current_index,
  const vector < pair < string, double >> & stock_data, vector < pair < double, double >> stock_data_for_ADX,
    const vector < double > & parameters_for_linear_regression,
      const vector < vector < double >> & stock_data_for_ML) {
  if (strategy == "BASIC") return monotonic_basic(current_index, stock_data);
  else if (strategy == "DMA") return standard_deviation(current_index, stock_data);
  else if (strategy == "DMA++") return better_DMA(current_index, stock_data);
  else if (strategy == "MACD") return SIGNAL(current_index, stock_data);
  else if (strategy == "RSI") return RSI(current_index, stock_data);
  else if (strategy == "LINEAR_REGRESSION") return Linear_Regression(current_index, stock_data, parameters_for_linear_regression, stock_data_for_ML);
  else if (strategy == "ADX") return ADX(current_index, stock_data, stock_data_for_ADX);

  else throw runtime_error("invalid option selected for strategy");
}

int main(int argc, char * argv[]) {
  strategy = argv[1];
  symbol = argv[2];
  n = stoi(argv[3]);
  x = stoi(argv[4]);
  p = stoi(argv[5]);
  max_hold_days = stoi(argv[6]);
  c1 = stod(argv[7]);
  c2 = stod(argv[8]);
  oversold_threshold = stoi(argv[9]);
  overbought_threshold = stoi(argv[10]);
  adx_threshold = stoi(argv[11]);
  threshold = stoi(argv[12]);
  symbol1 = argv[13];
  symbol2 = argv[14];

  string train_start_date_t {
    argv[15]
  }, train_end_date_t {
    argv[16]
  }, start_date_t {
    argv[17]
  }, end_date_t {
    argv[18]
  };
  // convert the date to the format in the csv file
  start_date = start_date_t.substr(6, 4) + '-' + start_date_t.substr(3, 2) + '-' + start_date_t.substr(0, 2);
  end_date = end_date_t.substr(6, 4) + '-' + end_date_t.substr(3, 2) + '-' + end_date_t.substr(0, 2);
  train_start_date = train_start_date_t.substr(6, 4) + '-' + train_start_date_t.substr(3, 2) + '-' + train_start_date_t.substr(0, 2);
  train_end_date = train_end_date_t.substr(6, 4) + '-' + train_end_date_t.substr(3, 2) + '-' + train_end_date_t.substr(0, 2);
  if (strategy == "PAIRS") {
    string csv_line1 {}, csv_line2 {};
    fstream fin;
    fin.open(symbol1 + ".csv", ios::in);
    // fin1.open("SBIN.csv");
    // fin2.open("ADANIENT.csv");
    vector < string > date_for_pair {};
    vector < pair < double, double >> stock_data_for_pair {};
    fin >> csv_line1;
    int it {
      -1
    };
    while (fin >> csv_line1) {
      string date {}, close1 {};
      stringstream s1 {
        csv_line1
      };
      getline(s1, date, ',');
      getline(s1, close1, ',');
      date_for_pair.push_back(date);
      stock_data_for_pair.push_back({
        stod(close1),
        1.0
      });
      if (date >= start_date) it++;
    }
    fin.close();
    fin.open(symbol2 + ".csv", ios::in);
    fin >> csv_line2;
    int index_for_stock_pair {};
    while (fin >> csv_line2) {
      string close2 {};
      stringstream s2 {
        csv_line2
      };
      getline(s2, close2, ',');
      getline(s2, close2, ',');
      stock_data_for_pair[index_for_stock_pair++].second = stod(close2);
    }
    fin.close();

    fstream daily_cashflow, order_statistics1, order_statistics2;

    daily_cashflow.open("daily_cashflow.csv", ios::out);
    daily_cashflow << "Date,Cashflow\n";
    order_statistics1.open("order_statistics_1.csv", ios::out);
    order_statistics1 << "Date,Order_dir,Quantity,Price\n";
    order_statistics2.open("order_statistics_2.csv", ios::out);
    order_statistics2 << "Date,Order_dir,Quantity,Price\n";
    int no_of_stocks1 {}, no_of_stocks2 {};
    double cash_flow {};
    for (int i = it; i >= 0; i--) {
      string t_date {
        corrected_date(date_for_pair[i])
      };
      double t_price1 {
        stock_data_for_pair[i].first
      }, t_price2 {
        stock_data_for_pair[i].second
      };
      //action will describe what are we going to do
      // if it is 1, then we are gonna buy some stonk, if it is -1, then we shall sell some stonks, else we do nothing
      int action {
        condition_for_pair(i, stock_data_for_pair)
      };
      if (action == 1 && no_of_stocks2 + 1 <= x && no_of_stocks1 - 1 >= -x) {
        cash_flow += t_price1;
        cash_flow -= t_price2;
        no_of_stocks1--;
        no_of_stocks2++;
        order_statistics1 << t_date + ",SELL,1," + to_string(t_price1) + '\n';
        order_statistics2 << t_date + ",BUY,1," + to_string(t_price2) + '\n';
      } else if (action == -1 && no_of_stocks1 + 1 <= x && no_of_stocks2 - 1 >= -x) {
        cash_flow -= t_price1;
        cash_flow += t_price2;
        no_of_stocks1++;
        no_of_stocks2--;
        order_statistics1 << t_date + ",BUY,1," + to_string(t_price1) + '\n';
        order_statistics2 << t_date + ",SELL,1," + to_string(t_price2) + '\n';
      }
      daily_cashflow << t_date + ',' + to_string(cash_flow) + '\n';
    }
    cash_flow += no_of_stocks1 * stock_data_for_pair[0].first + no_of_stocks2 * stock_data_for_pair[0].second;
    daily_cashflow.close();
    order_statistics1.close();
    order_statistics2.close();
    ofstream outputfile("final_pnl.txt");
    outputfile << cash_flow;
    outputfile.close();
  } else {
    fstream fin;
    fin.open(symbol + ".csv", ios::in);

    // fin.open("SBIN.csv",ios::in);
    string csv_line {};
    fin >> csv_line;
    // converting the obtained data in the form of pair of vector
    vector < pair < string, double >> stock_data {};
    vector < pair < double, double >> stock_data_for_ADX {};
    vector < vector < double >> stock_data_for_ML {};
    vector < vector < double >> data_points_for_linear_regression {};
    vector < double > train_data_outset {};
    vector < double > parameters_for_linear_regression {};
    int it {
      -1
    };
    double prev_close {}, prev_open {};
    while (fin >> csv_line) {
      stringstream s {
        csv_line
      };
      string date {}, close {}, high_value {}, low_value {}, open {}, trades {}, vwap {};
      getline(s, date, ',');
      getline(s, close, ',');
      getline(s, high_value, ',');
      getline(s, low_value, ',');
      getline(s, open, ',');
      getline(s, trades, ',');
      getline(s, vwap, ',');
      if (date >= start_date) it++;
      stock_data.push_back({
        date,
        stod(close)
      });
      if (strategy == "ADX") stock_data_for_ADX.push_back({
        stod(low_value),
        stod(high_value)
      });
      if (strategy == "LINEAR_REGRESSION") stock_data_for_ML.push_back(vector < double > {
        stod(open),
        stod(vwap),
        stod(low_value),
        stod(high_value),
        stod(trades)
      });
      if (it >= 0 && date >= train_start_date && date <= train_end_date && strategy == "LINEAR_REGRESSION") {
        train_data_outset.push_back(prev_close);
        vector < double > data_line(8);
        data_line[0] = 1;
        data_line[1] = stod(close);
        data_line[2] = stod(open);
        data_line[3] = stod(vwap);
        data_line[4] = stod(low_value);
        data_line[5] = stod(high_value);
        data_line[6] = stod(trades);
        data_line[7] = prev_open;
        data_points_for_linear_regression.push_back(data_line);
      }
      prev_close = stod(close);
      prev_open = stod(open);
    }

    if (strategy == "LINEAR_REGRESSION") {
      parameters_for_linear_regression = NormalEquation(data_points_for_linear_regression, train_data_outset);
    }
    iterator_for_day_one = it;

    fin.close();
    fstream daily_cashflow, order_statistics;
    daily_cashflow.open("daily_cashflow.csv", ios::out);
    daily_cashflow << "Date,Cashflow\n";
    order_statistics.open("order_statistics.csv", ios::out);
    order_statistics << "Date,Order_dir,Quantity,Price\n";

    int operation_date {};
    double cash_flow {};
    if (strategy == "BASIC" || strategy == "DMA" || strategy == "MACD" || strategy == "RSI" || strategy == "ADX" || strategy == "LINEAR_REGRESSION") {
      int no_of_stocks {};
      for (int i = it; i >= 0; i--) {
        string t_date {
          corrected_date(stock_data[i].first)
        };
        double t_price {
          stock_data[i].second
        };
        //action will describe what are we going to do
        // if it is 1, then we are gonna buy some stonk, if it is -1, then we shall sell some stonks, else we do nothing
        int action {
          condition(i, stock_data, stock_data_for_ADX, parameters_for_linear_regression, stock_data_for_ML)
        };
        if (action == 1 && no_of_stocks + 1 <= x) {
          cash_flow -= t_price;
          no_of_stocks++;
          order_statistics << t_date + ",BUY,1," + to_string(t_price) + '\n';
        } else if (action == -1 && no_of_stocks - 1 >= -x) {
          cash_flow += t_price;
          no_of_stocks--;
          order_statistics << t_date + ",SELL,1," + to_string(t_price) + '\n';
        }
        daily_cashflow << t_date + ',' + to_string(cash_flow) + '\n';
      }
      // string t_date{corrected_date(stock_data[0].first)};
      double t_price {
        no_of_stocks * stock_data[0].second
      };
      cash_flow += t_price;
      // if (no_of_stocks>0){
      //     order_statistics<<t_date+",SELL,"+to_string(no_of_stocks)+','+to_string(t_price)+'\n';
      // }else if (no_of_stocks<0){
      //     order_statistics<<t_date+",BUY,"+to_string(-no_of_stocks)+','+to_string(t_price)+'\n'; 
      // }
      // daily_cashflow<<t_date+','+to_string(cash_flow)+'\n';
    } else if (strategy == "DMA++") {
      queue < pair < string, int >> queue {};
      for (int i = it; i > 0; i--) {
        string t_date {
          corrected_date(stock_data[i].first)
        };
        double t_price {
          stock_data[i].second
        };
        //action will describe what are we going to do
        // if it is 1, then we are gonna buy some stonk, if it is -1, then we shall sell some stonks, else we do nothing
        int action {
          condition(i, stock_data, stock_data_for_ADX, parameters_for_linear_regression, stock_data_for_ML)
        };
        operation_date++;
        int previous_size {
          int(queue.size())
        };
        if (action == 1) {
          if (queue.empty()) queue.push({
            "BUY",
            operation_date
          });
          else {
            auto[previous_type, last_date] = queue.front();
            if (previous_type == "BUY") {
              queue.push({
                "BUY",
                operation_date
              });
              if (operation_date - last_date >= max_hold_days) queue.pop();
            } else {
              queue.pop();
            }
          }
        } else if (action == -1) {
          if (queue.empty()) queue.push({
            "SELL",
            operation_date
          });
          else {
            auto[previous_type, last_date] = queue.front();
            if (previous_type == "SELL") {
              queue.push({
                "SELL",
                operation_date
              });
              if (operation_date - last_date >= max_hold_days) queue.pop();
            } else {
              queue.pop();
            }
          }
        } else {
          if (!queue.empty()) {
            if (operation_date - queue.front().second >= max_hold_days) {
              queue.pop();
            }
          }
        }
        if (queue.size() > x) {
          queue.pop();
        }
        if (queue.size() > previous_size) {
          if (queue.back().first == "BUY") {
            cash_flow -= t_price;
            order_statistics << t_date + ",BUY,1," + to_string(t_price) + '\n';
          } else {
            cash_flow += t_price;
            order_statistics << t_date + ",SELL,1," + to_string(t_price) + '\n';
          }
        } else if (queue.size() < previous_size) {
          if (queue.back().first == "BUY") {
            cash_flow += t_price;
            order_statistics << t_date + ",SELL,1," + to_string(t_price) + '\n';
          } else {
            cash_flow -= t_price;
            order_statistics << t_date + ",BUY,1," + to_string(t_price) + '\n';
          }
        }
        daily_cashflow << t_date + ',' + to_string(cash_flow) + '\n';
      }
      // selling all the stocks on the last day
      string t_date {
        corrected_date(stock_data[0].first)
      };
      double t_price {
        int(queue.size()) * stock_data[0].second
      };

      if (queue.empty());
      else if (queue.front().first == "BUY") {
        cash_flow += t_price;
        order_statistics << t_date + ",SELL," + to_string(queue.size()) + ',' + to_string(t_price) + '\n';
      } else {
        cash_flow -= t_price;
        order_statistics << t_date + ",BUY," + to_string(queue.size()) + ',' + to_string(t_price) + '\n';
      }
      daily_cashflow << t_date + ',' + to_string(cash_flow) + '\n';
    }

    daily_cashflow.close();
    order_statistics.close();
    ofstream outputfile("final_pnl.txt");
    outputfile << cash_flow;
    outputfile.close();
  }
  return 0;
}