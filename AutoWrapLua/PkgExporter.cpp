#include "PkgExporter.h"
#include <list>
#include <shlwapi.h>

#define CommentLineFlag			_T("//")
#define CommentFlagL			_T("/*")
#define CommentFlagR			_T("*/")
#define ExportToLuaFlag			_T("//@ExportToLua")
#define BeginExportToLuaFlag	_T("//@BeginExportToLua")
#define EndExportToLuaFlag		_T("//@EndExportToLua")
#define LuaTemplateBindFlag		_T("//@TOLUA_TEMPLATE_BIND")
#define GrapFlag				_T(" \t\r\n")
#define SpaceTabFlag			_T(" \t")
#define ReturnNewlineFlag		_T("\r\n")
#define ReturnFlag				_T('\r')
#define NewlineFlag				_T('\n')
#define LineEFlag				_T("\r\n{}/\\")
#define LineNFlag				_T("\n")
#define FunctionBodySFlag		_T(";{")
#define ClassStructEFlag		_T("};")
#define DoubleColonFlag			_T("::")
#define NullCharFlag			_T('\0')
#define MatchAllFileDirFlag		_T("\\*.*")
#define PkgFileExtFlag			_T(".pkg")
#define PfileFlag				_T("$pfile")

#define ClassFlag				_T("class")
#define StructFlag				_T("struct")
#define PublicFlag				_T("public")
#define PrivateFlag				_T("private")
#define ProtectedFlag			_T("protected")
#define IncludeFlag				_T("#include")
#define NamespaceFlag			_T("namespace")
#define TemplateFlag			_T("template")
#define ExternFlag				_T("extern")
#define EnumFlag				_T("enum")
#define CFlag					_T("cC")

#define BracketFlagL			_T('<')
#define BracketFlagR			_T('>')
#define ParentheseFlagL			_T('(')
#define ParentheseFlagR			_T(')')
#define BraceFlagL				_T('{')
#define BraceFlagR				_T('}')
#define SemicolonFlag			_T(';')
#define ColonFlag				_T(':')
#define CommaFlag				_T(',')
#define TabFlag					_T('\t')
#define DotFlag					_T('.')
#define SpaceFlag				_T(' ')
#define DoubleQuotatioMarkFlag	_T('\"')
#define SlashFlag				_T('/')
#define BackslashFlag			_T('\\')
#define PoundSignFlag			_T('#')
#define DollarFlag				_T('$')

#define SourceExtFlags			{_T(".h"), _T(".hpp"), _T(".cpp"), _T(".ipp"), _T(".inc")}
#define HeaderExtFlags			{_T(".h"), _T(".hpp")}
#define ClassStructAccessFlag	{PublicFlag, PrivateFlag, ProtectedFlag}


INT  g_genPkgfileCount = 0;
INT  g_braceLayerCount = 0;
BOOL g_exportToLua = FALSE;
BOOL g_fourceExportToLua = FALSE;
LPCTSTR g_exportCfgFilename = NULL;
LPCTSTR g_workHomeDir = NULL;
LPCTSTR	g_workPrjDir = NULL;
LPCTSTR	g_pch = NULL;
LPCTSTR g_toluaAddPath = NULL;
LPCTSTR g_PkgPath = NULL; 


BOOL TryPropertyBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst);
BOOL TryClassStructBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst);
BOOL TryTemplateBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst);
BOOL TryIncludeBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst);
BOOL TryFunctionBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst);
BOOL TryEnumBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst);
BOOL TryAllBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst);

BOOL IsClassStructAccessDim(LPCTSTR pData);
BOOL IsLineJointDim(LPCTSTR pData);
BOOL IsAFunctionDim(LPCTSTR pData);
BOOL IsAClassStructDim(LPCTSTR pData);

inline BOOL IsFileDirExists(LPCTSTR pstr);

inline BOOL StartsWith(LPCTSTR psrc, TCHAR elem);
inline BOOL StartsWith(LPCTSTR psrc, LPCTSTR pdst);
inline BOOL StartsWith(LPCTSTR psrc, LPCTSTR MatchFlags[], INT arraySize);

inline BOOL EndsWith(LPCTSTR psrc, TCHAR elem);
inline BOOL EndsWith(LPCTSTR psrc, LPCTSTR pdst);
inline BOOL EndsWith(LPCTSTR psrc, LPCTSTR MatchFlags[], INT arraySize);

inline BOOL IsMatchFlag(TCHAR elem, LPCTSTR pFlags);
CString GetFilenameExt(LPCTSTR pstrFilename);
CString GetFilenameWithoutExt(LPCTSTR pstrFilename);
CString GetFileDir(LPCTSTR pstrFilename);

inline INT Append(LPCTSTR pData, INT indexStart, INT indexEnd, LPTSTR pOutData);
inline INT Append(LPCTSTR pData, LPTSTR pOutData);
inline INT Append(TCHAR elem, LPTSTR pOutData);
inline INT Append(TCHAR elem, INT count, LPTSTR pOutData);

INT FindFlag(LPCTSTR pData, INT index, TCHAR Flag);
INT FindFlag(LPCTSTR pData, INT index, LPCTSTR Flag, INT maxIndex = -1);
INT FindFlag(LPCTSTR pData, INT index, TCHAR FlagL, TCHAR FlagR);

VOID LeftTrim(LPCTSTR pData, INT& index);
VOID RightTrim(LPCTSTR pData, INT& indexR, INT len = -1);

inline INT GetLineLen(LPCTSTR pData, BOOL isLabelLine = TRUE);
inline INT GetWorldLen(LPCTSTR pData, BOOL skipSlashFlag = TRUE);
inline INT GetLabelLen(LPCTSTR pData);


//----------------------------------------------------------------------------------------------------------------------------------
template<typename T>
class PushPopFlag
{
public:
	PushPopFlag(T& data) : m_data(data), m_preData(data) {}
	~PushPopFlag() { m_data = m_preData; }
private:
	T& m_data;
	T  m_preData;
};


inline BOOL IsFileDirExists(LPCTSTR pstr)
{
	return _taccess(pstr, 0) == 0;
}

BOOL IsFileExists(LPCTSTR pstr)
{
	HANDLE hFile = CreateFile(pstr, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return FALSE;
	}
	CloseHandle(hFile);
	return TRUE;
}

BOOL IsFileNewer(LPCTSTR newFilename, LPCTSTR oldFilename)
{
	WIN32_FILE_ATTRIBUTE_DATA  attrNew;

	if (!GetFileAttributesEx(newFilename, GetFileExInfoStandard, &attrNew) || attrNew.dwFileAttributes == DWORD(-1))
	{
		return FALSE;
	}

	WIN32_FILE_ATTRIBUTE_DATA  attrOld;
	if (!GetFileAttributesEx(oldFilename, GetFileExInfoStandard, &attrOld) || attrOld.dwFileAttributes == DWORD(-1))
	{
		return TRUE;
	}

	return *(UINT64*)&attrNew.ftLastWriteTime > *(UINT64*)&attrOld.ftLastWriteTime;
}

