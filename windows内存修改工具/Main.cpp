#include <stdio.h>
#include <windows.h>
#include <memory.h>
#include <string>
#include "file.h"
#include "memory.h"
using namespace std;
void FunctionOne()
{
	string systemInfo, performanceInfo;
	systemInfo = getSystemInfo_toString();
	performanceInfo = getPerformanceInfo_toString();
	//ͬ���ļ�д��ʹ���ļ�ӳ��
	WriteFileWithFileMappingObject((void *)systemInfo.c_str(), systemInfo.length(), "filemappingsysteminfo.txt");
	printf("file mapping finished!Display system information.\n");

	//void *test = malloc(1 << 28);
	//WriteFileWithAsync(test, 1<<28, "async.txt", "async finished.\n");
	//printf("test\n");
	//�첽�ļ�д
	WriteFileWithAsync((void *)performanceInfo.c_str(), performanceInfo.length(), "asyncperformanceinfo.txt","async file write finished\n");
	printf("this file is async writing!\n");
	//ͬ���ļ���
	ReadFileWithFileMappingObject("filemappingsysteminfo.txt");
	printf("file read finished!\n");
	//�첽�ļ���
	ReadFileWithAsnc("filemappingsysteminfo.txt", "async read finished.");
	printf("this file is async reading!\n");
}
void FunctionTwo()
{
	//string processlist;
   //	processlist = getProcessList(vector<PROCESS_MEMORY_COUNTERS>& vct_pmc, vector<PROCESSENTRY32>& vct_pe32);
	vector<PROCESS_MEMORY_COUNTERS> v_pmc;
	vector<PROCESSENTRY32> v_pe32;
	string processListInfo = getProcessList(v_pmc, v_pe32);
	//ͬ��д�ļ���ʹ���ļ�ӳ��
	WriteFileWithFileMappingObject((void*)processListInfo.c_str(), processListInfo.length(), "filemappingprocesslist.txt");
	printf("file mapping finished!display processlist information.\n");

	//�첽�ļ�д
	WriteFileWithAsync((void *)processListInfo.c_str(), processListInfo.length(), "asyncprocesslist.txt", "async file write finished\n");
	printf("this file is async writing!\n");
	//ͬ���ļ���
	ReadFileWithFileMappingObject("filemappingprocesslist.txt");
	printf("file read finished!\n");
	//�첽�ļ���
	ReadFileWithAsnc("asyncprocesslist.txt", "async read finished.");
	printf("this file is async reading!\n");
}
void FunctionThree()
{
	//
	//Ĭ����Ȩ��
	init_for_mem_manage();
	mySetLastError(NOERROR);
	vector<PROCESS_MEMORY_COUNTERS> v_pmc;
	vector<PROCESSENTRY32> v_pe32;
	vector<MEMORY_BASIC_INFORMATION> v_mbi;
	DWORD pagesize;
	DWORD pid;
	HANDLE hProcess = INVALID_HANDLE_VALUE;
	int intval = 0;
	float floatval = 0;
	double doubleval = 0;
	string stringval = "";
	DWORD blockNum = 0;
	BOOL firstSearchSwitch = TRUE;
	BOOL GoOnSearch = TRUE;
	DWORD searchType;
	search_info my_search;
	string cmdString;
	string processListInfo = getProcessList(v_pmc, v_pe32);
	LPVOID targetAddress;
	cout << processListInfo << endl;
	pagesize = getSystemPageSize();
	cout << "��������Ҫ���ӵ��Ľ��̣�" << endl;
	cin >> pid;
	if (attachToProcess(pid, hProcess) == FALSE)
	{
		cout << "my error:" << myGetLastError() << endl;
		cout << "Last error:" << GetLastError() << endl;
		return;
	}
	if (getProcessMemBlockEX(hProcess, v_mbi, MYCOMMITED, MYPRIVATE, MYRW | MYRO, &blockNum) == FALSE)
	{
		cout << "my error:" << myGetLastError() << endl;
		cout << "Last error:" << GetLastError() << endl;
		CloseHandle(hProcess);
		return;
	}
onSearchFlag:cout << "��������Ҫִ�еĲ�����1.�������½�������2.�˳�" << endl;
	cin >> cmdString;
	if (cmdString == "1")
	{
		system("cls");
	}
	else
	{
		return;
	}
	my_search.i_list.clear();
	my_search.f_list.clear();
	my_search.d_list.clear();
	my_search.s_list.clear();
	GoOnSearch = TRUE;
	firstSearchSwitch = TRUE;
	while (GoOnSearch)
	{
		if (firstSearchSwitch)
		{
			int tmpT = 0;
		inputType:cout << "������Ҫ����������:1.int,2.float,3.double,4.string" << endl;
			cin >> tmpT;
			cout << "��������Ҫ������ֵ" << endl;
			switch (tmpT)
			{
			case 1:searchType = INT_T; cin >> intval; my_search.i_exp = intval; break;
			case 2:searchType = FLOAT_T; cin >> floatval; my_search.f_exp = floatval; break;
			case 3:searchType = DOUBLE_T; cin >> doubleval; my_search.d_exp = doubleval; break;
			case 4:searchType = STRING_T; cin >> stringval; my_search.s_exp = stringval; break;
			default:cout << "��Ч���룬����������" << endl; goto inputType;
			}
			if (searchAll(v_mbi, my_search, hProcess, searchType, pagesize, (char*)stringval.c_str(), &intval, &floatval, &doubleval) == FALSE)
			{
				cout << "my error:" << myGetLastError() << endl;
				cout << "Last error:" << GetLastError() << endl;
				CloseHandle(hProcess);
				return;
			}
			firstSearchSwitch = FALSE;
			cout << "���ϵĵ�ַ�б����£�" << endl;
			switch (tmpT)
			{
			case 1:for (int i = 0; i < my_search.i_list.size(); i++)
			{
				cout << LPVOID((char*)my_search.i_list[i].baseAddress + my_search.i_list[i].offset) << endl;
			}
				   break;
			case 2: for (int i = 0; i < my_search.f_list.size(); i++)
			{
				cout << LPVOID((char*)my_search.f_list[i].baseAddress + my_search.f_list[i].offset) << endl;
			}
					break;
			case 3: for (int i = 0; i < my_search.d_list.size(); i++)
			{
				cout << LPVOID((char*)my_search.d_list[i].baseAddress + my_search.d_list[i].offset) << endl;
			}
					break;
			case 4: for (int i = 0; i < my_search.i_list.size(); i++)
			{
				cout << LPVOID((char*)my_search.s_list[i].baseAddress + my_search.s_list[i].offset) << endl;
			}
					break;
			default:break;
			}
			cout << "�Ƿ�Ҫ����������1.�ǣ�2.������3.��" << endl;
			string tmpJ;
			cin >> tmpJ;
			if (tmpJ != "1")
			{
				GoOnSearch = FALSE;
			}

		}
		else
		{
			cout << "��������Ҫ������ֵ" << endl;
			switch (my_search.s_type)
			{
			case INT_T:searchType = INT_T; cin >> intval; my_search.i_exp = intval;
				if (nextScan(hProcess, my_search, &intval) == FALSE)
				{
					cout << "my error:" << myGetLastError() << endl;
					cout << "Last error:" << GetLastError() << endl;
					CloseHandle(hProcess);
					return;
				}
				break;
			case FLOAT_T:searchType = FLOAT_T; cin >> floatval; my_search.f_exp = floatval;
				if (nextScan(hProcess, my_search, &floatval) == FALSE)
				{
					cout << "my error:" << myGetLastError() << endl;
					cout << "Last error:" << GetLastError() << endl;
					CloseHandle(hProcess);
					return;
				}
				break;
			case DOUBLE_T:searchType = DOUBLE_T; cin >> doubleval; my_search.d_exp = doubleval;
				if (nextScan(hProcess, my_search, &doubleval) == FALSE)
				{
					cout << "my error:" << myGetLastError() << endl;
					cout << "Last error:" << GetLastError() << endl;
					CloseHandle(hProcess);
					return;
				}
				break;
			case STRING_T:searchType = STRING_T; cin >> stringval; my_search.s_exp = stringval;
				if (nextScan(hProcess, my_search, (char*)stringval.c_str()) == FALSE)
				{
					cout << "my error:" << myGetLastError() << endl;
					cout << "Last error:" << GetLastError() << endl;
					CloseHandle(hProcess);
					return;
				}
				break;
			}
			cout << "���ϵĵ�ַ�б����£�" << endl;
			switch (my_search.s_type)
			{
			case INT_T:for (int i = 0; i < my_search.i_list.size(); i++)
			{
				cout << LPVOID((char*)my_search.i_list[i].baseAddress + my_search.i_list[i].offset) << endl;
			}
				   break;
			case FLOAT_T: for (int i = 0; i < my_search.f_list.size(); i++)
			{
				cout << LPVOID((char*)my_search.f_list[i].baseAddress + my_search.f_list[i].offset) << endl;
			}
					break;
			case DOUBLE_T: for (int i = 0; i < my_search.d_list.size(); i++)
			{
				cout << LPVOID((char*)my_search.d_list[i].baseAddress + my_search.d_list[i].offset) << endl;
			}
					break;
			case STRING_T: for (int i = 0; i < my_search.i_list.size(); i++)
			{
				cout << LPVOID((char*)my_search.s_list[i].baseAddress + my_search.s_list[i].offset) << endl;
			}
					break;
			default:break;
			}
			cout << "�Ƿ�Ҫ����������1.�ǣ���������" << endl;
			string tmpJ;
			cin >> tmpJ;
			if (tmpJ != "1")
			{
				GoOnSearch = FALSE;
			}
		}
	}
	cout << "�Ƿ�����޸ģ�1.�ǣ���������" << endl;
	string whetherXiugai;
	cin >> whetherXiugai;
	if (whetherXiugai != "1")
	{
		goto onSearchFlag;
	}
	cout << "������Ҫ�޸ĵĵ�ַ��" << endl;
	cin >> targetAddress;
	cout << "������Ҫ��Ϊ��ֵ��" << endl;
	switch (my_search.s_type)
	{
	case INT_T:cin >> intval;
		if (writeData(hProcess, targetAddress, &intval, my_search.s_type, 0, 0) == FALSE)
		{
			cout << "my error:" << myGetLastError() << endl;
			cout << "Last error:" << GetLastError() << endl;
			CloseHandle(hProcess);
			return;
		}
		break;
	case FLOAT_T:cin >> floatval;
		if (writeData(hProcess, targetAddress, &floatval, my_search.s_type, 0, 0) == FALSE)
		{
			cout << "my error:" << myGetLastError() << endl;
			cout << "Last error:" << GetLastError() << endl;
			CloseHandle(hProcess);
			return;
		}
		break;
	case DOUBLE_T:cin >> doubleval;
		if (writeData(hProcess, targetAddress, &doubleval, my_search.s_type, 0, 0) == FALSE)
		{
			cout << "my error:" << myGetLastError() << endl;
			cout << "Last error:" << GetLastError() << endl;
			CloseHandle(hProcess);
			return;
		}
		break;
	case STRING_T: cin >> stringval;
		if (writeData(hProcess, targetAddress, &stringval, my_search.s_type, 1, '\0') == FALSE)
		{
			cout << "my error:" << myGetLastError() << endl;
			cout << "Last error:" << GetLastError() << endl;
			CloseHandle(hProcess);
			return;
		}
		break;
	}
	cout << "�޸ĳɹ���" << endl;
	goto onSearchFlag;
	CloseHandle(hProcess);
	
}
/**
���˵�����
*/
int main()
{
	BOOL hasAdminPrivileges = FALSE;
	if (EnablePrivileges() == TRUE)//��Ȩ
	{
		hasAdminPrivileges = TRUE;
	}
	else {
		printf("��Ȩʧ��\n");
	}
	int FunctionNumber;
	int PidNumber;
	printf("                      ProcessVirtualMemoryFileManagement\n\n");
	printf("------------------------------------------------------------------------\n");
	printf("         1. ��ȡϵͳ�ڴ��������Ϣ�������ļ�\n");
	printf("         2. ��ȡϵͳ�����б������ļ�\n");
	printf("         3. �ڴ�������޸�\n");
	printf("         4. ˢ��\n");
	printf("         5. �˳�����\n");
	printf("-------------------------------------------------------------------------\n");
	while (1)
	{
		printf("\n��������Ҫ�Ĺ������\n");
		scanf("%d", &FunctionNumber);
		if (FunctionNumber == 1)
		{
			FunctionOne();
			SleepEx(1, true);
		}
		
		else if (FunctionNumber == 2)
		{
			SleepEx(1, true);
			FunctionTwo();
		}
		else if (FunctionNumber == 3)
		{
			if (hasAdminPrivileges == FALSE)
			{
				printf("û��ȡ�ù���ԱȨ�ޣ�����ִ�д˲���\n");
			}
			else
			{
				FunctionThree();
			}
		}
		else if (FunctionNumber == 4)
		{
			system("cls");
			break;
		}
		else if (FunctionNumber == 5)
		{
			return 0;
			
		}
		else
		{
			printf("�������\n");
		}
	}
	SleepEx(INFINITE, TRUE);
	return 0;
}
