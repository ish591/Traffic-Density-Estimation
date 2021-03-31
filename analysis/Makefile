plots:
	python3 plotter.py

model:
	rm -rf model_result
	mkdir model_result
	cp results/* model_result/

requirements:
	python3 -m pip install -r requirements.txt

.DEFAULT: 
	@echo "No such command found."
	@echo "Run 'make' or 'make plots' for plotting graphs."
	@echo "Run 'make requirements' to download the requirements for python."

.SILENT: model requirements