import matplotlib.pyplot as plt 
count =0;
f = open("./results/plotted_files.txt","r");
for file_strings in f:
	count+=1
	file = open(file_strings[:-1])
	Time_second = [] 
	Static_Density= []
	Dynamic_Density = []  
	for x in file:
		tuplet=x.split()
		Time_second.append(float(tuplet[0])/15.0)
		Static_Density.append(float(tuplet[1]))
		Dynamic_Density.append(float(tuplet[2]))
	file.close()
	# plotting the line 1 points 
	plt.figure()
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
	#plt.show()
	plt.savefig("./results/plot_"+"method_"+str(file_strings[17])+"_function_"+str(count)+".png",dpi=300,bbox_inches='tight')
	plt.close()

f2 = open("./results/utility_runtime.txt")
runtimes = [[]]
static_utility =[[]]
dynamic_utility =[[]]
for i in range(6):
	runtimes.append([])
	static_utility.append([])
	dynamic_utility.append([])
#method_2_runtimes =[]
#method_2_static_utility=[]
#method_2_dynamic_utility=[]
#method_3_runtimes =[]
#method_3_static_utility=[]
#method_3_dynamic_utility=[]
#method_4_runtimes =[]
#method_4_static_utility=[]
for values in f2:
	val_list=values.split()
	method_num = int(val_list[0])
	runtimes[method_num].append(float(val_list[5]))
	static_utility[method_num].append(float(val_list[3]))
	dynamic_utility[method_num].append(float(val_list[4]))
f2.close()
for i in range(6):
	if (len(runtimes[i])>1):
		plt.figure()
		plt.plot(runtimes[i], static_utility[i], label = "Utility vs Runtime (static)", color='blue') 
		# plotting the line 2 points  
		plt.plot(runtimes[i], dynamic_utility[i], label = "Utility vs Runtime (dynamic)", color='orange')   
		# naming the x axis 
		plt.xlabel('Runtime (in seconds)') 
		# naming the y axis 
		plt.ylabel('Utility') 
		# giving a title to my graph 
		plt.title('Utility vs Runtime')   
		# show a legend on the plot 
		plt.legend(loc=2, fontsize= 'x-small') 
		#plt.show()
		plt.savefig("./results/plot_utility_runtime_"+"method_"+str(i)+".png",dpi=300,bbox_inches='tight')
		plt.close()
