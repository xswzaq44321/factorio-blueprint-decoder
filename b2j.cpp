#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
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

	/* The first character of factorio blueprint format is the version byte, which is unnecessary in decoding and so can be removed.*/
	buffer.erase(0, 1);

	// using base64 to decode
	cout << "Now decoding with base64... \n";
	string buffer2 = base64::decode(buffer);

	// using zlib to decode
	cout << "Now decoding with zlib... \n";
	int err;
	uLong uncomprLen, comprLen;
	comprLen = buffer2.length();
	uncomprLen = 2 * comprLen;
	Byte *uncompr = new Byte[uncomprLen];

	err = uncompress(uncompr, &uncomprLen, (const Bytef*)buffer2.c_str(), comprLen);

	if(err != Z_OK){
		cerr << "uncompress error: " << err << "\n";
		exit(1);
	}
	cout << "uncompress size: " << uncomprLen << "\n";
	cout << "origional content: \n\n";
	cout << (char*)uncompr;

	if(!ofilename.empty()){
		ofstream os(ofilename, std::ofstream::out);
		cout << "\n\nWriting " << uncomprLen << " characters into \"" << ofilename << "\"";
		os.write((char*)uncompr, uncomprLen);
		os.close();
	}

	delete[] uncompr;
	return 0;
}