BOOL IsEqual(LPCTSTR psrc1, LPCTSTR pstr2)
{
	INT len = _tcslen(psrc1);
	if (_tcslen(pstr2) != len)
	{
		return FALSE;
	}

	for (INT i = 0; i < len; ++i)
	{
		if (psrc1[i] != pstr2[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}

VOID Replace(LPTSTR pSrc, TCHAR cOld, TCHAR cNew)
{
	for (INT i = 0, len = _tcslen(pSrc); i < len; ++i)
	{
		if (pSrc[i] == cOld)
		{
			pSrc[i] = cNew;
		}
	}
}

VOID FilterPath(LPTSTR path)
{
	Replace(path, SlashFlag, BackslashFlag);
}

inline BOOL StartsWith(LPCTSTR psrc, TCHAR elem)
{
	return *psrc == elem;
}

inline BOOL StartsWith(LPCTSTR psrc, LPCTSTR pdst)
{
	while(*psrc != NullCharFlag && *pdst != NullCharFlag && *psrc == *pdst)
	{
		++psrc;
		++pdst;
	}
	return *pdst == NullCharFlag;
}

inline BOOL StartsWith(LPCTSTR psrc, LPCTSTR MatchFlags[], INT arraySize)
{
	for (INT i = 0; i < arraySize; ++i)
	{
		if (StartsWith(psrc, MatchFlags[i]))
		{
			return TRUE;
		}
	}
	return FALSE;
}

inline BOOL EndsWith(LPCTSTR psrc, TCHAR elem)
{
	INT strLen = _tcslen(psrc);
	return strLen > 0 && psrc[strLen - 1] == elem;
}

inline BOOL EndsWith(LPCTSTR psrc, LPCTSTR pdst)
{
	INT strLen = _tcslen(psrc);
	INT dstLen = _tcslen(pdst);

	return (strLen < dstLen) ? FALSE : StartsWith(psrc + strLen - dstLen, pdst);
}

inline BOOL EndsWith(LPCTSTR psrc, LPCTSTR MatchFlags[], INT arraySize)
{
	for (INT i = 0; i < arraySize; ++i)
	{
		if (EndsWith(psrc, MatchFlags[i]))
		{
			return TRUE;
		}
	}
	return FALSE;
}

inline BOOL IsMatchFlag(TCHAR elem, LPCTSTR pFlags)
{
	for (INT i = 0, cout = _tcslen(pFlags); i < cout; ++i)
	{
		if (elem == pFlags[i])
		{
			return TRUE;
		}
	}
	return FALSE;
}

inline BOOL IsGrapFlag(TCHAR elem)
{
	return IsMatchFlag(elem, GrapFlag);
}

inline BOOL IsNewLineFlag(TCHAR elem)
{
	return IsMatchFlag(elem, ReturnNewlineFlag);
}

inline BOOL IsSpaceTabFlag(TCHAR elem)
{
	return IsMatchFlag(elem, SpaceTabFlag);
}

inline BOOL IsSourceFile(LPCTSTR pstrFilename)
{
	LPCTSTR sourceExtNames[] = SourceExtFlags;
	return EndsWith(pstrFilename, sourceExtNames, ArraySize(sourceExtNames));
}

inline BOOL IsHeaderFile(LPCTSTR pstrFilename)
{
	LPCTSTR headerExtNames[] = HeaderExtFlags;
	return EndsWith(pstrFilename, headerExtNames, ArraySize(headerExtNames));
}

VOID DebugOutput(LPCTSTR fmt, ...)
{
	va_list argList;
	va_start(argList, fmt);
	TCHAR msg[512] = {0};
	_stprintf(msg, fmt, argList);
	va_end(argList);
	OutputDebugString(msg);
	OutputDebugString(_T("\n"));
} 

CString GetFilenameExt(LPCTSTR pstrFilename)
{
	CString extName;
	int len = pstrFilename == NULL ? 0 : _tcslen(pstrFilename);
	for (int i = len - 1; i >= 0; --i)
	{
		if (pstrFilename[i] == DotFlag)
		{
			extName.Preallocate(len - i - 1);
			for (++i; i < len; ++i)
			{
				extName.AppendChar(pstrFilename[i]);
			}
			break;
		}
	}
	return extName;
}

CString GetFilenameWithoutExt(LPCTSTR pstrFilename)
{
	CString filenameWithoutExt;
	INT len = pstrFilename == NULL ? 0 : _tcslen(pstrFilename);
	for (INT i = len - 1; i >= 0; --i)
	{
		if (pstrFilename[i] == DotFlag)
		{
			filenameWithoutExt.Preallocate(i);
			for (INT j = 0; j < i; ++j)
			{
				filenameWithoutExt.AppendChar(pstrFilename[j]);
			}
			break;
		}
	}
	return filenameWithoutExt;
}

CString GetFileDir(LPCTSTR pstrFilename)
{
	CString fileDir;
	LPTSTR fileName = StrRChr(pstrFilename, NULL, BackslashFlag);
	if (fileName != NULL)
	{
		INT len = fileName - pstrFilename + 1;
		fileDir.SetString(pstrFilename, len);
	}
	return fileDir;
}

inline INT Append(LPCTSTR pData, INT indexStart, INT indexEnd, LPTSTR pOutData)
{
	INT len = indexEnd - indexStart;
	while (indexStart <indexEnd)
	{
		*pOutData++ = pData[indexStart++];
	}
	*pOutData = NullCharFlag;
	return len;
}

inline INT Append(LPCTSTR pData, LPTSTR pOutData)
{
	INT srcLen = _tcslen(pData);
	Append(pData, 0, srcLen, pOutData);
	return srcLen;
}

inline INT Append(TCHAR elem, LPTSTR pOutData)
{
	*pOutData++ = elem;
	*pOutData = NullCharFlag;
	return 1;
}

inline INT Append(TCHAR elem, INT count, LPTSTR pOutData)
{
	for (INT i = 0; i < count; ++i)
	{
		*pOutData++ = elem;
	}
	*pOutData = NullCharFlag;
	return count;
}

INT FindFlag(LPCTSTR pData, INT index, TCHAR Flag)
{
	while(pData[index] != NullCharFlag)
	{
		if (pData[index] == Flag)
		{
			return index;
		}
		++index;
	}
	return -1;
}

INT FindFlag(LPCTSTR pData, INT index, LPCTSTR Flag, INT maxIndex)
{
	maxIndex = (maxIndex == -1) ? _tcslen(pData) : maxIndex;
	for (INT i = index, iMax = maxIndex - _tcslen(Flag); i <= iMax; ++i)
	{
		if (StartsWith(pData + i, Flag))
		{
			return i;
		}
	}
	return -1;
}

INT FindFlag(LPCTSTR pData, INT index, TCHAR FlagL, TCHAR FlagR)
{
	INT braceLayer = 0;
	if (pData[index] == FlagL)
	{
		while (pData[++index] != NULL)
		{
			if (pData[index] == FlagR)
			{
				if (braceLayer-- == 0)
				{
					return index;
				}
			}
			else if (pData[index] == FlagL)
			{
				++braceLayer;
			}
		}
	}
	else if (pData[index] == FlagR)
	{
		while(--index >=0)
		{
			if (pData[index] == FlagL)
			{
				if (braceLayer-- == 0)
				{
					return index;
				}
			}
			else if (pData[index] == FlagR)
			{
				++braceLayer;
			}
		}
	}
	return -1;
}

VOID LeftTrim(LPCTSTR pData, INT& index)
{
	while(pData[index] != NullCharFlag)
	{
		if (IsGrapFlag(pData[index]))
		{
			++index;
		}
		else
		{
			break;
		}
	}
}

VOID RightTrim(LPCTSTR pData, INT& indexR, INT len)
{
	INT dataLen = len > 0 ? len : _tcslen(pData);
	for (indexR = dataLen - 1; indexR >= 0; --indexR)
	{
		if (!IsGrapFlag(pData[indexR]))
		{
			break;
		}
	}
	++indexR;
}

inline INT GetLineLen(LPCTSTR pData, BOOL isLabelLine)
{
	INT index = 0;
	for (;;)
	{
		TCHAR elem = pData[index];
		if (elem == NullCharFlag || IsMatchFlag(elem, isLabelLine ? LineEFlag : LineNFlag))
		{
			if (index > 0 && isLabelLine)
			{
				RightTrim(pData, index, index);
			}
			break;
		}
		else
		{
			++index;
		}
	}
	return index;
}

inline INT GetWorldLen(LPCTSTR pData, BOOL skipSlashFlag)
{
	INT index = 0;
	while(pData[index] != NullCharFlag && !IsGrapFlag(pData[index]) && (!skipSlashFlag || pData[index] !=  SlashFlag))
	{
		++index;
	}
	return index;
}

inline INT GetLabelLen(LPCTSTR pData)
{
	INT index = 0;
	for(;;)
	{
		TCHAR elem = pData[index];
		if ( (_T('a') <= elem && elem <= _T('z')) ||
			(_T('A') <= elem && elem <= _T('Z')) ||
			(_T('0') <= elem && elem <= _T('9')) ||
			elem == _T('_'))
		{
			++index;
			continue;
		}
		else
		{
			break;
		}
	}
	return index;
}

BOOL SkipComment(LPCTSTR pData, INT& index)
{
	g_exportToLua = FALSE;
	while(pData[index] != NullCharFlag)
	{
		LeftTrim(pData, index);
		if (StartsWith(pData + index, ExportToLuaFlag) && IsGrapFlag(pData[index + _tcslen(ExportToLuaFlag)]))
		{
			index += _tcslen(ExportToLuaFlag);
			g_exportToLua = TRUE;
		}
		else if (StartsWith(pData + index, BeginExportToLuaFlag) && IsGrapFlag(pData[index + _tcslen(BeginExportToLuaFlag)]))
		{
			index += _tcslen(BeginExportToLuaFlag);
			g_fourceExportToLua = TRUE;
		}
		else if (StartsWith(pData + index, EndExportToLuaFlag) && IsGrapFlag(pData[index + _tcslen(EndExportToLuaFlag)]))
		{
			index += _tcslen(EndExportToLuaFlag);
			g_fourceExportToLua = FALSE;
		} 
		else if (StartsWith(pData + index, CommentLineFlag))
		{
			TCHAR elem;
			while((elem = pData[index]) != NullCharFlag)
			{
				++index;
				if (elem == NewlineFlag)
				{
					break;
				}
			}
		}
		else if (StartsWith(pData + index, CommentFlagL))
		{
			index += _tcslen(CommentFlagL);
			while(pData[index] != NullCharFlag)
			{
				if(StartsWith(pData + index, CommentFlagR))
				{
					index += _tcslen(CommentFlagR);
					break;
				}
				++index;
			}
		}
		else
		{
			break;
		}
	}
	return TRUE;
}

BOOL SkipStatement(LPCTSTR pData, INT& index)
{
	if (StartsWith(pData + index, PoundSignFlag))
	{
		for (;;)
		{
			INT lineLen = GetLineLen(pData + index);
			if (lineLen == 0)
			{
				break;
			}
			index += lineLen;
			INT preIndex = index;
			SkipComment(pData, index);
			if (IsLineJointDim(pData + index))
			{
				++index;
				INT newlineCount = 0;
				while(pData[index] != NullCharFlag)
				{
					if (pData[index] == ReturnFlag || IsSpaceTabFlag(pData[index]))
					{
						++index;
						continue;
					}
					if (pData[index] == NewlineFlag)
					{
						++index;
						++newlineCount;
						continue;
					}
					break;
				}
				if (newlineCount == 1)
				{
					continue;
				}
			}

			index = preIndex;
			break;
		}
	}
	else if (IsLineJointDim(pData + index))
	{
		++index;
	}
	else if(!StartsWith(pData + index, CommentLineFlag) && !StartsWith(pData + index, CommentFlagL))
	{
		BOOL isMacroDim = FALSE;

		INT parentheseFlagLIndex = FindFlag(pData, index, ParentheseFlagL);
		INT semicolonFlagIndex = FindFlag(pData, index, SemicolonFlag);
		INT braceFlagLIndex = FindFlag(pData, index, BraceFlagL);

		if (parentheseFlagLIndex != -1 && (parentheseFlagLIndex < semicolonFlagIndex || semicolonFlagIndex == -1) 
			&& (parentheseFlagLIndex < braceFlagLIndex || braceFlagLIndex == -1))
		{
			INT parentheseFlagRIndex = FindFlag(pData, parentheseFlagLIndex, ParentheseFlagL, ParentheseFlagR);
			if (parentheseFlagRIndex != -1)
			{
				INT nxtFlagIndex = parentheseFlagRIndex + 1;
				SkipComment(pData, nxtFlagIndex);
				if (!StartsWith(pData + nxtFlagIndex, SemicolonFlag) && !StartsWith(pData + nxtFlagIndex, BraceFlagL))
				{
					index = parentheseFlagRIndex + 1;
					isMacroDim = TRUE;
				}
			}
		}

		if (!isMacroDim)
		{
			if (semicolonFlagIndex == -1 && braceFlagLIndex == -1)
			{
				return FALSE;
			}

			if (braceFlagLIndex != -1 && (semicolonFlagIndex == -1 || braceFlagLIndex < semicolonFlagIndex))
			{
				INT braceFlagRIndex = FindFlag(pData, braceFlagLIndex, BraceFlagL, BraceFlagR);
				if (braceFlagRIndex == -1)
				{
					return FALSE;
				}
				index = braceFlagRIndex + 1;
			}
			else
			{
				index = semicolonFlagIndex + 1;
			}
		}
		
	}
	SkipComment(pData, index);
	return TRUE;
}

BOOL IsClassStructAccessDim(LPCTSTR pData)
{
	LPCTSTR classStructAccessFlags[] = ClassStructAccessFlag;
	return StartsWith(pData, classStructAccessFlags, ArraySize(classStructAccessFlags));
}

BOOL IsLineJointDim(LPCTSTR pData)
{
	return StartsWith(pData, BackslashFlag) && IsNewLineFlag(pData[1]);
}

BOOL IsAFunctionDim(LPCTSTR pData)
{
	if(IsClassStructAccessDim(pData))
	{
		return FALSE;
	}
	INT parentheseFlagLIndex = FindFlag(pData, 0, ParentheseFlagL);
	if (parentheseFlagLIndex < 1)
	{
		return FALSE;
	}

	INT colonFlagIndex = FindFlag(pData, 0, SemicolonFlag);
	if (colonFlagIndex != -1 && colonFlagIndex < parentheseFlagLIndex)
	{
		return FALSE;
	}

	INT parentheseFlagRIndex = FindFlag(pData, parentheseFlagLIndex, ParentheseFlagL, ParentheseFlagR);
	if (parentheseFlagRIndex == -1)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL IsAClassStructDim(LPCTSTR pData)
{
	INT index = 0;
	if (StartsWith(pData, TemplateFlag))
	{
		index = FindFlag(pData, 0, BracketFlagL);
		if (index == -1)
		{
			return FALSE;
		}
		index = FindFlag(pData, index, BracketFlagL, BracketFlagR);
		if (index == -1)
		{
			return FALSE;
		}
		++index;
		BOOL preExportToLua = g_exportToLua;
		BOOL preForceExportToLua = g_fourceExportToLua;
		SkipComment(pData, index);
		g_exportToLua = preExportToLua;
		g_fourceExportToLua = preForceExportToLua;
	}
	BOOL isAClass = StartsWith(pData + index, ClassFlag);
	BOOL isAStruct = StartsWith(pData + index, StructFlag);
	return isAClass || isAStruct;
}

BOOL TryPropertyBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst)
{
	INT curSrcIndex = isrc;
	INT curDstIndex = idst;

	INT semicolonFlagIndex = FindFlag(psrcData, curSrcIndex, SemicolonFlag);
	if (semicolonFlagIndex == -1)
	{
		pdstData[idst] = NullCharFlag;
		return TRUE;
	}

	if (semicolonFlagIndex == curSrcIndex)
	{
		++isrc;
		pdstData[idst] = NullCharFlag;
		return TRUE;
	}

	curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
	curDstIndex += Append(psrcData, curSrcIndex, semicolonFlagIndex + 1, pdstData + curDstIndex);
	curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

	curSrcIndex = semicolonFlagIndex + 1;
	SkipComment(psrcData, curSrcIndex);

	isrc = curSrcIndex;
	idst = curDstIndex;

	return TRUE;

}

BOOL TryTemplateBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst)
{
	if (!StartsWith(psrcData + isrc, TemplateFlag))
	{
		pdstData[idst] = NullCharFlag;
		return TRUE;
	}
	INT curSrcIndex = isrc;
	INT curDstIndex = idst;

	curSrcIndex += _tcslen(TemplateFlag);
	SkipComment(psrcData, curSrcIndex);
	INT bracketFlagRIndex = FindFlag(psrcData, curSrcIndex, BracketFlagL, BracketFlagR);
	if (bracketFlagRIndex == -1)
	{
		return FALSE;
	}

	curSrcIndex = bracketFlagRIndex + 1;
	SkipComment(psrcData, curSrcIndex);

	isrc = curSrcIndex;
	idst = curDstIndex;

	return TRUE;
}

BOOL TryIncludeBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst)
{
	for (;;)
	{
		if (!StartsWith(psrcData + isrc, IncludeFlag))
		{
			pdstData[idst] = NullCharFlag;
			return TRUE;
		}

		INT includeFlagLen = _tcslen(IncludeFlag);
		INT curSrcIndex = isrc + includeFlagLen;
		INT curDstIndex = idst;

		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
		curDstIndex += Append(DollarFlag, pdstData + curDstIndex);
		curDstIndex += Append(psrcData, isrc, curSrcIndex, pdstData + curDstIndex);
		curDstIndex += Append(SpaceFlag, pdstData + curDstIndex);
		SkipComment(psrcData, curSrcIndex);

		TCHAR FlagR = NullCharFlag;
		if (StartsWith(psrcData + curSrcIndex, BracketFlagL))
		{
			FlagR = BracketFlagR;
		}
		else if (StartsWith(psrcData + curSrcIndex, DoubleQuotatioMarkFlag))
		{
			FlagR = DoubleQuotatioMarkFlag;
		}
		else
		{
			return FALSE;
		}

		INT flagRIndex = FindFlag(psrcData, curSrcIndex + 1, FlagR);
		if (flagRIndex == -1)
		{
			return FALSE;
		}
		curDstIndex += Append(psrcData, curSrcIndex, flagRIndex + 1, pdstData + curDstIndex);
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

		curSrcIndex = flagRIndex + 1;
		SkipComment(psrcData, curSrcIndex);

		isrc = curSrcIndex;
		idst = curDstIndex;
	}
	return TRUE;
}

BOOL TryFunctionBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst)
{
	for (;g_exportToLua || g_fourceExportToLua;)
	{
		if (!IsAFunctionDim(psrcData + isrc))
		{
			pdstData[idst] = NullCharFlag;
			return TRUE;
		}

		INT curSrcIndex = isrc;
		INT curDstIndex = idst;

		curDstIndex += Append(NewlineFlag, pdstData + curDstIndex);

		if (!TryTemplateBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
		{
			return FALSE;
		}

		INT parentheseFlagLIndex = FindFlag(psrcData, curSrcIndex, ParentheseFlagL);
		if (parentheseFlagLIndex == -1)
		{
			return FALSE;
		}

		INT parentheseFlagRIndex = FindFlag(psrcData, parentheseFlagLIndex, ParentheseFlagL, ParentheseFlagR);
		INT nxtParentheseFlagIndex = parentheseFlagRIndex + 1;
		SkipComment(psrcData, nxtParentheseFlagIndex);
		if (StartsWith(psrcData + nxtParentheseFlagIndex, ParentheseFlagL))
		{
			parentheseFlagRIndex = FindFlag(psrcData, nxtParentheseFlagIndex, ParentheseFlagL, ParentheseFlagR);
		}

		if (parentheseFlagRIndex <= parentheseFlagLIndex)
		{
			return FALSE;
		}

		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);

		INT semicolonFlagIndex = FindFlag(psrcData, parentheseFlagRIndex, SemicolonFlag);
		INT braceFlagLIndex = FindFlag(psrcData, parentheseFlagRIndex, BraceFlagL);

		INT endFunctionDimIndex = ((braceFlagLIndex != -1 && braceFlagLIndex < semicolonFlagIndex) || semicolonFlagIndex == -1) ? braceFlagLIndex : semicolonFlagIndex;

		if (endFunctionDimIndex == -1)
		{
			return FALSE;
		}

		INT curIndex = endFunctionDimIndex;
		while(curIndex > parentheseFlagRIndex)
		{
			if (psrcData[curIndex] == ColonFlag)
			{
				break;
			}
			--curIndex;
		}
		endFunctionDimIndex = (curIndex > parentheseFlagRIndex) ? curIndex : endFunctionDimIndex;

		while(curSrcIndex < endFunctionDimIndex)
		{
			INT worldLen = min(endFunctionDimIndex - curSrcIndex, GetWorldLen(psrcData + curSrcIndex, FALSE));
			curDstIndex += Append(psrcData, curSrcIndex, curSrcIndex + worldLen, pdstData + curDstIndex);
			curDstIndex += Append(SpaceFlag, pdstData + curDstIndex);
			curSrcIndex += worldLen;
			SkipComment(psrcData, curSrcIndex);
		}
		RightTrim(pdstData, curDstIndex, curDstIndex);
		if (curSrcIndex == endFunctionDimIndex)
		{
			curDstIndex += Append(SemicolonFlag, pdstData + curDstIndex);
		}
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

		if (StartsWith(psrcData + curSrcIndex, BraceFlagL))
		{
			INT braceFlagRIndex = FindFlag(psrcData, curSrcIndex, BraceFlagL, BraceFlagR);
			if (braceFlagRIndex == -1)
			{
				return FALSE;
			}
			curSrcIndex = braceFlagRIndex;
		}
		else
		{
			curSrcIndex = endFunctionDimIndex;
		}

		++curSrcIndex;
		SkipComment(psrcData, curSrcIndex);

		isrc = curSrcIndex;
		idst = curDstIndex;
	}
	return TRUE;
}

