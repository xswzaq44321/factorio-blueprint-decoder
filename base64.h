#include <string>
#include <iostream>
#include <map>

namespace base64{
	const std::string base("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
	const std::map<char, int> baseMap{{'A', 0}, {'B', 1}, {'C', 2}, {'D', 3}, {'E', 4}, {'F', 5}, {'G', 6}, {'H', 7}, {'I', 8}, {'J', 9}, {'K', 10}, {'L', 11}, {'M', 12}, {'N', 13}, {'O', 14}, {'P', 15}, {'Q', 16}, {'R', 17}, {'S', 18}, {'T', 19}, {'U', 20}, {'V', 21}, {'W', 22}, {'X', 23}, {'Y', 24}, {'Z', 25}, {'a', 26}, {'b', 27}, {'c', 28}, {'d', 29}, {'e', 30}, {'f', 31}, {'g', 32}, {'h', 33}, {'i', 34}, {'j', 35}, {'k', 36}, {'l', 37}, {'m', 38}, {'n', 39}, {'o', 40}, {'p', 41}, {'q', 42}, {'r', 43}, {'s', 44}, {'t', 45}, {'u', 46}, {'v', 47}, {'w', 48}, {'x', 49}, {'y', 50}, {'z', 51}, {'0', 52}, {'1', 53}, {'2', 54}, {'3', 55}, {'4', 56}, {'5', 57}, {'6', 58}, {'7', 59}, {'8', 60}, {'9', 61}, {'+', 62}, {'/', 63}};

	std::string encode(std::string strin){
		std::string strout;
		for(int i = 0; i < strin.length(); i += 3){
			int temp = 0, lack = 0;
			for(int j = 0; j < 3; ++j){
				temp <<= 8;
				if(i + j >= strin.length()){
					++lack;
				}else{
					temp |= strin[i + j];
				}
			}
			for(int j = 3; j >= 0; --j){
				strout += base[(temp >> j*6) % 64];
			}
			if(lack){
				strout.replace(strout.length() - lack, lack, lack, '=');
			}
		}
		return strout;
	}

	std::string encode(const unsigned char* strin, const int len){
		std::string strout;
		for(int i = 0; i < len; i += 3){
			int temp = 0, lack = 0;
			for(int j = 0; j < 3; ++j){
				temp <<= 8;
				if(i + j >= len){
					++lack;
				}else{
					temp |= strin[i + j];
				}
			}
			for(int j = 3; j >= 0; --j){
				strout += base[(temp >> j*6) % 64];
			}
			if(lack){
				strout.replace(strout.length() - lack, lack, lack, '=');
			}
		}
		return strout;
	}

	std::string decode(std::string strin){
		std::string strout;
		for(int i = 0; i < strin.length(); i += 4){
			int temp = 0, lack = 0;
			for(int j = 0; j < 4; ++j){
				temp <<= 6;
				if(strin[i + j] == '='){
					++lack;
				}else{
					try{
						temp |= baseMap.at(strin[i + j]);
					}catch(const std::out_of_range& oor){
						std::cerr << "Out of Range error: " << oor.what() << "\n";
						std::cerr << "Did you try to decode a non-base64 code?\n";
						exit(1);
					}
				}
			}
			for(int j = 2; j >= 0; --j){
				strout += (char)((temp >> (j * 8)) % 256);
			}
			if(lack){
				strout.erase(strout.length() - lack, lack);
			}
		}
		return strout;
	}

};

