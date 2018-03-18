#include <windows.h>
#include <string>
#include <stdio.h>
#include<iostream>
#include<string>
#include<vector>
#include<stdlib.h>
#include <direct.h>
#include <tchar.h>
using namespace std;

#pragma comment(lib,"ws2_32.lib")


HINSTANCE hInst;
WSADATA wsaData;
void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename);
SOCKET connectToServer(char *szServerName, WORD portNum);
int getHeaderLength(char *content);
char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut);
void make_directory(const char* name);
int downloadfile(char *url, string path);
int downloadfolder(char *url, string path);
int createfolder(string path);
void StrToChar(string tmp, char* &ans);

#define link "http://web.hcmus.edu.vn/images/stories/logo-khtn.png"
#define link1 "http://students.iitk.ac.in/programmingclub/course/lectures/"
#define link2 "http://www.cse.hcmut.edu.vn/~sonsys/2016_OS_A01/"
#define link3 "http://lvcfund.org.vn/images/tin-tuc/2015/44/logo_hust.jpg"
#define link4 "http://www.cse.hcmut.edu.vn/~dat/Com_Net_VP/"
#define link5 "http://www.cse.hcmut.edu.vn/~sonsys/2016_OS_A01/Bai_Giang/Lecture05.pdf"

#define namefolder "myfolder1"
#define spath "C:ProgramData/abc.txt"
#define pa "c:/te.txt"

#define BUFSIZE MAX_PATH

const string mssv = "1512112_1512075_1512628";
const string defaulttype = "HTTP/1.1";

string type;

int main(int argc, char* argv[])
{
	char* url;
	
	if (argc > 1) url = argv[1]; else url = link2;
	if (argc > 2) type = argv[2]; else type = defaulttype;
	

	char* path = NULL;
	path = getcwd(NULL, 0);
	string ns, fpath, fn;
	mParseUrl(url, ns, fpath, fn);

	string s = url;


	if (s[s.length() - 1] != '/')
	{		
		fn = (string)path + "\\" + mssv + "_" + fn;
		for (int i = 0; i < fn.length(); i++)
		if (fn[i] == '\\') fn[i] = '/';
		
		downloadfile(url, fn);
	}		
	else
	{
		fpath.erase(fpath.length() - 1);
		int found1 = fpath.rfind("/");
		string foldername = fpath.substr(found1 + 1);
		foldername = (string)path + "\\" + mssv + "_" + foldername;

		for (int i = 0; i < foldername.length(); i ++)
		if (foldername[i] == '\\') foldername[i] = '/';
		//cout << foldername << endl;
		
		downloadfolder(url, foldername);

	}
		

	return 0;
}


int createfolder(string path)
{
	char *fname = new char[path.length() + 1];
	strcpy(fname, path.c_str());
	mkdir(fname);
	return 0;
}

int downloadfolder(char *url, string foldername)
{
	const int bufLen = 1024;
	char *szUrl = url;
	long fileSize;
	char *memBuffer, *headerBuffer;
	FILE *fp;

	memBuffer = headerBuffer = NULL;

	if (WSAStartup(0x101, &wsaData) != 0)
		return -1;

	string ns, fpath, fn;
	
	memBuffer = readUrl2(szUrl, fileSize, &headerBuffer);
	//cout << foldername << endl;
	createfolder(foldername);

	string sourcepage = memBuffer;
	size_t found, found1;
	found = found1 = 0;
	int cnt = 0;
	do{
		found = sourcepage.find("href=", found1);
		if (found == std::string::npos) break;
		found1 = sourcepage.find(">", found);
		if (found1 == std::string::npos) break;
		found += 6;
		found1--;
		
		if (++cnt > 4)
		{
			size_t found2, found3;
			found2 = found1 + 2;
			found3 = sourcepage.find("<", found2);
			
			string location;
			if (found3 != std::string::npos)
				location = sourcepage.substr(found2, found3 - found2);
			//cout << location << endl;

			string sublink = sourcepage.substr(found, found1 - found);
			
			if (sublink[sublink.length() - 1] != '/')
			{
				sublink = (string)url + sublink;

				char *csublink = new char[sublink.length() + 1];
				strcpy(csublink, sublink.c_str());

				string pathfile = foldername + "/" + location;
				//cout << pathfile << endl;
				downloadfile(csublink, pathfile);
				delete csublink;
			}
			else
			{
				if (sublink[0] != '/')
				{
					//cout << sublink << endl;
					createfolder(sublink);
					cout << sublink << endl;
					string subpath = (string)url + sublink;
					char* SubPath;
					StrToChar(subpath, SubPath);
					downloadfolder(SubPath, sublink);
					
				} 
			}

		}
		
	} while (1);



	WSACleanup();
	return 0;
}

int downloadfile(char *url, string path){
	const int bufLen = 1024;
	char *szUrl = url;
	long fileSize;
	char *memBuffer, *headerBuffer;
	FILE *fp;
	
	
	memBuffer = headerBuffer = NULL;

	if (WSAStartup(0x101, &wsaData) != 0)
		return -1;

	
	char *fname = new char[path.length() + 1];
	strcpy(fname, path.c_str());

	memBuffer = readUrl2(szUrl, fileSize, &headerBuffer);
	//cout << headerBuffer << endl;
	

	if (fileSize != 0)
	{
		printf("Download from %s with filesize = %d bytes Save to %s \n", szUrl, fileSize, fname);

		fp = fopen(fname, "wb");
		if (fp != NULL)
		{
			fwrite(memBuffer, 1, fileSize, fp);
			fclose(fp);
		}
		delete(memBuffer);
		delete(headerBuffer);
		delete(fname);
	}

	WSACleanup();
	return 0;
}

