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
#include <exception>

const char *hex="0123456789ABCDEF";

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

int c2d(char c) throw(int) {
	int idx=0;
	while(hex[idx]!=c){
		if(idx>15){
			throw(4);
		}
		idx++;
	}
	return idx;
}

class Record{

protected:
	int pad;
	int position;
public:
	Record():position(0){}
	virtual int proc(const unsigned char*,int,int,std::ostream &) =0;
	virtual void end(std::ostream *)=0;
	void set_pad(int v)
	{
		pad=v;
	}
	virtual ~Record(){}
};

class Xrecord:public Record{
private:
	int col_index;
	char readable[17];
public:
	Xrecord():col_index(0){}
	virtual int proc(const unsigned char *src,int address,int length, std::ostream &out){
		if(address<position)return 1;
		while(position<address){
			if(col_index==0){
				out<<"0x" << std::setfill('0') << std::setw(4) << std::hex << position<<' ';
			}
			out << "0x" << std::setfill('0') << std::setw(2) << std::hex <<pad;
			readable[col_index]='.';
			if(++col_index==16){
				readable[col_index]=0;
				col_index=0;
				out << '\t' << (char *)readable << std::endl;
			} else {
				out << ',';
			}
			position++;
		}
		for(int i=0;i<length;i++){
			if(col_index==0){
				out<<"0x" << std::setfill('0') << std::setw(4) << std::hex << position<<' ';
			}
			out << "0x" << std::setfill('0') << std::setw(2) << std::hex <<(int)src[i];
			if(src[i]<32 || src[i]>126){
				readable[col_index]='.';
			} else {
				readable[col_index]=src[i];
			}
			if(++col_index==16){
				readable[col_index]=0;
				col_index=0;
				out << '\t' << (char *)readable << std::endl;
			} else {
				
				out << ',';
			}
			position++;
		}
		return 0;
	}
	virtual void end(std::ostream *out){
		readable[col_index]=0;
		(*out) << '\t' << (char *)readable << std::endl;
	}
};
class Hrecord:public Record{
private:
	int col_index;
	char readable[17];
public:
	Hrecord():col_index(0){}
	virtual int proc(const unsigned char *src,int address,int length, std::ostream &out){
		if(address<position)return 1;
		while(position<address){
			if(col_index==0){
				out<< std::setfill('0') << std::setw(4) << std::hex << position<<"h ";
			}
			if(pad>0x9f)
				out << '0';
			else
				out << ' ' ;
			out << std::setfill('0') << std::setw(2) << std::hex <<pad << 'h';
			readable[col_index]='.';
			if(++col_index==16){
				readable[col_index]=0;
				col_index=0;
				out << '\t' << (char *)readable << std::endl;
			} else {
				out << ',';
			}
			position++;
		}
		for(int i=0;i<length;i++){
			if(col_index==0){
				out << std::setfill('0') << std::setw(4) << std::hex << position<<"h ";
			}
			if(src[i]>0x9f)
				out << '0';
			else
				out << ' ';
			out << std::setfill('0') << std::setw(2) << std::hex <<(int)src[i] << 'h';
			if(src[i]<32 || src[i]>126){
				readable[col_index]='.';
			} else {
				readable[col_index]=src[i];
			}
			if(++col_index==16){
				readable[col_index]=0;
				col_index=0;
				out << '\t' << (char *)readable << std::endl;
			} else {
				
				out << ',';
			}
			position++;
		}
		return 0;
	}
	virtual void end(std::ostream *out){
		readable[col_index]=0;
		(*out) << '\t' << (char *)readable << std::endl;
	}
};
class Brecord:public Record{
public:
	virtual int proc(const unsigned char *src,int address,int length, std::ostream &out){
		if(address<position)return 1;
		while(position<address){
			out.put(pad);
			position++;
		}
		for(int i=0;i<length;i++){
			out.put(src[i]);
			position++;
		}
		return 0;
	}
	virtual void end(std::ostream *out){
//		((*std::ofstream)out)->close();
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
	std::cout << base<<" [-x | -h] [-f] [target]\n";
	std::cout << 
"\t-x\tshow '0x' prefix\n\
\t-h\tshow 'h' suffix\n\
\t-f\tpadding with '0xff' instead of default 0.\n\
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
	unsigned char buf[128];
	int index;
	unsigned char pad=0;
	int space;
	if(argc==1){
		usage(argv[0]);
		return 0;
	}
	for(int i=1;i<argc;i++){
		if(std::strcmp(argv[i],"-x")==0){
			r=new Xrecord();
			outs=&std::cout;
		}
		if(std::strcmp(argv[i],"-h")==0){
			r=new Hrecord();
			outs=&std::cout;
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
		
		r=new Brecord();
		
	}
	r->set_pad(pad);
	if(ins==NULL)
		ins=&std::cin;
	try{
		int byte_count=0;
		while(!ins->eof()){
			int c=ins->get();
			int address;
			int cols;
			int record_type;
			if(c==':'){
				int sum;
				int chksum;
				index=0;
				int errcode;

				cols=c2d((char)ins->get())*16;
				cols+=c2d((char)ins->get());
				byte_count +=cols;
				sum = cols;

				address =c2d((char)ins->get())*16*16*16;
				address+=c2d((char)ins->get())*16*16;
				address+=c2d((char)ins->get())*16;
				address+=c2d((char)ins->get());
				sum += address +(address>>8);
				record_type=c2d((char)ins->get())*16;
				record_type+=c2d((char)ins->get());
				sum += record_type;
				if(record_type==1)break;
				while(index<cols){
					int data=c2d((char)ins->get())*16;
					data +=c2d((char)ins->get());
					sum += data;
					buf[index++]=data;
				}
				sum = (-sum) & 0xff;
				chksum=c2d((char)ins->get())*16;
				chksum+=c2d((char)ins->get());
				if(chksum != sum){
					throw (2);
				}
				errcode=r->proc(buf,address,cols,*outs);
				if(errcode){
					throw (errcode);
				}
				space=address+cols;
			}
		}
		r->end(outs);
		std::cout <<"Data size:" <<byte_count <<'/'<<space<< std::endl;
	}catch(const int e){
		message(e);
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

