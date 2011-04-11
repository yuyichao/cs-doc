" Vim syntax file
" Language:	idc
" Maintainer:	watercloud@xfocus.org
" Last Change:	2003-2-27

" For version 5.x: Clear all syntax items
" For version 6.x: Quit when a syntax file was already loaded
if version < 600
  syntax clear
elseif exists("b:current_syntax")
  finish
endif

syn case match

syn keyword idcConstant  BADADDR
syn keyword	idcKeyword      auto include function static break return continue if else elseif while for do define
syn keyword	idcType		char string long void success

syn keyword idcFunctions1  AddCodeXref AddConstEx AddEntryPoint AddEnum AddHotkey AddSourceFile AddStrucEx AddStrucMember AltOp AnalyseArea Analysis AskAddr AskFile AskIdent AskSeg AskSelector AskStr AskYN AutoMark AutoMark2 AutoShow Batch BeginEA Byte ChooseFunction CmtIndent Comment Comments Compile CreateArray DelArrayElement DelCodeXref DelConstEx DelEnum DelExtLnA DelExtLnB DelFixup DelFunction DelHashElement DelHotkey DelLineNumber DelSelector DelSourceFile DelStruc DelStrucMember DeleteAll DeleteArray Demangle Dfirst DfirstB Dnext DnextB Dword Exec

syn keyword idcFunctions2 Exit ExtLinA ExtLinB Fatal FindBinary FindCode FindData FindExplored FindFuncEnd FindImmediate FindProc FindSelector FindText FindUnexplored FindVoid FirstSeg GenerateFile GetArrayElement GetArrayId GetBmaskCmt GetBmaskName GetCharPrm GetConstBmask GetConstByName GetConstCmt GetConstEnum GetConstEx GetConstName GetConstValue GetCurrentLine GetEntryOrdinal GetEntryPoint GetEntryPointQty GetEnum GetEnumCmt GetEnumFlag GetEnumIdx GetEnumName GetEnumQty GetEnumSize GetFirstBmask GetFirstConst GetFirstHashKey GetFirstIndex GetFirstMember GetFirstStrucIdx GetFixupTgtDispl GetFixupTgtOff GetFixupTgtSel GetFixupTgtType GetFlags GetFrame GetFrameArgsSize GetFrameLvarSize GetFrameRegsSize GetFrameSize GetFuncOffset GetFunctionCmt GetFunctionFlags GetFunctionName GetHashLong GetHashString GetIdaDirectory GetIdbPath GetInputFile GetInputFilePath GetLastBmask GetLastConst GetLastHashKey GetLastIndex GetLastMember GetLastStrucIdx GetLineNumber GetLongPrm GetManualInsn GetMarkComment GetMarkedPos GetMemberComment GetMemberFlag

syn keyword idcFunctions3 GetMemberName GetMemberOffset GetMemberQty GetMemberSize GetMemberStrId GetMnem GetNextBmask GetNextConst GetNextFixupEA GetNextHashKey GetNextIndex GetNextStrucIdx GetOpType GetOperandValue GetOpnd GetPrevBmask GetPrevConst GetPrevFixupEA GetPrevHashKey GetPrevIndex GetPrevStrucIdx GetReg GetSegmentAttr GetShortPrm GetSourceFile GetSpDiff GetSpd GetStrucComment GetStrucId GetStrucIdByName GetStrucIdx GetStrucName GetStrucNextOff GetStrucPrevOff GetStrucQty GetStrucSize GetTrueName GetnEnum HighVoids Indent IsBitfield IsUnion ItemEnd ItemSize Jump LineA LineB LoadTil LocByName LowVoids MK_FP MakeAlign MakeArray MakeByte MakeCode MakeComm MakeDouble MakeDword MakeFloat MakeFrame MakeFunction MakeLocal MakeName MakeOword MakePackReal MakeQword MakeRptCmt MakeStr MakeStruct MakeTbyte MakeUnkn MakeVar MakeWord MarkPosition MaxEA Message MinEA Name NextAddr NextFunction NextHead NextNotTail NextSeg OpAlt OpBinary OpChr OpDecimal OpEnumEx OpHex OpHigh OpNot OpNumber OpOctal OpOff OpOffEx OpSeg OpSign OpStkvar OpStroffEx PatchByte PatchDword PatchWord PrevAddr PrevFunction PrevHead PrevNotTail RenameArray RenameEntryPoint Rfirst Rfirst0 RfirstB

syn keyword idcFunctions4 RfirstB0 Rnext Rnext0 RnextB RnextB0 RptCmt RunPlugin ScreenEA SegAddrng SegAlign SegBounds SegByBase SegByName SegClass SegComb SegCreate SegDefReg SegDelete SegEnd SegName SegRename SegStart SelEnd SelStart SetArrayLong SetArrayString SetBmaskCmt SetBmaskName SetCharPrm SetConstCmt SetConstName SetEnumBf SetEnumCmt SetEnumFlag SetEnumIdx SetEnumName SetFixup SetFlags SetFunctionCmt SetFunctionEnd SetFunctionFlags SetManualInsn SetHashLong SetHashString SetLineNumber SetLongPrm SetMemberComment SetMemberName SetMemberType SetPrcsr SetReg SetSegmentType SetSelector SetShortPrm SetSpDiff SetStatus SetStrucComment SetStrucIdx SetStrucName StringStp

syn keyword idcFunctions5 Tabs TailDepth Til2Idb Voids Wait Warning Word XrefShow XrefType add_dref atoa atol byteValue del_dref fclose fgetc filelength fopen form fprintf fputc fseek ftell hasName hasValue isBin0 isBin1 isChar0 isChar1 isCode isData isDec0 isDec1 isDefArg0 isDefArg1 isEnum0 isEnum1 isExtra isFlow isFop0 isFop1 isHead isHex0 isHex1 isLoaded isOct0 isOct1 isOff0 isOff1 isRef isSeg0 isSeg1 isStkvar0 isStkvar1 isStroff0 isStroff1 isTail isUnknown isVar loadfile ltoa readlong readshort readstr savefile set_start_cs set_start_ip strlen strstr substr writelong writeshort writestr xtol

"处理注释相关
syn region  idcString	 start=/"/ skip=/\\"/ end=/"/
syn match   idcCommentA     /\/\/.*$/
syn region  idcCommentB     start=/\/\*/ end=/\*\//

"syn match   idcEq         /==/

if version >= 508 || !exists("did_c_syn_inits")
  if version < 508
    let did_c_syn_inits = 1
    command -nargs=+ HiLink hi link <args>
  else
    command -nargs=+ HiLink hi def link <args>
  endif

  HiLink idcString		LineNr
  HiLink idcCommentA	Comment
  HiLink idcCommentB	Comment
  HiLink idcKeyword		Statement
  HiLink idcNumber		Number
  HiLink idcType		Type
  HiLink idcEq			PreProc
  HiLink idcFunctions1	Function
  HiLink idcFunctions2	Function
  HiLink idcFunctions3	Function
  HiLink idcFunctions4	Function
  HiLink idcFunctions5	Function
  HiLink idcConstant	Constant
  delcommand HiLink
endif


