#pragma once

class FileInterface
{
public:
	FileInterface();
	~FileInterface();

	bool OpenStream(const char* fname);
	void WriteToFile(const char* fname);
	bool opened;


protected:
	FILE *fp;

};

FileInterface::FileInterface()
{
	opened = false;
}

FileInterface::~FileInterface()
{
	if(opened)
	{
		fclose(fp);
		opened = false;
	}
}

bool FileInterface::OpenStream(const char* fname)
{
	if((fp = fopen(fname, "w+")) == NULL)
	{
		std::cout<<"[ERROR] Logger: Cannot open output file!\n";
		std::cout<<"Logging into STDOUT now\n";
		return false;
	}

	opened = true;

	return true;
}

void FileInterface::WriteToFile(const char* fname)
{
	if(!opened)
	{
		std::cout<<"[ERROR] Logger: Cannot write: file is not opened\n";
		return;
	}

	fprintf(fp, "%s",fname); 
	fflush(fp);
}
