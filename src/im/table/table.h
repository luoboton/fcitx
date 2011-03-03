/***************************************************************************
 *   Copyright (C) 2002~2005 by Yuking                                     *
 *   yuking_net@sohu.com                                                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef _TABLE_H
#define _TABLE_H

#include <X11/Xlib.h>

#include "utils/configfile.h"
#include "core/ime.h"
#include "utils/utarray.h"

#define MAX_CODE_LENGTH  30
#define PHRASE_MAX_LENGTH 10
#define FH_MAX_LENGTH  10
#define TABLE_AUTO_SAVE_AFTER 1024
#define AUTO_PHRASE_COUNT 10000
#define SINGLE_HZ_COUNT 66000

typedef struct _RULE_RULE {
    unsigned char   iFlag;	// 1 --> 正序   0 --> 逆序
    unsigned char   iWhich;	//第几个字
    unsigned char   iIndex;	//第几个编码
} RULE_RULE;

typedef struct _RULE {
    unsigned char   iWords;	//多少个字
    unsigned char   iFlag;	//1 --> 大于等于iWords  0 --> 等于iWords
    RULE_RULE      *rule;
} RULE;

typedef struct _TABLE {
    GenericConfig   config;
    char           *strPath;
    char           *strSymbolFile;
    char           *strName;
    char           *strIconName;
    char           *strInputCode;
    unsigned char   iCodeLength;
    unsigned char   iPYCodeLength;
    char           *strEndCode;	//中止键，按下该键相当于输入该键后再按一个空格
    char           *strIgnoreChars;
    char            cMatchingKey;
    char           *strSymbol;
    char            cPinyin;	//输入该键后，表示进入临时拼音状态
    unsigned char   bRule;

    RULE           *rule;	//组词规则
    int             iIMIndex;	//记录该码表对应于输入法的顺序
    unsigned int    iRecordCount;
    ADJUSTORDER     tableOrder;

    int             iPriority;
    boolean            bUsePY;	//使用拼音
    int             iTableAutoSendToClient;	//自动上屏
    int             iTableAutoSendToClientWhenNone;	//空码自动上屏
    boolean            bUseMatchingKey;	//是否模糊匹配
    boolean            bAutoPhrase;	//是否自动造词
    int             iSaveAutoPhraseAfter;	//选择N次后保存自动词组，0-不保存，1-立即保存
    boolean            bAutoPhrasePhrase;	//词组是否参与造词
    int             iAutoPhrase;	//自动造词长度
    boolean            bTableExactMatch;	//是否只显示精确匹配的候选字/词
    boolean            bPromptTableCode;	//输入完毕后是否提示编码

    boolean            bHasPinyin;		//标记该码表中是否有拼音
    char           *strChoose;		//设置选择键
    boolean            bEnabled;
} TABLE;

typedef struct _RECORD {
    char           *strCode;
    char           *strHZ;
    struct _RECORD *next;
    struct _RECORD *prev;
    unsigned int    iHit;
    unsigned int    iIndex;
    unsigned int    flag:1;
    unsigned int    bPinyin:1;
} RECORD;

/* 根据键码生成一个简单的索引，指向该键码起始的第一个记录 */
typedef struct _RECORD_INDEX {
    RECORD         *record;
    char            cCode;
} RECORD_INDEX;

typedef struct _FH {
    char            strFH[FH_MAX_LENGTH * 2 + 1];
} FH;

typedef struct _AUTOPHRASE {
    char           *strHZ;
    char           *strCode;
    char            iSelected;
    unsigned int    flag:1;
    struct _AUTOPHRASE *next;	//构造一个队列
} AUTOPHRASE;

//用union就会出错，不知道是啥原因
typedef struct {
    AUTOPHRASE     *autoPhrase;
    RECORD         *record;
    char            strPYPhrase[PHRASE_MAX_LENGTH * 2 + 1];
} CANDWORD;

typedef struct _TABLECANDWORD {
    unsigned int    flag:2;	//指示该候选字/词是自动组的词还是正常的字/词
    CANDWORD        candWord;
} TABLECANDWORD;

typedef enum {
    CT_NORMAL = 0,
    CT_AUTOPHRASE,
    CT_PYPHRASE			//临时拼音转换过来的候选字/词
} CANDTYPE;

