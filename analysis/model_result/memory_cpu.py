import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# +
#RESULTS_DIR = './results/'
#out_files = open(RESULTS_DIR+'plotted_files.txt', 'r')
method_3_cpu = pd.read_csv("method_3_CPU_usage.txt",delimiter = ' ')
method_3_mem = pd.read_csv("method_3_memory.txt", delimiter =' ')
method_4_cpu = pd.read_csv("method_4_CPU_usage.txt",delimiter = ' ')
method_4_mem = pd.read_csv("method_4_memory.txt", delimiter = ' ')
lg1 = method_3_cpu[['threads','percentage']].plot(title = 'CPU usage for method 3', style = ['.-'], color = ['orange'], kind = "line", x= 'threads', figsize= (10,5) ,legend = True )
lg1.set_xlabel ("Number of threads")
lg1.set_ylabel ("CPU used (%)")
fig1 = lg1.get_figure()
fig1.savefig("method_3_CPU_usage.jpg", facecolor =fig1.get_facecolor(), transparent =True, dpi=300,bbox_inches='tight')
plt.close()

lg2 = method_3_mem[['threads','memory']].plot(title = 'Memory usage for method 3', style = ['.-'], color = ['orange'], kind = "line", x= 'threads', figsize= (10,5) ,legend = True )
lg2.set_xlabel ("Number of threads")
lg2.set_ylabel ("Memory used (M)")
fig2 = lg2.get_figure()
fig2.savefig("method_3_memory.jpg", facecolor =fig2.get_facecolor(), transparent =True, dpi=300,bbox_inches='tight')
plt.close()

lg3 = method_4_cpu[['threads','percentage']].plot(title = 'CPU usage for method 4', style = ['.-'], color = ['orange'], kind = "line", x= 'threads', figsize= (10,5) ,legend = True )
lg3.set_xlabel ("Number of threads")
lg3.set_ylabel ("CPU used (%)")
fig3 = lg3.get_figure()
fig3.savefig("method_4_CPU_usage.jpg", facecolor =fig1.get_facecolor(), transparent =True, dpi=300,bbox_inches='tight')
plt.close()

lg4 = method_4_mem[['threads','memory']].plot(title = 'Memory usage for method 4', style = ['.-'], color = ['orange'], kind = "line", x= 'threads', figsize= (10,5) ,legend = True )
lg4.set_xlabel ("Number of threads")
lg4.set_ylabel ("Memory used (M)")
fig4 = lg4.get_figure()
fig4.savefig("method_4_memory.jpg", facecolor =fig2.get_facecolor(), transparent =True, dpi=300,bbox_inches='tight')
plt.close()



