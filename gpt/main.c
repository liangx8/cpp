#include <stdio.h>
#include <locale.h>
#include <wchar.h>
#include <malloc.h>
#include <stdint.h>

const wchar_t *hex=L"0123456789abcdef";
typedef struct {
	uint64_t signature;
	uint32_t revision;
	uint32_t header_size;
	uint32_t header_crc32;
	uint32_t reserved1;
	uint64_t my_lba;
	uint64_t alternate_lba;
	uint64_t first_usable_lba;
	uint64_t last_usable_lba;
	uint8_t disk_guid[16];
	uint64_t partition_entry_lba;
	uint32_t number_of_partition_entries;
	uint32_t size_of_partition_entry;
	uint32_t partition_entry_array_crc32;
} gpt_header;
typedef struct {
	uint8_t partition_type_guid[16];
	uint8_t unique_partition_guid[16];
	uint64_t start_lba;
	uint64_t end_lba;
	uint64_t attributes;
	uint16_t partition_name[36];
} gpt_entry;
void word2wchar(uint16_t src[],wchar_t *dst){
	int i=0;
	for(;i<36;i++){
		dst[i]=(wchar_t)src[i];
	}
}
void hexstring(uint8_t *src,wchar_t *dst,int length){
	int i=0;
	for(;i<length;i++){
		uint8_t v=src[i]>>4;
		dst[i*2]=hex[v];
		v=src[i]&0x0f;
		dst[i*2+1]=hex[v];
	}
	dst[i*2]=0;
}
int guid_zero(uint8_t guid[]){
	int i=0;
	for(;i<16;i++){
		if(guid[i]) return 0;
	}
	return -1;
}
int main(int argc, char **argv){
	gpt_header gpt;
	gpt_entry *entries;
	size_t entry_size;
	wchar_t *name;
	name=(wchar_t*)malloc(64*sizeof(wchar_t));
	int i;
	setlocale(LC_ALL,"");
	wprintf(L"GPT description\n");
	if (argc ==1) {
		wprintf(L"block device or file must be provided!\n");
		return 0;
	}
	FILE *fp=fopen(argv[1],"rb");
	if(fp==NULL){
		wprintf(L"open file error\n");
		return -1;
	}
	fseek(fp,512,SEEK_SET);
	fread(&gpt,92,1,fp);
	wprintf(L"signature:%lx\n",gpt.signature);
	wprintf(L"revision:%x\n",gpt.revision);
	wprintf(L"header size:%d\n",gpt.header_size);
	wprintf(L"header crc32:%x\n",gpt.header_crc32);
	wprintf(L"reserved:%x\n",gpt.reserved1);
	wprintf(L"my lba:%d\n",gpt.my_lba);
	wprintf(L"alternate lba:%ld\n",gpt.alternate_lba);
	wprintf(L"first_usable_lba:%ld\n",gpt.first_usable_lba);
	wprintf(L"last usable lba:%ld\n",gpt.last_usable_lba);
	hexstring(gpt.disk_guid,name,16);
	wprintf(L"disk GUID:%ls\n",name);
	wprintf(L"partition entry lba:%ld\n",gpt.partition_entry_lba);
	wprintf(L"number of partition entries:%d\n",gpt.number_of_partition_entries);
	wprintf(L"size of partition entry:%d\n",gpt.size_of_partition_entry);
	wprintf(L"partition entry array crc32:%x\n",gpt.partition_entry_array_crc32);
	entry_size = gpt.number_of_partition_entries * gpt.size_of_partition_entry;

	entries = (gpt_entry*) malloc(entry_size);
	fseek(fp,gpt.partition_entry_lba*512,SEEK_SET);
	fread(entries,entry_size,1,fp);
	for(i=0;i<gpt.number_of_partition_entries;i++){
		if(!guid_zero(entries[i].partition_type_guid)){
			wprintf(L"=================================================\npartition %d\n",i);
			wprintf(L"start lba:%ld\n",entries[i].start_lba);
			wprintf(L"end lba:%ld\n",entries[i].end_lba);
			hexstring(entries[i].partition_type_guid,name,16);
			wprintf(L"  partition type guid:%ls\n",name);
			hexstring(entries[i].unique_partition_guid,name,16);
			wprintf(L"unique partition guid:%ls\n",name);

			word2wchar(entries[i].partition_name,name);
			wprintf(L"partition name:%ls\n",name);
		}
	
	}	
	free(name);
	free(entries);
	fclose(fp);
	return 0;
}
