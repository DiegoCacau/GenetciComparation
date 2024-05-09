from datetime import datetime
from covid19dh import covid19


x, src = covid19("BRA", level = 1, verbose = False, end = "2022-12-30")

# print(x['date'],x['deaths'])


# for i in x:
# 	total = i['confirmed'] - i['deaths'] - i['recovered']
# 	print(total)

# x['new_deaths'] = (x.groupby(['date'])['deaths']
#                           .diff()
#                           .fillna(x['deaths'])						
#                           .astype(int))

x['new_deaths'] = (x['deaths']).diff()._get_numeric_data()

x['new_cases'] = x['confirmed'] - x['deaths'] - x['recovered']
x.to_csv('output.csv', columns=['date','deaths', 'confirmed', 'new_cases', 'new_deaths'])