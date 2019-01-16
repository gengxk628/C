//////////////////////
//实现了定长高精度数类HPN（high-precision-number）
//a[]={3,7890,3456,12}=>存储1234567890
//b[]={3,1234,5678,9000}=>储存0.123456789
//该数为1234567890.123456789
//////////////////////

#include<iostream>
using namespace std;

class HPN{  //高精度类
private:
	int a[129];  //整数部分,其中第0位表示整数占有的的长度
	int b[129];  //小数部分,其中第0位表示整数占有的的长度
	bool pm;     //plus-minus,true代表整数，false代表负数

private:
	int compareAbs(const HPN& x);
	//比较自己与x绝对值的大小
	static int compareAbs(const HPN& x, const HPN& y);
	//比较两个高精度数绝对值的大小，x>y返回1，x=y返回-1，x<y返回-1

	void addAbs(HPN x);
	//将自身加上x，不用按引用传值是为了避免x.addAbs(x)产生错误
	void addAbs(HPN x, HPN y);//将两个正的高精度数相加，结果储存在自己中

	void subAbs(HPN x);
	//将自身减去x（x小于自身时不做减法）
	void subAbs(HPN x, HPN y);
	//将两个高精度数绝对值相减，结果储存在自己中（x>=y）

	void mulAbs(HPN x, HPN y);
	//将两个高精度数绝对值相乘，结果储存在自己中

public:
	HPN();

	static void clear(HPN& x);
	void clear();
	friend void clear(HPN& x);
	//将数组清零

	static void print(const HPN& x);
	void print();
	friend void print(const HPN& x);
	//将高精度数打印到命令行

	void setNumber(int x);
	//用整形数给高精度数赋值
	void setNumber(int x, int y);
	//x代表整数部分，y代表小数部分
	void setNumber(char x[], char y[]);
	//x[]代表整数部分，y[]代表小数部分，忽略第一个非数字字符以后的内容
	void setNumber(char x[], char y[], bool pm);
	//pm为true代表正数，为false代表负数
	void setNumber(char x[]);
	//将字符串转化为高精度数，x[]="-12345.1234a12"，忽略第一个非数字字符以后的内容(+-除外)

	int getString(char s[]);
	//将该高精度数转化为字符串储存到s[]中，返回s[]需要的长度（有可能发生越界）
	bool getString(char s[], int length);
	//将该高精度数转化为字符串储存到s[]中,length为该数组的最大长度。
	//返回true代表完全转化；false代表长度不足，部分转化，截取前面部分

	void copy(HPN& x);
	//将高精度数x“赋值”给自己
	static void copy(HPN& x, HPN& y);
	//将高精度数y“赋值”给高精度数x 
	friend void copy(HPN& x, HPN& y);
	//将高精度数y“赋值”给高精度数x 

	int compare(const HPN& x);
	//比较自己与x的大小
	static int compare(const HPN& x, const HPN& y);
	//比较两个高精度数的大小，x>y返回1，x=y返回-1，x<y返回-1
	friend int compare(const HPN& x, const HPN& y);

	void add(HPN x);
	//将自身加上x
	void add(HPN x, HPN y);
	//将两个高精度数相加，结果储存在自己中

	void sub(HPN x);
	//将自身减去x
	void sub(HPN x, HPN y);
	//将两个高精度数相减，结果储存在自己中

	void mul(HPN x);
	//将自身乘以x
	//void mul(HPN x, HPN y);
	//将两个高精度数相乘，结果储存在自己中
};

void main(){
	HPN x,y;
	x.setNumber("-11111.111");
	y.setNumber("-11.111111");
	x.mul(y);
	printf("%.12f\n", 11111.111*11.111111);
	x.print();
}

