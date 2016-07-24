#define DLL
#include "memory.h"
/*ȫ�ֱ���*/
DWORD my_error = 0;
BOOL is_xd_machine = TRUE;//Ĭ��С�˻���

/*�������ڴ��������*/
void check_mem_align_scheme()
{
	//
	short x;
	char x0;
	x = 0x1122;
	x0 = ((char*)&x)[0]; //�͵�ַ��Ԫ
	if (x0 == 0x11)
		is_xd_machine = FALSE;
	else is_xd_machine = TRUE;
}
/*��Ҫ�ĳ�ʼ�������������ȳ�ʼ����Ŀǰֻ�м��������Ͳ���*/
void init_for_mem_manage()
{
	check_mem_align_scheme();
}
/*�ڲ���error code������ͨ���˻�ȡ�������ͣ�ͷ�ļ��д������Ͷ���*/
DWORD myGetLastError()
{
	return my_error;
}
/**/
void mySetLastError(DWORD e)
{
	my_error = e;
}

DWORD  WINAPI getSystemPageSize()
{
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	return system_info.dwPageSize;
}
string  WINAPI getSystemInfo_toString()
{
	SYSTEM_INFO system_info;
	GetSystemInfo(&system_info);
	string strInfo = "ϵͳ��Ϣ��\r\n";
	string strTemp = "";
	// OemId
	char trans[MAX_PATH] = { 0 };
	sprintf_s(trans, "OemId : %u\r\n", system_info.dwOemId);
	strTemp = trans;
	strInfo += strTemp;
	// �������ܹ�
	sprintf_s(trans, "�������ܹ� : %u\r\n", system_info.wProcessorArchitecture);
	strTemp = trans;
	strInfo += strTemp;
	// ҳ���С
	sprintf_s(trans, "ҳ���С : %u\r\n", system_info.dwPageSize);
	strTemp = trans;
	strInfo += strTemp;
	// Ӧ�ó�����С��ַ
	sprintf_s(trans, "Ӧ�ó�����С��ַ : %p\r\n", system_info.lpMinimumApplicationAddress);
	strTemp = trans;
	strInfo += strTemp;
	// Ӧ�ó�������ַ
	sprintf_s(trans, "Ӧ�ó�������ַ : %p\r\n", system_info.lpMaximumApplicationAddress);
	strTemp = trans;
	strInfo += strTemp;
	// ����������
	sprintf_s(trans, "���������� : %u\r\n", system_info.dwActiveProcessorMask);
	strTemp = trans;
	strInfo += strTemp;
	// ����������
	sprintf_s(trans, "���������� : %u\r\n", system_info.dwNumberOfProcessors);
	strTemp = trans;
	strInfo += strTemp;
	// ����������
	sprintf_s(trans, "���������� : %u\r\n", system_info.dwProcessorType);
	strTemp = trans;
	strInfo += strTemp;
	// �����ڴ��������
	sprintf_s(trans, "�����ڴ�������� : %u\r\n", system_info.dwAllocationGranularity);
	strTemp = trans;
	strInfo += strTemp;
	// ����������
	sprintf_s(trans, "���������� : %u\r\n", system_info.wProcessorLevel);
	strTemp = trans;
	strInfo += strTemp;
	// �������汾
	sprintf_s(trans, "�������汾 : %u\r\n", system_info.wProcessorRevision);
	strTemp = trans;
	strInfo += strTemp;
	return strInfo;
}
string  WINAPI getPerformanceInfo_toString()
{
	PERFORMANCE_INFORMATION pi;
	pi.cb = sizeof(pi);
	GetPerformanceInfo(&pi, sizeof(pi));
	string pInfo = "";
	/*typedef struct _PERFORMANCE_INFORMATION {
	DWORD  cb;
	SIZE_T CommitTotal;
	SIZE_T CommitLimit;
	SIZE_T CommitPeak;
	SIZE_T PhysicalTotal;
	SIZE_T PhysicalAvailable;
	SIZE_T SystemCache;
	SIZE_T KernelTotal;
	SIZE_T KernelPaged;
	SIZE_T KernelNonpaged;
	SIZE_T PageSize;
	DWORD  HandleCount;
	DWORD  ProcessCount;
	DWORD  ThreadCount;
	} PERFORMANCE_INFORMATION, *PPERFORMANCE_INFORMATION;*/
	char trans[MAX_PATH] = { 0 };
	string sTmp;
	sprintf_s(trans, "ϵͳ��ǰ�ύ��ҳ������ : %u\r\n", pi.CommitTotal);
	sTmp = trans;
	pInfo += trans;
	sprintf_s(trans, "ϵͳ���ύ��ҳ������ : %u\r\n", pi.CommitLimit);
	sTmp = trans;
	pInfo += trans;
	sprintf_s(trans, "ϵͳ��ʷ�ύҳ���ֵ : %u\r\n", pi.CommitPeak);
	sTmp = trans;
	pInfo += trans;
	sprintf_s(trans, "��ҳ������ܵ������ڴ� : %u\r\n", pi.PhysicalTotal);
	sTmp = trans;
	pInfo += trans;
	sprintf_s(trans, "ϵͳ��ǰ�ύ��ҳ������ : %u\r\n", pi.PhysicalAvailable);
	sTmp = trans;
	pInfo += trans;
	return pInfo;
}