typedef struct TableState {
    UT_array* table; /* 码表 */
    
    char            iTableIMIndex;
    char            iTableCount;
    
    char            iCurrentTableLoaded;
    
    RECORD         *currentRecord;
    RECORD	       *recordHead;
    
    RECORD         *tableSingleHZ[SINGLE_HZ_COUNT];		//Records the single characters in table to speed auto phrase
    RECORD	       *pCurCandRecord;	//Records current cand word selected, to update the hit-frequency information
    TABLECANDWORD   tableCandWord[MAX_CAND_WORD*2];
    
    RECORD_INDEX   *recordIndex;
    
    AUTOPHRASE     *autoPhrase;
    AUTOPHRASE     *insertPoint;
    
    uint            iAutoPhrase;
    uint            iTableCandDisplayed;
    uint            iTableTotalCandCount;
    char            strTableLegendSource[PHRASE_MAX_LENGTH * UTF8_MAX_LENGTH + 1];
    
    FH             *fh;
    int             iFH ;
    unsigned int    iTableIndex;
    
    boolean            bIsTableDelPhrase;
    HOTKEYS         hkTableDelPhrase[HOT_KEY_COUNT];
    boolean            bIsTableAdjustOrder;
    HOTKEYS         hkTableAdjustOrder[HOT_KEY_COUNT];
    boolean            bIsTableAddPhrase;
    HOTKEYS         hkTableAddPhrase[HOT_KEY_COUNT];
    
    char            iTableChanged;
    char            iTableNewPhraseHZCount;
    boolean            bCanntFindCode;	//Records if new phrase has corresponding code - should be always false
    char           *strNewPhraseCode;
    
    SINGLE_HZ       hzLastInput[PHRASE_MAX_LENGTH];	//Records last HZ input
    short           iHZLastInputCount ;
    boolean            bTablePhraseTips;
    
    ADJUSTORDER     PYBaseOrder;
    boolean		    isSavingTableDic;
} TableState;

extern TableState tbl;

void            LoadTableInfo (void);
boolean            LoadTableDict (void);
void            TableInit (void);
void            FreeTableIM (char index);
void            SaveTableIM (void);
void            SaveTableDict (void);
boolean            IsInputKey (int iKey);
boolean            IsIgnoreChar (char cChar);
boolean            IsEndKey (char cChar);
char            IsChooseKey (int iKey);

INPUT_RETURN_VALUE DoTableInput (unsigned int sym, unsigned int state, int keyCount);
INPUT_RETURN_VALUE TableGetCandWords (SEARCH_MODE mode);
void            TableAddCandWord (RECORD * wbRecord, SEARCH_MODE mode);
void            TableAddAutoCandWord (short which, SEARCH_MODE mode);
INPUT_RETURN_VALUE TableGetLegendCandWords (SEARCH_MODE mode);
void            TableAddLegendCandWord (RECORD * record, SEARCH_MODE mode);
INPUT_RETURN_VALUE TableGetFHCandWords (SEARCH_MODE mode);
INPUT_RETURN_VALUE TableGetPinyinCandWords (SEARCH_MODE mode);
void            TableResetStatus (void);
char           *TableGetLegendCandWord (int iIndex);
char           *TableGetFHCandWord (int iIndex);
boolean            HasMatchingKey (void);
int             TableCompareCode (char *strUser, char *strDict);
int             TableFindFirstMatchCode (void);
void            TableAdjustOrderByIndex (int iIndex);
void            TableDelPhraseByIndex (int iIndex);
void            TableDelPhraseByHZ (char *strHZ);
void            TableDelPhrase (RECORD * record);
RECORD         *TableHasPhrase (char *strCode, char *strHZ);
RECORD         *TableFindPhrase (char *strHZ);
void            TableInsertPhrase (char *strCode, char *strHZ);
char	       *_TableGetCandWord (int iIndex, boolean _bLegend);		//Internal
char           *TableGetCandWord (int iIndex);
void		TableUpdateHitFrequency (RECORD * record);
void            TableCreateNewPhrase (void);
void            TableCreatePhraseCode (char *strHZ);
boolean            TablePhraseTips (void);
void            TableSetCandWordsFlag (int iCount, boolean flag);
void            TableResetFlags (void);

void            TableCreateAutoPhrase (char iCount);

void            UpdateHZLastInput (char *);

ConfigFileDesc *GetTableConfigDesc();
CONFIG_BINDING_DECLARE(TABLE);

#endif
