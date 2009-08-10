CC     = gcc

#  Matlab mex compiler
MEX    = mex -I../
#  Octave mex compiler
#MEX    = mkoctfile --mex -I../

RM     = rm -f
CFLAGS = -Wall -fPIC -I../
LDLIBS = 

OBJ =	soapC.o \
	soapClient.o \
	stdsoap2.o \
        turblib.o

all: nullOp getVelocity getForce getVelocityAndPressure getVelocityGradient getVelocityHessian \
	getVelocityLaplacian getPressureGradient getPressureHessian

nullOp : $(OBJ)
	$(MEX) nullOp.c $(OBJ)	

getVelocity : $(OBJ)
	$(MEX) getVelocity.c $(OBJ)

getForce : $(OBJ)
	$(MEX) getForce.c $(OBJ)

getVelocityAndPressure : $(OBJ)
	$(MEX) getVelocityAndPressure.c $(OBJ)

getVelocityGradient : $(OBJ)
	$(MEX) getVelocityGradient.c $(OBJ)

getVelocityHessian : $(OBJ)
	$(MEX) getVelocityHessian.c $(OBJ)

getVelocityLaplacian : $(OBJ)
	$(MEX) getVelocityLaplacian.c $(OBJ)

getPressureGradient : $(OBJ)
	$(MEX) getPressureGradient.c $(OBJ)

getPressureHessian : $(OBJ)
	$(MEX) getPressureHessian.c $(OBJ)

soapC.o: ../soapC.c
	$(CC) $(CFLAGS) -c $<

soapClient.o: ../soapClient.c
	$(CC) $(CFLAGS) -c $<

stdsoap2.o: ../stdsoap2.c
	$(CC) $(CFLAGS) -c $<

turblib.o: ../turblib.c
	$(CC) $(CFLAGS) -c $<

clean:
	$(RM) *.o *.mex*

.SUFFIXES: .o .c .x

#.c.o:
#	$(CC) $(CFLAGS) -c $< 

