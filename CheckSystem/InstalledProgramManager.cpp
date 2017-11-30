#include "stdafx.h"
#include "InstalledProgramManager.h"

#include <Msi.h>
#include <WinError.h>

InstalledProgramManager::InstalledProgramManager()
{
    init();
}

void InstalledProgramManager::init()
{
    _findingPrograms.clear();

    searchInstalledPrograms();
}

bool InstalledProgramManager::searchInstalledPrograms()
{
    // from : https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa368279(v=vs.85).aspx
    WCHAR wszProductCode[CCH_GUID_SIZE + 1] = {0};
    WCHAR wszAssignmentType[10] = {0};
    DWORD cchAssignmentType = 
            sizeof(wszAssignmentType)/sizeof(wszAssignmentType[0]);
    DWORD dwIndex = 0;

    LV_ITEM programItem;
    memset(&programItem, 0, sizeof(programItem));
    programItem.mask = LVIF_TEXT | LVIF_PARAM;
    programItem.iSubItem = 0;
    programItem.pszText = L"";

    DWORD cchProductName = MAX_PATH;
    WCHAR* lpProductName = new WCHAR[cchProductName];
    if (!lpProductName) {
        return false; // ERROR_OUTOFMEMORY
    }

    UINT uiStatus = ERROR_SUCCESS;

    // enumerate all visible products
    do {
        uiStatus = MsiEnumProducts(dwIndex, wszProductCode);
        if (ERROR_SUCCESS == uiStatus) {
            cchAssignmentType = sizeof(wszAssignmentType) / sizeof(wszAssignmentType[0]);
            BOOL fPerMachine = FALSE;
            BOOL fManaged = FALSE;

            // Determine assignment type of product
            // This indicates whether the product
            // instance is per-user or per-machine
            if (ERROR_SUCCESS ==
                MsiGetProductInfo(wszProductCode, INSTALLPROPERTY_ASSIGNMENTTYPE, wszAssignmentType, &cchAssignmentType)) {
                if (L'1' == wszAssignmentType[0]) {
                    fPerMachine = TRUE;
                }
            }
            else {
                // This halts the enumeration and fails. Alternatively the error
                // could be logged and enumeration continued for the
                // remainder of the products
                uiStatus = ERROR_FUNCTION_FAILED;
                break;
            }

            // determine the "managed" status of the product.
            // If fManaged is TRUE, product is installed managed
            // and runs with elevated privileges.
            // If fManaged is FALSE, product installation operations
            // run as the user.
            if (ERROR_SUCCESS != MsiIsProductElevated(wszProductCode, &fManaged)) {
                // This halts the enumeration and fails. Alternatively the error
                // could be logged and enumeration continued for the
                // remainder of the products
                uiStatus = ERROR_FUNCTION_FAILED;
                break;
            }

            // obtain the user friendly name of the product
            UINT uiReturn = MsiGetProductInfo(wszProductCode, INSTALLPROPERTY_PRODUCTNAME, lpProductName, &cchProductName);
            if (ERROR_MORE_DATA == uiReturn) {
                // try again, but with a larger product name buffer
                delete [] lpProductName;

                // returned character count does not include
                // terminating NULL
                ++cchProductName;

                lpProductName = new WCHAR[cchProductName];
                if (!lpProductName) {
                    uiStatus = ERROR_OUTOFMEMORY;
                    break;
                }

                uiReturn = MsiGetProductInfo(wszProductCode, INSTALLPROPERTY_PRODUCTNAME, lpProductName, &cchProductName);
            }

            if (ERROR_SUCCESS != uiReturn) {
                // This halts the enumeration and fails. Alternatively the error
                // could be logged and enumeration continued for the
                // remainder of the products
                uiStatus = ERROR_FUNCTION_FAILED;
                break;
            }

            CString programName(lpProductName);

            //int insertedIndex = _programList.InsertItem(&programItem);
            //_programList.SetItemText(insertedIndex, 0, lpProductName);

            // obtain the user friendly name of the product version
            uiReturn = MsiGetProductInfo(wszProductCode, INSTALLPROPERTY_VERSIONSTRING, lpProductName, &cchProductName);
            if (ERROR_MORE_DATA == uiReturn) {
                // try again, but with a larger product name buffer
                delete [] lpProductName;

                // returned character count does not include
                // terminating NULL
                ++cchProductName;

                lpProductName = new WCHAR[cchProductName];
                if (!lpProductName) {
                    uiStatus = ERROR_OUTOFMEMORY;
                    break;
                }

                uiReturn = MsiGetProductInfo(wszProductCode, INSTALLPROPERTY_VERSIONSTRING, lpProductName, &cchProductName);
            }

            if (ERROR_SUCCESS != uiReturn) {
                // This halts the enumeration and fails. Alternatively the error
                // could be logged and enumeration continued for the
                // remainder of the products
                uiStatus = ERROR_FUNCTION_FAILED;
                break;
            }

            CString versionString(lpProductName);
            
            ProgramInfo programInfo(programName, versionString);

            // output information
            CString str;
            str.AppendFormat(L" Product %s:\n", lpProductName);
            str.AppendFormat(L"\t%s\n", wszProductCode);
            str.AppendFormat(L"\tInstalled %s %s\n", fPerMachine ? L"per-machine" : L"per-user", fManaged ? L"managed" : L"non-managed");
            //wprintf(L" Product %s:\n", lpProductName);
            //wprintf(L"\t%s\n", wszProductCode);
            //            wprintf(L"\tInstalled %s %s\n", 
            //    fPerMachine ? L"per-machine" : L"per-user",
            //    fManaged ? L"managed" : L"non-managed");
            ::OutputDebugString(str);
        }
        dwIndex++;
    }
    while (ERROR_SUCCESS == uiStatus);

    if (lpProductName)
    {
        delete [] lpProductName;
        lpProductName = NULL;
    }

    return (ERROR_NO_MORE_ITEMS == uiStatus) ? true : false;

}
