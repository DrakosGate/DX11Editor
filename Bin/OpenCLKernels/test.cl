__kernel void ArrayAdd(__global float* _pArrayA, __global float* _pArrayB, __global float* _pArrayOut)
{
	unsigned int iIndex = get_global_id(0);
	_pArrayOut[iIndex] = _pArrayA[iIndex] + _pArrayB[iIndex];
}