all = serve.o serve_func.o
serve:$(all)
	gcc $(all) -o serve -Wall -pthread -lsqlite3
.PYTHON:clean
clean:
	rm $(all) serve