string  WINAPI processInfoToString(PROCESSENTRY32 pe, PROCESS_MEMORY_COUNTERS *pmc)
{
	char trans[MAX_PATH] = { 0 };
	string ID, Name, VMemory, cntThreads, th32ParentProcessID;
	sprintf_s(trans, "����ID�� %u\r\n", pe.th32ProcessID);
	ID = trans;
	sprintf_s(trans, "������ID�� %u\r\n", pe.th32ParentProcessID);
	th32ParentProcessID = trans;
	sprintf_s(trans, "�������ƣ� %s\r\n", pe.szExeFile);//�˴�������%u->%s
	Name = trans;
	sprintf_s(trans, "�߳����� %u\r\n", pe.cntThreads);
	cntThreads = trans;
	if (pmc != NULL) sprintf_s(trans, "����С�� %u KB\r\n", pmc->WorkingSetSize / 1024);
	VMemory = trans;
	return ID + th32ParentProcessID + Name + cntThreads + VMemory;
}
/*
��ȡ�����б�
*/
string  WINAPI getProcessList(vector<PROCESS_MEMORY_COUNTERS>& vct_pmc, vector<PROCESSENTRY32>& vct_pe32)
{
	/*
	typedef struct tagPROCESSENTRY32 {
	DWORD     dwSize;//�ýṹ�Ĵ�С��ʹ��ǰҪ��ʼ��������Process32First��ʧ��
	DWORD     cntUsage;//����ʹ�õ�ֵ
	DWORD     th32ProcessID;//process id
	ULONG_PTR th32DefaultHeapID;//����ʹ�õ�ֵ
	DWORD     th32ModuleID;//����ʹ�õ�ֵ
	DWORD     cntThreads;//�߳���
	DWORD     th32ParentProcessID;//������id
	LONG      pcPriClassBase;//�̵߳Ļ�������Ȩ
	DWORD     dwFlags;//����ʹ�õ�ֵ
	TCHAR     szExeFile[MAX_PATH];//
	} PROCESSENTRY32, *PPROCESSENTRY32;
	*/
	/*
	typedef struct _PROCESS_MEMORY_COUNTERS {
	DWORD  cb;
	DWORD  PageFaultCount;//The number of page faults.
	SIZE_T PeakWorkingSetSize;//����С��ֵ
	SIZE_T WorkingSetSize;//����С
	SIZE_T QuotaPeakPagedPoolUsage;//��ҳ��ʹ�õķ�ֵ
	SIZE_T QuotaPagedPoolUsage;//��ǰʹ�õķ�ҳ��
	SIZE_T QuotaPeakNonPagedPoolUsage;//�Ƿ�ҳ��ʹ�õķ�ֵ
	SIZE_T QuotaNonPagedPoolUsage;//
	SIZE_T PagefileUsage;//�ύ���ڴ����� in bytes
	SIZE_T PeakPagefileUsage;//PagefileUsage�ķ�ֵ
	} PROCESS_MEMORY_COUNTERS, *PPROCESS_MEMORY_COUNTERS;
	*/
	PROCESSENTRY32 pe;
	pe.dwSize = sizeof(pe);
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	BOOL bMore = Process32First(hProcessSnap, &pe);
	PROCESS_MEMORY_COUNTERS pmc;
	string pceInfo = "";
	while (bMore)
	{
		HANDLE hP = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
		ZeroMemory(&pmc, sizeof(pmc));
		if (GetProcessMemoryInfo(hP, &pmc, sizeof(pmc)) == TRUE)
		{
			vct_pe32.push_back(pe);
			vct_pmc.push_back(pmc);
		}
		pceInfo = pceInfo + processInfoToString(pe, &pmc);
		bMore = Process32Next(hProcessSnap, &pe);
		CloseHandle(hP);
	}
	CloseHandle(hProcessSnap);
	return pceInfo;
}
string  WINAPI getProtectionInfoString(DWORD dwTarget)
{
	string rt = "";
	if (dwTarget == PAGE_READONLY)
	{
		rt = "READONLY";
	}

	if (dwTarget == PAGE_GUARD)
	{
		rt = "GUARD";
	}

	if (dwTarget == PAGE_NOCACHE)
	{
		rt = "NOCACHE";
	}

	if (dwTarget == PAGE_READWRITE)
	{
		rt = "READWRITE";
	}

	if (dwTarget == PAGE_WRITECOPY)
	{
		rt = "WRITECOPY";
	}

	if (dwTarget == PAGE_EXECUTE)
	{
		rt = "EXECUTE";
	}

	if (dwTarget == PAGE_EXECUTE_READ)
	{
		rt = "EXECUTE_READ";
	}

	if (dwTarget == PAGE_EXECUTE_READWRITE)
	{
		rt = "EXECUTE_READWRITE";
	}

	if (dwTarget == PAGE_EXECUTE_WRITECOPY)
	{
		rt = "EXECUTE_WRITECOPY";
	}
	return rt;
}
BOOL WINAPI EnablePrivileges()
{
	//��Ȩ��
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;
	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return(FALSE);
	LookupPrivilegeValue(NULL, SE_DEBUG_NAME,
		&tkp.Privileges[0].Luid);
	tkp.PrivilegeCount = 1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0,
		(PTOKEN_PRIVILEGES)NULL, 0);
	CloseHandle(hToken);
	if (GetLastError() != ERROR_SUCCESS)
		return FALSE;
	return TRUE;
}
string  WINAPI memBlockInfoToString(MEMORY_BASIC_INFORMATION memBasicInfo, HANDLE handle)
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		mySetLastError(INVALID_HANDLE);
		return FALSE;
	}
	string Info = "";
	char trans[MAX_PATH];
	char szSize[MAX_PATH];
	LPCVOID pEnd = (PBYTE)memBasicInfo.BaseAddress + memBasicInfo.RegionSize;
	sprintf_s(szSize, "%u", memBasicInfo.RegionSize);
	//��ʾ�����ַ�ͳ���
	string Start, End;
	const LONG64 S = (LONG64)memBasicInfo.BaseAddress, E = (LONG64)pEnd;
	sprintf_s(trans, "%llx", S);
	Start = trans;
	sprintf_s(trans, "%llx", E);
	End = trans;
	while (Start.length() < 16)
		Start = "0" + Start;
	while (End.length() < 16)
		End = "0" + End;
	Info = Info + Start + "-" + End + " ( " + szSize + " BYTES )";
	//��ʾ���״̬
	switch (memBasicInfo.State)
	{
	case MEM_COMMIT:
		Info += " , committed";
		break;
	case MEM_FREE:
		Info += " , free";
		break;
	case MEM_RESERVE:
		Info += " , reserved";
		break;
	}
	//��ʾ����
	if (memBasicInfo.Protect == 0 && memBasicInfo.State != MEM_FREE)
	{
		memBasicInfo.Protect = PAGE_READONLY;
	}
	Info = Info + " , " + getProtectionInfoString(memBasicInfo.Protect);//
	//��ʾ����
	switch (memBasicInfo.Type)
	{
	case MEM_IMAGE:
		Info += " , Image";
		break;
	case MEM_MAPPED:
		Info += " , Mapped";
		break;
	case MEM_PRIVATE:
		Info += " , Private";
		break;
	}
	//����ִ�е�ӳ��
	char szFilename[MAX_PATH];
	if (GetModuleFileNameEx(handle, (HMODULE)memBasicInfo.BaseAddress, szFilename, MAX_PATH) > 0)
	{
		//my_PathStripPath(szFilename);//�Լ�ʵ�ֵ�PathStripPath //Obsolete opt,������ʾ������·��
		Info += " , Module: ";
		Info += szFilename;
	}
	Info += "\r\n";
	return Info;
}
BOOL WINAPI search_in_buffer(unsigned char * buffer, LPCVOID baseAddress, search_info& v_s_rslt, char *expval, int *surplus)
{
	*surplus = sizeof(expval);
	int i = 0, j = 0;
	int ri, rs = *surplus;//record i,surplus
	search_s_list tmp_s_rslt;
	while (i < READ_BUFFER_SIZE&&*surplus>0)
	{
		ri = i;
		while (i < READ_BUFFER_SIZE&&*surplus>0)
		{
			if (buffer[i] == expval[j])
			{
				i++;
				j++;
				(*surplus)--;
			}
			else{
				break;
			}
		}
		if ((*surplus) == 0)
		{
			tmp_s_rslt.baseAddress = baseAddress;
			tmp_s_rslt.offset = ri;
			tmp_s_rslt.s_val = expval;
			v_s_rslt.s_list.push_back(tmp_s_rslt);
		}
		i = ri + 1;
		j = 0;
		*surplus = rs;
	}
	return TRUE;
}
BOOL WINAPI search_in_buffer(unsigned char * buffer, LPCVOID baseAddress, search_info& v_s_rslt, int *expval)
{
	int t = 0;
	int i = 0;
	search_i_list s;
	int msize = sizeof(int);
	while (i < READ_BUFFER_SIZE)
	{
		t = 0;
		for (int j = 0; j < msize; j++)
		{
			if (is_xd_machine == FALSE)
			{
				int tmp = 0;
				tmp = tmp | buffer[i + j];
				t = (tmp << (msize - j - 1) * 8) | t;
			}
			else
			{
				int tmp = 0;//
				tmp = tmp | buffer[i + j];
				int tt = (tmp << (8 * j));
				t = tt | t;
			}
		}
		if (t == *expval)
		{
			s.baseAddress = baseAddress;
			s.offset = i;
			s.i_val = t;
			v_s_rslt.i_list.push_back(s);
		}
		if (i > 2560)
		{
			int qq = i;
			qq++;
		}
		i += msize;
	}
	return TRUE;
}
BOOL WINAPI search_in_buffer(unsigned char * buffer, LPCVOID baseAddress, search_info& v_s_rslt, float *expval)
{
	float f = 0;
	int *t = (int *)&f;
	int i = 0;
	search_f_list s;
	int msize = sizeof(float);
	while (i < READ_BUFFER_SIZE)
	{
		memset(&f, 0, sizeof(f));
		for (int j = 0; j < msize; j++)
		{
			if (is_xd_machine == FALSE)
			{
				int tmp = 0;
				tmp = tmp | (unsigned int)buffer[i + j];
				*t = (tmp << (msize - j - 1) * 8) | *t;
			}
			else
			{
				int tmp = 0;
				tmp = tmp | (unsigned int)buffer[i + j];
				int tt = (tmp << (8 * j));
				*t = tt | *t;
			}
		}
		if (fabs(f - *expval)<1)
		{
			s.baseAddress = baseAddress;
			s.offset = i;
			s.f_val = f;
			v_s_rslt.f_list.push_back(s);
		}
		if (i >= 3460)
		{
			int debug = 0;
		}
		i += msize;
	}
	return TRUE;
}
BOOL WINAPI search_in_buffer(unsigned char * buffer, LPCVOID baseAddress, search_info& v_s_rslt, double *expval)
{
	double f = 0;
	LONG64 *t = (LONG64 *)&f;
	int i = 0;
	search_d_list s;
	int msize = sizeof(double);
	while (i < READ_BUFFER_SIZE)
	{
		memset(&f, 0, sizeof(f));
		for (int j = 0; j < msize; j++)
		{
			if (is_xd_machine == FALSE)
			{
				LONG64 tmp = 0;
				tmp = tmp | (unsigned int)buffer[i + j];
				*t = (tmp << (msize - j - 1) * 8) | *t;
			}
			else
			{
				LONG64 tmp = 0;
				tmp = tmp | (unsigned int)buffer[i + j];
				*t = (tmp << (8 * j)) | *t;

			}
		}
		if (fabs(f - *expval)<1)
		{
			s.baseAddress = baseAddress;
			s.offset = i;
			s.d_val = f;
			v_s_rslt.d_list.push_back(s);
		}
		i += msize;
	}
	return TRUE;
}
BOOL WINAPI search_one_mem_block(MEMORY_BASIC_INFORMATION memBasicInfo, search_info& v_s_rslt, HANDLE handle, DWORD PageSize)
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		mySetLastError(INVALID_HANDLE);
		return FALSE;
	}
	int i;
	int block_num = memBasicInfo.RegionSize / PageSize;
	if (memBasicInfo.RegionSize%PageSize != 0)
	{
		mySetLastError(PageSizeErrorIn_S_MEM_BLOCK);
		return FALSE;
	}
	SIZE_T readnum = 0;
	unsigned char readBuffer[READ_BUFFER_SIZE] = { 0 };
	DWORD dwOldProt, dwNewProt;
	if (!VirtualProtectEx(handle, (void*)memBasicInfo.BaseAddress, memBasicInfo.RegionSize, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
	{
		mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
		return FALSE;
	}
	for (i = 0; i < block_num; i++)
	{
		LPCVOID pBase = (LPCVOID)((unsigned char*)memBasicInfo.BaseAddress + PageSize*i);
		if (!ReadProcessMemory(handle, pBase, readBuffer, (SIZE_T)PageSize, &readnum))
		{
			mySetLastError(RPM_FAIL);
			return FALSE;

		}
		int sur = 0;
		switch (v_s_rslt.s_type)
		{
		case INT_T:search_in_buffer(readBuffer, pBase, v_s_rslt, &v_s_rslt.i_exp); break;
		case FLOAT_T:search_in_buffer(readBuffer, pBase, v_s_rslt, &v_s_rslt.f_exp); break;
		case DOUBLE_T:search_in_buffer(readBuffer, pBase, v_s_rslt, &v_s_rslt.d_exp); break;
		case STRING_T:search_in_buffer(readBuffer, pBase, v_s_rslt, (char *)v_s_rslt.s_exp.c_str(), &sur); break;
		default:
			break;
		}
	}
	if (!VirtualProtectEx(handle, (void*)memBasicInfo.BaseAddress, memBasicInfo.RegionSize, dwOldProt, &dwNewProt) && GetLastError() != 87)
	{
		mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
		return FALSE;
	}
	return TRUE;
}
/*
�״�����
vector<MEMORY_BASIC_INFORMATION> memInfo,ɸѡ���Ĵ�������
search_info &v_s_rslt,����������������ݽṹ
HANDLE handle,���̾��
DWORD s_type,�������ͣ�֧��FLOAT_T INT_T DOUBLE_T STRING_T
DWORD pagesize,ϵͳҳ���С
char* charexpval,Ҫ������STRING_Tֵ
int *intexpval,Ҫ������INT_Tֵ
float *floatexpval,Ҫ������FLOAT_Tֵ
double *doubleexpvalҪ������DOUBLE_Tֵ
*/
BOOL WINAPI searchAll(vector<MEMORY_BASIC_INFORMATION> memInfo, search_info &v_s_rslt, HANDLE handle, DWORD s_type, DWORD pagesize, char* charexpval, int *intexpval, float *floatexpval, double *doubleexpval)
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		mySetLastError(INVALID_HANDLE);
		return FALSE;
	}
	v_s_rslt.s_type = s_type;
	switch (s_type)
	{
	case INT_T:v_s_rslt.i_exp = *intexpval; break;
	case FLOAT_T:v_s_rslt.f_exp = *floatexpval; break;
	case DOUBLE_T:v_s_rslt.d_exp = *doubleexpval; break;
	case STRING_T:v_s_rslt.s_exp = charexpval; break;
	default:
		break;
	}
	int block_num = memInfo.size();
	int i;
	for (i = 0; i < block_num; i++)
	{
		if (search_one_mem_block(memInfo[i], v_s_rslt, handle, pagesize) == FALSE)
		{
			return FALSE;
		}
	}
	return TRUE;
}
/*
DWORD IN pid,Ҫ�򿪵Ľ���
DWORD OUT hProcess�ɹ��󷵻صĽ��̾��
*/
BOOL WINAPI attachToProcess(DWORD Pid, HANDLE & hProcess)
{
	hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
	if (hProcess == NULL)
	{
		mySetLastError(ATTACH_TO_PROCESS_FAIL);
		return FALSE;
	}
	return TRUE;
}
/*
������ɸѡҪ�������ڴ��
HANDLE IN hProcess ���̾��
vector OUT memInfo �����ڴ����Ϣ�����ݽṹ vector<MEMORY_BASIC_INFORMATION>
DWORD IN stateFilter ��state���͹����ڴ��
DWORD IN typeFilter  ��type���͹����ڴ��
DWORD IN protectFilter  ��protect���͹����ڴ��
*/
BOOL WINAPI getProcessMemBlockEX(HANDLE hProcess, vector<MEMORY_BASIC_INFORMATION>& memInfo, DWORD stateFilter, DWORD typeFilter, DWORD protectFilter, DWORD* memSize)
{
	/*
	typedef struct _MEMORY_BASIC_INFORMATION {
	PVOID  BaseAddress;//һ��ָ����������ַ��ָ�롣/��ѯ�ڴ����ռ�ĵ�һ��ҳ�����ַ
	PVOID  AllocationBase;//�ڴ����ռ�ĵ�һ���������ַ��С�ڵ���BaseAddress��
	//Ҳ����˵BaseAddressһ��������AllocationBase����ķ�Χ��
	DWORD  AllocationProtect;//���򱻳��α���ʱ����ı�������
	SIZE_T RegionSize;//��BaseAddress��ʼ��������ͬ���Ե�ҳ��Ĵ�С  ����������Ҫ����ȡ�������̵�ַ�ռ�ʱ�����԰�ҳ�Ĵ�С��ȡ
	DWORD  State;//ҳ���״̬�������ֿ���ֵ��MEM_COMMIT��MEM_FREE��MEM_RESERVE��
	//���������������˵������Ҫ���ˣ��������Ǳ��ָ֪���ڴ�ҳ���״̬��
	//committed:Indicates committed pages for which physical storage has been allocated, either in memory or in the paging file on disk.
	//������������ϴ���
	//free�����еģ����ɷ��ʣ��ɷ���ġ�
	//reserved��Ԥ����
	DWORD  Protect;//ҳ������ԣ�����ܵ�ȡֵ��AllocationProtect��ͬ
	DWORD  Type;//���ڴ������ͣ������ֿ���ֵ��MEM_IMAGE��MEM_MAPPED��MEM_PRIVATE
	} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;
	*/
	/*
	typedef struct _SYSTEM_INFO {
	union {
	DWORD dwOemId;          // Obsolete field...do not use
	struct {
	WORD wProcessorArchitecture;
	WORD wReserved;
	} DUMMYSTRUCTNAME;
	} DUMMYUNIONNAME;
	DWORD dwPageSize;
	LPVOID lpMinimumApplicationAddress;
	LPVOID lpMaximumApplicationAddress;
	DWORD_PTR dwActiveProcessorMask;
	DWORD dwNumberOfProcessors;
	DWORD dwProcessorType;
	DWORD dwAllocationGranularity;
	WORD wProcessorLevel;
	WORD wProcessorRevision;
	} SYSTEM_INFO, *LPSYSTEM_INFO;
	*/
	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
	if (hProcess == INVALID_HANDLE_VALUE){
		mySetLastError(IHV_IN_GPM);
		return FALSE;
	}
	SYSTEM_INFO si;		//ϵͳ��Ϣ�ṹ
	ZeroMemory(&si, sizeof(si));
	GetSystemInfo(&si);
	MEMORY_BASIC_INFORMATION memBasicInfo;//���������ڴ�ռ�Ļ�����Ϣ�ṹ
	ZeroMemory(&memBasicInfo, sizeof(memBasicInfo));
	//ѭ������Ӧ�ó���ĵ�ַ�ռ�
	LPCVOID pStart = (LPCVOID)si.lpMinimumApplicationAddress;
	if (memSize != NULL) *memSize = 0;
	while (pStart < si.lpMaximumApplicationAddress)
	{
		//�����һ�������ڴ�����Ϣ
		if (VirtualQueryEx(hProcess, pStart, &memBasicInfo, sizeof(memBasicInfo)) == sizeof(memBasicInfo))
		{
			if (((stateFilter&memBasicInfo.State)>0) && ((typeFilter&memBasicInfo.Type)>0) && ((protectFilter&memBasicInfo.Protect)>0))
			{
				memInfo.push_back(memBasicInfo);
				if (memSize != NULL) *memSize = *memSize + memBasicInfo.RegionSize;
			}
			//����β�����ַ
			LPCVOID pEnd = (PBYTE)pStart + memBasicInfo.RegionSize;
			//�ƶ���ָ���Ի����һ��ģ��
			pStart = pEnd;
		}
	}
	return TRUE;
}
/*
int size,���������С
HANDLE handle���̾��
*/
LPVOID WINAPI AllocVirtual(int size, HANDLE handle)
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		mySetLastError(INVALID_HANDLE);
		return FALSE;
	}
	LPVOID Buffer = (LPVOID)VirtualAllocEx(handle, NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	if (Buffer == NULL)
	{
		mySetLastError(inherit_last_error_in_alloc);
		//cout << "����ʧ��,getlasterror:" << GetLastError() << endl;
	}
	return Buffer;
}
/*
LPVOID Buffer,�ͷŵ����ַָ��
HANDLE handle���̾��
*/
BOOL WINAPI FreeVirtual(LPVOID Buffer, HANDLE handle)
{
	if (handle == INVALID_HANDLE_VALUE)
	{
		mySetLastError(INVALID_HANDLE);
		return FALSE;
	}
	if (VirtualFreeEx(handle, Buffer, 0, MEM_RELEASE) == 0)
	{
		mySetLastError(inherit_last_error_in_free);
		return FALSE;
	}
	return TRUE;
}
/*
HANDLE hProcess,//���̾��
search_info & v_s_rslt,//�����ϴ�������Ϣ�����ݽṹ
LPVOID expected_val//�µ�����ֵ
*/
BOOL WINAPI nextScan(HANDLE hProcess, search_info & v_s_rslt, LPVOID expected_val)
{
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		mySetLastError(INVALID_HANDLE);
		return FALSE;
	}
	int readsize = 0;
	int listnum = 0;
	SIZE_T real_read_num;
	unsigned char buffer[READ_BUFFER_SIZE];
	vector<search_i_list>::iterator pIIter = v_s_rslt.i_list.begin();;
	vector<search_f_list>::iterator pFIter = v_s_rslt.f_list.begin();;
	vector<search_d_list>::iterator pDIter = v_s_rslt.d_list.begin();;
	vector<search_s_list>::iterator pSIter = v_s_rslt.s_list.begin();;
	switch (v_s_rslt.s_type)
	{
	case INT_T:
		readsize = sizeof(int);
		memset(buffer, 0, sizeof(buffer));
		for (; pIIter != v_s_rslt.i_list.end();)
		{

			DWORD dwOldProt, dwNewProt = 0;
			real_read_num = 0;
			if (!VirtualProtectEx(hProcess, (LPVOID)((unsigned char*)pIIter->baseAddress + pIIter->offset), readsize, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
			{
				mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
				return FALSE;
			}

			if (!ReadProcessMemory(hProcess, (LPCVOID)((unsigned char*)pIIter->baseAddress + pIIter->offset), buffer, readsize, &real_read_num))
			{
				mySetLastError(NextScan_READFAIL);
				return FALSE;
			}
			int t = 0;
			for (int j = 0; j < readsize; j++)
			{
				if (is_xd_machine == FALSE)
				{
					int tmp = 0;
					tmp = tmp | buffer[j];
					t = (tmp << (readsize - j - 1) * 8) | t;
				}
				else
				{
					int tmp = 0;//
					tmp = tmp | buffer[j];
					int tt = (tmp << (8 * j));
					t = tt | t;
				}
			}
			if (!VirtualProtectEx(hProcess, (LPVOID)((unsigned char*)pIIter->baseAddress + pIIter->offset), readsize, dwOldProt, &dwNewProt) && GetLastError() != 87)
			{
				mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
				return FALSE;
			}
			if (*(int*)expected_val == t)
			{
				pIIter->i_val = t;
				pIIter++;
			}
			else
			{
				pIIter = v_s_rslt.i_list.erase(pIIter);
			}

		}
		break;
	case FLOAT_T:readsize = sizeof(float);
		memset(buffer, 0, sizeof(buffer));
		for (; pFIter != v_s_rslt.f_list.end();)
		{

			DWORD dwOldProt, dwNewProt = 0;
			if (!VirtualProtectEx(hProcess, (LPVOID)((unsigned char*)pFIter->baseAddress + pFIter->offset), readsize, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
			{
				mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
				return FALSE;
			}

			if (!ReadProcessMemory(hProcess, (LPCVOID)((unsigned char*)pFIter->baseAddress + pFIter->offset), buffer, readsize, &real_read_num))
			{
				mySetLastError(NextScan_READFAIL);
				return FALSE;
			}
			float f = 0;
			int *t = (int *)&f;
			for (int j = 0; j < readsize; j++)
			{
				if (is_xd_machine == FALSE)
				{
					int tmp = 0;
					tmp = tmp | buffer[j];
					*t = (tmp << (readsize - j - 1) * 8) | *t;
				}
				else
				{
					int tmp = 0;//
					tmp = tmp | buffer[j];
					int tt = (tmp << (8 * j));
					*t = tt | *t;
				}
			}
			if (!VirtualProtectEx(hProcess, (LPVOID)((unsigned char*)pFIter->baseAddress + pFIter->offset), readsize, dwOldProt, &dwNewProt) && GetLastError() != 87)
			{
				mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
				return FALSE;
			}
			if (fabs(*(float*)expected_val - f)<1)
			{
				pFIter->f_val = f;
				pFIter++;
			}
			else
			{
				pFIter = v_s_rslt.f_list.erase(pFIter);
			}

		}
		break;
	case DOUBLE_T:readsize = sizeof(double);
		memset(buffer, 0, sizeof(buffer));
		for (; pDIter != v_s_rslt.d_list.end();)
		{

			DWORD dwOldProt, dwNewProt = 0;
			if (!VirtualProtectEx(hProcess, (LPVOID)((unsigned char*)pDIter->baseAddress + pDIter->offset), readsize, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
			{
				mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
				return FALSE;
			}
			if (ReadProcessMemory(hProcess, (LPCVOID)((unsigned char*)pDIter->baseAddress + pDIter->offset), buffer, readsize, &real_read_num) == FALSE)
			{
				mySetLastError(NextScan_READFAIL);
				return FALSE;
			}
			double f = 0;
			LONG64 *t = (LONG64 *)&f;
			for (int j = 0; j < readsize; j++)
			{
				if (is_xd_machine == FALSE)
				{
					LONG64 tmp = 0;
					tmp = tmp | buffer[j];
					*t = (tmp << (readsize - j - 1) * 8) | *t;
				}
				else
				{
					LONG64 tmp = 0;//
					tmp = tmp | buffer[j];
					LONG64 tt = (tmp << (8 * j));
					*t = tt | *t;
				}
			}
			if (!VirtualProtectEx(hProcess, (LPVOID)((unsigned char*)pDIter->baseAddress + pDIter->offset), readsize, dwOldProt, &dwNewProt) && GetLastError() != 87)
			{
				mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
				return FALSE;
			}
			if (fabs(*(double*)expected_val - f) < 1)
			{
				pDIter->d_val = f;
				pDIter++;
			}
			else
			{
				pDIter = v_s_rslt.d_list.erase(pDIter);
			}

		}
		break;
	case STRING_T:readsize = strlen((char*)expected_val);
		memset(buffer, '0', sizeof(buffer));
		for (; pSIter != v_s_rslt.s_list.end();)
		{

			DWORD dwOldProt, dwNewProt = 0;
			if (!VirtualProtectEx(hProcess, (LPVOID)((unsigned char*)pSIter->baseAddress + pSIter->offset), readsize, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
			{
				mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
				return FALSE;
			}
			if (!ReadProcessMemory(hProcess, (LPCVOID)((unsigned char*)pSIter->baseAddress + pSIter->offset), buffer, readsize, &real_read_num))
			{
				mySetLastError(NextScan_READFAIL);
				return FALSE;
			}
			int t = 0;

			if (!VirtualProtectEx(hProcess, (LPVOID)((unsigned char*)pSIter->baseAddress + pSIter->offset), readsize, dwOldProt, &dwNewProt) && GetLastError() != 87)
			{
				mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
				return FALSE;
			}
			if (strcmp((char*)buffer, (char*)expected_val) == 0)
			{
				pSIter->s_val = (char*)expected_val;
				pSIter++;
			}
			else
			{
				pSIter = v_s_rslt.s_list.erase(pSIter);
			}

		}
		break;
	default:
		mySetLastError(WrongSearchType);
		return FALSE;
	}
	return TRUE;
}
/*
HANDLE hProcess,//Ҫд��Ľ���
LPVOID address,//Ҫд��ĵ�ַ
void * write_val,//Ҫд��ı���ָ�룬ֻ����string,int,double,float����
DWORD valType,//д��ı������ͣ���ѡֵ��ͷ�ļ��궨��
int stringAppendZeroNUM,//�ַ�����׷�ӵ��ַ�����
char endSet//�ַ�����׷���ַ����ڴ���string��char��β��ͬ
*/
BOOL WINAPI writeData(HANDLE hProcess, LPVOID address, void * write_val, DWORD valType, int stringAppendZeroNUM, char endSet)
{
	if (hProcess == INVALID_HANDLE_VALUE)
	{
		mySetLastError(INVALID_HANDLE);
		return FALSE;
	}
	if (write_val == NULL)
	{
		mySetLastError(NULL_POINTER);
		return FALSE;
	}
	int size = 0;
	SIZE_T real_writenum = 0;
	DWORD dwOldProt, dwNewProt = 0;
	unsigned char buffer[READ_BUFFER_SIZE];
	switch (valType)
	{
	case INT_T:size = sizeof(int);
		if (is_xd_machine == TRUE)
		{
			for (int i = 0; i < size; i++)
			{
				buffer[i] = ((0xff << (i * 8))&(*(unsigned int *)write_val)) >> (i * 8);
			}
		}
		else{
			for (int i = 0; i < size; i++)
			{
				buffer[i] = (((0xff << (8 * size - 1)) >> (i * 8))&(*(unsigned int *)write_val)) >> (size - i - 1);
			}
		}

		if (!VirtualProtectEx(hProcess, address, size, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
		{
			mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
			return FALSE;
		}
		while (real_writenum < size)
		{
			if (WriteProcessMemory(hProcess, (LPVOID)((char*)address + real_writenum), buffer + real_writenum, size - real_writenum, &real_writenum) == FALSE)
			{
				mySetLastError(WriteDataFalse);
				return FALSE;
			}
		}
		if (!VirtualProtectEx(hProcess, address, size, dwOldProt, &dwNewProt) && GetLastError() != 87)
		{
			mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
			return FALSE;
		}
		break;
	case FLOAT_T:size = sizeof(float);
		if (is_xd_machine == TRUE)
		{
			for (int i = 0; i < size; i++)
			{
				buffer[i] = ((0xff << (i * 8))&(*(unsigned int *)write_val)) >> (i * 8);
			}
		}
		else{
			for (int i = 0; i < size; i++)
			{
				buffer[i] = (((0xff << (8 * size - 1)) >> (i * 8))&(*(unsigned int *)write_val)) >> (size - i - 1);
			}
		}

		if (!VirtualProtectEx(hProcess, address, size, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
		{
			mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
			return FALSE;
		}

		while (real_writenum < size)
		{
			if (WriteProcessMemory(hProcess, (LPVOID)((char*)address + real_writenum), buffer + real_writenum, size - real_writenum, &real_writenum) == FALSE)
			{
				mySetLastError(WriteDataFalse);
				return FALSE;
			}
		}
		if (!VirtualProtectEx(hProcess, address, size, dwOldProt, &dwNewProt) && GetLastError() != 87)
		{
			mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
			return FALSE;
		}
		break;

	case DOUBLE_T:size = sizeof(double);
		if (is_xd_machine == TRUE)
		{
			for (int i = 0; i < size; i++)
			{
				buffer[i] = ((0xff << (i * 8))&(*(unsigned int *)write_val)) >> (i * 8);
			}
		}
		else{
			for (int i = 0; i < size; i++)
			{
				buffer[i] = (((0xff << (8 * size - 1)) >> (i * 8))&(*(unsigned int *)write_val)) >> (size - i - 1);
			}
		}

		if (!VirtualProtectEx(hProcess, address, size, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
		{
			mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
			return FALSE;
		}

		while (real_writenum < size)
		{
			if (WriteProcessMemory(hProcess, (LPVOID)((char*)address + real_writenum), buffer + real_writenum, size - real_writenum, &real_writenum) == FALSE)
			{
				mySetLastError(WriteDataFalse);
				return FALSE;
			}
		}
		if (!VirtualProtectEx(hProcess, address, size, dwOldProt, &dwNewProt) && GetLastError() != 87)
		{
			mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
			return FALSE;
		}
		break;

	case STRING_T:size = (*(string *)write_val).length();

		strcpy_s((char*)buffer, sizeof(buffer), (*(string *)write_val).c_str());
		if (size + stringAppendZeroNUM>READ_BUFFER_SIZE)//��鳤���Ƿ�Ϸ�
		{
			mySetLastError(StringAppendZeroNUM_TOOBIG);
			return FALSE;
		}
		for (int i = size; i < stringAppendZeroNUM + size; i++)
		{
			buffer[i] = (unsigned char)endSet;
		}
		if (!VirtualProtectEx(hProcess, address, size, PAGE_READWRITE, &dwOldProt) && GetLastError() != 87)
		{
			mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
			return FALSE;
		}
		while (real_writenum < size + stringAppendZeroNUM)
		{
			if (WriteProcessMemory(hProcess, (LPVOID)((char*)address + real_writenum), buffer + real_writenum, size - real_writenum, &real_writenum) == FALSE)
			{
				mySetLastError(WriteDataFalse);
				return FALSE;
			}
		}
		if (!VirtualProtectEx(hProcess, address, size, dwOldProt, &dwNewProt) && GetLastError() != 87)
		{
			mySetLastError(FAIL_TO_CHANGE_PAGE_INF);
			return FALSE;
		}
		break;
	default:
		mySetLastError(WrongSearchType);
		return FALSE;
	}
	return TRUE;
}
