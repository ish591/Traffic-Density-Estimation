# ---
# jupyter:
#   jupytext:
#     text_representation:
#       extension: .py
#       format_name: light
#       format_version: '1.5'
#       jupytext_version: 1.11.1
#   kernelspec:
#     display_name: Python 3
#     language: python
#     name: python3
# ---

import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# +
out_files = open('./model_result/plotted_files.txt', 'r')
df0 = pd.read_csv('./model_result/method_0.txt', delimiter = ' ')
for file in out_files:
    df = pd.read_csv(file.rstrip('\n'), delimiter = ' ')
    df['Time'] = df['frame_number']/15
    df['static_density_baseline'] = df0['static_density']
    df['dynamic_density_baseline'] = df0['dynamic_density']
#     print(file)
    line_graph = df[['Time', 'static_density', 'dynamic_density', 'static_density_baseline', 'dynamic_density_baseline']].plot(title=file, style=['-','-','--', '--'], color=['green','orange','pink','brown'], kind = "line", x = 'Time', figsize=(10,5), legend = True)
    fig=line_graph.get_figure()
    fig.savefig(file[:-4]+'jpg', facecolor=fig.get_facecolor(), transparent=True, dpi=300,bbox_inches='tight')

# pd.read_csv('../results/utility_runtime.txt', delimiter=' ')
    

# -

df




# +
df = pd.read_csv('./model_result/utility_runtime.txt', delimiter = ' ', header = 0)

df['static_utility'] = 1/df['static_error']
df['dynamic_utility'] = 1/df['dynamic_error']


for idx, row in df.iterrows():
    if row['method'] == 0:
        df.at[idx, 'parameter1'] = 1
        df.at[idx, 'parameter2'] = 1088*1920
        df.at[idx, 'parameter3'] = 1
        df.at[idx, 'parameter4'] = 1
        df.at[idx, 'parameter5'] = 0
    elif row['method'] == 2:
        df.at[idx, 'parameter'+str(int(row['method']))] = row['argument1']*row['argument2']
    else:
        df.at[idx, 'parameter'+str(int(row['method']))] = row['argument1']

df = df.sort_values('runtime')



for i in range(1,6):
    df1 = df[(df['method'] == i) | (df['method'] == 0)]
    bar_graph = df1[['static_utility', 'dynamic_utility', 'runtime']].plot(kind = "bar", x = 'runtime',figsize=(10,5), legend = True, title = "method"+str(i))
    fig=bar_graph.get_figure()
    fig.savefig('method'+str(i)+'.jpg', facecolor=fig.get_facecolor(), transparent=True, dpi=300,bbox_inches='tight')

# -



df




for i in range(1, 3):
    df1 = df[(df['method'] == i) | (df['method'] == 0)]
    df1 = df1.sort_values('parameter'+str(i), ascending = True)
    bar_graph = df1[['static_utility', 'dynamic_utility', 'parameter' + str(i)]].plot(kind = "bar", x = 'parameter'+str(i),figsize=(10,5), legend = True, title = "method"+str(i))
    fig=line_graph.get_figure()
    fig.savefig(file[:-4]+'jpg', facecolor=fig.get_facecolor(), transparent=True, dpi=300,bbox_inches='tight')

for i in range(1, 6):
    df1 = df[(df['method'] == i) | (df['method'] == 0)]
    df1 = df1.sort_values('parameter'+str(i), ascending = True)
    line_graph = df1[['runtime', 'parameter' + str(i)]].plot(kind = "line", x = 'parameter'+str(i),figsize=(10,5), legend = True, title = "method"+str(i))
    fig=line_graph.get_figure()
    fig.savefig('method'+str(i)+'_line.jpg', facecolor=fig.get_facecolor(), transparent=True, dpi=300,bbox_inches='tight')

# !jupytext --to py notebook.ipynb