BOOL TryEnumBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst)
{
	for (;g_exportToLua;)
	{
		if (!StartsWith(psrcData + isrc, EnumFlag))
		{
			pdstData[idst] = NullCharFlag;
			return TRUE;
		}

		INT curSrcIndex = isrc;
		INT curDstIndex = idst;
		INT exportToLuaCount = 0;

		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
		curDstIndex += Append(EnumFlag, pdstData + curDstIndex);
		curDstIndex += Append(SpaceFlag, pdstData + curDstIndex);

		curSrcIndex += _tcslen(EnumFlag);
		SkipComment(psrcData, curSrcIndex);

		INT labelLen = GetLabelLen(psrcData + curSrcIndex);
		curDstIndex += Append(psrcData, curSrcIndex, curSrcIndex + labelLen, pdstData + curDstIndex);
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

		curSrcIndex += labelLen;
		SkipComment(psrcData, curSrcIndex);

		if (StartsWith(psrcData + curSrcIndex, SemicolonFlag))
		{
			++curSrcIndex;
			SkipComment(psrcData, curSrcIndex);
			isrc = curSrcIndex;
			pdstData[idst] = NullCharFlag;
			return TRUE;
		}

		INT braceFlagRIndex = FindFlag(psrcData, curSrcIndex, BraceFlagL, BraceFlagR);
		if (braceFlagRIndex == -1)
		{
			return FALSE;
		}

		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
		curDstIndex += Append(BraceFlagL, pdstData + curDstIndex);
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
		++g_braceLayerCount;
		{
			PushPopFlag<BOOL> pushPopForceExp(g_fourceExportToLua);
			++curSrcIndex;
			SkipComment(psrcData, curSrcIndex);
			while(curSrcIndex < braceFlagRIndex)
			{
				INT recordLen = GetLineLen(psrcData + curSrcIndex);
				if (recordLen == 0)
				{
					return FALSE;
				}
				curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
				curDstIndex += Append(psrcData, curSrcIndex, curSrcIndex + recordLen, pdstData + curDstIndex);
				curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
				++exportToLuaCount;
				curSrcIndex += recordLen;
				SkipComment(psrcData, curSrcIndex);
			}
		}
		--g_braceLayerCount;
		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
		curDstIndex += Append(BraceFlagR, pdstData + curDstIndex);
		curDstIndex += Append(SemicolonFlag, pdstData + curDstIndex);
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

		if (exportToLuaCount == 0)
		{
			pdstData[idst] = NullCharFlag;
			curDstIndex = idst;
		}

		curSrcIndex = braceFlagRIndex + 1;
		INT semicolonFlagIndex = FindFlag(psrcData, curSrcIndex, SemicolonFlag);
		if (semicolonFlagIndex == -1)
		{
			return FALSE;
		}
		curSrcIndex = semicolonFlagIndex + 1;
		SkipComment(psrcData, curSrcIndex);

		isrc = curSrcIndex;
		idst = curDstIndex;
	}
	return TRUE;
}