void HPN::addAbs(HPN x){
	int length=x.b[0]>b[0]?x.b[0]:b[0];  //先加小数
	int index;  //计算小数位数
	for(int i=length;i>=1;i--){
		b[i]+=x.b[i];
		if(b[i]>=10000 && i>=2){  //i==1时要向整数位进位
			b[i-1]+=b[i]/10000;
			b[i]%=10000;
		}
	}
	if(b[1]>=10000){  //i==1时要向整数位进位
		a[1]+=b[1]/10000;
		b[1]%=10000;
	}
	for(index=length;index>=1 && b[index]==0;index--);  //计算小数的数位，从后往前
	b[0]=index;

	//再处理整数位
	length=x.a[0]>a[0]?x.a[0]:a[0];
	for(int i=1;i<=length;i++){
		a[i]+=x.a[i];
		if(a[i]>=10000){
			a[i+1]+=a[i]/10000;
			a[i]%=10000;
		}
	}
	if(a[length+1]>0)  //计算整数的位数
		a[0]=length+1;
	else
		a[0]=length;
}
void HPN::addAbs(HPN x,HPN y){
	clear();  //先将自身初始化
	int length=x.b[0]>y.b[0]?x.b[0]:y.b[0];  //先加小数
	int index;  //计算小数位数
	for(int i=length;i>=1;i--){
		b[i]+=x.b[i]+y.b[i];
		if(b[i]>=10000 && i>=2){  //i==1时要向整数位进位
			b[i-1]+=b[i]/10000;
			b[i]%=10000;
		}
	}
	if(b[1]>=10000){  //i==1时要向整数位进位
		a[1]+=b[1]/10000;
		b[1]%=10000;
	}
	for(index=length;index>=1 && b[index]==0;index--);  //计算小数的数位，从后往前
	b[0]=index;

	//再处理整数位
	length=x.a[0]>y.a[0]?x.a[0]:y.a[0];
	for(int i=1;i<=length;i++){
		a[i]+=x.a[i]+y.a[i];
		if(a[i]>=10000){
			a[i+1]+=a[i]/10000;
			a[i]%=10000;
		}
	}
	if(a[length+1]>0)  //计算整数的位数
		a[0]=length+1;
	else
		a[0]=length;
}

void HPN::subAbs(HPN x){
	if(compareAbs(x)<0) return;  //自身绝对值小于x时不做处理

	int length=b[0]>x.b[0]?b[0]:x.b[0];
	int index;  //计算小数位数
	for(int i=length;i>=2;i--){  //先处理小数位
		b[i]-=x.b[i];
		if(b[i]<0){  //借位
			b[i-1]-=1;
			b[i]+=10000;
		}
	}
	b[1]-=x.b[1];
	if(b[1]<0){  //向整数位借位
		a[1]-=1;
		b[1]+=10000;
	}
	for(index=length;index>=1&&b[index]==0;index--);  //计算小数的数位，从后往前
	b[0]=index;

	//再处理整数位
	length=x.a[0]>a[0]?x.a[0]:a[0];
	for(int i=1;i<=length;i++){
		a[i]-=x.a[i];
		if(a[i]<0){
			a[i+1]-=1;
			a[i]+=10000;
		}
	}
	for(index=length;index>=1&&a[index]==0;index--);  //计算整数的数位，从前往后
	a[0]=index;
}
void HPN::subAbs(HPN x, HPN y){
	if(compareAbs(x,y)<0) return;  //x绝对值小于y时不做处理
	clear();  //先将自身初始化

	int length=x.b[0]>y.b[0]?x.b[0]:y.b[0];
	int index;  //计算小数位数
	for(int i=length;i>=2;i--){  //先处理小数位
		b[i]+=x.b[i]-y.b[i];
		if(b[i]<0){  //借位
			b[i-1]-=1;
			b[i]+=10000;
		}
	}
	b[1]+=x.b[1]-y.b[1];
	if(b[1]<0){  //向整数位借位
		a[1]-=1;
		b[1]+=10000;
	}
	for(index=length;index>=1&&b[index]==0;index--);  //计算小数的数位，从后往前
	b[0]=index;

	//再处理整数位
	length=y.a[0]>x.a[0]?y.a[0]:x.a[0];
	for(int i=1;i<=length;i++){
		a[i]+=x.a[i]-y.a[i];
		if(a[i]<0){
			a[i+1]-=1;
			a[i]+=10000;
		}
	}
	for(index=length;index>=1&&a[index]==0;index--);  //计算整数的数位，从前往后
	a[0]=index;
}

