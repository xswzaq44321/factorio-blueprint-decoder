#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include "base64.h"
#include "zlib.h"
using namespace std;

int main(int argc, char *argv[]){
	string buffer;
	string ifilename, ofilename;
	try{
		int pos = 1;
		while(pos < argc){
			if(argv[pos][0] == '-'){
				switch(argv[pos][1]){
					case 'f':
						if(pos + 1 >= argc || argv[pos + 1][0] == '-')
							throw "missing argument after [-f]";
						ifilename.assign(argv[pos + 1]);
						pos += 2;
					break;
					case 'o':
						if(pos + 1 >= argc || argv[pos + 1][0] == '-')
							throw "missing argument after [-o]";
						ofilename.assign(argv[pos + 1]);
						pos += 2;
					break;
					default:
						throw "syntax error";
					break;
				}
			}else{
				throw "syntax error";
			}
		}
	}catch(const char *errmsg){
		cerr << "Error!\n";
		if(!strcmp(errmsg, "syntax error")){
			cerr << "Usage: " << __FILE__ << "\b\b\b\b" << " [-f][filename] [-o][filename]\n";
		}else{
			cerr << errmsg << endl;
		}
		exit(1);
	}

	if(!ifilename.empty()){
		ifstream is(ifilename, std::ifstream::binary);
		if(is){
			is.seekg(0, is.end);
			int length = is.tellg();
			is.seekg(0, is.beg);
			char *temp = new char[length];

			cout << "Reading " << length << " characters... \n";
			is.read(temp, length);
			buffer.assign(temp);
			// last on is eof character
			buffer.erase(buffer.end() - 1);

			if(is)
				cout << "all character read successfully.\n";
			else
				cout << "error: only " << is.gcount() << " could be read.\n";
			is.close();
			delete[] temp;
		}else{
			cerr << "error: could not find file " << ifilename << ".\n";
			exit(1);
		}
	}else{
		getline(cin, buffer);
	}

	// using zlib to encode
	cout << "Now encoding with zlib\n";
	int err;
	uLong len = buffer.length();
	/* 
	 * When compress with small data, zlib don't perform well.
	 * And by that, it would be better to have a minimal size for compr in case of Z_BUF_ERROR.
	 */
	uLong comprLen = (len > 200 ? len : 200);
	Byte *compr = new Byte[comprLen];

	err = compress(compr, &comprLen, (const Bytef*)buffer.c_str(), len);

	if(err != Z_OK){
		cerr << "comperss error: " << err << "\n";
		exit(1);
	}
	cout << "compress size: " << comprLen << "\n";

	// using base64 to encode
	cout << "Now encoding with base64\n";
	string buffer2 = base64::encode((unsigned char*)compr, comprLen);
	cout << "base64 size: " << buffer2.length() << "\n";
	// according to factorio blueprint's protocal, we add a '0' to the beginning.
	buffer2.insert(buffer2.begin(), '0');

	cout << "Blueprint: \n\n";
	cout << buffer2;

	if(!ofilename.empty()){
		ofstream os(ofilename, std::ofstream::out);

		cout << "\n\nWriting " << buffer2.length() << " characters into \"" << ofilename << "\"";
		os.write(buffer2.c_str(), buffer2.length());
		os.close();
	}

	delete[] compr;
	return 0;
}