BOOL TryClassStructBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst)
{
	for (;;)
	{
		if (!IsAClassStructDim(psrcData + isrc))
		{
			pdstData[idst] = NullCharFlag;
			return TRUE;
		}

		INT curSrcIndex = isrc;
		INT curDstIndex = idst;
		INT exportToLuaCount = 0;
		INT publicFlagCount = 0;

		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
		if (!TryTemplateBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
		{
			return FALSE;
		}

		BOOL isAClass  = StartsWith(psrcData + curSrcIndex, ClassFlag);
		BOOL isAStruct = StartsWith(psrcData + curSrcIndex, StructFlag);
		if (!isAClass && !isAStruct)
		{
			pdstData[idst] = NullCharFlag;
			return TRUE;
		}

		INT semicolonFlagIndex = FindFlag(psrcData, curSrcIndex, SemicolonFlag);
		if (semicolonFlagIndex == -1)
		{
			return FALSE;
		}
		INT braceFlagLIndex = FindFlag(psrcData, curSrcIndex, BraceFlagL);
		if (braceFlagLIndex == -1 || braceFlagLIndex > semicolonFlagIndex)
		{
			isrc = semicolonFlagIndex + 1;
			SkipComment(psrcData, isrc);
			pdstData[idst] = NullCharFlag;
			return TRUE;
		}
		INT braceFlagRIndex = FindFlag(psrcData, braceFlagLIndex, BraceFlagL, BraceFlagR);
		if (braceFlagRIndex == -1)
		{
			return FALSE;
		}

		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);

		INT wordCount = 0;
		INT nxtIndex = curSrcIndex;
		INT endIndex = FindFlag(psrcData, nxtIndex, ColonFlag);
		if (endIndex == -1 || endIndex > braceFlagLIndex)
		{
			endIndex = braceFlagLIndex;
		}
		endIndex -= 1;
		while(nxtIndex < endIndex)
		{
			SkipComment(psrcData, nxtIndex);
			if (nxtIndex >= endIndex)
			{
				break;
			}
			INT wordLen = GetWorldLen(psrcData + nxtIndex);
			if (wordLen > 0)
			{
				nxtIndex += wordLen;
				++wordCount;
			}
			else
			{
				break;
			}
		}

		if (wordCount > 2)
		{
			INT wordLen = GetWorldLen(psrcData + curSrcIndex);
			curDstIndex += Append(psrcData, curSrcIndex, curSrcIndex + wordLen, pdstData + curDstIndex);
			curDstIndex += Append(SpaceFlag, pdstData + curDstIndex);
			curSrcIndex += wordLen;
			SkipComment(psrcData, curSrcIndex);
			wordLen = GetWorldLen(psrcData + curSrcIndex);
			curSrcIndex += wordLen;
		}

		SkipComment(psrcData, curSrcIndex);
		wordCount = 1;
		while (curSrcIndex < braceFlagLIndex)
		{
			++wordCount;
			INT labelLen = GetWorldLen(psrcData + curSrcIndex);
			TCHAR lstElem = labelLen > 1 ? psrcData[curSrcIndex + labelLen - 1] : NullCharFlag;
			if (lstElem == ColonFlag || lstElem == CommaFlag)
			{
				--labelLen;
			}
			if (psrcData[curSrcIndex] == ColonFlag || psrcData[curSrcIndex] == CommaFlag)
			{
				wordCount = 0;
			}
			BOOL isPrivate = (labelLen == _tcslen(PrivateFlag) && StartsWith(psrcData + curSrcIndex, PrivateFlag));
			BOOL isProtected = (labelLen == _tcslen(ProtectedFlag) && StartsWith(psrcData + curSrcIndex, ProtectedFlag));
			BOOL isPublic = (labelLen == _tcslen(PublicFlag) && StartsWith(psrcData + curSrcIndex, PublicFlag)) || isAStruct;
			if (wordCount == 1 && (isPrivate || isProtected || !isPublic))
			{
				curSrcIndex = FindFlag(psrcData, curSrcIndex, CommaFlag) + 1;
				if (curSrcIndex == 0)
				{
					break;
				}
				wordCount = 0;
			}
			else
			{
				curDstIndex += Append(psrcData, curSrcIndex, curSrcIndex + labelLen, pdstData + curDstIndex);
				curDstIndex += Append(SpaceFlag, pdstData + curDstIndex);
				curSrcIndex += labelLen;
			}
			SkipComment(psrcData, curSrcIndex);
		}

		while(curDstIndex > 1)
		{
			TCHAR lstElem = pdstData[curDstIndex - 1];
			if (lstElem == ColonFlag || lstElem == CommaFlag || lstElem == SpaceFlag)
			{
				--curDstIndex;
			}
			else
			{
				break;
			}
		}
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
		curDstIndex += Append(BraceFlagL, pdstData + curDstIndex);
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

		++g_braceLayerCount;
		{
			PushPopFlag<BOOL> pushPopForceExp(g_fourceExportToLua);
			curSrcIndex = braceFlagLIndex + 1;
			LeftTrim(psrcData, curSrcIndex);
			if (StartsWith(psrcData + curSrcIndex, LuaTemplateBindFlag))
			{
				INT lineLen = GetLineLen(psrcData + curSrcIndex, FALSE);
				curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
				curDstIndex += Append(psrcData, curSrcIndex + 3, curSrcIndex + lineLen, pdstData + curDstIndex);
				curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
				curSrcIndex += lineLen;
			}
			SkipComment(psrcData, curSrcIndex);
			BOOL isPublic = isAStruct;
			while(curSrcIndex < braceFlagRIndex)
			{
				if (IsClassStructAccessDim(psrcData + curSrcIndex))
				{
					isPublic = StartsWith(psrcData + curSrcIndex, PublicFlag);
					INT colonFlagIndex = FindFlag(psrcData, curSrcIndex, ColonFlag);
					if (colonFlagIndex == -1 || colonFlagIndex > braceFlagRIndex)
					{
						return FALSE;
					}

					if (isPublic && ++publicFlagCount == 1)
					{
						curDstIndex += Append(TabFlag, g_braceLayerCount - 1, pdstData + curDstIndex);
						curDstIndex += Append(PublicFlag, pdstData + curDstIndex);
						curDstIndex += Append(ColonFlag, pdstData + curDstIndex);
						curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
					}

					curSrcIndex = colonFlagIndex + 1;
					SkipComment(psrcData, curSrcIndex);
					continue;
				}

				if (isPublic)
				{
					INT preDstIndex = curDstIndex;
					if (!TryAllBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
					{
						return FALSE;
					}
					else
					{
						if (pdstData[preDstIndex] != NullCharFlag)
						{
							++exportToLuaCount;
							continue;
						}
					}
				}
				else
				{
					if (!SkipStatement(psrcData, curSrcIndex))
					{
						return FALSE;
					}
				}
			}
		}
		--g_braceLayerCount;

		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);

		++curSrcIndex;
		SkipComment(psrcData, curSrcIndex);
		INT labelLen = GetLabelLen(psrcData + curSrcIndex);
		if (labelLen == 0)
		{
			curDstIndex += Append(ClassStructEFlag, pdstData + curDstIndex);
		}
		else
		{
			curDstIndex += Append(BraceFlagR, pdstData + curDstIndex);
			curDstIndex += Append(SpaceFlag, pdstData + curDstIndex);
			curDstIndex += Append(psrcData, curSrcIndex, curSrcIndex + labelLen, pdstData + curDstIndex);
			curDstIndex += Append(SemicolonFlag, pdstData + curDstIndex);

			curSrcIndex += labelLen;
			SkipComment(psrcData, curSrcIndex);
		}

		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

		if (!StartsWith(psrcData + curSrcIndex, SemicolonFlag))
		{
			return FALSE;
		}

		if (exportToLuaCount == 0)
		{
			pdstData[idst] = NullCharFlag;
			curDstIndex = idst;
		}

		++curSrcIndex;
		SkipComment(psrcData, curSrcIndex);

		isrc = curSrcIndex;
		idst = curDstIndex;
	}
	return TRUE;
}

