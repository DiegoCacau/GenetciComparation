from datetime import datetime
from covid19dh import covid19


x, src = covid19("BRA", level = 3, verbose = False)

x.to_csv('total_data.csv')

# print(x['date'],x['deaths'])

# for i in x.head():
# 	print(i)


# x['new_deaths'] = (x['deaths']).diff()._get_numeric_data()

# x['new_cases'] = x['confirmed'] - x['deaths'] - x['recovered']
# x.to_csv('output.csv', columns=['date','deaths', 'confirmed', 'new_cases', 'new_deaths'])

# x.to_csv('output_new_deaths.csv', columns=['date', 'new_deaths'])