import pandas as pd
import matplotlib.pyplot as plt

# Plot daily_cashflow.csv
df_cashflow = pd.read_csv('daily_cashflow.csv')
df_cashflow['Date'] = pd.to_datetime(df_cashflow['Date'], format='%d/%m/%Y')

plt.figure(figsize=(10, 5))
plt.plot(df_cashflow['Date'], df_cashflow['Cashflow'], marker='o', linestyle='-')
plt.title('Daily Cashflow')
plt.xlabel('Date')
plt.ylabel('Cashflow')
plt.grid(True)
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('daily_cashflow.png')
plt.show()

# Plot order_statistics_1.csv
df_orders = pd.read_csv('order_statistics_1.csv')
df_orders['Date'] = pd.to_datetime(df_orders['Date'], format='%d/%m/%Y')

buy_orders = df_orders[df_orders['Order_dir'] == 'BUY']
sell_orders = df_orders[df_orders['Order_dir'] == 'SELL']

plt.figure(figsize=(10, 5))
plt.plot(buy_orders['Date'], buy_orders['Price'], marker='o', linestyle='-', color='green', label='BUY')
plt.plot(sell_orders['Date'], sell_orders['Price'], marker='o', linestyle='-', color='red', label='SELL')
plt.title('Order Statistics 1')
plt.xlabel('Date')
plt.ylabel('Price')
plt.legend()
plt.grid(True)
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('order_statistics_1.png')
plt.show()

# Plot order_statistics_2.csv
df_orders2 = pd.read_csv('order_statistics_2.csv')
df_orders2['Date'] = pd.to_datetime(df_orders2['Date'], format='%d/%m/%Y')

buy_orders2 = df_orders2[df_orders2['Order_dir'] == 'BUY']
sell_orders2 = df_orders2[df_orders2['Order_dir'] == 'SELL']

plt.figure(figsize=(10, 5))
plt.plot(buy_orders2['Date'], buy_orders2['Price'], marker='o', linestyle='-', color='blue', label='BUY')
plt.plot(sell_orders2['Date'], sell_orders2['Price'], marker='o', linestyle='-', color='orange', label='SELL')
plt.title('Order Statistics 2')
plt.xlabel('Date')
plt.ylabel('Price')
plt.legend()
plt.grid(True)
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('order_statistics_2.png')
plt.show()