BOOL IsExternCDim(LPCTSTR psrcData, INT isrc)
{
	if (StartsWith(psrcData + isrc, ExternFlag))
	{
		isrc += _tcslen(ExternFlag);
		do 
		{
			SkipComment(psrcData, isrc);
			if (!StartsWith(psrcData + isrc, DoubleQuotatioMarkFlag))
				break;

			SkipComment(psrcData, ++isrc);
			if (!IsMatchFlag(psrcData[isrc], CFlag))
				break;

			SkipComment(psrcData, ++isrc);
			if (!StartsWith(psrcData + isrc, DoubleQuotatioMarkFlag))
				break;

			SkipComment(psrcData, ++isrc);
			if (!StartsWith(psrcData + isrc, BraceFlagL))
				break;

			return TRUE;

		} while (0);
	}
	return FALSE;
}

BOOL TryAllBlock(LPCTSTR psrcData, INT& isrc, LPTSTR pdstData, INT& idst)
{
	INT curSrcIndex = isrc;
	INT curDstIndex = idst;

	BOOL addBraceLayer = FALSE;
	BOOL exportToLuaCount = 0;

	INT bracketFlagLIndex = FindFlag(psrcData, curSrcIndex, BraceFlagL);
	INT endSrcIndex = curSrcIndex + 1;

	if (StartsWith(psrcData + curSrcIndex, NamespaceFlag))
	{
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
		addBraceLayer = TRUE;
	}
	else if (IsExternCDim(psrcData, curSrcIndex))
	{
		addBraceLayer = TRUE;
	}

	if (addBraceLayer)
	{
		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
		if (bracketFlagLIndex == -1)
		{
			return FALSE;
		}
		INT bracketFlagRIndex = FindFlag(psrcData, bracketFlagLIndex, BraceFlagL, BraceFlagR);
		if (bracketFlagRIndex == -1)
		{
			return FALSE;
		}
		endSrcIndex = bracketFlagRIndex;

		while(curSrcIndex < bracketFlagLIndex)
		{
			INT worldLen = GetLabelLen(psrcData + curSrcIndex);
			curDstIndex += Append(psrcData, curSrcIndex, curSrcIndex + worldLen, pdstData + curDstIndex);
			curDstIndex += Append(SpaceFlag, pdstData + curDstIndex);

			curSrcIndex += worldLen;
			SkipComment(psrcData, curSrcIndex);
		}

		RightTrim(pdstData, curDstIndex, curDstIndex);
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);
		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
		curDstIndex += Append(BraceFlagL, pdstData + curDstIndex);
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

		curSrcIndex = bracketFlagLIndex + 1;
		SkipComment(psrcData, curSrcIndex);

		++g_braceLayerCount;
	}


	while(curSrcIndex < endSrcIndex)
	{
		INT preDstIndex = curDstIndex;

		if (StartsWith(psrcData + curSrcIndex, NamespaceFlag) ||
			IsExternCDim(psrcData, curSrcIndex))
		{
			PushPopFlag<BOOL> pushPopForceExp(g_fourceExportToLua);
			if(!TryAllBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
			{
				return FALSE;
			}
			else
			{
				if (pdstData[preDstIndex] != NullCharFlag)
				{
					++exportToLuaCount;
					continue;
				}
			}
		}

		if (!TryClassStructBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
		{
			return FALSE;
		}
		else
		{
			if (pdstData[preDstIndex] != NullCharFlag)
			{
				++exportToLuaCount;
				continue;
			}
		}

		if (!TryIncludeBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
		{
			return FALSE;
		}
		else
		{
			if (pdstData[preDstIndex] != NullCharFlag)
			{
				++exportToLuaCount;
				continue;
			}
		}

		if (!g_exportToLua && !g_fourceExportToLua)
		{
			if (!IsClassStructAccessDim(psrcData + curSrcIndex) 
					&& !StartsWith(psrcData + curSrcIndex, BraceFlagR)
					&& !IsAClassStructDim(psrcData + curSrcIndex))
			{
				if (!SkipStatement(psrcData, curSrcIndex))
				{
					return FALSE;
				}
			}
			continue;
		}

		if(!TryEnumBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
		{
			return FALSE;
		}
		else
		{
			if (pdstData[preDstIndex] != NullCharFlag)
			{
				++exportToLuaCount;
				continue;
			}
		}

		if(!TryFunctionBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
		{
			return FALSE;
		}
		else
		{
			if (pdstData[preDstIndex] != NullCharFlag)
			{
				++exportToLuaCount;
				continue;
			}
		}

		if(!TryPropertyBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
		{
			return FALSE;
		}
		else
		{
			if (pdstData[preDstIndex] != NullCharFlag)
			{
				++exportToLuaCount;
				continue;
			}
		}
	}

	if (addBraceLayer)
	{
		--g_braceLayerCount;
		curDstIndex += Append(TabFlag, g_braceLayerCount, pdstData + curDstIndex);
		curDstIndex += Append(BraceFlagR, pdstData + curDstIndex);
		curDstIndex += Append(ReturnNewlineFlag, pdstData + curDstIndex);

		curSrcIndex = endSrcIndex + 1;
		SkipComment(psrcData, curSrcIndex);
	}
	if (exportToLuaCount == 0)
	{
		pdstData[idst] = NullCharFlag;
		curDstIndex = idst;
	}

	isrc = curSrcIndex;
	idst = curDstIndex;
	return TRUE;
}

LPVOID GetFileData(LPCTSTR pstrFilename)
{
	HANDLE hFile = CreateFile(pstrFilename, GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return NULL;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	if (fileSize == 0)
	{
		return NULL;
	}

	BYTE* pData = new BYTE[fileSize + 1];
	DWORD readNum;
	if(!ReadFile(hFile, pData, fileSize, &readNum, NULL) || readNum != fileSize)
	{
		SafeDelArray(pData);
		CloseHandle(hFile);
		return NULL;
	}

	pData[fileSize] = 0;
	CloseHandle(hFile);
	return pData;
}

CHAR* TCHARToCHAR(LPCTSTR pData)
{
	CHAR* pOutdata;
#ifdef UNICODE
	INT dataLen = _tcslen(pData) * 2 + 1;
	pOutdata = new CHAR[dataLen];
	wcstombs(pOutdata, pData, dataLen);
#else
	INT dataLen = _tcslen(pData) + 1;
	pOutdata = new CHAR[dataLen];
	strcpy(pOutdata, pData);
#endif
	return pOutdata;
}

BOOL FixLuaWrapSrc(LPCSTR filename)
{
#ifdef UNICODE
	const int size = MultiByteToWideChar(CP_ACP, 0, filename, -1, NULL, 0);
	LPTSTR theFilename = new TCHAR[size];
	MultiByteToWideChar(CP_ACP, 0, filename, -1, theFilename, size);
#else
	LPCSTR theFilename = filename;
#endif
	//LogMsg << "Preparing...";
	LPVOID pFileData;
	do 
	{
		::Sleep(1000);
	} while (NULL == (pFileData = GetFileData(theFilename)));
	
	CString srcData = (char*) pFileData;
	SafeDelArray(pFileData);
	LPCTSTR psrcData = srcData.GetBuffer();
	INT srcDataLen = _tcslen(psrcData);

	LPTSTR pdstData = new TCHAR[srcDataLen + 1024];
	pdstData[0] = NullCharFlag;

	std::list<std::pair<INT, INT>> headerFiles;
	std::list<std::pair<INT, INT>> strLables;
	INT layerCount = -1;
	INT skipStrLbCount = 0;
	BOOL tryInclude = TRUE;
	BOOL finish = FALSE;

	INT isrc = 0;
	INT idst = 0;

	idst += Append(_T("/*\n** Lua Wrap: "), pdstData + idst);
	idst += Append(_T(g_workPrjDir), pdstData + idst);
	idst += Append(_T("\n** Generated automatically by AutoWrapLua.exe\n*/\n\n"), pdstData + idst);

	if (g_pch)
	{
		idst += Append(_T("#include \""), pdstData + idst);
		idst += Append(g_pch, pdstData + idst);
		idst += Append(_T("\"\n"), pdstData + idst);
		g_pch = NULL;
	}

	idst += Append(_T("#include \"AutoRegToLua.h\"\n"), pdstData + idst);
	
	SkipComment(psrcData, isrc);

	while (!finish && psrcData[isrc] != 0)
	{
		INT progress = INT(FLOAT(isrc) / FLOAT(srcDataLen) * 100);
		//LogMsg << "\rGenerating lua export functions...";
		//LogMsg << progress << "%100";

		INT lineLen = GetLineLen(psrcData + isrc, FALSE) + 1;
		INT lineStart = isrc;
		isrc += lineLen;

		while(psrcData[lineStart] != NullCharFlag)
		{
			if (IsSpaceTabFlag(psrcData[lineStart]))
			{
				++lineStart;
			}
			else
			{
				break;
			}
		}

		if (tryInclude && StartsWith(psrcData + lineStart, IncludeFlag))
		{
			INT doubleQuotatioIndexL = FindFlag(psrcData, lineStart, DoubleQuotatioMarkFlag);
			INT bracketFlagLIndex = FindFlag(psrcData, lineStart, BracketFlagL);
			INT indexL = ((doubleQuotatioIndexL != -1 && doubleQuotatioIndexL < bracketFlagLIndex) || bracketFlagLIndex == -1) ? doubleQuotatioIndexL : bracketFlagLIndex;
			if (indexL == -1)
			{
				continue;
			}
			TCHAR FlagR = (indexL == doubleQuotatioIndexL) ? DoubleQuotatioMarkFlag : BracketFlagR;
			++indexL;

			LeftTrim(psrcData, indexL);

			INT indexR = FindFlag(psrcData, indexL, FlagR);
			if (indexR == -1)
			{
				continue;
			}

			RightTrim(psrcData, indexR, indexR);
			
			BOOL isfind = FALSE;
			for (std::list<std::pair<INT, INT>>::iterator it = headerFiles.begin(), endIt = headerFiles.end(); it != endIt && !isfind; ++it)
			{
				if (it->second - it->first != indexR - indexL)
				{
					continue;
				}
				isfind = TRUE;
				for (int i = it->first, j = indexL; i < it->second; ++i, ++j)
				{
					TCHAR elem0 = isupper(psrcData[i]) ? _tolower(psrcData[i]) : psrcData[i];
					TCHAR elem1 = isupper(psrcData[j]) ? _tolower(psrcData[j]) : psrcData[j];
					if (elem0 != elem1)
					{
						isfind = FALSE;
						break;
					}
				}
			}
			if (isfind)
			{
				continue;
			}
			headerFiles.push_back(std::make_pair(indexL, indexR));
		}
		else
		{
			INT icur = lineStart;
			SkipComment(psrcData, icur);
			if (StartsWith(psrcData + icur, ToluaWrapFuncBegin))
			{
				layerCount = 0;
				strLables.clear();
				if (tryInclude)
				{
					//idst += Append(_T("\nusing namespace LuaWrap;\n"), pdstData + idst);
				}
				tryInclude = FALSE;
			}
			else if (StartsWith(psrcData + icur, BraceFlagL))
			{
				if (layerCount >=0)
				{
					++layerCount;
				}
			}
			else if (StartsWith(psrcData + icur, BraceFlagR))
			{
				if(--layerCount == 0)
				{
					layerCount = -1;
				}
			}
			else if (StartsWith(psrcData + icur, _T("return")))
			{
				if (skipStrLbCount > 0)
				{
					icur += _tcslen(_T("return"));
					SkipComment(psrcData, icur);
					INT retCount = _ttoi(psrcData + icur);
					retCount -= skipStrLbCount;
					TCHAR buff[100];
					_itot(retCount, buff, 10);

					idst += Append(SpaceFlag, layerCount, pdstData + idst);
					idst += Append(_T("return "),  pdstData + idst);
					idst += Append(buff, pdstData + idst);
					idst += Append(SemicolonFlag, pdstData + idst);
					idst += Append(ReturnNewlineFlag, pdstData + idst);

					skipStrLbCount = 0;

					strLables.clear();

					continue;
				}
			}
			else if(lineLen >  40)
			{
				if (layerCount > 0)
				{
					INT itoCppStr = FindFlag(psrcData, icur, _T("tolua_tocppstring"), isrc);
					if (itoCppStr != -1)
					{
						INT istrLbDim = FindFlag(psrcData, icur, _T("const std::string"), isrc);
						if (istrLbDim != -1)
						{
							static const int CSDTStrLen = _tcslen(_T("const std::string"));
							INT istrLb = icur + CSDTStrLen;
							SkipComment(psrcData, istrLb);
							INT strLbLen = GetLabelLen(psrcData + istrLb);
							strLables.push_back(std::make_pair(istrLb, istrLb + strLbLen));
						}
					}
					else if (StartsWith(psrcData + icur, _T("tolua_pushcppstring(tolua_S,(const char*)")))
					{
						static const int  PushCppStrDimLen = _tcslen(_T("tolua_pushcppstring(tolua_S,(const char*)"));
						INT istrLb = icur + PushCppStrDimLen;
						BOOL finding = TRUE;
						for (std::list<std::pair<INT, INT>>::iterator it = strLables.begin(), endIt = strLables.end(); finding && it != endIt; ++it)
						{
							finding = FALSE;
							for (INT i = it->first, iMax = it->second, j = istrLb; i < iMax; ++i, ++j)
							{
								if (psrcData[i] != psrcData[j])
								{
									finding = TRUE;
									break;
								}
							}
						}
						if (!finding)
						{
							++skipStrLbCount;
							continue;
						}
					}
				}
			}
		}
		
		for (int i = isrc - lineLen; i < isrc; ++i)
		{
			if (StartsWith(psrcData + i, _T("_cstring const string")))
			{
				i += _tcslen(_T("_cstring const string")) - 1;
				idst += Append(_T("const char*"), pdstData + idst);
			}
			else if (StartsWith(psrcData + i, _T("#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501")))
			{
				idst += Append(_T("static AutoRegToLua s_autoRegToLua(tolua_"), pdstData + idst);
				idst += Append(g_workPrjDir, pdstData + idst);
				idst += Append(_T("_open, \""), pdstData + idst);
				idst += Append(g_workPrjDir, pdstData + idst);
				idst += Append(_T("\");\n\n"), pdstData + idst);


				idst += Append(_T("FORCE_LINK_DECL("), pdstData + idst);
				idst += Append(g_workPrjDir, pdstData + idst);
				idst += Append(_T(");\n"), pdstData + idst);

				finish = TRUE;

				break;
			}
			else
			{
				if (tryInclude && g_toluaAddPath && StartsWith(psrcData + i, _T("tolua++.h")))
				{
					idst += Append(g_toluaAddPath, pdstData + idst);
				}
				pdstData[idst++] = psrcData[i];
			}
		}
	}
	pdstData[idst] = NullCharFlag;

	//LogMsg << "\rGenerating lua export functions...100%100\n";
	LogMsg << filename;
	::Sleep(1000);
	SetFileData(theFilename, pdstData, FALSE);
#ifdef UNICODE
	SafeDelArray(theFilename);
#endif
	SafeDelArray(pdstData);
	return TRUE;
}

BOOL SetFileData(LPCTSTR pstrFilename, LPCTSTR pData, BOOL isAppen)
{
	CHAR* pOutdata = TCHARToCHAR(pData);
	BOOL isSuccess = FALSE;

	CString fileDir(GetFileDir(pstrFilename));
	if (fileDir.GetLength() > 0 && !IsFileDirExists(fileDir))
	{
		char cmd[256];
		char* path = TCHARToCHAR(fileDir);
		sprintf(cmd, "md %s", path);
		system(cmd);
		SafeDelArray(path);
	}

	HANDLE hFile = CreateFile(pstrFilename, GENERIC_WRITE, 0, NULL, isAppen ? OPEN_ALWAYS : CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		if (isAppen)
		{
			SetFilePointer(hFile, 0,  NULL,  FILE_END); 
		}

		INT len = strlen(pOutdata);
		DWORD writeNum;
		if (WriteFile(hFile, pOutdata, len, &writeNum, NULL) && len == writeNum)
		{
			isSuccess = TRUE;
		}
		CloseHandle(hFile);
	}

	SafeDelArray(pOutdata);

	return isSuccess;
}

CString GetPkgFilename(LPCTSTR srcFilename)
{
	CString pkgFilename;
	LPCTSTR base = g_workHomeDir ? _tcsstr(srcFilename, g_workHomeDir) : NULL;
	if (g_PkgPath == NULL || base == NULL)
	{
		pkgFilename = GetFilenameWithoutExt(srcFilename) + PkgFileExtFlag;
	}
	else
	{
		base += _tcslen(g_workHomeDir);
		pkgFilename = g_PkgPath;
		if (_tcslen(pkgFilename) > 0 && !EndsWith(pkgFilename, SlashFlag) && !EndsWith(pkgFilename, BackslashFlag))
		{
			pkgFilename += "\\";
		}
		pkgFilename += GetFilenameWithoutExt(base) + PkgFileExtFlag;
	}
	return pkgFilename;
}

VOID AddExportCfgRecord(LPCTSTR filename)
{
	CString pkgFilename(GetPkgFilename(filename));

	if (!IsFileExists(pkgFilename))
	{
		return;
	}

	LPTSTR exportConfigData = new TCHAR[_tcslen(filename) + MAX_PATH];
	if (IsHeaderFile(filename))
	{
		LPCTSTR base = g_workHomeDir ?  _tcsstr(filename, g_workHomeDir) : NULL;
		LPCTSTR incFile = base ? base + _tcslen(g_workHomeDir) : filename;
		while(StartsWith(incFile, BackslashFlag) || StartsWith(incFile, SlashFlag))
		{
			++incFile;
		}
		sprintf(exportConfigData, _T("\r\n$#include \"%s\""), incFile);
		SetFileData(g_exportCfgFilename, exportConfigData, TRUE);
	}

	sprintf(exportConfigData, _T("\r\n%s\t\"%s\"\r\n"), PfileFlag, pkgFilename);
	SetFileData(g_exportCfgFilename, exportConfigData, TRUE);
	SafeDelArray(exportConfigData);

	CHAR* info = TCHARToCHAR(pkgFilename);
	LogMsg << info << '\n';
	SafeDelArray(info);
}

BOOL IsManualPkgNew(LPCTSTR pstrMain, LPCTSTR pstrLuaWrapFilename)
{
	if (IsFileNewer(pstrMain, pstrLuaWrapFilename))
	{
		return TRUE;
	}

	LPVOID pFileData = GetFileData(pstrMain);
	if (pFileData == NULL)
	{
		return TRUE;
	}

	CString srcData = (char*) pFileData;
	SafeDelArray(pFileData);
	LPCTSTR psrcData = srcData.GetBuffer();
	TCHAR  filePath[MAX_PATH];

	TCHAR  exportCfgFilename[MAX_PATH];
	Append(g_exportCfgFilename, 0, _tcslen(g_exportCfgFilename), exportCfgFilename);
	FilterPath(exportCfgFilename);

	INT srcDataLen = _tcslen(srcData);
	INT isrc = 0;
	SkipComment(psrcData, isrc);
	while(isrc < srcDataLen)
	{
		if (!StartsWith(psrcData + isrc, PfileFlag))
		{
			++isrc;
			continue;
		}
		isrc += _tcslen(PfileFlag);
		SkipComment(psrcData, isrc);
		INT doubleQuotatioMarkFlagRIndex = FindFlag(psrcData, isrc, DoubleQuotatioMarkFlag, DoubleQuotatioMarkFlag);
		if (doubleQuotatioMarkFlagRIndex == -1)
		{
			continue;
		}
		Append(psrcData, isrc + 1, doubleQuotatioMarkFlagRIndex, filePath);
		FilterPath(filePath);
		if (!IsEqual(exportCfgFilename, filePath) && IsFileNewer(filePath, pstrLuaWrapFilename))
		{
			return TRUE;
		}
		isrc = doubleQuotatioMarkFlagRIndex + 1;
		SkipComment(psrcData, isrc);
	}
	return FALSE;
}

VOID ParseFile(LPCTSTR pstrFilename)
{
	PushPopFlag<BOOL> pushPopForceExp(g_fourceExportToLua);
	if (!IsSourceFile(pstrFilename))
	{
		return;
	}

	DebugOutput(pstrFilename);

	CString pkgFilename(GetPkgFilename(pstrFilename));

	if (!IsFileNewer(pstrFilename, pkgFilename))
	{
		AddExportCfgRecord(pstrFilename);
		return;
	}

	LPVOID pFileData = GetFileData(pstrFilename);
	if (pFileData == NULL)
	{
		return;
	}

	CString srcData = (char*) pFileData;
	SafeDelArray(pFileData);
	LPCTSTR psrcData = srcData.GetBuffer();

	DeleteFile(pkgFilename);
	if (FindFlag(psrcData, 0, ExportToLuaFlag) != -1 || FindFlag(psrcData, 0, BeginExportToLuaFlag) != -1)
	{
		INT srcDataLen = _tcslen(psrcData);
		LPTSTR pdstData = new TCHAR[srcDataLen + 10];
		pdstData[0] = NullCharFlag;

		INT curSrcIndex = 0;
		INT curDstIndex = 0;
		BOOL isSuccess = TRUE;

		SkipComment(psrcData, curSrcIndex);
		while(curSrcIndex < srcDataLen)
		{
			if(!TryAllBlock(psrcData, curSrcIndex, pdstData, curDstIndex))
			{
				isSuccess = FALSE;
				break;
			}
		}

		if (FindFlag(pdstData, 0, SemicolonFlag) != -1)
		{
			SetFileData(pkgFilename, ExportConfigDim, FALSE);
			SetFileData(pkgFilename, pdstData, TRUE);
			AddExportCfgRecord(pstrFilename);
			++g_genPkgfileCount;
		}
		SafeDelArray(pdstData);
	}
}

VOID ParseDir(LPCTSTR pstrDirname, INT basePathLen)
{ 
	CFileFind finder; 
	CString strDir(pstrDirname); 
	strDir += MatchAllFileDirFlag; 

	BOOL find = finder.FindFile(strDir); 
	while (find) 
	{ 
		find = finder.FindNextFile();
		if (basePathLen == -1)
		{
			basePathLen = max(0, finder.GetRoot().GetLength() - _tcslen(pstrDirname) - 1);
		}

		if (finder.IsDots()) 
		{
			continue;
		}
		else if (finder.IsDirectory()) 
		{ 
			ParseDir(finder.GetFilePath(), basePathLen); 
		}
		else
		{
			ParseFile(static_cast<LPCTSTR>(finder.GetFilePath()) + basePathLen);
		}
	} 
	finder.Close(); 
}
