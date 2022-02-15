/* Copyright (c) 1993-2015, NVIDIA CORPORATION. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of NVIDIA CORPORATION nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <stdlib.h>
#include <stdio.h>
#include <v3.h>

int main(int argc, char ** argv)
{
	int n = 1000000;
	if(argc > 1)	{ n = atoi(argv[1]);}     // Number of particles
	if(argc > 2)	{	srand(atoi(argv[2])); } // Random seed

	v3 * vArray = new v3[n];
	v3 * vArray_d = NULL;
	cudaMalloc(&vArray_d, n*sizeof(v3));
	cudaMemcpy(vArray_d, vArray, n*sizeof(v3), cudaMemcpyHostToDevice);
        doVecScram(vArray_d,n);
        cudaDeviceSynchronize();
	cudaMemcpy(vArray, vArray_d, n*sizeof(v3), cudaMemcpyDeviceToHost);
	v3 totalDistance(0,0,0);
	v3 temp;
	for(int i=0; i<n; i++)
	{
		temp = vArray[i];
		totalDistance.x += temp.x;
		totalDistance.y += temp.y;
		totalDistance.z += temp.z;
	}
	float avgX = totalDistance.x /(float)n;
	float avgY = totalDistance.y /(float)n;
	float avgZ = totalDistance.z /(float)n;
	float avgNorm = sqrt(avgX*avgX + avgY*avgY + avgZ*avgZ);
	printf(	"%d particles random vecs. Average position is |(%f, %f, %f)| = %f\n", 
					n, avgX, avgY, avgZ, avgNorm);
	return 0;
}
