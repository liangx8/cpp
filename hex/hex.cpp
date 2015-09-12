/*
Author:liangxiaowz@gmail.com
December 2012
NO WARRANTY EXPRESSED OR IMPLIED. USE AT YOUR OWN RISK. you can
redistribute/modify this program without advise author.
DESCRIPTIONS:
This is a program to transfer Intel HEX file from to raw bin file,
and show them in readable format.
NOTE:
Please report bugs to author.
*/

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <sstream>
#include <exception>
#include <vector>


#define BUF_CAPACITY 32*1024

const char *hex="0123456789ABCDEF";
/*
void message(int errcode)
{
	std::cout << std::endl;
	switch(errcode){
	case 1:
		std::cout << "Address is not in order" << std::endl;
		break;
	case 2:
		std::cout << "Checksum is not corrected!" << std::endl;
		break;
	case 3:
		std::cout << "function is not implemented!" << std::endl;
		break;
	case 4:
		std::cout << "hex file is incorrected!" << std::endl;
	}
}
*/
int c2d(char c) throw(std::string &) {
	int idx=0;
	while(hex[idx]!=c){
		if(idx>15){
			throw(std::string("Error:hex to integer error!"));
		}
		idx++;
	}
	return idx;
}

class Record{
private:
	unsigned position;
	int cap;
	uint8_t *buf;
protected:
public:
	Record(int capacity):position(0),cap(capacity){
		this->buf=new uint8_t[capacity];
	}
	std::string& proc(std::istream *ins) throw(std::string &){
		std::string *err;
		std::stringstream ss;
//		std::vector<uint8_t> row;
		int lineno=0;
		while(!ins->eof()){
			int c=ins->get();
			if(c==':'){
				int sum;
				int colsize=c2d((char)ins->get() )*16;
				colsize+=c2d((char)ins->get());
				sum=colsize;
				unsigned address =c2d((char)ins->get())*16*16*16;
				address+=c2d((char)ins->get())*16*16;
				address+=c2d((char)ins->get())*16;
				address+=c2d((char)ins->get());
				sum += address +(address>>8);
				int record_type=c2d((char)ins->get())*16;
				record_type+=c2d((char)ins->get());
				sum += record_type;
				if(record_type==1)break;
				if(record_type==0){
					if (address < this->position){
						ss << "address define backward warning at line(L:";
						ss << std::dec << lineno << ",A:0x" << std::setfill('0') << std::setw(4) << std::hex << address << ",C:"  << std::setfill('0') << std::setw(4) << this->position << ')' << std::endl;
						this->position=address;
					}
				} else {
						ss << "Warning:record type:" << record_type << " is ignored!(L:" << lineno <<')' << std::endl;
				}
				for(int i=0;i<colsize;i++){
					int data=c2d((char)ins->get())*16;
					data += c2d((char)ins->get());
					sum += data;
					if(record_type==0){
						this->buf[this->position]=data;
						this->position++;
					}
				}
				sum = (-sum) & 0xff;
				int chksum=c2d((char)ins->get())*16;
				chksum+=c2d((char)ins->get());
				if(chksum != sum){
					ss << "Error:check summer error at line(L:";
					ss << c << ",A:0x" << std::setfill('0') << std::setw(4) << std::hex << address << ')' << std::endl;
					throw ss.str();
				}
				lineno++;
			}
		}
		err = new std::string(ss.str());
		unsigned idx=0;
		unsigned rest=0;
		while(idx < (unsigned)this->position){
			if(this->position - idx <16){
				rest=this->position-idx;
				break;
			}
			this->each16(idx,&(this->buf[idx]));
			idx += 16;
		}
		this->end(idx,rest,&(this->buf[idx]));
		return *err;
	}
	virtual void each16(int start,uint8_t *buf) =0;
	virtual void end(unsigned start,unsigned rest,uint8_t *p)=0;
	void set_pad(int v)
	{
		for(int i=0;i<this->cap;i++){
			this->buf[i]=(uint8_t)v;
		}
	}
	virtual ~Record(){
		delete [] this->buf;
	}
};
// 1b2d
class Xrecord:public Record{
private:
	char readable[17];
	std::ostream *outs;
public:
	Xrecord(std::ostream *o):Record(BUF_CAPACITY),outs(o){}
	virtual void each16(int start,uint8_t *p){
		char readable[17];
		*(this->outs) << "0x" << std::setfill('0') << std::setw(4) << std::hex << start<<": ";
		for (int i=0;i<16;i++){
			*(this->outs) << "0x" << std::setfill('0') << std::setw(2) << std::hex <<(int)p[i] << ' ';
			if(p[i]<32 || p[i]>126){
				readable[i]='.';
			} else {
				readable[i]=p[i];
			}
		}
		readable[16]='\0';
		*(this->outs) << ' ' << readable << std::endl;
	}
	virtual void end(unsigned start,unsigned rest,uint8_t *p){
		if (rest == 0) return;
		char readable[17];
		*(this->outs) << "0x" << std::setfill('0') << std::setw(4) << std::hex << start<<": ";
		int i;
		for (i=0;i<(int)rest;i++){
			*(this->outs) << "0x" << std::setfill('0') << std::setw(2) << std::hex <<(int)p[i] << ' ';
			if(p[i]<32 || p[i]>126){
				readable[i]='.';
			} else {
				readable[i]=p[i];
			}
		}
		readable[i]='\0';
		for (;i<16;i++){
			*(this->outs) << "     ";
		}
		*(this->outs) << ' ' << readable << std::endl;
	}
};
class Hrecord:public Record{
private:
	std::ostream *outs;
public:
	Hrecord(std::ostream *o):Record(BUF_CAPACITY),outs(o){}
	virtual void each16(int start,uint8_t *p){
		char readable[17];
		*(this->outs) << std::setfill('0') << std::setw(4) << std::hex << start<<"h: ";
		for (int i=0;i<16;i++){
			
			*(this->outs) << std::setfill('0') << std::setw(3) << std::hex <<(int)p[i] << "h ";
			if(p[i]<32 || p[i]>126){
				readable[i]='.';
			} else {
				readable[i]=p[i];
			}
		}
		readable[16]='\0';
		*(this->outs) << ' ' << readable << std::endl;

	}
	virtual void end(unsigned start,unsigned rest,uint8_t *p){
		if (rest == 0) return;
		char readable[17];
		*(this->outs) << std::setfill('0') << std::setw(4) << std::hex << start<<"h: ";
		int i;
		for (i=0;i<(int)rest;i++){
			*(this->outs) << std::setfill('0') << std::setw(3) << std::hex <<(int)p[i] << "h ";
			if(p[i]<32 || p[i]>126){
				readable[i]='.';
			} else {
				readable[i]=p[i];
			}
		}
		readable[i]='\0';
		for (;i<16;i++){
			*(this->outs) << "     ";
		}
		*(this->outs) << ' ' << readable << std::endl;
	}
};
class Brecord:public Record{
private:
	std::ostream *outs;
public:
	Brecord(std::ostream *o):Record(BUF_CAPACITY),outs(o){}

