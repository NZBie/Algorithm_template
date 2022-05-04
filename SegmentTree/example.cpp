
// https://www.luogu.com.cn/problem/SP1716

#include <bits/stdc++.h>
#include "SegmentTree.cpp"

using namespace std;

const int MAX_N = 2e5 + 7;
const int inf = 1e9 + 7;

struct Matrix {
	int a[3][3];
	inline Matrix operator + (const Matrix& mat) const {
		Matrix res;
		for(int i=0;i<3;i++) {
			for(int j=0;j<3;j++) {
				for(int k=0;k<3;k++) {
					if(!k) res.a[i][j] = a[i][k] + mat.a[k][j];
					else res.a[i][j] = max(res.a[i][j], a[i][k] + mat.a[k][j]);
				}
			}
		}
		return res;
	}
	void print() {
		for(int i=0;i<3;i++) {
			cout<<"[";
			for(int j=0;j<3;j++) {
				cout<<a[i][j]<<' ';
			}
			cout<<"]"<<endl;
		}
	}
} a[MAX_N];

int main()
{
	int N;
	cin>>N;
	for(int i=0;i<N;i++) {
		int x;
		cin>>x;
		Matrix mat = {
			0,		-inf,	-inf,
			x,		x,		-inf,
			x,		x,		0
		};
		a[i] = mat;
	}
	SegmentTree<Matrix> tree(N, a);
	int Q;
	cin>>Q;
	while(Q--) {
		int q, l, r;
		cin>>q>>l>>r;
		if(q == 1) {
			l--, r--;
			Matrix ans = tree.query(l, r);
			cout<<ans.a[2][0]<<endl;
		}
		else {
			l--;
			Matrix mat = {
				0,		-inf,	-inf,
				r,		r,		-inf,
				r,		r,		0
			};
			tree.update(l, mat, cover);
		}
	}
	return 0;
}