void HPN::mulAbs(HPN x, HPN y) {
	clear();

	int index;
	//处理小数*小数部分
	for (int i = x.b[0]; i >= 1; i--) {
		for (int j = y.b[0]; j >= 1; j--) {
			b[i + j] += x.b[i] * y.b[j];
			if (b[i + j] >= 10000) {
				b[i + j - 1] += b[i + j] / 10000;
				b[i + j] %= 10000;
			}
		}
	}
	//处理小数*整数部分
	for (int i = x.b[0]; i >= 1; i--) {
		for (int j = 1; j <= y.a[0]; j++) {
			if (i >= j) {
				b[i + 1 - j] += x.b[i] * y.a[j];
				if (b[i + 1 - j] >= 10000) {
					if (i + 1 - j == 1) {  //要进位到整数
						a[1] += b[1] / 10000;
						b[1] %= 10000;
					}
					else {
						b[i - j] += b[i + 1 - j] / 10000;
						b[i + 1 - j] %= 10000;
					}
				}
			}
			else {
				a[j - i] += x.b[i] * y.a[j];
				if (a[j - i] >= 10000) {
					a[j - i + 1] += a[j - i] / 10000;
					a[j - i] %= 10000;
				}
			}
		}
	}
	//处理整数*小数部分
	for (int i = y.b[0]; i >= 1; i--) {
		for (int j = 1; j <= x.a[0]; j++) {
			if (i >= j) {
				b[i + 1 - j] += y.b[i] * x.a[j];
				if (b[i + 1 - j] >= 10000) {
					if (i + 1 - j == 1) {  //要进位到整数
						a[1] += b[1] / 10000;
						b[1] %= 10000;
					}
					else {
						b[i - j] += b[i + 1 - j] / 10000;
						b[i + 1 - j] %= 10000;
					}
				}
			}
			else {
				a[j - i] += y.b[i] * x.a[j];
				if (a[j - i] >= 10000) {
					a[j - i + 1] += a[j - i] / 10000;
					a[j - i] %= 10000;
				}
			}
		}
	}
	//处理整数*整数部分
	for (int i = 1; i <= x.a[0]; i++) {
		for (int j = 1; j <= y.a[0]; j++) {
			a[i + j - 1] += x.a[i] * y.a[j];
			if (a[i + j - 1] >= 10000) {  //处理进位
				a[i + j] += a[i + j - 1] / 10000;
				a[i + j - 1] %= 10000;
			}
		}
	}
	for (index = x.b[0] + y.b[0]; index >= 1 && b[index] == 0; index--);  //计算小数的数位，从后往前
	b[0] = index;
	for (index = x.a[0] + y.a[0]; index >= 1 && a[index] == 0; index--);  //计算整数的数位，从前往后
	a[0] = index;
}

int HPN::compareAbs(const HPN& x){
	if(a[0]>x.a[0])
		return 1;
	if(a[0]<x.a[0])
		return -1;
	if(a[0]==x.a[0]){  //位数相同
		for(int i=a[0];i>=1;i--){
				if(a[i]>x.a[i])
					return 1;
				if(a[i]<x.a[i])
					return -1;		
		}
		//整数部分大小相同
		int length=b[0]>x.b[0]?b[0]:x.b[0];  //取较大的小数位数
		for(int i=1;i<=length;i++){
			if(b[i]>x.b[i])
				return 1;
			if(b[i]<x.b[i])
				return -1;
		}
		return 0;  //小数部分也相同
	}
}
int HPN::compareAbs(const HPN& x, const HPN& y){
	if(x.a[0]>y.a[0])
		return 1;
	if(x.a[0]<y.a[0])
		return -1;
	if(x.a[0]==y.a[0]){  //位数相同
		for(int i=x.a[0];i>=1;i--){
				if(x.a[i]>y.a[i])
					return 1;
				if(x.a[i]<y.a[i])
					return -1;		
		}
		//整数部分大小相同
		int length=x.b[0]>y.b[0]?x.b[0]:y.b[0];  //取较大的小数位数
		for(int i=1;i<=length;i++){
			if(x.b[i]>y.b[i])
				return 1;
			if(x.b[i]<y.b[i])
				return -1;
		}
		return 0;  //小数部分也相同
	}
}

