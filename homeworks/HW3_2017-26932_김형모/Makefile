
TRG=test
SRC=test.c
OUT=out

all:
	gcc -o $(TRG) $(SRC) -O1 -pthread

run: $(TRG)
	./$(TRG) TAS > tas.$(OUT)
	./$(TRG) TTAS > ttas.$(OUT)
	./$(TRG) CAS > cas.$(OUT)
	./$(TRG) FAA > faa.$(OUT)

clean:
	rm $(TRG)
	rm *.$(OUT)
