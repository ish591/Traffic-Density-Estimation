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
RESULTS_DIR = './results/'
out_files = open(RESULTS_DIR+'plotted_files.txt', 'r')
df0 = pd.read_csv(RESULTS_DIR+'method_0.txt', delimiter = ' ')
for file in out_files:
    df = pd.read_csv(RESULTS_DIR+file.rstrip('\n'), delimiter = ' ')
    df['Time'] = df['frame_number']/15
    df['static_density_baseline'] = df0['static_density']
    df['dynamic_density_baseline'] = df0['dynamic_density']
#     print(file)
    line_graph = df[['Time', 'static_density', 'dynamic_density']].plot(title='Static/Dynamic Densities', style=['-','-'], color=['green','orange'], kind = "line", x = 'Time', figsize=(10,5), legend = True)
    line_graph.set_xlabel("Time (in seconds)")
    line_graph.set_ylabel("Density (Static/Dynamic)")
    fig=line_graph.get_figure()
    fig.savefig(RESULTS_DIR+file[:-5]+'_densities.'+'jpg', facecolor=fig.get_facecolor(), transparent=True, dpi=300,bbox_inches='tight')
    plt.close()
# pd.read_csv('../results/utility_runtime.txt', delimiter=' ')
    

# -

df




# +
df = pd.read_csv(RESULTS_DIR+'utility_runtime.txt', delimiter = ' ', header = 0)

df['static_utility'] = 1/(df['static_error'])
df['dynamic_utility'] = 1/(df['dynamic_error'])


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
    df1 = df[(df['method'] == i)]
    if (len(df1)>0):
        line_graph = df1[['static_utility', 'dynamic_utility', 'runtime']].plot(kind = "line",style=['.-','.-'],x = 'runtime',figsize=(10,5), legend = True, title = "Method "+str(i)+" Utility Vs Runtime")
        fig=line_graph.get_figure()
        line_graph.set_xlabel("Runtime (in seconds)")
        line_graph.set_ylabel("Utility (Static/Dynamic)")
        fig.savefig(RESULTS_DIR+'method'+str(i)+'_utility_runtime.jpg', facecolor=fig.get_facecolor(), transparent=True, dpi=300,bbox_inches='tight')
        plt.close()
# -


df
for i in range(1, 6):
    df1 = df[(df['method'] == i)]
    if (len(df1)>0):
        df1 = df1.sort_values('parameter'+str(i), ascending = True)
        line_graph = df1[['static_utility', 'dynamic_utility', 'parameter' + str(i)]].plot(kind = "line", x = 'parameter'+str(i),style=['.-','.-'],figsize=(10,5), legend = True, title = "Method "+str(i)+" Utility Vs Parameter")
        if (i==1):
            line_graph.set_xlabel("Skip-factor")
        elif (i==2):
            line_graph.set_xlabel("Total Pixels")
        elif (i==3 or i==4):
            line_graph.set_xlabel("Number of Threads")
        else:
            line_graph.set_xlabel("Sparse/Dense Optical flow")

        line_graph.set_ylabel("Utility (Static/Dynamic)")
        fig=line_graph.get_figure()
        fig.savefig(RESULTS_DIR+'method'+str(i)+'_utility_parameter'+'.jpg', facecolor=fig.get_facecolor(), transparent=True, dpi=300,bbox_inches='tight')
        plt.close()
for i in range(1, 6):
    df1 = df[(df['method'] == i) | (df['method']==0)]
    if (len(df1)>1):
        df1 = df1.sort_values('parameter'+str(i), ascending = True)
        line_graph = df1[['runtime', 'parameter' + str(i)]].plot(kind = "line",style=['.-','.-'] ,x = 'parameter'+str(i),figsize=(10,5), legend = True, title = "Method "+str(i)+" Runtime Vs Parameter")
        if (i==1):
            line_graph.set_xlabel("Skip-factor")
        elif (i==2):
            line_graph.set_xlabel("Total Pixels")
        elif (i==3 or i==4):
            line_graph.set_xlabel("Number of Threads")
        else:
            line_graph.set_xlabel("Sparse/Dense Optical flow")
        line_graph.set_ylabel("Runtime (in seconds)")
        fig=line_graph.get_figure()
        fig.savefig(RESULTS_DIR+'method'+str(i)+'_runtime_parameter.jpg', facecolor=fig.get_facecolor(), transparent=True, dpi=300,bbox_inches='tight')
        plt.close()
# !jupytext --to py notebook.ipynb



