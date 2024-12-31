import sys
import pandas as pd
from jugaad_data.nse import stock_df
from datetime import datetime, timedelta, date
import time
import os

def get_date(_,n):
    modified_date = (datetime.strptime(_, '%d/%m/%Y') - timedelta(days=2 * n + 100)).strftime("%d/%m/%Y")
    d,m,y = map(int, modified_date.split('/'))
    return date(y,m,d)

# get data 
def get_datatwo(symbol1, symbol2, start, end, n):
    from_date = get_date(start,n)
    to_date = get_date(end,0)
    # Fetch and process data for the first symbol
    df = stock_df(symbol=symbol1, from_date=from_date, to_date=to_date, series="EQ")
    df = df[['DATE', 'CLOSE', 'HIGH', 'LOW', 'OPEN', 'NO OF TRADES', 'VWAP']]
    df = df.rename(columns={'NO OF TRADES': 'TRADES'})
    df.to_csv(f"{symbol1}.csv", sep=',', index=False, encoding='utf-8')

    # Fetch and process data for the second symbol
    df = stock_df(symbol=symbol2, from_date=from_date, to_date=to_date, series="EQ")
    df = df[['DATE', 'CLOSE', 'HIGH', 'LOW', 'OPEN', 'NO OF TRADES', 'VWAP']]
    df = df.rename(columns={'NO OF TRADES': 'TRADES'})
    df.to_csv(f"{symbol2}.csv", sep=',', index=False, encoding='utf-8')


def get_stock_data(symbol, start, end, n):
    from_date = datetime.strptime(start, '%d/%m/%Y') - timedelta(days=2 * n + 100)
    to_date = datetime.strptime(end, '%d/%m/%Y')

    # Fetch and process data for the single symbol
    df = stock_df(symbol=symbol, from_date=from_date, to_date=to_date, series="EQ")
    df = df[['DATE', 'CLOSE', 'HIGH', 'LOW', 'OPEN', 'NO OF TRADES', 'VWAP']]
    df = df.rename(columns={'NO OF TRADES': 'TRADES'})
    df.to_csv(f"{symbol}.csv", sep=',', index=False, encoding='utf-8')

try:
    # e.g. python generate_csv.py PAIRS SBIN 14 SBIN ADANIENT 10/01/2023 15/05/2023 01/01/2023 01/01/2024
    argument=sys.argv

    # e.g. Arg  ['generate_csv.py', 'PAIRS', 'SBIN', '14', 'SBIN', 'ADANIENT', '10/01/2023', '15/05/2023', '01/01/2023', '01/01/2024']
    
    a=argument[-2]
    if argument[1]=="LINEAR_REGRESSION":
        a=argument[-4]

    if argument[1]=="PAIRS":
        get_datatwo(argument[4],argument[5],argument[-2],argument[-1],int(argument[3]))
    else:
        get_stock_data(argument[2],a,argument[-1],int(argument[3]))
    
except:
    raise RuntimeError('Invalid input format!!')
