#include <iostream>

using namespace std;

class Example {
private:
	int x;
public:
	void SetX(int i);
	int GetX();
};

int main() 
{
	Example example;

	example.SetX(5);

	cout << example.GetX() << endl;
}

void Example::SetX(int i) 
{
	x = i;
}

int Example::GetX() 
{
	return x;
}