void mParseUrl(char *mUrl, string &serverName, string &filepath, string &filename)
{
	string::size_type n;
	string url = mUrl;

	if (url.substr(0, 7) == "http://")
		url.erase(0, 7);

	n = url.find('/');
	if (n != string::npos)
	{
		serverName = url.substr(0, n);
		filepath = url.substr(n);
		n = filepath.rfind('/');
		filename = filepath.substr(n + 1);
	}

	else
	{
		serverName = url;
		filepath = "/";
		filename = "";
	}
}

SOCKET connectToServer(char *szServerName, WORD portNum)
{
	struct hostent *hp;
	unsigned int addr;
	struct sockaddr_in server;
	SOCKET conn;

	conn = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (conn == INVALID_SOCKET)
		return NULL;

	if (inet_addr(szServerName) == INADDR_NONE)
	{
		hp = gethostbyname(szServerName);
	}
	else
	{
		addr = inet_addr(szServerName);
		hp = gethostbyaddr((char*)&addr, sizeof(addr), AF_INET);
	}

	if (hp == NULL)
	{
		closesocket(conn);
		return NULL;
	}

	server.sin_addr.s_addr = *((unsigned long*)hp->h_addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(portNum);
	if (connect(conn, (struct sockaddr*)&server, sizeof(server)))
	{
		closesocket(conn);
		return NULL;
	}
	return conn;
}

int getHeaderLength(char *content)
{
	const char *srchStr1 = "\r\n\r\n", *srchStr2 = "\n\r\n\r";
	char *findPos;
	int ofset = -1;

	findPos = strstr(content, srchStr1);
	if (findPos != NULL)
	{
		ofset = findPos - content;
		ofset += strlen(srchStr1);
	}

	else
	{
		findPos = strstr(content, srchStr2);
		if (findPos != NULL)
		{
			ofset = findPos - content;
			ofset += strlen(srchStr2);
		}
	}
	return ofset;
}

int val(char ch)
{
	if ('0' <= ch && ch <= '9') 
		return ch - '0'; 
	else 
		return (ch - 'a' + 10);
}

int HexToDec(string tmp)
{
	int res = 0;
	for (int i = 0; i < tmp.length(); i++)
		res = res * 16 + val(tmp[i]);
	return res;
}

void StrToChar(string tmp, char* &ans)
{
	ans = new char[tmp.length() + 1];
	strcpy(ans, tmp.c_str());
}

void chunkprocess(char* &res)
{
	string tmp = res;
	
	//cout << tmp << endl;
	int cur = 0;
	do
	{
		int pos = tmp.find("\r\n", cur);
		if (pos == string::npos) break;
		string t = tmp.substr(cur, pos - cur);
		//cout << cur << " "<< pos << ":" << t << endl;
		
		tmp.erase(cur, t.length());
		
		tmp.erase(cur, 2);
		cur = pos - t.length() - 2 + 4 + HexToDec(t);
		
		
	} while (true);
	StrToChar(tmp, res);
}

char *readUrl2(char *szUrl, long &bytesReturnedOut, char **headerOut)
{
	const int bufSize = 512;
	char readBuffer[bufSize], sendBuffer[bufSize], tmpBuffer[bufSize];
	char *tmpResult = NULL, *result;
	SOCKET conn;
	string server, filepath, filename;
	long totalBytesRead, thisReadSize, headerLen;

	mParseUrl(szUrl, server, filepath, filename);

	///////////// step 1, connect //////////////////////
	conn = connectToServer((char*)server.c_str(), 80);

	
	///////////// step 2, send GET request /////////////
	sprintf(tmpBuffer, "GET %s %s", filepath.c_str(), type.c_str());
	strcpy(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	sprintf(tmpBuffer, "Host: %s", server.c_str());
	strcat(sendBuffer, tmpBuffer);
	strcat(sendBuffer, "\r\n");
	strcat(sendBuffer, "\r\n");
	send(conn, sendBuffer, strlen(sendBuffer), 0);

	//    SetWindowText(edit3Hwnd, sendBuffer);
	printf("Buffer being sent:\n%s", sendBuffer);

	///////////// step 3 - get received bytes ////////////////
	// Receive until the peer closes the connection
	totalBytesRead = 0;
	while (1)
	{
		memset(readBuffer, 0, bufSize);
		thisReadSize = recv(conn, readBuffer, bufSize, 0);

		if (thisReadSize <= 0)
			break;

		tmpResult = (char*)realloc(tmpResult, thisReadSize + totalBytesRead);

		memcpy(tmpResult + totalBytesRead, readBuffer, thisReadSize);
		totalBytesRead += thisReadSize;
	}

	headerLen = getHeaderLength(tmpResult);
	long contenLen = totalBytesRead - headerLen;
	result = new char[contenLen + 1];
	memcpy(result, tmpResult + headerLen, contenLen);
	result[contenLen] = 0x0;
	char *myTmp;

	myTmp = new char[headerLen + 1];
	strncpy(myTmp, tmpResult, headerLen);
	myTmp[headerLen] = NULL;
	delete(tmpResult);
	*headerOut = myTmp;

	bytesReturnedOut = contenLen;
	closesocket(conn);
	
	string res = myTmp;
	if (res.find("Transfer-Encoding: chunked") != string::npos && (type == "HTTP/1.1"))
		chunkprocess(result);
	
	return(result);
}