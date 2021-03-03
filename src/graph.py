import matplotlib.pyplot as plt 
  
# line 1 points 
Time_second = [] 
Static_Density= []
Dynamic_Density = []  
f=open("./results/out.txt","r");
for x in f:
	tuplet=x.split()
	Time_second.append(float(tuplet[0])/15.0)
	Static_Density.append(float(tuplet[1]))
	Dynamic_Density.append(float(tuplet[2]))

f.close
# plotting the line 1 points  
plt.plot(Time_second, Static_Density, label = "Queue Density", color='blue') 
# plotting the line 2 points  
plt.plot(Time_second, Dynamic_Density, label = "Moving Density", color='orange')   
# naming the x axis 
plt.xlabel('Time (in seconds)') 
# naming the y axis 
plt.ylabel('Density') 
# giving a title to my graph 
plt.title('Static and Dynamic densities')   
# show a legend on the plot 
plt.legend(loc=2, fontsize= 'x-small') 
plt.savefig('./results/plot.png',dpi=300,bbox_inches='tight');  
# function to show the plot 
plt.show() 