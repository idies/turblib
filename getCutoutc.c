//	Copyright 2011 Johns Hopkins University
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.


#include <stdio.h>
#include <float.h>
#include <math.h>
#include "turblib.h"

/*
 * Turbulence Database sample C client code
 */
#define N 8

int main(int argc, char *argv[])
{

	char * authtoken = "edu.jhu.pha.turbulence.testing-201406";
	char * dataset = "isotropic1024coarse";
	char * field;

	float *result;

	int t0 = 0, x0 = 0, y0 = x0, z0 = x0, nx = 2, ny = nx, nz = nx;
	int x_step = 1, y_step = 1, z_step = 1, filter_width = 1;
	int size, i;

	/* Initialize gSOAP */
	soapinit();

	/* Enable exit on error.  See README for details. */
	turblibSetExitOnError(1);

	printf("\n.........getAnyCutoutWeb.........\n");

	printf("\n.........channel u.........\n");
	field = "u";
	if (field[0] == 'p')
	{
		size = nx * ny*nz;
	}
	else if (field[0] == 'u')
	{
		size = nx * ny*nz * 3;
	}
	result = (float *)malloc(sizeof(float)*size);
	getAnyCutoutWeb(authtoken, "channel", field, t0, x0, y0, z0, nx, ny, nz, x_step, y_step, z_step, filter_width, result);
	for (i = 0; i < size; i++) {
		printf("%d: u=%f\n", i, result[i]);
	}
	free(result);

	printf("\n.........transition_bl p.........\n");
	field = "p";
	if (field[0] == 'p')
	{
		size = nx * ny*nz;
	}
	else if (field[0] == 'u')
	{
		size = nx * ny*nz * 3;
	}
	result = (float *)malloc(sizeof(float)*size);
	getAnyCutoutWeb(authtoken, "transition_bl", field, t0, x0, y0, z0, nx, ny, nz, x_step, y_step, z_step, filter_width, result);
	for (i = 0; i < size; i++) {
		printf("%d: p=%f\n", i, result[i]);
	}

	printf("\n.........isotropic1024coarse u filter.........\n");
	field = "u";
	size = 2 * 2 * 2 * 3;
	result = (float *)malloc(sizeof(float)*size);
	getAnyCutoutWeb(authtoken, "isotropic1024coarse", field, t0, x0, y0, z0, nx * 4, ny * 4, nz * 4, x_step * 4, y_step * 4, z_step * 4, filter_width * 4, result);
	for (i = 0; i < size; i++) {
		printf("%d: u=%f\n", i, result[i]);
	}
	free(result);
	/* Free gSOAP resources */
	soapdestroy();

	return 0;
}
