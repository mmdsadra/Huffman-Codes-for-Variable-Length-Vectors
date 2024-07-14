#include <iostream>
#include <string>
//#include <iomanip>
#include "Eigen/Dense"

using namespace std;

//generating blocks 
string blockgen(int n) {
	if (n == 0) {
		string tmp = {};
		return tmp;
	}
	else {
		// blocks from n-2 and n-1 plus two and one new lifs 
		string tmp = {};
		tmp = blockgen(n - 2);
	}
}

//Huddman Tree nodes:
struct Node {
	string tag;
	double val;
	Node* left;
	Node* right;
	string code;
	Node() {
		tag = "";
		val = -9999999999999;
		left = right = NULL;
		code = "";
	}
	/*Node(string t, double v) {
		val = v;
		tag = t;
		left = right = NULL;
		code = "";
	}*/
};

//Making Huffman tree code:
void codeHTree(Node* h, double &t, double &b) {
	if (h != NULL) {
		
		if (h->tag != "No block")
		{
			t += h->val * h->code.length();
			b += h->val * h->tag.length();
			cout << "tag:" << h->tag << " val:" << "   " << h->val << " code:" << h->code << endl;
		}
		if(h->left != NULL)
		{
			h->left->code = h->code + '1';
			codeHTree(h->left, t, b);
		}
		if(h->right != NULL)
		{
			h->right->code = h->code + '0';
			codeHTree(h->right, t, b);
		}
	}
}

//helping function for sort:
bool compare(const Node& lhs, const Node& rhs)
{
	return lhs.val < rhs.val;
}

//Making Huffman tree:
double huffmanTree(double a[], string blocks[], int n) {
	Node hn[25];
	int cnt = n - 1;
	for (int i = 0; i < n; i++) {
		hn[i].tag = blocks[i];
		hn[i].val = a[i];
	}
	sort(hn, hn + cnt, compare);
	while (cnt > 0)
	{
		Node* tmp = new Node ();
		tmp->left = new Node (hn[0]);
		//tmp->left->tag.append("0");

		tmp->right = new Node(hn[1]);
		//tmp->right->tag.append("1");

		tmp->val = tmp->left->val + tmp->right->val;
		tmp->tag = "No block";
		hn[0] = *tmp;
		swap(hn[1], hn[cnt]);
		sort(hn, hn + cnt, compare);
		cnt--;
	}
	double top = 0, bo = 0;
	codeHTree(hn, top, bo);
	//cout << top << "   " << bo;
	return (top / bo);
}

//computing XYZ:
double countXYZ(double a[3][3], string blocks[]) {
	int k = 0;
	//augmented matrix:
	double A[3][4] = {};
	for (int i = 0; i < 3; i++) {
		A[i][i] = -1;
	}
	//for all blocks;
	while (blocks[k] != "")
	{
		//for all of P(a(i)|ß):
		for (int row = 0; row < 3; row++)
		{

			double prod = 1;
			char prev = '0' + row;
			//for every charachter in blocks
			for (int i = blocks[k].size() - 1; i >= 0; i--)
			{
				prod *= a[blocks[k][i] - '0'][prev - '0'];
				prev = blocks[k][i];
			}
			A[row][blocks[k][0] - '0'] += prod;
		}
		k++;
	}
	//changing side of equation:
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
            A[i][j] = -A[i][j];
			//cout << A[i][j] << "  ";
		}
	}
	//solving equation system:(using eigen library)
	Eigen::MatrixXd A1(4,3);
	A1 << A[0][0], A[0][1], A[0][2],
		A[1][0], A[1][1], A[1][2],
		A[2][0], A[2][1], A[2][2],
		1, 1, 1;
	Eigen::VectorXd B1(4);
	B1 << 0, 0, 0, 1;
	Eigen::VectorXd ans(4);
	ans = A1.colPivHouseholderQr().solve(B1);
	
	//puting P(a(i)|ß)s back in the chain:
	k = 0;
	double answerBlocks[25] = {};
	while (blocks[k] != "") {

		double prod = 1;
		char prev = blocks[k][0];
		if (prev == '0')
			prod *= ans.x();
		else if (prev == '1')
			prod *= ans.y();
		else
			prod *= ans.z();
		
		for (int i = 1; i < blocks[k].size(); i++)
		{
			prod *= a[prev - '0'][blocks[k][i] - '0'];
		}
		answerBlocks[k] = prod;
		k++;
	}
	cout << "X:" << ans.x() << endl;
	cout << "Y:" << ans.y() << endl;
	cout << "Z:" << ans.z() << endl;
	double l = huffmanTree(answerBlocks, blocks, k);
	return l;
}

int main() {
	//getprobabilities()
	
	//blocks have to be generated 
	string blocks[25] = { "000", "001", "002", "01", "02", "1", "2" };//these are default
	/*cout << "enter number of entities:";
	int n;
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> blocks[i];
	}*/
	//probabilities of string:(have to take from user)
	double a[3][3] = { {0.9, 0.05, 0.05}, {0.15, 0.8, 0.05}, {0.25, 0.15, 0.6} };//these are default
	/*for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << "enter P(" << i << "|" << j << "):";
			cin >> a[i][j];
		}
	}*/
	cout << "answer of this block is :" << countXYZ(a, blocks) << endl;
}