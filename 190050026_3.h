#include<bits/stdc++.h>
using namespace std;

class quad_tree{
public:
	int n;
	int value;
	long long int zeros;
	quad_tree* tl = NULL;
	quad_tree* bl = NULL;
	quad_tree* tr = NULL;
	quad_tree* br = NULL;

	quad_tree(int n){
		this->n = n;
		this->value = 0;
		long long int x=1;
		this->zeros = (x<<(2*n));
	}
	~quad_tree(){
		if(value==-1){
			delete tl; delete bl; delete tr; delete br;
		}
	}
	quad_tree(quad_tree const &Q){
		this->n = Q.n;
		this->value = Q.value;
		this->zeros = Q.zeros; 
		if(value==-1) {
			tl = new quad_tree(*Q.tl);
			bl = new quad_tree(*Q.bl);
			tr = new quad_tree(*Q.tr);
			br = new quad_tree(*Q.br);
		}
	}
	void set(int x1, int y1, int x2, int y2, int b){
		if(n==0) {value=b; if(b==1) zeros=0; if(b==0) zeros=1; return;}
		if(value==b) return;
		long long int x=1;
		int power_n = (1<<(n-1));
		if((value!=-1)&&(x1==0)&&(x2==2*power_n-1)&&(y1==0)&&(y2==2*power_n-1)) {value=b; if(b==1) zeros=0; if(b==0) zeros=(x<<(2*n)); return;}
		if((value==-1)&&(x1==0)&&(x2==2*power_n-1)&&(y1==0)&&(y2==2*power_n-1)) {value=b; if(b==1) zeros=0; if(b==0) zeros=(x<<(2*n)); delete tl; delete bl; delete tr; delete br; return;}
		if(value!=-1) {
			tl = new quad_tree(n-1); tl->value = value; if(value==1) tl->zeros=0;
			bl = new quad_tree(n-1); bl->value = value; if(value==1) bl->zeros=0;
			tr = new quad_tree(n-1); tr->value = value;	if(value==1) tr->zeros=0;
			br = new quad_tree(n-1); br->value = value; if(value==1) br->zeros=0;
			value=-1;
		}
		if((x1<power_n)&&(x2<power_n)&&(y1<power_n)&&(y2<power_n)) {tl->set(x1,y1,x2,y2,b);}
		if((x1<power_n)&&(x2<power_n)&&(y1<power_n)&&(y2>=power_n)) {tl->set(x1,y1,x2,power_n-1,b); tr->set(x1,0,x2,y2-power_n,b);}
		if((x1<power_n)&&(x2<power_n)&&(y1>=power_n)&&(y2>=power_n)) {tr->set(x1,y1-power_n,x2,y2-power_n,b);}
		if((x1<power_n)&&(x2>=power_n)&&(y1<power_n)&&(y2<power_n)) {tl->set(x1,y1,power_n-1,y2,b); bl->set(0,y1,x2-power_n,y2,b);}
		if((x1<power_n)&&(x2>=power_n)&&(y1<power_n)&&(y2>=power_n)) {tl->set(x1,y1,power_n-1,power_n-1,b); bl->set(0,y1,x2-power_n,power_n-1,b); tr->set(x1,0,power_n-1,y2-power_n,b); br->set(0,0,x2-power_n,y2-power_n,b);}
		if((x1<power_n)&&(x2>=power_n)&&(y1>=power_n)&&(y2>=power_n)) {tr->set(x1,y1-power_n,power_n-1,y2-power_n,b); br->set(0,y1-power_n,x2-power_n,y2-power_n,b);}
		if((x1>=power_n)&&(x2>=power_n)&&(y1<power_n)&&(y2<power_n)) {bl->set(x1-power_n,y1,x2-power_n,y2,b);}
		if((x1>=power_n)&&(x2>=power_n)&&(y1<power_n)&&(y2>=power_n)) {bl->set(x1-power_n,y1,x2-power_n,power_n-1,b); br->set(x1-power_n,0,x2-power_n,y2-power_n,b);}
		if((x1>=power_n)&&(x2>=power_n)&&(y1>=power_n)&&(y2>=power_n)) {br->set(x1-power_n,y1-power_n,x2-power_n,y2-power_n,b);}
		zeros = tl->zeros + bl->zeros + tr->zeros + br->zeros;
		return;
	}
	int get(int x1, int y1) const{
		if(value!=-1) return value;
		long long int x=1;
		int power_n = (1<<(n-1));
		if((x1<power_n)&&(y1<power_n)) return tl->get(x1,y1);
		if((x1>=power_n)&&(y1<power_n)) return bl->get(x1-power_n,y1);
		if((x1<power_n)&&(y1>=power_n)) return tr->get(x1,y1-power_n);
		if((x1>=power_n)&&(y1>=power_n)) return br->get(x1-power_n,y1-power_n);
	}
	int size() const{
		return n;
	}
	void overlap(quad_tree const &Q){
		long long int x=1;
		if((value!=-1)&&(Q.value!=-1)) {value=(value|Q.value); if(value==1) zeros=0; if(value==0) zeros=(x<<(2*n)); return;}
		if((value==-1)&&(Q.value==1)) {value=1; zeros=0; delete tl; delete bl; delete tr; delete br; return;}
		if((value==0)&&(Q.value==-1)) {
			value=-1;
			zeros=Q.zeros;
			tl = new quad_tree(*Q.tl);
			bl = new quad_tree(*Q.bl);
			tr = new quad_tree(*Q.tr);
			br = new quad_tree(*Q.br);
			return;
		}
		if((value==-1)&&(Q.value==-1)) {
			tl->overlap(*Q.tl); bl->overlap(*Q.bl); tr->overlap(*Q.tr); br->overlap(*Q.br);
			zeros = tl->zeros + bl->zeros + tr->zeros + br->zeros;
			if((tl->value==0)&&(bl->value==0)&&(tr->value==0)&&(br->value==0)) {value=0; delete tl; delete bl; delete tr; delete br; return;}
			if((tl->value==1)&&(bl->value==1)&&(tr->value==1)&&(br->value==1)) {value=1; delete tl; delete bl; delete tr; delete br; return;}
		}
	}
	void intersect(quad_tree const &Q){
		long long int x=1;
		if((value!=-1)&&(Q.value!=-1)) {value=(value&Q.value); if(value==1) zeros=0; if(value==0) zeros=(x<<(2*n)); return;}
		if((value==-1)&&(Q.value==0)) {value=0; zeros=(x<<(2*n)); delete tl; delete bl; delete tr; delete br; return;}
		if((value==1)&&(Q.value==-1)) {
			value=-1;
			zeros=Q.zeros;
			tl = new quad_tree(*Q.tl);
			bl = new quad_tree(*Q.bl);
			tr = new quad_tree(*Q.tr);
			br = new quad_tree(*Q.br);
		}
		if((value==-1)&&(Q.value==-1)) {
			tl->intersect(*Q.tl); bl->intersect(*Q.bl); tr->intersect(*Q.tr); br->intersect(*Q.br);
			if((tl->value==0)&&(bl->value==0)&&(tr->value==0)&&(br->value==0)) {value=0; zeros=(x<<(2*n)); delete tl; delete bl; delete tr; delete br; return;}
			if((tl->value==1)&&(bl->value==1)&&(tr->value==1)&&(br->value==1)) {value=1; zeros=0; delete tl; delete bl; delete tr; delete br; return;}
			zeros = tl->zeros + bl->zeros + tr->zeros + br->zeros;
		}
	}
	void complement(){
		long long int x=1;
		if(value==0) {value=1; zeros=0; return;}
		if(value==1) {value=0; zeros=(x<<(2*n)); return;}
		tl->complement(); bl->complement(); tr->complement(); br->complement();
		zeros = tl->zeros + bl->zeros + tr->zeros + br->zeros;
	}
	void resize(int m){
		if(m>=n) {
			long long int x=1;
			n=m;
			if(value==0) zeros=(x<<(2*n));
			if(value==-1){tl->resize(m-1); bl->resize(m-1); tr->resize(m-1); br->resize(m-1); zeros = tl->zeros + bl->zeros + tr->zeros + br->zeros;}
			return;
		}
		if(m==0){
			// Get max and value=max, make n=0 and delete children if they exist
			long long int x = 1;
			long long int y = (x<<(2*n-1));
			if(value==-1) {delete tl; delete bl; delete tr; delete br;}
			if(zeros>y) {zeros=1; value=0;}
			else {zeros=0; value=1;}
			n=0;
			return;
		}
		// resize children to m-1 if the exist or just change n to m if 0 children
		n=m;
		if(value==-1) {
			tl->resize(m-1); bl->resize(m-1); tr->resize(m-1); br->resize(m-1); zeros = tl->zeros + bl->zeros + tr->zeros + br->zeros;
			if((tl->value==0)&&(bl->value==0)&&(tr->value==0)&&(br->value==0)) {value=0; delete tl; delete bl; delete tr; delete br;}
			if((tl->value==1)&&(bl->value==1)&&(tr->value==1)&&(br->value==1)) {value=1; delete tl; delete bl; delete tr; delete br;}
		}
		if(value==0) {long long int x=1; zeros=(x<<(2*n));}
		return;
	}
	quad_tree* extract_tree(int x1, int y1, int m){
		// If total box req is in 1 quadrant then write
		long long int x=1;
		if(m==0) {quad_tree* Q = new quad_tree(0); Q->value=this->get(x1,y1); if(Q->value==1) Q->zeros=0; return Q;}
		int check = (x<<(n-1))-(x<<m);
		int power_n = (x<<(n-1));
		int power_m = (x<<(m-1));
		if(value==0) {quad_tree* Q = new quad_tree(m); return Q;}
		if(value==1) {quad_tree* Q = new quad_tree(m); Q->value=1; Q->zeros=0; return Q;}
		if((x1<=check)&&(y1<=check)) {return tl->extract_tree(x1,y1,m);}
		if((x1<=check)&&(y1>=power_n)) { return tr->extract_tree(x1,y1-power_n,m);}
		if((x1>=power_n)&&(y1<=check)) { return bl->extract_tree(x1-power_n,y1,m);}
		if((x1>=power_n)&&(y1>=power_n)) { return br->extract_tree(x1-power_n,y1-power_n,m);}
		quad_tree* Q = new quad_tree(m);
		Q->value=-1;
		Q->tl = this->extract_tree(x1,y1,m-1);
		Q->bl = this->extract_tree(x1+power_m,y1,m-1);
		Q->tr = this->extract_tree(x1, y1+power_m, m-1);
		Q->br = this->extract_tree(x1+power_m,y1+power_m,m-1);
		Q->zeros = Q->tl->zeros + Q->bl->zeros + Q->tr->zeros + Q->br->zeros;
		if((Q->tl->value==0)&&(Q->bl->value==0)&&(Q->tr->value==0)&&(Q->br->value==0)) {Q->value=0; delete Q->tl; delete Q->bl; delete Q->tr; delete Q->br;}
		if((Q->tl->value==1)&&(Q->bl->value==1)&&(Q->tr->value==1)&&(Q->br->value==1)) {Q->value=1; delete Q->tl; delete Q->bl; delete Q->tr; delete Q->br;}
		return Q;
	}
	void extract(int x1, int y1, int m){
		quad_tree* Q = this->extract_tree(x1,y1,m);
		if(value==-1){
			delete tl; delete bl; delete tr; delete br;
		}
		value = Q->value; zeros = Q->zeros; n = Q->n;
		if(value==-1){
			tl = new quad_tree(*Q->tl);
			bl = new quad_tree(*Q->bl);
			tr = new quad_tree(*Q->tr);
			br = new quad_tree(*Q->br);
		}
		delete Q;
	}
};