HPN::HPN(){
	pm=true;
	for(int i=0;i<129;i++){
		a[i]=0;
		b[i]=0;
	}
}

void HPN::clear(HPN& x){
	x.pm=true;
	for(int i=0;i<129;i++){
		x.a[i]=0;
		x.b[i]=0;
	}
}
void HPN::clear(){
	pm=true;
	for(int i=0;i<129;i++){
		a[i]=0;
		b[i]=0;
	}
}
void clear(HPN& x){
	x.pm=true;
	for(int i=0;i<129;i++){
		x.a[i]=0;
		x.b[i]=0;
	}
}

void HPN::print(const HPN& x){
	if(!x.pm) printf("-");

	printf("%d",x.a[x.a[0]]);  //无前导0
	for(int i=x.a[0]-1;i>=1;i--){
		printf("%04d",x.a[i]);  //有前导零，注意储存方向与输出方向
	}

	if(x.b[0]!=0){  //存在小数部分
		printf(".");
		for(int i=1;i<=x.b[0];i++){
			printf("%04d",x.b[i]);
		}
	}
}
void HPN::print(){
	HPN::print(*this);
}
void print(const HPN& x){
	if(!x.pm) printf("-");

	printf("%d",x.a[x.a[0]]);  //无前导0
	for(int i=x.a[0]-1;i>=1;i--){
		printf("%04d",x.a[i]);  //有前导零，注意储存方向与输出方向
	}

	if(x.b[0]!=0){  //存在小数部分
		printf(".");
		for(int i=1;i<=x.b[0];i++){
			printf("%04d",x.b[i]);
		}
	}
}

void HPN::setNumber(int x){
	clear();
	if(x==0) return;

	if(x<0){
		x=-x;
		pm=false;
	}

	if(x<10000){
		a[1]=x;
		a[0]=1;
	}else if(x<100000000){
		a[1]=x%10000;
		a[2]=x/10000;
		a[0]=2;
	}else{
		a[1]=x%10000;
		a[2]=(x%100000000)/10000;
		a[3]=x/100000000;
		a[0]=3;
	}
}
void HPN::setNumber(int x, int y){
	clear();
	setNumber(x);
	if(y==0) return;
	if(y<0) y=-y;  //强制将小数部分转化为正数

	if(y<10){
		b[1]=y*1000;
		b[0]=1;
	}else if(y<100){
		b[1]=y*100;
		b[0]=1;
	}
	else if(y<1000){
		b[1]=y*10;
		b[0]=1;
	}else if(y<10000){
		b[1]=y;
		b[0]=1;
	}else if(y<100000){
		b[2]=y%10*1000;
		b[1]=y/10;
		b[0]=2;
	}else if(y<1000000){
		b[2]=y%100*100;
		b[1]=y/100;
		b[0]=2;
	}else if(y<10000000){
		b[2]=y%1000*10;
		b[1]=y/1000;
		b[0]=2;
	}else if(y<100000000){
		b[2]=y%10000;
		b[1]=y/10000;
		b[0]=2;
	}else if(y<1000000000){
		b[3]=y%10*1000;
		b[2]=(y%100000)/10;
		b[1]=y/100000;
		b[0]=3;
	}else{
		b[3]=y%100*100;
		b[2]=(y%1000000)/100;
		b[1]=y/1000000;
		b[0]=3;
	}
}

