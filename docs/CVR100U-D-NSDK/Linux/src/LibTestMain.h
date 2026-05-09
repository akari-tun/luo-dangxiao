#ifndef __LIBTESTMAIN_H__
#define __LIBTESTMAIN_H__

typedef int (*p_CVR_GetSAMID)(char *SAMID, int *length);
typedef int (*p_CVR_GetStatus)();
typedef int (*p_CVR_InitComm)(const char *path, int protocolType);
typedef int (*p_CVR_CloseComm)();   
typedef int (*p_CVR_AuthenticateForNoJudge)();
typedef int (*p_CVR_Read_Content)(int active);
typedef int (*p_GetPeopleName)(char *strTmp, int *strLen);
typedef int (*p_GetPeopleSex)(char *strTmp, int *strLen);
typedef int (*p_GetPeopleNation)(char *strTmp, int *strLen);
typedef int (*p_GetPeopleBirthday)(char *strTmp, int *strLen);
typedef int (*p_GetPeopleIDCode)(char *strTmp, int *strLen);
typedef int (*p_GetDepartment)(char *strTmp, int *strLen);
typedef int (*p_GetStartDate)(char *strTmp, int *strLen);
typedef int (*p_GetEndDate)(char *strTmp, int *strLen);
typedef int (*p_GetPeopleAddress)(char *strTmp, int *strLen);
typedef int (*p_GetBMPData)(unsigned char *pData, int * pLen); 
typedef int (*p_GetCertType)(unsigned char * strTmp, int *strLen);
typedef int (*p_GetPeopleOldIDCardNumber)(unsigned char * strTmp, int *strLen);
typedef int (*p_GetPeopleChineseName)(unsigned char * strTmp, int *strLen);
typedef int (*p_GetIssuesNum)(unsigned char * strTmp, int *strLen);

typedef int (*p_GetOldForeignerIDCode)(unsigned char * strTmp, int *strLen);

typedef int (*p_CVR_GetUID)(char *UID, int *length);

typedef int (*p_CVR_Get125KCardID)(unsigned char * FisCardID,int* FisCardIDLen);

void* handle_100ud = NULL;
p_CVR_GetSAMID CVR_GetSAMID = NULL;
p_CVR_GetStatus CVR_GetStatus = NULL;
p_CVR_InitComm CVR_InitComm = NULL;
p_CVR_CloseComm CVR_CloseComm = NULL;
p_CVR_AuthenticateForNoJudge CVR_AuthenticateForNoJudge = NULL;
p_CVR_Read_Content CVR_Read_Content = NULL;
p_GetPeopleName GetPeopleName = NULL;
p_GetPeopleSex GetPeopleSex = NULL;
p_GetPeopleNation GetPeopleNation = NULL;
p_GetPeopleBirthday GetPeopleBirthday = NULL;
p_GetPeopleIDCode GetPeopleIDCode = NULL;
p_GetDepartment GetDepartment = NULL;
p_GetStartDate GetStartDate = NULL;
p_GetEndDate GetEndDate = NULL;
p_GetPeopleAddress GetPeopleAddress = NULL;
p_GetBMPData GetBMPData = NULL;
p_GetCertType GetCertType = NULL;
p_GetPeopleChineseName GetPeopleChineseName = NULL;
p_GetIssuesNum GetIssuesNum = NULL;
p_GetPeopleOldIDCardNumber GetPeopleOldIDCardNumber = NULL;
p_CVR_GetUID CVR_GetUID = NULL;
p_GetOldForeignerIDCode GetOldForeignerIDCode = NULL;

p_CVR_Get125KCardID CVR_Get125KCardID = NULL;

#endif