	virtual void each16(int start,uint8_t *p){
		for(int i=0;i<16;i++){
			*(this->outs) << p[i];
		}
	}
	virtual void end(unsigned  start,unsigned rest,uint8_t *p){
		for(unsigned i=0;i<rest;i++){
			*(this->outs) << p[i];
		}
	}
};
void usage(const char *p){
	const char *base=p;
	int i=0;
	while(*(p+i)!='\0'){
		if(*(p+i)=='\\' ||*(p+i)=='/'){
			base=p+i+1;
		}
		i++;
	}
	std::cout<<"This is Intel HEX utility. Version 0.1 December 2012\n";
	std::cout<<"This program can be used without any limited. USE AT YOUR OWN RISK.\n";
	std::cout<<"Please report bugs to liangxiaowz@gmail.com\n\nUsage:\n";
	std::cout << base<<" [-l <filename>] [-x | -h] [-f] [target]\n";
	std::cout << 
"\t-x\t\tshow '0x' prefix\n\
\t-h\t\tshow 'h' suffix\n\
\t-f\t\tpadding with '0xff' instead of default 0.\n\
\t-l filename\tlog error to file.\n\
\tA raw file will be generated if omitting both '-x' and '-h'.\n\n\n";
	std::cout << "\t "<<base<<" -x obiwen.hex\n";
	std::cout << "\t "<<base<<" -h << obiwen.hex\n";
	std::cout << "\t "<<base << " obiwen.hex" << std::endl;
}

int main(int argc,char **argv){
	Record *r=NULL;
	std::istream *ins=NULL;
	std::ostream *outs=NULL;
	std::ifstream *ifs=NULL;
	std::ofstream *ofs=NULL;

	char *outfile=NULL;
//	unsigned char buf[128];
//	int index;
	unsigned char pad=0;
//	int space;
	if(argc==1){
		usage(argv[0]);
		return 0;
	}
	for(int i=1;i<argc;i++){
		if(std::strcmp(argv[i],"-x")==0){
			r=new Xrecord(&std::cout);
		}
		if(std::strcmp(argv[i],"-h")==0){
			r=new Hrecord(&std::cout);
		}
		if(std::strcmp(argv[i],"-f")==0){
			pad=0xff;
		}
		
		if(argv[i][0] != '-'){
			ifs=new std::ifstream(argv[i],std::ios::in);
			if(ifs->is_open()){
				int j=0;
				
				ins=ifs;
				
				outfile=new char[256];
				while(*(argv[i]+j)){
					outfile[j]=*(argv[i]+j);
					if(outfile[j++]=='.')break;
				}
				outfile[j++]='b';
				outfile[j++]='i';
				outfile[j++]='n';
				outfile[j]=0;
				
			} else {
				delete ifs;
				std::cout << "file "<<argv[i]<<" is not found!" << std::endl;
				if(r)delete r;
				return 0;
			}
		}
	}
	if(r==NULL){
		if(outfile==NULL){
			ofs=new std::ofstream("out.bin",std::ios::binary);
			outs=ofs;
		}else{
			ofs=new std::ofstream(outfile,std::ios::binary);
			outs=ofs;
			delete [] outfile;
		}
		if(!ofs->is_open()){
			std::cout << "output file was not opened!" << std::endl;
			ifs->close();
			delete ifs;
			delete ofs;
			
			return 0;
		}
		
		r=new Brecord(outs);
		
	}
	r->set_pad(pad);
	if(ins==NULL)
		ins=&std::cin;
	try {
		std::string err=r->proc(ins);
		if (err.size()>0){
			std::ofstream oerr("err.log",std::ios::trunc|std::ios::out);
			oerr << err << std::endl;
			oerr.close();
		}
	} catch (std::string &e){
		std::ofstream oerr("err.log",std::ios::trunc|std::ios::out);
		oerr << e << std::endl;
		oerr.close();
	}
	if(ifs){
		ifs->close();
		delete ifs;
	}
	if(ofs){
		ofs->close();
		delete ofs;
	}
	
	delete r;

}

