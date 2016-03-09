
class Bit{

	int k;//位数
	std::vector<unsigned char> data;//数据

public:
	Bit();
	Bit(int k);//设置位数
		
	Bit(const Bit& a) :k(a.k), data(a.data){};
	Bit& operator=(const Bit &);
	
	void setK(int n);
	
	int getK(){
		return k;
	}
	void setBit(int n,bool);//设置bit位，从0开始
	bool getBit(int n);//获取bit位，从0开始
	void reSet();//所有数据重置为0
	void print();
};


Bit::Bit()
{
	int k = 0;
	data.clear();
	data.resize(0);
}

Bit::Bit(int k){//设置位数
	assert(k >= 0);
	this->k = k;
	int len = k / (sizeof(unsigned char)* 8);
	if ((k % (sizeof(unsigned char)* 8)) == 0)
	{
		data.clear();
		data.resize(len, 0);
	}
	else
	{
		data.clear();
		data.resize(len + 1, 0);	
	}
}


Bit& Bit::operator=(const Bit &a)
{
	k = a.k;
	data = a.data;
	return *this;
}

void Bit::setK(int k)
{
	assert(k >= 0);
	this->k = k;
	int len = k / (sizeof(unsigned char)* 8);
	if ((k % (sizeof(unsigned char)* 8)) == 0)
	{
		data.clear();
		data.resize(len, 0);
		
	}
	else
	{
		data.clear();
		data.resize(len + 1, 0);
	}
}

void Bit::setBit(int n,bool sign)
{
	assert(n < this->k&&n >= 0);
	int len = n / (sizeof(unsigned char)* 8);
	int k = n % (sizeof(unsigned char)* 8);
	if (true == sign)
	{
		unsigned char tmp = 1 << k;
		data[len] |= tmp;
	}
	else{
		unsigned char tmp = 1 << k;
		tmp= ~tmp;
		data[len] &= tmp;
	}
}

bool Bit::getBit(const int n)
{
	assert(n < this->k&&n >= 0);
	int len = n / (sizeof(unsigned char)* 8);
	int k = n % (sizeof(unsigned char)* 8);
	unsigned char tmp = 1 << k;
	tmp=data[len]&tmp;
	if (tmp > 0)
		return true;
	else return false;
}
void Bit::reSet()
{
	int len = k / (sizeof(unsigned char)* 8);
	if ((k % (sizeof(unsigned char)* 8)) == 0)
	{
		for (int i = 0; i < len; i++)
		{
			data[i] = 0;
		}
		

	}
	else
	{
		for (int i = 0; i <= len; i++)
		{
			data[i]=0;
		}
	}
}
void Bit::print()
{
	for (int i = 0; i < k; i++)
	{
		std::cout << getBit(i);
	}
	std::cout << std::endl;
}