#include <bits/stdc++.h>
#include "DSU.cpp"

using namespace std;

const int MAX_N = 2e5 + 7;
const int inf = 1e9 + 7;

void print(vector<list<unsigned>> vct) {
	for(auto i:vct) {
		printf("{ ");
		for(auto j:i) {
			printf("%d ", j);
		}
		printf("}");
	} cout<<endl;
}

int main()
{
	const DSU x(5);
	cout<<x._nodes<<endl;
	cout<<&x<<endl;
	DSU y(4);
	y = x;
	cout<<y._nodes<<endl;
	cout<<&y<<endl;
}
