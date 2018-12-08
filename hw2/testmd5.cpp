/**
 * 中山大学数据科学与计算机学院  16级软件工程电子政务方向 
 * author：唐育涛 16340209 
 * md5码的设计实现 
 */

#include <iostream>
#include "md5.cpp"
using namespace std;
int main() {
	
	cout<<"这是自动测试："<<endl; 
	printMD5("");
	printMD5("a");
	printMD5("abc");
	printMD5("abc abc abc abc abc abc abc abc abc");
	
	string test_str;
	string test_res;
	while(1){
		cout<<"请输入你要加密的信息："<<endl;
		cin>>test_str;
		cout<<"你输入的信息长度为： "<<test_str.length()<<endl;
		test_res = MD5(test_str).toString() ;
		cout<<"加密后的信息长度为： "<<test_res.length()<<endl;
		cout<<"加密后信息为： "<<test_res<<endl;
	}
	return 0;
}
