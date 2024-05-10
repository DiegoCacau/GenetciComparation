import pandas as pd

# file = '/home/cacau/Downloads/cases-brazil-cities-time_2020.csv'
# df = pd.read_csv(file)
# df['ibgeID'] = df['ibgeID'].astype(str)
# nit = df.loc[df['ibgeID'] == '3303302']
# nit.to_csv('niteroi_total.csv')


file_nit = 'niteroi_total.csv'
df = pd.read_csv(file_nit)

df.to_csv('niteroi_new_cases.csv', columns=['date', 
											'newCases', 
											'totalCases', 
											'newDeaths', 
											'deaths'])