void HPN::setNumber(char x[], char y[]){
	int i,num,index;
	clear();
	//处理整数部分
	for(num=0;x[num]>='0' && x[num]<='9';num++);  //num为字符串中数字的位数
	for(i=num-1,index=0;i-3>=0;i=i-4){    //index为对应a[]的位数,i为字符串中字符的位数,从后往前，每4位一处理
		a[++index]=(x[i]-'0')+(x[i-1]-'0')*10+(x[i-2]-'0')*100+(x[i-3]-'0')*1000;
	}

	if(i==-1){  //对x[]中剩余的不足4位的数字进行处理
		a[0]=index;
	}else if(i==0){
		a[0]=index+1;
		a[index+1]=x[0]-'0';
	}else if(i==1){
		a[0]=index+1;
		a[index+1]=(x[0]-'0')*10+(x[1]-'0');
	}else if(i==2){
		a[0]=index+1;
		a[index+1]=(x[0]-'0')*100+(x[1]-'0')*10+(x[2]-'0');
	}

	//处理小数部分
	for(num=0;y[num]>='0' && y[num]<='9';num++);  //num为字符串中数字的位数
	for(i=0,index=0;i+3<=num-1;i=i+4){  //index为对应b[]的位数,i为字符串中字符的位数,从前往后，每4位一处理
		b[++index]=(y[i]-'0')*1000+(y[i+1]-'0')*100+(y[i+2]-'0')*10+(y[i+3]-'0');
	}

	if(i==num){  //对y[]中剩余的不足4位的数字进行处理
		b[0]=index;
	}else if(i==num-1){
		b[0]=index+1;
		b[index+1]=(y[num-1]-'0')*1000;
	}else if(i==num-2){
		b[0]=index+1;
		b[index+1]=(y[num-2]-'0')*1000+(y[num-1]-'0')*100;
	}else if(i==num-3){
		b[0]=index+1;
		b[index+1]=(y[num-3]-'0')*1000+(y[num-2]-'0')*100+(y[num-1]-'0')*10;
	}
}
void HPN::setNumber(char x[], char y[], bool pm){
	setNumber(x,y);
	if(a[0]!=0 || b[0]!=0)  //防止-0出现
		this->pm=pm;
}
void HPN::setNumber(char x[]){
	char sa[513],sb[513];
	int i,j;
	clear();
	if(x[0]>='0' && x[0]<='9'){
		for(i=0;x[i]>='0' && x[i]<='9';i++){  //将是数字的部分赋值
			sa[i]=x[i];
		}
		sa[i]='\0';  //整数部分结束
		if(x[i]=='.'){  //开始赋值小数部分
			for(j=++i;x[j]>='0' && x[j]<='9';j++){
				sb[j-i]=x[j];
			}
			sb[j]='\0';  //小数部分结束
		}

		setNumber(sa,sb);
	}else if(x[0]=='+'){  //第一位为+
		for(i=1;x[i]>='0' && x[i]<='9';i++){  //将是数字的部分赋值
			sa[i-1]=x[i];
		}
		sa[i-1]='\0';  //整数部分结束
		if(x[i]=='.'){  //开始赋值小数部分
			for(j=++i;x[j]>='0' && x[j]<='9';j++){
				sb[j-i]=x[j];
			}
			sb[j]='\0';  //小数部分结束
		}
		setNumber(sa,sb,true);
	}else if(x[0]=='-'){  //第一位为-
		for(i=1;x[i]>='0' && x[i]<='9';i++){  //将是数字的部分赋值
			sa[i-1]=x[i];
		}
		sa[i-1]='\0';  //整数部分结束
		if(x[i]=='.'){  //开始赋值小数部分
			for(j=++i;x[j]>='0' && x[j]<='9';j++){
				sb[j-i]=x[j];
			}
			sb[j]='\0';  //小数部分结束
		}
		setNumber(sa,sb,false);
	}
}

