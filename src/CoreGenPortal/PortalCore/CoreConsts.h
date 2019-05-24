//
// _CoreConsts_h_
//
// Copyright (C) 2017-2019 Tactical Computing Laboratories, LLC
// All Rights Reserved
// contact@tactcomplabs.com
//
// See LICENSE in the top level directory for licensing details
//

#ifndef _CORECONSTS_H_
#define _CORECONSTS_H_

// wxWidgets headers
#include <wx/wxprec.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/textfile.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

// standard C++ headers
#include <string>

//-- Version Information
const wxString PORTAL_VERSION = "0.2.0";
const wxString PORTAL_COPYRIGHT = "Copyright 2017-2019 Tactical Computing Laboratories, LLC";

//-- Environment Variables
const std::string PORTAL_ENV_USER_CONFIG = "COREGENPORTAL_CONFIG";

//-- Standard File Locations
const wxString PORTAL_USER_DIR = ".CoreGenPortal";
const wxString PORTAL_USER_CONFIG = PORTAL_USER_DIR + wxT("/CoreGenPortal.conf");

//-- IR Keywords
const std::string L0KeyWords = "Registers RegClasses ISAs InstFormats Insts PseudoInsts Caches Scratchpads MemoryControllers Extensions Cores Socs";  ///< L0 IR Keywords
const std::string L1KeyWords = "RegName RegisterClassName ISAName InstFormatName Inst PseudoInst Cache Scratchpad MemoryController MajorVersion MinorVersion Type Extension Core Soc";  ///< L1 IR Keywords
const std::string L2KeyWords = "Width Index PseudoName IsFixedValue IsSIMD RWReg ROReg CSRReg AMSReg Shared Registers ISA FormatWidth Fields FieldName FieldType FieldWidth StartBit EndBit MandatoryField InstFormat Encodings EncodingField EncodingWidth EncodingValue Sets Ways SubLevel MemSize RqstPorts RspPorts StartAddr Ports RTLFile RegClass"; ///< L2 IR Keywords

#endif

// EOF
