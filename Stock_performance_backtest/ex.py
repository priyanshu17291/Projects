import sys
import pandas as pd
from jugaad_data.nse import stock_df
from datetime import datetime, timedelta, date
import os
import time

start = "01/01/2021"
adjusted_start = (datetime.strptime("01/01/2020", "%d/%m/%Y") - timedelta(days=25)).strftime("%d/%m/%Y")
print("adf", adjusted_start)
end = "01/01/2022"
d,m,y = map(int, adjusted_start.split('/'))
print(d,m,y)

n = 14
from_date = (datetime.strptime(start, '%d/%m/%Y') - timedelta(days=2 * n + 100)).strftime('%Y-%m-%d')
to_date=datetime.strptime(end, '%d/%m/%Y').strftime('%Y-%m-%d')
print(from_date, to_date)
print(date(2020,1,1))