int HPN::getString(char s[]){
	int index=0;  //当前s[]中应被写入的位置
	if(!pm) s[index++]='-';
	if(a[a[0]]<10){  //单独处理最高位（无需前导零）
		s[index++]=a[a[0]]+'0';
	}else if(a[a[0]]<100){
		s[index++]=a[a[0]]/10+'0';
		s[index++]=a[a[0]]%10+'0';
	}else if(a[a[0]]<1000){
		s[index++]=a[a[0]]/100+'0';
		s[index++]=a[a[0]]%100/10+'0';
		s[index++]=a[a[0]]%10+'0';
	}else{
		s[index++]=a[a[0]]/1000+'0';
		s[index++]=a[a[0]]%1000/100+'0';
		s[index++]=a[a[0]]%100/10+'0';
		s[index++]=a[a[0]]%10+'0';
	}

	for(int i=a[0]-1;i>=1;i--){  //处理整数位
		s[index++]=a[i]/1000+'0';
		s[index++]=a[i]%1000/100+'0';
		s[index++]=a[i]%100/10+'0';
		s[index++]=a[i]%10+'0';
	}

	if(b[0]!=0){  //存在小数
		s[index++]='.';
		for(int i=1;i<=b[0];i++){  //处理小数位
			s[index++]=b[i]/1000+'0';
			s[index++]=b[i]%1000/100+'0';
			s[index++]=b[i]%100/10+'0';
			s[index++]=b[i]%10+'0';
		}
	}

	s[index++]='\0';
	return index;
}
bool HPN::getString(char s[], int length){

	char adequate[1026];
	int len=getString(adequate);  //需要的长度
	int i;
	if(length>=len){
		for(i=0;s[i]!='0';i++)
			s[i]=adequate[i];  //复制数组
		s[++i]='\0';
		return true;
	}
	else{
		for(i=0;i<length-1;i++)
			s[i]=adequate[i];  //复制部分数组
		s[++i]='\0';
		return false;
	}
}

void HPN::copy(HPN& x, HPN& y){
	if(&x != &y){  //防止copy(x,x)
		x.clear();
		x.pm=y.pm;
		for(int i=0;i<=y.a[0];i++){
			x.a[i]=y.a[i];
		}
		for(int i=0;i<=y.b[0];i++){
			x.b[i]=y.b[i];
		}
	}
}
void HPN::copy(HPN& x){
	HPN::copy(*this,x);
}
void copy(HPN& x, HPN& y){
	HPN::copy(x,y);
}

int HPN::compare(const HPN& x, const HPN& y){
	if(x.pm==true&&y.pm==false)
		return 1;
	if(x.pm==false&&y.pm==true)
		return -1;
	if(x.pm==true&&y.pm==true)  //均为正数
		return HPN::compareAbs(x,y);
	if(x.pm==false&&y.pm==false)  //均为负数
		return -HPN::compareAbs(x,y);
}
int HPN::compare(const HPN& x){
	return HPN::compare(*this,x);
}
int compare(const HPN& x, const HPN& y){
	return HPN::compare(x,y);
}

void HPN::add(HPN x){
	if(x.pm && pm){//都是正数
		addAbs(x);
	}else if(!x.pm && !pm){  //都是负数
		addAbs(x);
	}else if(pm && !x.pm){  //自己正x负
		if(HPN::compareAbs(x)>=0){  //自己的绝对值大
			subAbs(x);
		}else{
			subAbs(x,*this);
			pm=false;
		}
	}else if(!pm && x.pm){  //自己负x正
		if(HPN::compareAbs(x)>=0){  //自己的绝对值大
			subAbs(x);
			pm=false;
		}else{
			subAbs(x,*this);
		}
	}
}
void HPN::add(HPN x, HPN y){
	clear();  //初始化
	x.add(y);
	copy(x);
}

void HPN::sub(HPN x){
	x.pm=!x.pm;
	add(x);
}
void HPN::sub(HPN x, HPN y){
	y.pm=!y.pm;
	add(x,y);
}

void HPN::mul(HPN x) {
	bool sign=pm;  //临时存储符号
	mulAbs(*this, x);
	if (x.pm && sign || !x.pm && !sign) {  //同正同负
		pm = true;
	}
	else {
		pm = false;
	}
}
