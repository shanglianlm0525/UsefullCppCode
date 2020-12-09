#include "stdafx.h"
#include "LabelingPixelFromImage.h"

#define WIDTH 160
#define HEIGHT 160

int g_Patten[(WIDTH * HEIGHT + 1) / 2 + 1];

//Find the root of the tree of node i
int findRoot(const int *P, int i)
{
	int root = i;
	while (P[root] < root)
	{
		root = P[root];
	}
	return root;
}

//Make all nodes in the path of node i point to root
void setRoot(int *P, int i, int root)
{
	int j;
	while (P[i] < i)
	{
		j = P[i];
		P[i] = root;
		i = j;
	}
	P[i] = root;
}

//Find the root of the tree of the node i and compress the path in the process
int find(int *P, int i)
{
	int root = findRoot(P, i);
	setRoot(P, i, root);
	return root;
}

//unite the two trees containing nodes i and j and return the new root
int get_union_set(int *P, int i, int j)
{
	int root, rootj;
	root = findRoot(P, i);
	if (i != j)
	{
		rootj = findRoot(P, j);
		if (root > rootj)
		{
			root = rootj;
		}
		setRoot(P, j, root);
	}
	setRoot(P, i, root);
	return root;
}

int flattenL(int *P, int length)
{
	int k = 1;
	int i;
	for (i = 1; i < length; ++i)
	{
		if (P[i] < i)
		{
			P[i] = P[P[i]];
		}
		else
		{
			P[i] = k;
			k = k + 1;
		}
	}
	return k;
}

/* LabelingPixelFromImage
p q r
t x
*/
int LabelingPixelFromImage(unsigned char* img, int width, int height, int* labels)
{
	int i, j;
	unsigned char *img_row;
	unsigned char *img_row_prev;
	int *label_row;
	int *label_row_prev;
	int * img_row_start;
	int * img_row_end;
	int unique = 1;
	int nLabels = 0;
	g_Patten[0] = 0;
	for (i = 1; i < height; ++i)
	{
		img_row = img + i*width;
		img_row_prev = img_row - width;

		label_row = labels + i*width;
		label_row_prev = label_row - width;
		for (j = 1; j < width; ++j)
		{
			if (img_row[j] > 0)
			{
				if (img_row_prev[j] > 0)
				{
					// q = 1
					label_row[j] = label_row_prev[j];
				}
				else
				{
					//q = 0
					if (j < width - 1 && img_row_prev[j + 1]>0)
					{
						// r =1
						if (img_row_prev[j - 1] > 0)
						{
							// p =1
							label_row[j] = get_union_set(g_Patten, label_row_prev[j - 1], label_row_prev[j + 1]);
						}
						else
						{
							// p = 0
							if (img_row[j - 1] > 0)
							{
								// t = 1
								label_row[j] = get_union_set(g_Patten, label_row[j - 1], label_row_prev[j + 1]);
							}
							else
							{
								// t = 0
								label_row[j] = label_row_prev[j + 1];
							}
						}
					}
					else
					{
						// r =0
						if (img_row_prev[j - 1] > 0)
						{
							//p =1
							label_row[j] = label_row_prev[j - 1];
						}
						else
						{
							// p= 0
							if (img_row[j - 1]>0)
							{
								// t =1
								label_row[j] = label_row[j - 1];
							}
							else
							{
								// t = 0  new start
								label_row[j] = unique;
								g_Patten[unique] = unique;
								unique = unique + 1;
							}
						}

					}
				}
			}
			else
			{
				//background pixel
				label_row[j] = 0;
			}
		}
	}

	//analysis
	nLabels = flattenL(g_Patten, unique);

	for (i = 0; i < height; ++i)
	{
		img_row_start = labels + i * width;
		img_row_end = img_row_start + width;
		for (j = 0; img_row_start != img_row_end; ++img_row_start, ++j)
		{
			*img_row_start = g_Patten[*img_row_start];
		}
	}

	return nLabels;
}