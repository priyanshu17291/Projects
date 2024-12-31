.PHONY:soup clean run plot csv
strategy?=PAIRS
symbol?=SBIN
n?=1
x?=3
p?=2
max_hold_days?=5
c1?=2
c2?=0.2
oversold_threshold?=30
overbought_threshold?=70
adx_threshold?=25
threshold?=2
symbol1=SBIN
symbol2=ADANIENT
train_start_date=01/05/2023
train_end_date=01/09/2024
start_date?=01/01/2023
end_date?=30/12/2024
soup:
	python generate_csv.py $(strategy) $(symbol) $(n) $(symbol1) $(symbol2) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
	g++ main.cpp
	./a.exe $(strategy) $(symbol) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) $(threshold) $(symbol1) $(symbol2) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
	python plot.py
clean:
	rm -f a.out
csv:
	python generate_csv.py $(strategy) $(symbol) $(n) $(symbol1) $(symbol2) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
run:
	g++ main.cpp
	./a.exe $(strategy) $(symbol) $(n) $(x) $(p) $(max_hold_days) $(c1) $(c2) $(oversold_threshold) $(overbought_threshold) $(adx_threshold) $(threshold) $(symbol1) $(symbol2) $(train_start_date) $(train_end_date) $(start_date) $(end_date)
plot:
	python plot.py
