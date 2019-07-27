#pragma once

template<typename T>
int partition(int low, int high,  T * target) 
{
	T pivot = target[low + (high - low) / 2];
	bool done = false;
	while (!done) 
	{
		while (target[low] < pivot) 
		{
			low++;
		}

		while (target[high] > pivot) 
		{
			high--;
		}

		if (low >= high) 
		{
			done = true;
			
		}
		else
		{
			T temp = target[low];
			target[low] = target[high];
			target[high] = temp;
			low++;
			high--;			
		}
	}
	return high; //returns the starting index for high partition;
}

/*****************************************************************
 *  Recursively compare the array values to the pivot value(mid of array) and by
 *  putting them in the correct partition, it sorts the array
 * 	data:  array of integers.
 *	low:   index.
 * 	high:  index.
 *****************************************************************/
template<typename T>
void quicksort(int low, int high, T& target) //target will be an array
{
	if (low < high) 
	{
		int last = partition(low, high, target);

		quicksort(low, last, target);
		quicksort(last + 1, high, target);
	}
}