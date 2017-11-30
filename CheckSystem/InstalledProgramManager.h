#pragma once

#include <vector>

class ProgramInfo {
public:
    ProgramInfo(const CString& programName, const CString& versionString)
        : _programName(programName)
        , _versionString(versionString)
        , _found(false) {}

private:
    CString _programName;
    CString _versionString;
    bool _found;

public:
    void setFound() { _found = true; }

    const CString& programName() const { return _programName; }
    const CString& versionString() const { return _versionString; }
};

class InstalledProgramManager {
public:
    InstalledProgramManager();

private:
    enum {
        CCH_GUID_SIZE = 38
    };

    std::vector<ProgramInfo> _findingPrograms;

public:
    void init();

private:
    bool searchInstalledPrograms();

};
