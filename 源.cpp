#include <stdio.h>
#include <Windows.h>

typedef void(*PFN_DLL)();//����ָ��

int main(void) {
	//1.�������ǵ�DLL��������
	//����һ��ģ����
	//���:һ�������ֵ,ͨ�������ҵ���Ӧ����Դ
	//��Ҫ�ȿ������Լ���DLL
	HMODULE hDLL = LoadLibraryA("hackerDLL.dll");
	if (hDLL == NULL) {
		printf("����DLL�ļ�ʧ��!ʧ��ԭ��:[%d]\n",GetLastError());
		return 1;
	}
	
	//���ð�װ���ӵĺ���
	PFN_DLL HookStart = (PFN_DLL)GetProcAddress(
		hDLL,//DLL(��̬��)��ģ����
		"HookStart");//������ʹ��"�ַ���"!!
	PFN_DLL HookStop = (PFN_DLL)GetProcAddress(hDLL, "HookStop");
	HookStart();//��װ����

	system("pause");

	//ж�ع���
	HookStop();

	//�ͷ�ָ���Ķ�̬���ӿ�
	FreeLibrary(hDLL);
	
	system("pause");
	return 0;
}