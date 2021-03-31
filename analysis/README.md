## Analysis

An analysis of the results along with the plots can be read in [report.pdf](report.pdf)
Link to report at OverLeaf: [Report at Overleaf](https://www.overleaf.com/read/rpzfyjrwcxmc)

### Contents

- results/ : The results obtained on running the program are saved in the results directory.
- model_result/ : This directory demonstrates the model results used to analyse the runtime/utility of the code. 

### Plotting

```bash
cd analysis
make plots
# plots the graphs of static and dynamic queue densities as functions of time using matplotlib of python.
# The densities are taken from a text file named out.txt, and the graph is stored as a png image in the results directory.
```
### Requirements
To install the various requirements for plottting graphs , run the follwoing commands :

```bash
cd analysis
make requirements
#installs the requirements mentioned in the file requirements.txt, which include matplotlib, numpy and pandas (the versions used by us in this assignment)
```
### Files

The various output files produced in results/ are :
- method_0.txt:  This contains the output of  method 0 (baseline method, contains queue densities and frame number).
- out_1_skipped.txt: This is an auxillary output file which is produced only when method 1 is executed. It contans the output with the frames skipped (skipping factor is decided by the user)
- out_x_y.txt: This is an auxillary output file which is produced only when method 3 or 4 is executed. Here x is either 3 or 4, and y is the thread for which we have printed the output.
- utility_runtime.txt : This directory demonstrates the model results used to analyse the runtime of the code.
- method_x_function_y.txt: This contains the output of a particular method used with its parameter (queue densities and frame number). Here x denotes the method number (which ranges from 1 to 5) and y denotes the number of the function (specified by the user in input.txt in code folder)
- plotted_files.txt : This contains the list of the output files of the form method_0.txt and method_x_function_y.txt. These are the final output files which are used for plotting and analysis.
- utility_runtime.txt : This contains a list of methods (which have been executed by the user), the arguments passed to them, their static and dynamic errors and the runtime of the corresponding programs.
- method_0_densities.jpg: Plot for static/dynamic queue densities for method 0 against time.
- method_x_function_y_densities.jpg: Plot for static/dynamic queue densities for method x and function number y against time.
- methodx_utility_parameter.jpg: Plot for utility vs parameter for method number x.
- methodx_utility_runtime.jpg: Plot for utility vs runtime for method number x.
- plotted_files.txt : This contains the list of the output files of the form method_0.txt and method_x_function_y.txt. These are the final output files which are used for plotting and analysis.

The various output files present in model_result are :

- All the types of output files enlisted above run on various parameters for all the methods
- method_x_CPU_usage.txt : Here, x can be 3 or 4. This contains the values of CPU usage for various values of threads run on methods 3 and 4.
- method_x_memory.txt : Here, x can be 3 or 4. This contains the values of memory usage for various values of threads run on methods 3 and 4.
- method_x_CPU_usage.jpg : Here, x can be 3 or 4. This contains the plots of CPU usage vs number of threads run on methods 3 and 4
- method_x_memory.jpg : Here, x can be 3 or 4. This contains the plots of memory usage for number of threads run on methods 3 and 4
- memory_cpu.py : This python file is used for plotting the above mentioned graphs.
