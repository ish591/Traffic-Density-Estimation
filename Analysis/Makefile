plots:
	python3 plotter.py

model:
	rm -rf model_result
	mkdir model_result
	cp results/* model_result/

.DEFAULT: 
	@echo "No such command found."
	@echo "Run 'make' or 'make plots' for plotting graphs."

.SILENT: model